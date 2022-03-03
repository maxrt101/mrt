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
  size_t m_size = 0;

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
    fseek(m_file, 0, SEEK_END);
    m_size = ftell(m_file);
    rewind(m_file);
  }

  inline void close() {
    if (m_file) {
      fclose(m_file);
      m_file = nullptr;
    }
    m_filename = "";
    m_size = 0;
  }

  inline size_t size() const {
    return m_size;
  }

  inline void setClose(bool close) {
    m_close = close;
  }

  inline FILE* getHandle() {
    return m_file;
  }

  inline char* read(size_t size = 0) {
    size = size ? size : this->size();
    char* buffer = new char[size];
    fread(buffer, 1, size, m_file);
    buffer[size-1] = 0;
    return buffer;
  }

  inline std::string readString(size_t size = 0) {
    size = size ? size : this->size();
    char* cstr = this->read(size);
    std::string str = cstr;
    delete [] cstr;
    return str;
  }

  inline bool readLine(std::string& res) {
    size_t bufferSize = kBufferSize;
    char* buffer = new char[bufferSize];
    size_t size = 0;
    int ch = 0;
    do {
      if (size >= bufferSize) {
        bufferSize *= 2;
        buffer = mem::realloc(buffer, bufferSize/2, bufferSize);
      }
      ch = fgetc(m_file);
      buffer[size++] = ch;
    } while (ch != EOF && ch != '\n');
    if (ch == EOF) {
      delete [] buffer;
      return false;
    }
    buffer[size-1] = '\0';
    res = std::string(buffer);
    delete [] buffer;
    return true;
  }

  inline std::vector<std::string> readLines() {
    std::string buffer = this->readString();
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

} /* namespace mrt */

#endif
