// <gold/bits/concepts/string.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_CONCEPTS_STRING_HPP
#define __GOLD_BITS_CONCEPTS_STRING_HPP

#include <string_view>
#include <bits/stringfwd.h>

#include <gold/bits/concepts/types.hpp>

namespace gold {

    /// enable_qualified_string
    template <typename T>
    inline constexpr bool enable_qualified_string = false;

    template <typename CharT, typename Traits, typename Alloc>
    inline constexpr bool enable_qualified_string<std::basic_string<CharT, Traits, Alloc>> = true;

    template <typename CharT, typename Traits>
    inline constexpr bool enable_qualified_string<std::basic_string_view<CharT, Traits>> = true;

    /// qualified_basic_string
    template <typename T, typename C>
    concept qualified_basic_string =
        (std::is_array_v<std::remove_cvref_t<T>>
        && std::same_as<C, std::remove_cvref_t<std::remove_extent_t<std::remove_cvref_t<T>>>>)
     || (std::is_pointer_v<std::remove_cvref_t<T>>
        && std::same_as<C, std::remove_cvref_t<std::remove_pointer_t<std::remove_cvref_t<T>>>>)
     || (std::convertible_to<T, std::basic_string_view<C>>
        && enable_qualified_string<T>)
     && char_type<C>;

    /// qualified_string
    template <typename T>
    concept qualified_string = qualified_basic_string<T, char>;

    /// qualified_wstring
    template <typename T>
    concept qualified_wstring = qualified_basic_string<T, wchar_t>;

} // namespace gold

#endif // __GOLD_BITS_CONCEPTS_STRING_HPP
