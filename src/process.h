#ifndef _MRT_PROCCESS_H_
#define _MRT_PROCCESS_H_ 1

#include <mrt/threads/executor.h>
#include <mrt/strutils.h>
#include <mrt/file.h>
#include <functional>
#include <string>
#include <fcntl.h>
#include <unistd.h>
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

  static inline Process execute(const std::string& cmd) {
    Process process(cmd);
    int stdoutPipe[2], stderrPipe[2];
    pipe(stdoutPipe);
    pipe(stderrPipe);

    process.m_pid = fork();
    if (process.m_pid == 0) {
      dup2(stdoutPipe[1], STDOUT_FILENO);
      dup2(stderrPipe[1], STDERR_FILENO);
      close(stdoutPipe[1]);
      close(stdoutPipe[0]);
      close(stderrPipe[1]);
      close(stderrPipe[0]);
      char** argv = process.createArgv();
      execvp(argv[0], argv);
      exit(EXIT_FAILURE);
    } else if (process.m_pid < 0) {
      process.m_status = FAILED;
      return process;
    }

    process.m_status = RUNNING;
    close(stdoutPipe[1]);
    close(stderrPipe[1]);

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

    return process;
  }

  static inline mrt::Executor executeAsync(const std::string& cmd, CallbackType cb) {
    Process process(cmd);
    int stdoutPipe[2], stderrPipe[2];
    pipe(stdoutPipe);
    pipe(stderrPipe);

    process.m_pid = fork();
    if (process.m_pid == 0) {
      dup2(stdoutPipe[1], STDOUT_FILENO);
      dup2(stderrPipe[1], STDERR_FILENO);
      close(stdoutPipe[1]);
      close(stdoutPipe[0]);
      close(stderrPipe[1]);
      close(stderrPipe[0]);
      char** argv = process.createArgv();
      execvp(argv[0], argv);
      exit(EXIT_FAILURE);
    } else if (process.m_pid < 0) {
      process.m_status = FAILED;
      cb(process);
    }

    process.m_status = RUNNING;
    close(stdoutPipe[1]);
    close(stderrPipe[1]);

    mrt::Executor executor;
    executor.run([cb, stdoutPipe, stderrPipe](Process process) {
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
    }, process);

    return std::move(executor);
  }

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

 private:
  Status m_status = NOT_STARTED;
  pid_t m_pid;
  int m_exitCode = 0;
  std::string m_cmd;
  std::string m_stdout;
  std::string m_stderr;
};

} /* namespace mrt */

#endif
