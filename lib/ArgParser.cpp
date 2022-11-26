#include <vector>
#include <iostream>
#include "ArgParser.h"

ArgumentParser::ArgParser::ArgParser(const char* name) {
    name_ = name;
    call_help_ = false;
    has_help_ = false;
    positional_int_ = nullptr;
    positional_str_ = nullptr;
}

void ArgumentParser::ArgParser::AddHelp(char short_name,
                                        const std::string& full_name,
                                        const std::string& text) {
    help_short_name_ = short_name;
    help_full_name_ = full_name;
    help_text_ = text;
    has_help_ = true;
}

bool ArgumentParser::ArgParser::IntArgsInitialised() {
    for (const auto& elem: int_arg_names_) {
        if (!elem.second->IsInitialised()) {
            return false;
        }
    }
    return true;
}

bool ArgumentParser::ArgParser::StrArgsInitialised() {
    for (const auto& elem: str_arg_names_) {
        if (!elem.second->IsInitialised()) {
            return false;
        }
    }
    return true;
}

bool ArgumentParser::ArgParser::BoolArgsInitialised() {
    for (const auto& elem: bool_arg_names_) {
        if (!elem.second->IsInitialised()) {
            return false;
        }
    }
    return true;
}


bool ArgumentParser::ArgParser::IsInitialised() {
    return IntArgsInitialised() && StrArgsInitialised() &&
           BoolArgsInitialised();
}

