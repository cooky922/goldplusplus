// <gold/bits/type_traits/is_scalar_numeric_type_v.hpp> - gold++ library

// Copyright (C) [ 2025 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TYPE_TRAITS_IS_SCALAR_NUMERIC_TYPE_HPP
#define __GOLD_BITS_TYPE_TRAITS_IS_SCALAR_NUMERIC_TYPE_HPP

#include <type_traits>
#include <gold/bits/type_traits/is_character_type_v.hpp>

namespace gold {

    /// is_scalar_numeric_type_v
    template <typename T>
    inline constexpr bool is_scalar_numeric_type_v = false;

    template <>
    inline constexpr bool is_scalar_numeric_type_v<bool> = false;

    template <typename T>
        requires std::is_integral_v<T>
              && (!is_character_type_v<T>)
    inline constexpr bool is_scalar_numeric_type_v<T> = true;

    template <typename T>
        requires std::is_floating_point_v<T>
    inline constexpr bool is_scalar_numeric_type_v<T> = true;

} // namespace gold

#endif // __GOLD_BITS_TYPE_TRAITS_IS_SCALAR_NUMERIC_TYPE_HPP

