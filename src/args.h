#ifndef _MRT_ARGS_H_
#define _MRT_ARGS_H_ 1

#include <initializer_list>
#include <functional>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>

#include <mrt/enum.h>

namespace mrt {
namespace args {

/* Represents type of command line option */
enum OptionType {
  FLAG,
  WITH_VALUE,
  POSITIONAL,
};

struct Option {
  std::string name;
  OptionType type;
  std::vector<std::string> options;
  std::string helpString;
  bool many = false; // for POSITIONAL type

  inline Option(const std::string& name, OptionType type,
         const std::vector<std::string>& options,
         const std::string& helpString, bool many = false) 
    : name(name), type(type), options(options), helpString(helpString), many(many) {}
};

class ParserResult {
  friend class Parser;

public:
  using callback_type = std::function<void(const std::vector<std::string>&)>;

 public:
  inline bool exists(const std::string& optionName) const {
    return m_parsed.find(optionName) != m_parsed.end();
  }

  inline void ifExists(const std::string& optionName, callback_type cb) const {
    if (exists(optionName)) {
      cb(get(optionName));
    }
  }

  inline const std::vector<std::string>& get(const std::string& optionName) const {
    return m_parsed.at(optionName);
  }

  inline const std::vector<std::string>& getOr(const std::string& optionName, const std::vector<std::string>& defaultValue) const {
    if (exists(optionName)) {
      return get(optionName);
    }
    return defaultValue;
  }

  inline const std::string& getFirst(const std::string& optionName) const {
    return get(optionName)[0];
  }

  inline const std::string& getFirstOr(const std::string& optionName, const std::string& defaultValue) const {
    if (exists(optionName)) {
      return get(optionName)[0];
    }
    return defaultValue;
  }

  inline bool hasUnrecognizedParams() const {
    return !m_unrecognizedParams.empty();
  }

  inline std::vector<std::string>& getUnrecognizedParams() {
    return m_unrecognizedParams;
  }

 private:
  int m_positionalCount = 0;
  std::map<std::string, std::vector<std::string>> m_parsed; // Stores parsed options
  std::vector<std::string> m_unrecognizedParams;           // Stores parsed unregistered parameters
};

class Parser {
 public:
  inline Parser(const std::string& help_string) : m_helpString(help_string) {}
  inline Parser(const std::string& help_string, const std::initializer_list<Option>& il) : m_helpString(help_string) {
    for (auto& opt : il) {
      if (opt.type == OptionType::POSITIONAL) {
        m_positional.push_back(opt);
      } else {
        m_options.push_back(opt);
      }
    }
  }

  inline void addOption(const Option& option) {
    if (option.type == OptionType::POSITIONAL) {
      m_positional.push_back(option);
    } else {
      m_options.push_back(option);
    }
  }

  inline ParserResult parse(int argc, const char ** argv) {
    ParserResult result;
    for (int i = 1; i < argc; ++i) {
      if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
        std::cout << m_helpString << std::endl
                  << "Usage: " << argv[0] << " [OPTIONS] ";
        for (auto& opt : m_positional) {
          std::cout << "[" << opt.name;
          if (opt.many) {
            std::cout << "...";
          }
          std::cout << "] ";
        }
        addOption({"help", OptionType::FLAG, {"-h", "--help"}, "Shows this help message",});

        int maxOptionLength = 0;
        std::vector<std::pair<std::string, std::string>> optionsToPrint;
        std::vector<std::pair<std::string, std::string>> positionalsToPrint;

        for (auto& opt : m_options) {
          std::string names;
          for (int i = 0; i < opt.options.size(); i++) {
            names += opt.options[i];
            if (i+1 != opt.options.size()) {
              names += ", ";
            }
          }
          maxOptionLength = std::max(maxOptionLength, (int)names.size());
          optionsToPrint.push_back({names, opt.helpString});
        }

        for (auto& opt : m_positional) {
          maxOptionLength = std::max(maxOptionLength, (int)opt.name.size());
          positionalsToPrint.push_back({opt.name, opt.helpString});
        }
        
        std::cout << "\nOptions:\n";
        for (auto& opt : optionsToPrint) {
          std::cout << "\t" << std::left
                    << std::setw(maxOptionLength) << opt.first
                    << " - " << opt.second << std::endl;
        }

        if (m_positional.size()) std::cout << "\nPositoinal:\n";
        for (auto& opt : positionalsToPrint) {
          std::cout << "\t" << std::left
                    << std::setw(maxOptionLength) << opt.first
                    << " - " << opt.second << std::endl;
        }
        exit(EXIT_SUCCESS);
      } else {
        auto it = std::find_if(m_options.begin(), m_options.end(),
          [&](auto& option) {
            return std::find(option.options.begin(), option.options.end(), argv[i]) != option.options.end();
          });
        if (it != m_options.end()) { // FLAG & WITH_VALUE
          Option& option = *it;

          if (result.exists(option.name) && !option.many) {
            std::cout << "Error: option '" << argv[i] << "' can be passed only once\n";
            exit(EXIT_FAILURE);
          }

          if (option.type == OptionType::FLAG) {
            result.m_parsed[option.name].push_back("");
          } else if (option.type == OptionType::WITH_VALUE) {
            if (i+1 >= argc) {
              throw std::invalid_argument("No value supplied for option '" + option.name + "'");
            } else {
              result.m_parsed[option.name].push_back(argv[++i]);
            }
          }
        } else  {
          if (result.m_positionalCount < m_positional.size()) { // POSITIONAL
            Option& option = m_positional[result.m_positionalCount++];
            result.m_parsed[option.name].push_back(argv[i]);
          } else if (m_positional.size() && m_positional.back().many) {
            Option& option = m_positional.back();
            result.m_parsed[option.name].push_back(argv[i]);
          } else { // Unrecognized parameters end up here
            result.m_unrecognizedParams.push_back(argv[i]);
          }
        }
      }
    }
    return result;
  }

 private:
  std::string m_helpString;
  std::vector<Option> m_options;                             // Stores registered options
  std::vector<Option> m_positional;
};

} // namespace args
} // namespace mrt

#endif
