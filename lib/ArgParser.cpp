#include <vector>
#include "ArgParser.h"

ArgumentParser::ArgParser::ArgParser(const char* name) {
    name_ = name;
}

bool is_number(std::string& str) {
    size_t i = 0;
    for (; i < str.size() && str[i] == '-'; ++i);
    if (i == str.size()) {
        return false;
    }

    for (auto c: str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

bool ArgumentParser::ArgParser::AddArgValue(std::string& name,
                                            std::string& str_value) {
    Argument* argument = arg_names[name];
    int32_t value = atoi(str_value.c_str());

    if (!argument->multi_value && argument->values.size() != 0) {
        return false;
    } else {
        argument->values.push_back(value);
        if (argument->value_to_store != nullptr) {
            argument->value_to_store = &value;
        } else if (argument->values_to_store != nullptr) {
            argument->values_to_store->push_back(value);
        }
        return true;
    }
}


bool ArgumentParser::ArgParser::ParseArg(std::string& arg) {
    // TODO: flags
    // TODO: string arguments can also be positional

    if (is_number(arg)) {
        // positional
        Argument* positional_arg = nullptr;
        std::string name;
        for (auto elem: arg_names) {
            if (elem.second->positional) {
                if (positional_arg == elem.second) {
                    continue;
                } else if (positional_arg == nullptr) {
                    positional_arg = elem.second;
                    name = elem.first;
                } else {
                    return false;
                }
            }
        }
        return AddArgValue(name, arg);
    } else {
        size_t find_eq = arg.find('=');

        if (find_eq != std::string::npos) {
            std::string arg_name;

            std::string name = arg.substr(0, find_eq);
            std::string value = arg.substr(find_eq + 1, arg.size());
            if (name.size() == 1 || value.size() == 0) {
                return false;
            } else if (name.size() == 2) {
                if (name[0] == '-') {
                    arg_name = name[1];
                } else {
                    return false;
                }
            } else {
                if (name[0] == name[1] && name[1] == '-') {
                    arg_name = name.substr(2, arg.size()).c_str();
                } else {
                    return false;
                }
            }

            return AddArgValue(arg_name, value);
        } else {
            return false;
        }
    }
}

bool ArgumentParser::ArgParser::Parse(std::vector<std::string> argv) {
    for (size_t i = 1; i < argv.size(); ++i) {
        std::string arg = argv[i];
        if (!ParseArg(arg)) {
            return false;
        }
    }
    return IsInitialised();
}

ArgumentParser::Argument&
ArgumentParser::ArgParser::AddIntArgument(const char* full_name) {
    Argument* new_arg = new Argument;
    arg_names.insert(std::make_pair(full_name, new_arg));
    return *new_arg;
}

ArgumentParser::Argument&
ArgumentParser::ArgParser::AddIntArgument(char short_name,
                                          const char* full_name) {
    Argument* new_arg = new Argument;
    arg_names.insert(std::make_pair(full_name, new_arg));
    arg_names.insert(std::make_pair(std::string(1, short_name), new_arg));
    return *new_arg;
}

int32_t
ArgumentParser::ArgParser::GetIntValue(const char* arg_name, uint32_t index) {
    return arg_names[arg_name]->values[index];
}

bool ArgumentParser::ArgParser::IsInitialised() {
    for (auto elem: arg_names) {
        if (elem.second->values.size() < elem.second->min_args_count) {
            return false;
        }
    }
    return true;
}


