// <gold/bits/numeric/math_int.hpp> - gold++ library

// Copyright (C) [ 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_NUMERIC_MATH_INT_HPP
#define __GOLD_BITS_NUMERIC_MATH_INT_HPP

#include <bit>
#include <numeric>
#include <gold/utility>

// Integer Math Functions

namespace gold::math {

    /// math::midpoint
    template <std::integral T>
    inline constexpr T midpoint(const T& a, const T& b) {
        return (a & b) + ((a ^ b) >> 1);
    }

    /// math::signum
    template <std::integral T>
    inline constexpr int signum(const T& a) noexcept {
        if constexpr (std::is_unsigned_v<T>)
            return T(0) < a;
        else
            return (T(0) < a) - (a < T(0));
    }

    /// math::power
    template <std::integral T>
    inline constexpr T power(T base, std::size_t expo) {
        // exponent 0 case: base ^ 0 = 1
        if (expo == 0) return 1;

        // exponent 1 case: base ^ 1 = base
        if (expo == 1) return base;

        // base -1 case
        if constexpr (std::is_signed_v<T>) {
            if (base == static_cast<T>(-1))
                return (expo & 1) ? static_cast<T>(-1) : static_cast<T>(1);
        }

        // base 2 case
        if (base == static_cast<T>(2))
            return static_cast<T>(1) << expo;

        // normal case
        T result = 1;

        while (expo) {
            // if exponent is odd
            if (expo & 1) result *= base;
            // divide exponent by 2
            expo >>= 1;
            // multiply itself
            base *= base;
        }

        return result;
    }

    /// math::log_2
    template <std::integral T>
    inline constexpr T log_2(const T& x) {
        return std::bit_width(gold::to_unsigned(x)) - 1;
    }

    /// math::square_root
    template <std::integral T>
    inline constexpr T square_root(const T& a) {
        gold::uint64_t l = 0, r = a;
        return [&, a = static_cast<gold::uint64_t>(a)] constexpr -> T {
            while (l <= r) [[likely]] {
                auto mid_ = gold::math::midpoint(l, r);
                auto mid_2_ = mid_ * mid_;

                if ((mid_2_ <= a) && (a < (mid_ + 1) * (mid_ + 1)))
                    return mid_;

                (mid_2_ > a ? r : l) = mid_ + (mid_2_ > a ? -1 : 1);
            }
            gold::unreachable();
        }();
    }

    /// math::cube_root
    template <std::integral T>
    inline constexpr T cube_root(const T& x) {
        /// conversion to 64-bit unsigned int
        gold::uint64_t n = static_cast<gold::uint64_t>(x), r0 = 1, r1;

        /// 0 ^ (1/3) == 0
        if (n == 0) return 0;

        int b = 64 - std::countl_zero(n);
        r0 <<= (b + 2) / 3;

        do {
            r1 = std::move(r0);
            r0 = (2 * r1 + n / (r1 * r1)) / 3;
        } while (r0 < r1);

        return static_cast<T>(r1);
    }

    /// math::is_prime
    template <std::integral T>
    inline constexpr bool is_prime(const T& n) {
        if (n == 0 || n == 1 || n < 0)
            return false;
        for (auto i = 2; i <= (n / 2); ++i) {
            if (n % i == 0)
                return false;
        }
        return true;
    }

    /// math::are_co_prime
    template <std::integral T>
    inline constexpr bool are_co_prime(const T& a, const T& b) {
        return std::gcd(a, b) == 1;
    }

    /// math::factorial - must be unsigned :>
    template <std::unsigned_integral T = gold::uint64_t>
    inline constexpr T factorial(const T& n) {
        T result = 1;
        for (T i = 2; i <= n; ++i)
            result *= i;
        return result;
    }

    /// math::permutation
    template <std::unsigned_integral T = gold::uint64_t>
    inline constexpr T permutation(const T& n, const T& k) {
        return factorial(n) / factorial(n - k);
    }

    /// math::combination
    template <std::unsigned_integral T = gold::uint64_t>
    inline constexpr T combination(const T& n, const T& k) {
        return factorial(n) / (factorial(k) * factorial(n - k));
    }

    /// math::binomial_coeff - synonym to combination
    template <std::unsigned_integral T = gold::uint64_t>
    inline constexpr T binomial_coeff(const T& n, const T& k) {
        return combination(n, k);
    }

    /// math::digit_count
    template <std::integral T>
    inline constexpr T digit_count(const T& a) {
        auto b = static_cast<unsigned long long>(a > 0 ? a : -a);
        static constexpr decltype(0ull) powers_10_table [] {
            1ull,          // 10 ^ 0
            10ull,         // 10 ^ 1
            100ull,        // 10 ^ 2
            1000ull,       // 10 ^ 3
            10000ull,      // 10 ^ 4
            100000ull,     // 10 ^ 5
            1000000ull,    // 10 ^ 6
            10000000ull,   // 10 ^ 7
            100000000ull,  // 10 ^ 8
            1000000000ull, // 10 ^ 9
            10000000000ull // 10 ^ 10
        };

        return (
            b < powers_10_table[1] ? 1 :
            b < powers_10_table[2] ? 2 :
            b < powers_10_table[3] ? 3 :
            b < powers_10_table[4] ? 4 :
            b < powers_10_table[5] ? 5 :
            b < powers_10_table[6] ? 6 :
            b < powers_10_table[7] ? 7 :
            b < powers_10_table[8] ? 8 :
            b < powers_10_table[9] ? 9 :
            b < powers_10_table[10] ? 10 :
            10 + digit_count(b / powers_10_table[10])
        );
    }

} // namespace gold::math

#endif // __GOLD_BITS_NUMERIC_MATH_INT_HPP
