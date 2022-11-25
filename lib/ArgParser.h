#pragma once

#include <string>
#include <map>
#include <vector>

namespace ArgumentParser {
    template <typename Type>
    struct Argument {
        std::vector<Type> values;

        // MultiValue
        bool multi_value;
        size_t min_args_count;

        // StoreValue(s)
        std::vector<Type>* values_to_store;
        Type* value_to_store;

        // Positional
        bool positional;

        Argument() {
            multi_value = false;
            min_args_count = 1;

            values_to_store = nullptr;
            value_to_store = nullptr;
            positional = false;

        }

        Argument& MultiValue(size_t min_args_count0 = 2) {
            multi_value = true;
            min_args_count = min_args_count0;
            return *this;
        }

        Argument& StoreValue(Type& value) {
            value_to_store = &value;
            return *this;
        }

        Argument& StoreValues(std::vector<Type>& values) {
            values_to_store = &values;
            return *this;
        }

        Argument& Positional() {
            positional = true;
            return *this;
        }
        Argument& Default(const Type& value) {
            values.push_back(value);
            return *this;
        }
    };

    struct BoolArgument{
        bool* value_to_store;
        bool value;
        bool is_initialised;

        BoolArgument& Default(bool value0) {
            value = value0;
            is_initialised = true;
            return *this;
        }
        BoolArgument& StoreValue(bool& value) {
            value_to_store = &value;
            return *this;
        }

        BoolArgument() {
            is_initialised = false;
            value_to_store = nullptr;
        }

    };

    class ArgParser {
    private:
        const char* name_;

        char help_short_name_;
        std::string help_full_name_;
        std::string help_text_;
        bool call_help_;
        bool has_help_;

        Argument <int32_t>* positional_int;
        Argument <std::string>* positional_str;


        std::map<std::string, Argument <int32_t>*> int_arg_names;
        std::map<std::string, Argument <std::string>*> str_arg_names;
        std::map<std::string, BoolArgument*> bool_arg_names;


    public:
        ArgParser(const char* name);

        void AddHelp(char short_name, const std::string& full_name, const std::string& text);

        bool Help();

        void GetPositional();

        bool BoolArgsInitialised();

        bool IntArgsInitialised();

        bool StrArgsInitialised();

        bool IsInitialised();

        template<typename Type>
        bool A(Argument <Type>* argument, const Type& value);

        bool AddBoolArgValue(const std::string& name);

        bool AddIntArgValue(std::string& name, std::string& str_value);

        bool AddStrArgValue(std::string& name, std::string& value);

        bool AddArgValue(std::string& name, std::string& str_value);

        bool ParseArg(std::string& arg);

        bool Parse(const std::vector<std::string>& argv);


        BoolArgument& AddFlag(const char* full_name);

        BoolArgument& AddFlag(char short_name, const char* full_name);

        bool GetFlag(const char* arg_name);

        Argument <int32_t>& AddIntArgument(const char* full_name);

        Argument <int32_t>& AddIntArgument(char short_name, const char* full_name);

        int32_t GetIntValue(const char* arg_name, uint32_t index = 0);

        Argument <std::string>& AddStringArgument(const char* full_name);

        Argument <std::string>& AddStringArgument(char short_name, const char* full_name);

        std::string GetStringValue(const char* arg_name, uint32_t index = 0);

    };

} // namespace ArgumentParser