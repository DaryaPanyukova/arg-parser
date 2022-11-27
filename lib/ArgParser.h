#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>

#include "argument.h"
#include "boolargument.h"

namespace ArgumentParser {

    class ArgParser {
    public:
        ArgParser(const char* name);

        void AddHelp(char short_name, const std::string& full_name,
                     const std::string& text);

        bool Help();

        void GetPositional();

        bool BoolArgsInitialised();

        bool IntArgsInitialised();

        bool StrArgsInitialised();

        bool IsInitialised();

        bool AddBoolArgValue(const std::string& name);

        bool
        AddIntArgValue(const std::string& name, const std::string& str_value);

        bool AddStrArgValue(const std::string& name, const std::string& value);

        bool AddArgValue(const std::string& name, const std::string& str_value);

        bool ParseArg(const std::string& arg);

        bool Parse(const std::vector<std::string>& argv);


        BoolArgument& AddFlag(const char* full_name, const std::string& description = "");

        BoolArgument& AddFlag(char short_name, const char* full_name, const std::string& description = "");

        bool GetFlag(const char* arg_name);

        Argument<int32_t>& AddIntArgument(const char* full_name, const std::string& description = "");

        Argument<int32_t>&
        AddIntArgument(char short_name, const char* full_name, const std::string& description = "");

        int32_t GetIntValue(const char* arg_name, uint32_t index = 0);

        Argument<std::string>& AddStringArgument(const char* full_name, const std::string& description = "");

        Argument<std::string>&
        AddStringArgument(char short_name, const char* full_name, const std::string& description = "");

        std::string GetStringValue(const char* arg_name, uint32_t index = 0);

    private:
        std::string name_;

        char help_short_name_;
        std::string help_full_name_;
        std::string help_text_;
        bool call_help_;
        bool has_help_;

        Argument<int32_t>* positional_int_;
        Argument<std::string>* positional_str_;


        std::map<std::string, std::shared_ptr<Argument<int32_t>>> int_arg_names_;
        std::map<std::string, std::shared_ptr<Argument<std::string>>> str_arg_names_;
        std::map<std::string, std::shared_ptr<BoolArgument>> bool_arg_names_;
    };

} // namespace ArgumentParser