// <gold/bits/numeric/sat_ops.hpp> - gold++ library

// Copyright (C) [ 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_NUMERIC_SAT_OPS_HPP
#define __GOLD_BITS_NUMERIC_SAT_OPS_HPP

#include <type_traits>
#include <concepts>
#include <limits>

namespace gold {

    namespace __num {

        /// __num::is_safe_convertible_without_narrowing
        template <typename From, typename To>
        inline constexpr bool is_safe_convertible_without_narrowing = requires (From from) {
            To { from };
        };

    } // namespace __num

    // [gold++.numeric.sat_ops] saturated arithmetic

    /// sat_add
    template <std::integral T>
    [[gnu::always_inline]] constexpr T sat_add(T lhs, std::type_identity_t<T> rhs) noexcept {
        T result;
        if (!__builtin_add_overflow(lhs, rhs, &result))
            return result;
        if constexpr (std::is_unsigned_v<T>)
            return std::numeric_limits<T>::max();
        else {
            if (lhs > 0 && rhs > 0)
                return std::numeric_limits<T>::max();
            return std::numeric_limits<T>::min();
        }
    }

    /// sat_sub
    template <std::integral T>
    [[gnu::always_inline]] constexpr T sat_sub(T lhs, std::type_identity_t<T> rhs) noexcept {
        T result;
        if (!__builtin_sub_overflow(lhs, rhs, &result))
            return result;
        if constexpr (std::is_unsigned_v<T>)
            return T(0);
        else {
            if (lhs > 0)
                return std::numeric_limits<T>::max();
            else
                return std::numeric_limits<T>::min();
        }
    }

    /// sat_mul
    template <std::integral T>
    [[gnu::always_inline]] constexpr T sat_mul(T lhs, std::type_identity_t<T> rhs) noexcept {
        T result;
        if (!__builtin_mul_overflow(lhs, rhs, &result))
            return result;
        if constexpr (std::is_unsigned_v<T>)
            return std::numeric_limits<T>::max();
        else {
            if (lhs > 0 && rhs > 0 ||
                lhs < 0 && rhs < 0)
                return std::numeric_limits<T>::max();
            return std::numeric_limits<T>::min();
        }
    }

    /// sat_div
    template <std::integral T>
    [[gnu::always_inline]] constexpr T sat_div(T lhs, std::type_identity_t<T> rhs) noexcept {
        [[assume(rhs != 0)]];
        if constexpr (std::is_signed_v<T>)
            if (lhs == std::numeric_limits<T>::min() && rhs == T(-1))
                return std::numeric_limits<T>::max();
        return lhs / rhs;
    }

    /// saturate_cast
    template <std::integral To, std::integral From>
    [[gnu::always_inline]] constexpr To saturate_cast(From from) noexcept {
        if constexpr (__is_same(To, From))
            return from;
        else if constexpr (__num::is_safe_convertible_without_narrowing<From, To>)
            return static_cast<To>(from);
        if constexpr (std::is_unsigned_v<To> && std::is_unsigned_v<From>) {
            if (static_cast<From>(std::numeric_limits<To>::max()) < from)
                return std::numeric_limits<To>::max();
        } else if constexpr (std::is_unsigned_v<To>) {
            // to unsigned
            if (from < 0)
                return static_cast<To>(0);
            else if (static_cast<From>(std::numeric_limits<To>::max()) < from)
                return std::numeric_limits<To>::max();
        } else if constexpr (std::is_unsigned_v<From>) {
            // to signed
            if (static_cast<From>(std::numeric_limits<To>::max()) < from)
                return std::numeric_limits<To>::max();
        } else {
            if (static_cast<From>(std::numeric_limits<To>::max()) < from)
                return std::numeric_limits<To>::max();
            else if (from < static_cast<From>(std::numeric_limits<To>::min()))
                return std::numeric_limits<To>::min();
        }
        return static_cast<To>(from);
    }

} // namespace gold

#endif // __GOLD_BITS_NUMERIC_SAT_OPS_HPP
