// <gold/bits/concepts/string.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_CONCEPTS_STRING_HPP
#define __GOLD_BITS_CONCEPTS_STRING_HPP

#include <string_view>
#include <bits/ranges_base.h>
#include <gold/bits/concepts/class_or_union_or_enum.hpp>
#include <gold/bits/concepts/types.hpp>

namespace gold {

    /// disable_string_like
    template <typename>
    inline constexpr bool disable_string_like = false;

    namespace __concepts {

        /// __concepts::basic_string_like_impl
        template <typename T, typename C>
        concept basic_string_like_impl =
             (!disable_string_like<T>) &&
             gold::char_type<C> && (
            (std::is_pointer_v<T>
                                && std::same_as<C, std::remove_const_t<std::remove_pointer_t<T>>>) ||
            (std::is_array_v<T> && (std::is_bounded_array_v<T> && std::extent_v<T> != 0 || true)
                                && std::same_as<C, std::remove_const_t<std::remove_extent_t<T>>>)  ||
            (__concepts::class_or_union<T>
                                && std::convertible_to<T, std::basic_string_view<C>>
                                && std::ranges::contiguous_range<T>
                                && std::ranges::sized_range<T>));

    } // namespace __concepts

    /// basic_string_like
    template <typename T, typename C>
    concept basic_string_like = __concepts::basic_string_like_impl<std::remove_cvref_t<T>, C>;

    /// string_like
    template <typename T>
    concept string_like = basic_string_like<T, char>;

    /// wstring_like
    template <typename T>
    concept wstring_like = basic_string_like<T, wchar_t>;

} // namespace gold

#endif // __GOLD_BITS_CONCEPTS_STRING_HPP
