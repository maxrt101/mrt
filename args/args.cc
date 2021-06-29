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



bool mrt::args::ParserResult::exists(const std::string& option_name) const {
  return m_parsed.find(option_name) != m_parsed.end();
}


void mrt::args::ParserResult::ifExists(const std::string& option_name, callback_type cb) const {
  if (exists(option_name)) {
    cb(get(option_name));
  }
}


const std::vector<std::string>& mrt::args::ParserResult::get(const std::string& option_name) const {
  return m_parsed.at(option_name);
}


bool mrt::args::ParserResult::hasFreeParams() const {
  return !m_free_params.empty();
}


std::vector<std::string>& mrt::args::ParserResult::getFreeParams() {
  return m_free_params;
}


mrt::args::Parser::Parser(const std::string& help_string) : m_help_string(help_string) {}


mrt::args::Parser::Parser(const std::string& help_string, const std::initializer_list<Option>& il)
    : m_help_string(help_string) {
  for (auto& opt : il) {
    m_options.push_back(opt);
  }  
}


void mrt::args::Parser::addOption(const Option& option) {
  m_options.push_back(option);
}


mrt::args::ParserResult mrt::args::Parser::parse(int argc, const char ** argv) {
  ParserResult result;
  for (int i = 1; i < argc; ++i) {
    if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
      std::cout << m_help_string << std::endl
                << "Usage: " << argv[0] << " [OPTIONS]\n"
                << "Options:\n";
      // AddOption(OptionType::kFlag, "Shows this help message", {"-h", "--help"});
      int max_option_length = 0;
      std::vector<std::pair<std::string, std::string>> m_optionsto_print;

      for (auto& opt : m_options) {
        std::string names;
        for (int i = 0; i < opt.options.size(); i++) {
          names += opt.options[i];
          if (i+1 != opt.options.size()) {
            names += ", ";
          }
        }
        max_option_length = std::max(max_option_length, (int)names.size());
        m_optionsto_print.push_back({names, opt.help_string});
      }
      
      for (auto& opt : m_optionsto_print) {
        std::cout << "\t" << std::left
                  << std::setw(max_option_length) << opt.first
                  << " - " << opt.second << std::endl;
      }
      exit(EXIT_SUCCESS);
    } else {
      
      auto it = std::find_if(m_options.begin(), m_options.end(),
          [&](auto& option) {
            return std::find(option.options.begin(), option.options.end(), argv[i]) != option.options.end();
          });
      if (it != m_options.end()) {
        Option& option = *it;

        if (option.type == OptionType::kFlag) {
          result.m_parsed[option.name].push_back("");
        } else if (option.type == OptionType::kWithValue) {
          if (i+1 >= argc) {
            std::cerr << "Error: Expected argument\n";
            exit(EXIT_FAILURE);
          }
          result.m_parsed[option.name].push_back(argv[++i]);
        }
      } else {
        result.m_free_params.push_back(argv[i]);
      }
    }
  }
  return result;
}

