// <gold/bits/enum_ops.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ENUM_OPS_HPP
#define __GOLD_BITS_ENUM_OPS_HPP

#include <type_traits>

namespace gold::enum_ops {

    /// enable_enum_ops
    template <typename T>
    inline constexpr bool enable_enum_ops = false;

    namespace __enum_ops {

        /// enum_ops::__enum_ops::scoped_enum
        template <typename T>
        concept scoped_enum = std::is_scoped_enum_v<T> && enable_enum_ops<T>;

        /// enum_ops::__enum_ops::to_enum_underlying
        template <typename Enum>
        constexpr auto to_enum_underlying(Enum e) noexcept {
            return static_cast<std::underlying_type_t<Enum>>(e);
        }

    } // namespace __enum_ops

    /// bitwise operators for scoped enums
    template <__enum_ops::scoped_enum T>
    constexpr T operator~(T x) noexcept { return T(~__enum_ops::to_enum_underlying(x)); }

    template <__enum_ops::scoped_enum T>
    constexpr T& operator|=(T& x, T y) noexcept { return x = T(__enum_ops::to_enum_underlying(x) | __enum_ops::to_enum_underlying(y)); }

    template <__enum_ops::scoped_enum T>
    constexpr T operator|(T x, T y) noexcept { return x |= y; }

    template <__enum_ops::scoped_enum T>
    constexpr T& operator&=(T& x, T y) noexcept { return x = T(__enum_ops::to_enum_underlying(x) & __enum_ops::to_enum_underlying(y)); }

    template <__enum_ops::scoped_enum T>
    constexpr T operator&(T x, T y) noexcept { return x &= y; }

    template <__enum_ops::scoped_enum T>
    constexpr T& operator^=(T& x, T y) noexcept { return x = T(__enum_ops::to_enum_underlying(x) ^ __enum_ops::to_enum_underlying(y)); }

    template <__enum_ops::scoped_enum T>
    constexpr T operator^(T x, T y) noexcept { return x ^= y; }

    template <__enum_ops::scoped_enum T>
    constexpr T& operator<<=(T& x, T y) noexcept { return x = T(__enum_ops::to_enum_underlying(x) << __enum_ops::to_enum_underlying(y)); }

    template <__enum_ops::scoped_enum T>
    constexpr T operator<<(T x, T y) noexcept { return x <<= y; }

    template <__enum_ops::scoped_enum T>
    constexpr T& operator>>=(T& x, T y) noexcept { return x = T(__enum_ops::to_enum_underlying(x) >> __enum_ops::to_enum_underlying(y)); }

    template <__enum_ops::scoped_enum T>
    constexpr T operator>>(T x, T y) noexcept { return x >>= y; }

} // namespace gold::enum_ops

#endif // __GOLD_BITS_ENUM_OPS_HPP
