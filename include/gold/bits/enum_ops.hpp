// <gold/bits/enum_ops.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ENUM_OPS_HPP
#define __GOLD_BITS_ENUM_OPS_HPP

#include <utility>

namespace gold::enum_ops {

    /// enable_enum_ops
    template <typename T>
    inline constexpr bool enable_enum_ops = false;

    namespace __enum_ops {
        template <typename T>
        concept scoped_enum = std::is_scoped_enum_v<T> && enable_enum_ops<T>;
    } // namespace __enum_ops

    /// bitwise operators for scoped enums
    template <__enum_ops::scoped_enum T>
    constexpr T operator~(T x) { return T(~std::to_underlying(x)); }

    template <__enum_ops::scoped_enum T>
    constexpr T& operator|=(T& x, T y) { return x = T(std::to_underlying(x) | std::to_underlying(y)); }

    template <__enum_ops::scoped_enum T>
    constexpr T operator|(T x, T y) { return x |= y; }

    template <__enum_ops::scoped_enum T>
    constexpr T& operator&=(T& x, T y) { return x = T(std::to_underlying(x) & std::to_underlying(y)); }

    template <__enum_ops::scoped_enum T>
    constexpr T operator&(T x, T y) { return x &= y; }

    template <__enum_ops::scoped_enum T>
    constexpr T& operator^=(T& x, T y) { return x = T(std::to_underlying(x) ^ std::to_underlying(y)); }

    template <__enum_ops::scoped_enum T>
    constexpr T operator^(T x, T y) { return x ^= y; }

    template <__enum_ops::scoped_enum T>
    constexpr T& operator<<=(T& x, T y) { return x = T(std::to_underlying(x) << std::to_underlying(y)); }

    template <__enum_ops::scoped_enum T>
    constexpr T operator<<(T x, T y) { return x <<= y; }

    template <__enum_ops::scoped_enum T>
    constexpr T& operator>>=(T& x, T y) { return x = T(std::to_underlying(x) >> std::to_underlying(y)); }

    template <__enum_ops::scoped_enum T>
    constexpr T operator>>(T x, T y) { return x >>= y; }

} // namespace gold::enum_ops

#endif // __GOLD_BITS_ENUM_OPS_HPP
