// <gold/bits/type_traits/is_character_type_v.hpp> - gold++ library

// Copyright (C) [ 2025 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TYPE_TRAITS_IS_CHARACTER_TYPE_HPP
#define __GOLD_BITS_TYPE_TRAITS_IS_CHARACTER_TYPE_HPP

namespace gold {

    /// is_character_type_v
    template <typename T>
    inline constexpr bool is_character_type_v = false;

    template <>
    inline constexpr bool is_character_type_v<char> = true;

    template <>
    inline constexpr bool is_character_type_v<wchar_t> = true;

    template <>
    inline constexpr bool is_character_type_v<char8_t> = true;

    template <>
    inline constexpr bool is_character_type_v<char16_t> = true;

    template <>
    inline constexpr bool is_character_type_v<char32_t> = true;

} // namespace gold

#endif // __GOLD_BITS_TYPE_TRAITS_IS_CHARACTER_TYPE_HPP

