#ifndef _MRT_CONFIG_H_
#define _MRT_CONFIG_H_ 1

#include <functional>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include <mrt/strutils.h>
#include <mrt/log.h>

namespace mrt {

class Config {
 public:
  using Section = std::map<std::string, std::string>;

 private:
  mutable std::map<std::string, Section> m_sections;

 public:
  inline Config() = default;
  inline ~Config() = default;

  inline bool exists(const std::string& section, const std::string& key) const {
    if (m_sections.find(section) != m_sections.end()) {
      if (m_sections[section].find(key) != m_sections[section].end()) {
        return true;
      }
    }
    return false;
  }

  inline void ifExists(const std::string& section, const std::string& key, std::function<void(const std::string&)> cb) const {
    if (m_sections.find(section) != m_sections.end()) {
      auto itr = m_sections[section].find(key);
      if (itr != m_sections[section].end()) {
        cb(itr->second);
      }
    }
  }

  inline Section& getSection(const std::string& section) const {
    return m_sections[section];
  }

  inline std::string getValue(const std::string& section, const std::string& key) const {
    return m_sections[section][key];
  }

  inline std::string getValueOr(const std::string& section, const std::string& key, const std::string& defaultValue) const {
    if (m_sections.find(section) != m_sections.end()) {
      if (m_sections[section].find(key) != m_sections[section].end()) {
        return m_sections[section][key];
      }
    }
    return defaultValue;
  }

  inline void setValue(const std::string& section, const std::string& key, const std::string& value) {
    m_sections[section][key] = value;
  }

  inline Section& operator[](const std::string& section) {
    return m_sections[section];
  }

  inline std::string toString() const {
    std::stringstream ss;
    for (auto& [sectionName, section] : m_sections) {
      ss << "[" << sectionName << "]\n";
      for (auto& [key, value] : section) {
        ss << key << " = " << value << "\n";
      }
      ss << "\n";
    }
    return ss.str();
  }

  inline void toFile(const std::string& fileName) const {
    std::ofstream file(fileName);
    if (!file) {
      #ifndef MRT_NO_ERROR_LOG
        fatal("Can't open file '%s'", fileName.c_str());
      #endif
      #ifndef MRT_NO_THROW
        throw "Can't open file";
      #else
        return;
      #endif
    }
    file << toString();
  }

  inline static Config fromFile(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file) {
      #ifndef MRT_NO_ERROR_LOG
        fatal("Can't open file '%s'", fileName.c_str());
      #endif
      #ifndef MRT_NO_THROW
        throw "Can't open file";
      #else
        return;
      #endif
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return Config::fromString(ss.str());
  }

  inline static Config fromString(const std::string& src) {
    std::vector<std::string> lines = str::split(src, "\n");
    std::string currentSection;
    Config config;

    int lineNumber = 0;
    for (auto& line : lines) {
      lineNumber++;
      if (!line.size() || line.front() == '#') continue;

      if (line.front() == '[') {
        if (line.back() == ']') {
          currentSection = line.substr(1, line.size()-2);
        } else {
          #ifndef MRT_NO_ERROR_LOG
            fatal("Config Parsing Error:\n    Near '%s' (line %d)\n    Expected ']' at the end of section name", line.c_str(), lineNumber);
          #endif
          #ifndef MRT_NO_THROW
            throw "Parsing Error: Expected ']' at the end of section name";
          #else
            return;
          #endif
        }
      } else {
        int i = line.find(" = ");
        if (i != std::string::npos) {
          std::string key = line.substr(0, i), value = line.substr(i+3);
          config.setValue(currentSection, key, value);
        } else {
          #ifndef MRT_NO_ERROR_LOG
            fatal("Config Parsing Error:\n    Near '%s' (line %d)\n    Expected key-value pair", line.c_str(), lineNumber);
          #endif
          #ifndef MRT_NO_THROW
            throw "Parsing Error: Expected key-value pair";
          #else
            return;
          #endif
        }
      }
    }
    return config;
  }
};

} /* namespace mrt */

#endif
