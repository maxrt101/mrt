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

class Option {
 public:
  std::string name;
  OptionType type;
  std::vector<std::string> options;
  std::string help_string;
  bool optional = true;
 
 public:
  Option(const std::string& name, OptionType type,
         const std::vector<std::string>& options,
         const std::string& help_string, bool optional = true);
};

class ParserResult {
 friend class Parser;
 public:
  using callback_type = std::function<void(const std::vector<std::string>&)>;

 private:
  std::map<std::string, std::vector<std::string>> parsed_;  // Stores parsed options
  std::vector<std::string> free_params_;                    // Stores parsed unregistered parameters

 public:
  bool Exists(const std::string& option_name) const;
  void IfExists(const std::string& option_name, callback_type cb) const;
  const std::vector<std::string>& Get(const std::string& option_name) const;
  bool HasFreeParams() const;
  std::vector<std::string>& GetFreeParams();
};

class Parser {
 public:
  Parser(const std::string& help_string);
  Parser(const std::string& help_string, const std::initializer_list<Option>& il);

  void AddOption(const Option& option);

  ParserResult Parse(int argc, const char ** argv);

 private:
  std::string help_string_;
  std::vector<Option> options_;                             // Stores registered options
};

} // namespace args
} // namespace mrt

#endif
