#include "args/args.h"

#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <algorithm>

mrt::args::Option::Option(const std::string& name,
                          OptionType type,
                          const std::vector<std::string>& options,
                          const std::string& help_string,
                          bool optional)
    : name(name), type(type), options(options), help_string(help_string), optional(optional) {}



bool mrt::args::ParserResult::Exists(const std::string& option_name) const {
  return parsed_.find(option_name) != parsed_.end();
}


void mrt::args::ParserResult::IfExists(const std::string& option_name, callback_type cb) const {
  if (Exists(option_name)) {
    cb(Get(option_name));
  }
}


const std::vector<std::string>& mrt::args::ParserResult::Get(const std::string& option_name) const {
  return parsed_.at(option_name);
}


bool mrt::args::ParserResult::HasFreeParams() const {
  return !free_params_.empty();
}


std::vector<std::string>& mrt::args::ParserResult::GetFreeParams() {
  return free_params_;
}


mrt::args::Parser::Parser(const std::string& help_string) : help_string_(help_string) {}



mrt::args::Parser::Parser(const std::string& help_string, const std::initializer_list<Option>& il)
    : help_string_(help_string) {
  for (auto& opt : il) {
    options_.push_back(opt);
  }  
}



void mrt::args::Parser::AddOption(const Option& option) {
  options_.push_back(option);
}


mrt::args::ParserResult mrt::args::Parser::Parse(int argc, const char ** argv) {
  ParserResult result;
  for (int i = 1; i < argc; ++i) {
    if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
      std::cout << help_string_ << std::endl
                << "Usage: " << argv[0] << " [OPTIONS]\n"
                << "Options:\n";
      // AddOption(OptionType::kFlag, "Shows this help message", {"-h", "--help"});
      int max_option_length = 0;
      std::vector<std::pair<std::string, std::string>> options_to_print;

      for (auto& opt : options_) {
        std::string names;
        for (int i = 0; i < opt.options.size(); i++) {
          names += opt.options[i];
          if (i+1 != opt.options.size()) {
            names += ", ";
          }
        }
        max_option_length = std::max(max_option_length, (int)names.size());
        options_to_print.push_back({names, opt.help_string});
      }
      
      for (auto& opt : options_to_print) {
        std::cout << "\t" << std::left
                  << std::setw(max_option_length) << opt.first
                  << " - " << opt.second << std::endl;
      }
      exit(EXIT_SUCCESS);
    } else {
      
      auto it = std::find_if(options_.begin(), options_.end(),
          [&](auto& option) {
            return std::find(option.options.begin(), option.options.end(), argv[i]) != option.options.end();
          });
      if (it != options_.end()) {
        Option& option = *it;

        if (option.type == OptionType::kFlag) {
          result.parsed_[option.name].push_back("");
        } else if (option.type == OptionType::kWithValue) {
          if (i+1 >= argc) {
            std::cerr << "Error: Expected argument\n";
            exit(EXIT_FAILURE);
          }
          result.parsed_[option.name].push_back(argv[++i]);
        }
      } else {
        result.free_params_.push_back(argv[i]);
      }
    }
  }
  return result;
}

