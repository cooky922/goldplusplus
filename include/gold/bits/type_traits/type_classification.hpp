// <gold/bits/type_traits/type_classification.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TYPE_TRAITS_TYPE_CLASSIFICATION_HPP
#define __GOLD_BITS_TYPE_TRAITS_TYPE_CLASSIFICATION_HPP

#include <type_traits>
#include <compare>

namespace gold {

    /// is_char_v
    template <typename T>
    inline constexpr bool is_char_v = false;

    template <>
    inline constexpr bool is_char_v<char> = true;

    template <>
    inline constexpr bool is_char_v<wchar_t> = true;

    template <>
    inline constexpr bool is_char_v<char8_t> = true;

    template <>
    inline constexpr bool is_char_v<char16_t> = true;

    template <>
    inline constexpr bool is_char_v<char32_t> = true;

    /// is_char
    template <typename T>
    using is_char = std::bool_constant<is_char_v<T>>;

    /// is_ordering_type_v
    template <typename T>
    inline constexpr bool is_ordering_type_v = false;

    template <>
    inline constexpr bool is_ordering_type_v<std::strong_ordering> = true;

    template <>
    inline constexpr bool is_ordering_type_v<std::weak_ordering> = true;

    template <>
    inline constexpr bool is_ordering_type_v<std::partial_ordering> = true;

    /// is_ordering_type
    template <typename T>
    using is_ordering_type = std::bool_constant<is_ordering_type_v<T>>;

} // namespace gold

#endif // __GOLD_BITS_TYPE_TRAITS_TYPE_CLASSIFICATION_HPP