bool is_number(std::string& str) {
    size_t i = 0;
    for (; i < str.size() && str[i] == '-'; ++i);
    if (i == str.size()) {
        return false;
    }

    for (const auto& c: str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}


// TODO: value and value_to_store - объединить

// TODO: last test


bool ArgumentParser::ArgParser::AddIntArgValue(const std::string& name,
                                               const std::string& str_value) {
    auto argument = int_arg_names_[name];
    int32_t value = stoi(str_value);
    return argument->AddValue(value);
}

bool ArgumentParser::ArgParser::AddStrArgValue(const std::string& name,
                                               const std::string& value) {
    auto argument = str_arg_names_[name];
    return argument->AddValue(value);
}

bool ArgumentParser::ArgParser::AddArgValue(const std::string& name,
                                            const std::string& str_value) {
    if (int_arg_names_.count(name)) {
        return AddIntArgValue(name, str_value);
    } else if (str_arg_names_.count(name)) {
        return AddStrArgValue(name, str_value);
    }
    return false;
}

bool ArgumentParser::ArgParser::AddBoolArgValue(const std::string& name) {
    if (bool_arg_names_.count(name) == 0) {
        return false;
    }
    auto argument = bool_arg_names_[name];
    argument->AddValue(true);
    return true;
}


bool ArgumentParser::ArgParser::ParseArg(const std::string& arg) {
    if (has_help_) {
        std::string tmp1 = "-" + std::string(help_short_name_, 1);
        std::string tmp2 = "--" + help_full_name_;
        if (arg == tmp1 || arg == tmp2) {
            call_help_ = true;
            return true;
        }
    }

    if (arg[0] != '-') {
        // positional
        if (positional_int_ != nullptr) {
            return positional_int_->AddValue(stoi(arg));
        } else if (positional_str_ != nullptr) {
            return positional_str_->AddValue(arg);
        } else {
            return false;
        }
    } else {
        size_t eq_ind = arg.find('=');
        if (eq_ind != std::string::npos) {
            // str or int argument

            if (eq_ind == arg.size() - 1) {
                return false;
            }

            std::string arg_name;
            std::string name = arg.substr(0, eq_ind);
            std::string value = arg.substr(eq_ind + 1, arg.size());

            if (name.size() < 2) {
                return false;
            } else if (name.size() == 2) {
                arg_name = name[1];
            } else if (name[1] == '-') {
                arg_name = name.substr(2, arg.size()).c_str();
            } else {
                return false;
            }
            return AddArgValue(arg_name, value);
        } else {
            // flag
            if (arg.size() < 2) {
                return false;
            } else if (arg[1] == '-') {
                // long name
                std::string name = arg.substr(2, arg.size()).c_str();
                return AddBoolArgValue(name);
            } else {
                // short name
                for (size_t i = 1; i < arg.size(); ++i) {
                    if (!AddBoolArgValue(std::string(1, arg[i]))) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}


bool ArgumentParser::ArgParser::Parse(const std::vector<std::string>& argv) {
    GetPositional();
    for (size_t i = 1; i < argv.size(); ++i) {
        std::string arg = argv[i];
        if (!ParseArg(arg)) {
            return false;
        }
    }
    return IsInitialised();
}


Argument<int32_t>&
ArgumentParser::ArgParser::AddIntArgument(const char* full_name) {
    auto new_arg = std::make_shared<Argument<int32_t>>();
    int_arg_names_.insert(std::make_pair(full_name, new_arg));
    return *new_arg;
}

Argument<int32_t>&
ArgumentParser::ArgParser::AddIntArgument(char short_name,
                                          const char* full_name) {
    auto new_arg = std::make_shared<Argument<int32_t>>();
    int_arg_names_.insert(std::make_pair(full_name, new_arg));
    int_arg_names_.insert(std::make_pair(std::string(1, short_name), std::shared_ptr(new_arg)));
    return *new_arg;
}

int32_t
ArgumentParser::ArgParser::GetIntValue(const char* arg_name, uint32_t index) {
    return int_arg_names_[arg_name]->GetValue(index);
}

Argument<std::string>&
ArgumentParser::ArgParser::AddStringArgument(const char* full_name) {
    auto new_arg = std::make_shared<Argument<std::string>>();
    str_arg_names_.insert(std::make_pair(full_name, new_arg));
    return *new_arg;
}

Argument<std::string>&
ArgumentParser::ArgParser::AddStringArgument(char short_name,
                                             const char* full_name) {
    auto new_arg = std::make_shared<Argument<std::string>>();
    str_arg_names_.insert(std::make_pair(full_name, new_arg));
    str_arg_names_.insert(std::make_pair(std::string(1, short_name), std::shared_ptr(new_arg)));
    return *new_arg;
}

std::string ArgumentParser::ArgParser::GetStringValue(const char* arg_name,

                                                      uint32_t index) {
    return str_arg_names_[arg_name]->GetValue(index);
}
BoolArgument&
ArgumentParser::ArgParser::AddFlag(const char* full_name) {
    auto new_arg =  std::make_shared<BoolArgument>();
    bool_arg_names_.insert(std::make_pair(full_name, new_arg));
    return *new_arg;
}

BoolArgument&
ArgumentParser::ArgParser::AddFlag(char short_name, const char* full_name) {
    auto new_arg =  std::make_shared<BoolArgument>();
    bool_arg_names_.insert(std::make_pair(full_name, new_arg));
    bool_arg_names_.insert(std::make_pair(std::string(1, short_name), std::shared_ptr(new_arg)));
    return *new_arg;
}


bool ArgumentParser::ArgParser::Help() {
    std::cout << help_text_;
    return call_help_;
}

void ArgumentParser::ArgParser::GetPositional() {
    for (const auto& elem: int_arg_names_) {
        if (elem.second->IsPositional()) {
            if (positional_int_ != nullptr) {
                std::cerr << "Only one argument can be positional" << '\n';
            }
            positional_int_ = elem.second.get();
        }
    }

    for (const auto& elem: str_arg_names_) {
        if (elem.second->IsPositional()) {
            if (positional_str_ != nullptr || positional_int_ != nullptr) {
                std::cerr << "Only one argument can be positional" << '\n';
            }
            positional_str_ = elem.second.get();
        }
    }
}


bool ArgumentParser::ArgParser::GetFlag(const char* arg_name) {
    return bool_arg_names_[arg_name]->GetValue();
}

