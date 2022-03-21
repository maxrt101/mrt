#ifndef _MRT_FILE_H_
#define _MRT_FILE_H_ 1

#include <exception>
#include <iostream>
#include <cstdint>
#include <cstdio>

#include <mrt/strutils.h>
#include <mrt/format.h>
#include <mrt/memory.h>

namespace mrt {
namespace io {

class FileNotExistsError : public std::exception {
  constexpr static size_t kBufferSize = 256;
  std::string m_file;

 public:
  inline FileNotExistsError(const std::string& file) : m_file(file) {}

  inline std::string getFile() const {
    return m_file;
  }

  inline const char* what() const throw() {
    char* buffer = new char[kBufferSize];
    snprintf(buffer, kBufferSize, "File '%s' doesn't exist", m_file.c_str());
    return buffer;
  }
};

class File {
  constexpr static size_t kBufferSize = 256;

 private:
  std::string m_filename;
  FILE* m_file = nullptr;
  bool m_close = true;

 public:
  inline File() {}

  inline File(FILE* file, bool close = true) {
    m_file = file;
    if (m_file) {
      m_close = close;
    }
  }

  inline File(const char* filename, const std::string& mode = "r") {
    this->open(filename, mode);
  }

  inline File(const std::string& filename, const std::string& mode = "r") {
    this->open(filename, mode);
  }

  inline File(const File& file) = delete;

  inline ~File() {
    if (m_close) {
      this->close();
    }
  }

  inline void open(const std::string& filename, const std::string& mode = "r") {
    this->close();
    m_filename = filename;
    m_file = fopen(filename.c_str(), mode.c_str());
    if (!m_file) {
      throw FileNotExistsError(filename);
    }
  }

  inline void close() {
    if (m_file) {
      fclose(m_file);
      m_file = nullptr;
    }
    m_filename = "";
  }

  inline size_t countSize() const {
    fseek(m_file, 0, SEEK_END);
    size_t m_size = ftell(m_file);
    rewind(m_file);
    return m_size;
  }

  inline void setClose(bool close) {
    m_close = close;
  }

  inline FILE* getHandle() {
    return m_file;
  }

  inline std::string read(size_t size = 0) {
    std::string str;
    if (size > 0) {
      char* buffer = new char[size];
      fread(buffer, 1, size, m_file);
      buffer[size-1] = 0;
      str += buffer;
      delete [] buffer;
    } else {
      int c;
      while ((c = fgetc(m_file)) != EOF) {
        str += c;
      }
    }
    return str;
  }

  inline bool readLine(std::string& res) {
    std::string result;
    int ch = 0;
    while (ch != EOF && ch != '\n') {
      ch = fgetc(m_file);
      result += ch;
    }
    result.pop_back();
    res = result;
    return !(ch == EOF && result.size() == 0);
  }

  inline std::vector<std::string> readLines() {
    std::string buffer = this->read();
    return str::split(buffer, "\n");
  }

  inline void write(const std::string& str) {
    fwrite(str.c_str(), 1, str.size(), m_file);
  }

  inline void writeln(const std::string& str) {
    this->write(str);
    this->write("\n");
  }

  template <typename... Args>
  inline void writef(const std::string& fmt, Args&&... args) {
    this->write(format(fmt, args...));
  }

  // inline std::vector<uint8_t> readBytes() {} // TODO:
  // inline void writeBytes(const std::vector<uint8_t>& bytes) {} // TODO:

};

} /* namespace io */

using File = io::File;

} /* namespace mrt */

#endif
