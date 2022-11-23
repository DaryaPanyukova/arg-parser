#pragma once

#include <string>
#include <map>
#include <vector>

namespace ArgumentParser {
    struct Argument {
        // TODO: values and values_to_store are the same
        std::vector<int> values;

        // MultiValue
        bool multi_value;
        size_t min_args_count;

        // StoreValue(s)
        std::vector<int>* values_to_store;
        int* value_to_store;

        // Positional
        bool positional;

        Argument() {
            multi_value = false;
            min_args_count = 1;

            value_to_store = nullptr;
            value_to_store = nullptr;
            positional = false;

        }

        Argument& MultiValue(size_t min_args_count0 = 2) {
            multi_value = true;
            min_args_count = min_args_count0;
            return *this;
        }

        Argument& StoreValue(int& values) {
            value_to_store = &values;
            return *this;
        }

        Argument& StoreValues(std::vector<int>& values) {
            values_to_store = &values;
            return *this;
        }

        Argument& Positional() {
            positional = true;
            return *this;
        }
    };

    class ArgParser {
    private:
        const char* name_;

        std::map<std::string, Argument*> arg_names;

    public:
        ArgParser(const char* name);

        bool AddArgValue(std::string& name, std::string& str_value);

        bool ParseArg(std::string& arg);

        bool Parse(std::vector<std::string> argv);

        Argument& AddIntArgument(const char* full_name);

        Argument& AddIntArgument(char short_name, const char* full_name);

        int32_t GetIntValue(const char* arg_name, uint32_t index = 0);

        bool IsInitialised();
    };

} // namespace ArgumentParser