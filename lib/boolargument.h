#pragma once


class BoolArgument {
private:
    bool* value_to_store_;
    bool value_;
    bool is_initialised_;

public:
    BoolArgument& Default(bool value0);

    BoolArgument& StoreValue(bool& value);

    BoolArgument();

    bool IsInitialised();

    bool AddValue(bool value0);

    bool GetValue() {
        return value_;
    }
};

