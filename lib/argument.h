//
// Created by Пользователь on 27.11.2022.
//


#pragma once

#include <iostream>
#include <vector>


template<class Type>
class Argument {
private:
    std::string description_;

    // MultiValue
    bool multi_value_;
    size_t min_args_count_;

    // StoreValue(s)
    bool store_values_;
    std::vector<Type>* values_;
    Type* value_to_store_;

    // Positional
    bool positional_;

public:
    Argument(const std::string& description = "");

    ~Argument();

    Argument& MultiValue(size_t min_args_count0 = 2);

    Argument& StoreValue(Type& value);

    Argument& StoreValues(std::vector<Type>& values);

    Argument& Positional();

    Argument& Default(const Type& value);

    bool AddValue(const Type& value);

    bool IsInitialised();

    bool IsPositional();

    Type GetValue(size_t index);
};

