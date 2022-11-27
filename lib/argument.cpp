#include "argument.h"

template class Argument<int32_t>;
template class Argument<std::string>;

template <class Type>
Argument<Type>::Argument(const std::string& description) {
    description_ = description;
    store_values_ = false;

    multi_value_ = false;
    min_args_count_ = 1;

    values_ = new std::vector <Type>;
    value_to_store_ = nullptr;
    positional_ = false;

}

template <class Type>
Argument<Type>& Argument<Type>::MultiValue(size_t min_args_count0) {
    multi_value_ = true;
    min_args_count_ = min_args_count0;
    return *this;
}

template <class Type>
Argument<Type>& Argument<Type>::StoreValue(Type& value) {
    value_to_store_ = &value;
    return *this;
}

template <class Type>
Argument<Type>& Argument<Type>::StoreValues(std::vector <Type>& values_to_store) {
    values_ = &values_to_store;
    store_values_ = true;
    return *this;
}

template <class Type>
Argument<Type>& Argument<Type>::Positional() {
    positional_ = true;
    return *this;
}

template <class Type>
Argument<Type>& Argument<Type>::Default(const Type& value) {
    values_->push_back(value);
    return *this;
}


template <class Type>
bool Argument<Type>::AddValue(const Type& value) {
    if (!multi_value_ && !values_->empty()) {
        return false;
    } else {
        values_->push_back(value);
        if (value_to_store_ != nullptr) {
            *value_to_store_ = value;
        }
        return true;
    }
}

template <class Type>
bool Argument<Type>::IsInitialised() {
    return values_->size() >= min_args_count_;
}

template <class Type>
bool Argument<Type>::IsPositional() {
    return positional_;
}

template <class Type>
Type Argument<Type>::GetValue(size_t index) {
    return (*values_)[index];
}

template<class Type>
Argument<Type>::~Argument() {
    if (!store_values_) {
        delete values_;
    }
}
