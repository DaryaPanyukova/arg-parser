#include "boolargument.h"



BoolArgument::BoolArgument(const std::string& description) {
    is_initialised_ = false;
    value_to_store_ = nullptr;
    description_ = description;
}

BoolArgument& BoolArgument::Default(bool value0) {
    value_ = value0;
    is_initialised_ = true;
    return *this;
}

BoolArgument& BoolArgument::StoreValue(bool& value) {
    value_to_store_ = &value;
    return *this;
}


bool BoolArgument::IsInitialised() {
    return is_initialised_;
}

bool BoolArgument::AddValue(bool value0) {
    value_ = true;
    if (value_to_store_ != nullptr) {
        *value_to_store_ = true;
    }
    is_initialised_ = true;
    return true;
}
