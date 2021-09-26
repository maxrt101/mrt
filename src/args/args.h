#ifndef _MRT_ARGS_H_
#define _MRT_ARGS_H_ 1

#include <initializer_list>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>

#include <mrt/enum.h>

namespace mrt {
namespace args {

/* Represents type of command line option */
MRT_DEF_ENUM(OptionType, FLAG, WITH_VALUE, POSITIONAL)
 public:
  inline OptionType(char c) {
    switch (c) {
      case 'F': m_value = OptionType::FLAG.m_value; break;
      case 'V': m_value = OptionType::WITH_VALUE.m_value; break;
      case 'P': m_value = OptionType::POSITIONAL.m_value; break;
      default:
        throw std::invalid_argument("Invalid option char");
    }
  }
};

struct Option {
  std::string name;
  OptionType type;
  std::vector<std::string> options;
  std::string help_string;
  bool many = false; // for POSITIONAL type

  Option(const std::string& name, OptionType type,
         const std::vector<std::string>& options,
         const std::string& help_string, bool many = false);
};

class ParserResult {
  friend class Parser;

public:
  using callback_type = std::function<void(const std::vector<std::string>&)>;

 public:
  bool exists(const std::string& option_name) const;
  void ifExists(const std::string& option_name, callback_type cb) const;
  const std::vector<std::string>& get(const std::string& option_name) const;
  const std::vector<std::string>& getOr(const std::string& option_name,
                                        const std::vector<std::string>& default_value) const;
  const std::string& getFirst(const std::string& option_name) const;
  const std::string& getFirstOr(const std::string& option_name,
                                const std::string& default_value) const;
  bool hasUnrecognizedParams() const;
  std::vector<std::string>& getUnrecognizedParams();
 
 private:
  int m_positional_count = 0;
  std::map<std::string, std::vector<std::string>> m_parsed; // Stores parsed options
  std::vector<std::string> m_unrecognized_params;           // Stores parsed unregistered parameters
};

class Parser {
 public:
  Parser(const std::string& help_string);
  Parser(const std::string& help_string, const std::initializer_list<Option>& il);

  void addOption(const Option& option);

  ParserResult parse(int argc, const char ** argv);

 private:
  std::string m_help_string;
  std::vector<Option> m_options;                             // Stores registered options
  std::vector<Option> m_positional;
};

} // namespace args
} // namespace mrt

#endif
