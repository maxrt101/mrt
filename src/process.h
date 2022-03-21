#ifndef _MRT_PROCCESS_H_
#define _MRT_PROCCESS_H_ 1

#include <mrt/threads/executor.h>
#include <mrt/strutils.h>
#include <mrt/file.h>
#include <functional>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

namespace mrt {

class Process {
 public:
  enum Status {
    NOT_STARTED,
    RUNNING,
    SUCCESS,
    FAILED,
  };

  using CallbackType = std::function<void(Process)>;

 public:
  inline Process() {}
  inline Process(const std::string& cmd) : m_cmd(cmd) {}
  inline Process(const std::string& cmd, const std::string& input) : m_cmd(cmd), m_stdin(input) {}

  static inline Process execute(const std::string& cmd) {
    return executeBase<Process>(cmd, "",
      [](Process process, int stdoutPipe[2], int stderrPipe[2], int stdinPipe[2]) {
        return waitAndReadOutput(process, stdoutPipe, stderrPipe, stdinPipe);
      }
    );
  }

  static inline Process execute(const std::string& cmd, const std::string& input) {
    return executeBase<Process>(cmd, input,
      [](Process process, int stdoutPipe[2], int stderrPipe[2], int stdinPipe[2]) {
        write(stdinPipe[1], process.m_stdin.c_str(), process.m_stdin.size());
        return waitAndReadOutput(process, stdoutPipe, stderrPipe, stdinPipe);
      }
    );
  }

  static inline mrt::Executor executeAsync(const std::string& cmd, CallbackType cb) {
    return executeBase<mrt::Executor>(cmd, "",
      [cb](Process process, int stdoutPipe[2], int stderrPipe[2], int stdinPipe[2]) {
        return std::move(asyncWaitAndReadOutput(process, stdoutPipe, stderrPipe, stdinPipe, cb));
      }
    );
  }

  static inline mrt::Executor executeAsync(const std::string& cmd, const std::string& input, CallbackType cb) {
    return executeBase<mrt::Executor>(cmd, input,
      [cb](Process process, int stdoutPipe[2], int stderrPipe[2], int stdinPipe[2]) {
        write(stdinPipe[1], process.m_stdin.c_str(), process.m_stdin.size());
        return std::move(asyncWaitAndReadOutput(process, stdoutPipe, stderrPipe, stdinPipe, cb));
      }
    );
  }

  // FIXME: name clashes with execute(cmd, input)
  static inline int execute(const std::string& cmd, std::string& output) {
    FILE* out = popen(cmd.c_str(), "r");
    if (!out) {
      return -1;
    }
    output = io::File(out, false).read();
    pclose(out);
    return 0;
  }

  bool isRunning() const {
    return m_status == RUNNING;
  }

  Status getStatus() const {
    return m_status;
  }

  int getExitCode() const {
    return m_exitCode;
  }

  std::string getStdout() const {
    return m_stdout;
  }

  std::string getStderr() const {
    return m_stderr;
  }

  void kill(int sig = SIGTERM) {
    if (m_status == RUNNING) {
      ::kill(m_pid, sig);
    }
  }

 private:
  inline char** createArgv() {
    auto args = mrt::str::splitQuoted(m_cmd, " ");
    char** argv = new char*[args.size()+1] {0};
    for (int i = 0; i < args.size(); i++) {
      argv[i] = new char[args[i].size()+1] {0};
      memcpy(argv[i], args[i].c_str(), args[i].size());
    }
    return argv;
  }

  inline void freeArgv(char** argv) {
    int i = 0;
    while (argv[i]) {
      delete [] argv[i];
    }
    delete [] argv;
  }

  template <typename T, typename F>
  static inline T executeBase(const std::string& cmd, const std::string& input, F fn) {
    Process process(cmd, input);
    int *stdoutPipe = new int[2];
    int *stderrPipe = new int[2];
    int *stdinPipe = new int[2];
    pipe(stdoutPipe);
    pipe(stderrPipe);
    if (!input.empty()) {
      pipe(stdinPipe);
    }

    process.m_pid = fork();
    if (process.m_pid == 0) {
      dup2(stdoutPipe[1], STDOUT_FILENO);
      dup2(stderrPipe[1], STDERR_FILENO);
      close(stdoutPipe[1]);
      close(stdoutPipe[0]);
      close(stderrPipe[1]);
      close(stderrPipe[0]);
      if (!input.empty()) {
        dup2(stdinPipe[0], STDIN_FILENO);
        close(stdinPipe[1]);
        close(stdinPipe[0]);
      }
      char** argv = process.createArgv();
      execvp(argv[0], argv);
      exit(EXIT_FAILURE);
    } else if (process.m_pid < 0) {
      process.m_status = FAILED;
      return fn(process, stdoutPipe, stderrPipe, stdinPipe);
    }

    process.m_status = RUNNING;
    close(stdoutPipe[1]);
    close(stderrPipe[1]);
    if (!input.empty()) {
      close(stdinPipe[0]);
    }

    return fn(process, stdoutPipe, stderrPipe, stdinPipe);
  }

  static inline Process waitAndReadOutput(Process process, int* stdoutPipe, int* stderrPipe, int* stdinPipe) {
    int status = 0;
    if (waitpid(process.m_pid, &status, 0) == -1) {
      process.m_status = FAILED;
    }
    if (WIFEXITED(status)) {
      process.m_exitCode = WEXITSTATUS(status);
      process.m_status = (process.m_exitCode == 0) ? SUCCESS : FAILED;
    }

    int count, ch;
    while ((count = read(stdoutPipe[0], &ch, 1)) > 0) {
      process.m_stdout += ch;
    }
    while ((count = read(stderrPipe[0], &ch, 1)) > 0) {
      process.m_stderr += ch;
    }

    delete [] stdoutPipe;
    delete [] stderrPipe;
    delete [] stdinPipe;

    return process;
  }

  template <typename F>
  static inline Executor asyncWaitAndReadOutput(Process process, int* stdoutPipe, int* stderrPipe, int* stdinPipe, F cb) {
    mrt::Executor executor;
    executor.run([cb, stdoutPipe, stderrPipe, stdinPipe](Process process) {
      int status = 0;
      if (waitpid(process.m_pid, &status, 0) == -1) {
        process.m_status = FAILED;
      }
      if (WIFEXITED(status)) {
        process.m_exitCode = WEXITSTATUS(status);
        process.m_status = (process.m_exitCode == 0) ? SUCCESS : FAILED;
      }

      int count, ch;
      while ((count = read(stdoutPipe[0], &ch, 1)) > 0) {
        process.m_stdout += ch;
      }
      while ((count = read(stderrPipe[0], &ch, 1)) > 0) {
        process.m_stderr += ch;
      }

      cb(process);

      delete [] stdoutPipe;
      delete [] stderrPipe;
      delete [] stdinPipe;
    }, process);

    return std::move(executor);
  }

 private:
  Status m_status = NOT_STARTED;
  pid_t m_pid;
  int m_exitCode = 0;
  std::string m_cmd;
  std::string m_stdin;
  std::string m_stdout;
  std::string m_stderr;
};

} /* namespace mrt */

#endif
