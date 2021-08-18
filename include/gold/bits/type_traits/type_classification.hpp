// <gold/bits/type_traits/type_classification.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TYPE_TRAITS_TYPE_CLASSIFICATION_HPP
#define __GOLD_BITS_TYPE_TRAITS_TYPE_CLASSIFICATION_HPP

#include <type_traits>

namespace gold {

    /// is_char
    template <typename T> struct is_char : std::false_type {};
    template <> struct is_char<char> : std::true_type {};
    template <> struct is_char<wchar_t> : std::true_type {};
    template <> struct is_char<char8_t> : std::true_type {};
    template <> struct is_char<char16_t> : std::true_type {};
    template <> struct is_char<char32_t> : std::true_type {};

    /// is_char_v
    template <typename T>
    inline constexpr bool is_char_v = is_char<T>::value;

}

#endif // __GOLD_BITS_TYPE_TRAITS_TYPE_CLASSIFICATION_HPP

