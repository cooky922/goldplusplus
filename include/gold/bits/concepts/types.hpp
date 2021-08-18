// <gold/bits/concepts/types.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_CONCEPTS_TYPES_HPP
#define __GOLD_BITS_CONCEPTS_TYPES_HPP

#include <concepts>

#include <gold/bits/type_traits/type_classification.hpp>

namespace gold {

    /// char_type
    template <typename T>
    concept char_type = is_char<T>::value;

    /// qualified_numeric
    template <typename T>
    concept qualified_numeric = std::integral<T> || std::floating_point<T>;

}

#endif // __GOLD_BITS_CONCEPTS_TYPES_HPP
