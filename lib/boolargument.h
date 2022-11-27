#pragma once

#include <iostream>

class BoolArgument {
private:
    bool* value_to_store_;
    bool value_;
    bool is_initialised_;
    std::string description_;

public:
    BoolArgument& Default(bool value0);

    BoolArgument& StoreValue(bool& value);

    BoolArgument(const std::string& description = "");

    bool IsInitialised();

    bool AddValue(bool value0);

    bool GetValue() {
        return value_;
    }
};

