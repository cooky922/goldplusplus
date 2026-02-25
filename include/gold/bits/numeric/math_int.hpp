// <gold/bits/numeric/math_int.hpp> - gold++ library

// Copyright (C) [ 2024 - 2026 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_NUMERIC_MATH_INT_HPP
#define __GOLD_BITS_NUMERIC_MATH_INT_HPP

#include <bit>
#include <numeric>
#include <gold/utility>
#include <gold/bits/assume.hpp>

// Integer Math Functions

namespace gold::math {

    /// math::midpoint
    template <std::integral T>
    constexpr T midpoint(T a, std::type_identity_t<T> b) {
        return (a & b) + ((a ^ b) >> 1);
    }

    /// math::signum
    template <std::integral T>
    constexpr int signum(T a) noexcept {
        if constexpr (std::is_unsigned_v<T>)
            return T(0) < a;
        else
            return (T(0) < a) - (a < T(0));
    }

    /// math::power
    template <std::integral T>
    constexpr T power(T base, std::size_t expo) {
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
    constexpr T log_2(T x) {
        return std::bit_width(gold::to_unsigned(x)) - 1;
    }

    /// math::is_perfect_square
    template <std::unsigned_integral T>
    constexpr bool is_perfect_square(T n) {
        if (n <= 1)
            return true;
        T left = 1, right = n;
        while (left <= right) {
            T mid = left + (right - left) / 2;
            if (mid > n / mid)
                right = mid - 1;
            else if (mid * mid == n)
                return true;
            else
                left = mid + 1;
        }
        return false;
    }

    /// math::square_root
    template <std::integral T>
    constexpr T square_root(T a) {
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
    constexpr T cube_root(T x) {
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

    /// math::factorial - must be unsigned :>
    template <std::unsigned_integral T = gold::uint64_t>
    constexpr T factorial(T n) {
        T result = 1;
        for (T i = 2; i <= n; ++i)
            result *= i;
        return result;
    }

    /// math::permutation
    template <std::unsigned_integral T = gold::uint64_t>
    constexpr T permutation(T n, std::type_identity_t<T> k) {
        // effectively computes: factorial(n) / factorial(n - k)
        if (k < 0 || k > n)
            return 0;
        T result = 1;
        for (T i = 0; i < k; ++i)
            result *= n - i;
        return result;
    }

    /// math::combination
    template <std::unsigned_integral T = gold::uint64_t>
    constexpr T combination(T n, std::type_identity_t<T> k) {
        // effectively computes: factorial(n) / (factorial(k) * factorial(n - k))
        if (k < 0 || k > n)
            return 0;
        if (k * 2 > n)
            k = n - k;
        if (k == 0)
            return 1;
        T result = n;
        for (T i = 2; i <= k; ++i) {
            result *= n - i + 1;
            result /= i;
        }
        return result;
    }

    /// math::binomial_coeff
    // synonymous with combination
    template <std::unsigned_integral T = gold::uint64_t>
    constexpr T binomial_coeff(T n, std::type_identity_t<T> k) {
        return combination(n, k);
    }

    /// math::digit_count
    template <std::integral T>
    constexpr T digit_count(T a) {
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

    /// math::first_prime_factor
    template <std::unsigned_integral T>
    constexpr T first_prime_factor(T n) noexcept {
        T result = 2;
        while (true) {
            if (n % result == 0)
                return result;
            ++result;
        }
        __builtin_unreachable();
    }

    /// math::is_prime
    template <std::unsigned_integral T>
    constexpr bool is_prime(T n) {
        return n >= 2 && first_prime_factor(n) == n;
    }

    /// math::next_prime
    template <std::unsigned_integral T>
    constexpr T next_prime(T n) {
        ++n;
        while (!is_prime(n))
            ++n;
        return n;
    }

    /// math::prev_prime
    template <std::unsigned_integral T>
    constexpr T prev_prime(T n) {
        if (n <= 2)
            return n;
        --n;
        while (n > 2 && !is_prime(n))
            --n;
        return n;
    }

    /// math::are_co_prime
    template <std::unsigned_integral T>
    constexpr bool are_co_prime(T a, std::type_identity_t<T> b) {
        return std::gcd(a, b) == 1;
    }

    /// math::euler_phi
    template <std::unsigned_integral T>
    constexpr T euler_phi(T n) {
        T result = n;
        for (T p = 2; p * p <= n; ++p) {
            if (n % p == 0) {
                while (n % p == 0)
                    n /= p;
                result -= result / p;
            }
        }
        if (n > 1)
            result -= result / n;
        return result;
    }

    /// math::extended_gcd_result
    template <std::signed_integral T>
    struct extended_gcd_result {
        T g;
        T x;
        T y;
    };

    /// math::extended_gcd
    // given the equation 'ax + by = gcd(a, b)',
    // find a pair (x, y) that will satisfy the equation above
    template <std::signed_integral T>
    constexpr extended_gcd_result<T> extended_gcd(T a, std::type_identity_t<T> b) {
        T old_x = 1;
        T old_y = 0;
        T curr_x = 0;
        T curr_y = 1;
        while (b != 0) {
            T quo = a / b;
            T rem = a % b;
            a = b;
            b = rem;
            old_x = std::exchange(curr_x, old_x - quo * curr_x);
            old_y = std::exchange(curr_y, old_y - quo * curr_y);
        }
        return {a, old_x, old_y};
    }

    /// math::mod_residue
    // get the remainder, which is always positive.
    // an operator '%' isn't suited for negative numbers
    template <typename T>
    constexpr T mod_residue(T a, std::type_identity_t<T> n) {
        return (a % n + n) % n;
    }

    /// math::mod_mul
    // solve ab (mod n)
    template <std::signed_integral T>
    constexpr T mod_mul(T a, std::type_identity_t<T> b, std::type_identity_t<T> n) {
        const T ar = mod_residue(a, n);
        const T br = mod_residue(b, n);
        return mod_residue(ar * br, n);
    }

    /// math::mod_inv
    // find a^-1 (mod n) [Modular Inverse]
    template <std::signed_integral T>
    constexpr T mod_inv(T a, std::type_identity_t<T> n) {
        a = mod_residue(a, n);
        using U = std::make_unsigned_t<T>;
        gold::assume(are_co_prime(U(a), n));
        const auto [_, inv, _] = extended_gcd(a, n);
        return mod_residue(inv, n);
    }

    /// math::mod_div
    // find a/b (mod n)
    template <std::signed_integral T>
    constexpr T mod_div(T a, std::type_identity_t<T> b, std::type_identity_t<T> n) {
        return mod_mul(a, mod_inv(b, n), n);
    }

    /// math::mod_pow
    // find a^b (mod n) [Modular Exponentiation]
    template <std::signed_integral T>
    constexpr T mod_pow(T a, std::type_identity_t<T> b, std::type_identity_t<T> n) {
        if (n == 1)
            return 0;
        a = mod_residue(a, n);
        if (b < 0)
            return mod_pow(mod_inv(a, n), -b, n);
        T result = 1;
        while (b > 0) {
            if (b & 1 == 1)
                result = (result * a) % n;
            b >>= 1;
            a = (a * a) % n;
        }
        return result;
    }

} // namespace gold::math

#endif // __GOLD_BITS_NUMERIC_MATH_INT_HPP
