#include <mrt/args/args.h>

#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <algorithm>

MRT_DEF_ENUM_FIELDS(mrt::args::OptionType,
  mrt::args::OptionType::FLAG(0),
  mrt::args::OptionType::WITH_VALUE(1),
  mrt::args::OptionType::POSITIONAL(2)
);

mrt::args::Option::Option(const std::string& name,
                          OptionType type,
                          const std::vector<std::string>& options,
                          const std::string& help_string,
                          bool many)
    : name(name), type(type), options(options), help_string(help_string), many(many) {}



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


bool mrt::args::ParserResult::hasUnrecognizedParams() const {
  return !m_unrecognized_params.empty();
}


std::vector<std::string>& mrt::args::ParserResult::getUnrecognizedParams() {
  return m_unrecognized_params;
}


mrt::args::Parser::Parser(const std::string& help_string) : m_help_string(help_string) {}


mrt::args::Parser::Parser(const std::string& help_string, const std::initializer_list<Option>& il)
    : m_help_string(help_string) {
  for (auto& opt : il) {
    if (opt.type == OptionType::POSITIONAL) {
      m_positional.push_back(opt);
    } else {
      m_options.push_back(opt);
    }
  }
}


void mrt::args::Parser::addOption(const Option& option) {
  if (option.type == OptionType::POSITIONAL) {
    m_positional.push_back(option);
  } else {
    m_options.push_back(option);
  }
}


mrt::args::ParserResult mrt::args::Parser::parse(int argc, const char ** argv) {
  ParserResult result;
  for (int i = 1; i < argc; ++i) {
    if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
      std::cout << m_help_string << std::endl
                << "Usage: " << argv[0] << " [OPTIONS] ";
      for (auto& opt : m_positional) {
        std::cout << "[" << opt.name;
        if (opt.many) {
          std::cout << "...";
        }
        std::cout << "] ";
      }
      addOption({"help", OptionType::FLAG, {"-h", "--help"}, "Shows this help message",});

      int max_option_length = 0;
      std::vector<std::pair<std::string, std::string>> options_to_print;
      std::vector<std::pair<std::string, std::string>> positionals_to_print;

      for (auto& opt : m_options) {
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

      for (auto& opt : m_positional) {
        max_option_length = std::max(max_option_length, (int)opt.name.size());
        options_to_print.push_back({opt.name, opt.help_string});
      }
      
      std::cout << "\nOptions:\n";
      for (auto& opt : options_to_print) {
        std::cout << "\t" << std::left
                  << std::setw(max_option_length) << opt.first
                  << " - " << opt.second << std::endl;
      }

      if (m_positional.size()) std::cout << "\nPositoinal:\n";
      for (auto& opt : positionals_to_print) {
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
      if (it != m_options.end()) { // FLAG & WITH_VALUE
        Option& option = *it;

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
        if (result.m_positional_count < m_positional.size()) { // POSITIONAL
          Option& option = m_positional[result.m_positional_count++];
          result.m_parsed[option.name].push_back(argv[i]);
        } else if (m_positional.back().many) {
          Option& option = m_positional.back();
          result.m_parsed[option.name].push_back(argv[i]);
        } else { // Unrecognized parameters end up here
          result.m_unrecognized_params.push_back(argv[i]);
        }
      }
    }
  }
  return result;
}

