#ifndef MRT_ARGS_H_
#define MRT_ARGS_H_ 1

#include <initializer_list>
#include <functional>
#include <string>
#include <vector>
#include <map>

namespace mrt {
namespace args {

/* Represents type of command line option */
enum class OptionType {
  kFlag,
  kWithValue,
};

struct Option {
  std::string name;
  OptionType type;
  std::vector<std::string> options;
  std::string help_string;
  bool optional = true;

  Option(const std::string& name, OptionType type,
         const std::vector<std::string>& options,
         const std::string& help_string, bool optional = true);
};

class ParserResult {
  friend class Parser;

public:
  using callback_type = std::function<void(const std::vector<std::string>&)>;

 public:
  bool exists(const std::string& option_name) const;
  void ifExists(const std::string& option_name, callback_type cb) const;
  const std::vector<std::string>& get(const std::string& option_name) const;
  bool hasFreeParams() const;
  std::vector<std::string>& getFreeParams();
 
 private:
  std::map<std::string, std::vector<std::string>> m_parsed;  // Stores parsed options
  std::vector<std::string> m_free_params;                    // Stores parsed unregistered parameters

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
};

} // namespace args
} // namespace mrt

#endif
