// <gold/bits/numeric/math_range.hpp> - gold++ library

// Copyright (C) [ 2025 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_NUMERIC_MATH_RANGES_HPP
#define __GOLD_BITS_NUMERIC_MATH_RANGES_HPP

#include <bits/ranges_base.h>
#include <gold/bits/concepts/scalar_numeric_type.hpp>

// Math Ranges Functions
// - numeric algorithms

namespace gold::math {

    namespace __math {

        /// __math::scalar_numeric_range
        template <typename R>
        concept scalar_numeric_range =
            std::ranges::input_range<R> &&
            gold::scalar_numeric_type<std::ranges::range_value_t<R>>;

    } // namespace __math

    /// math::sum
    template <__math::scalar_numeric_range R, typename T = std::ranges::range_value_t<R>>
    constexpr T sum(R&& r) {
        auto res = T(0);
        for (const auto& elem : r)
            res += elem;
        return res;
    }

    /// math::product
    template <__math::scalar_numeric_range R, typename T = std::ranges::range_value_t<R>>
    constexpr T product(R&& r) {
        auto res = T(1);
        for (const auto& elem : r) {
            if (elem == T(0)) {
                res = elem;
                break;
            }
            res *= elem;
        }
        return res;
    }

    /// math::inner_product
    // [TODO] precondition: both ranges must have the same size
    template <__math::scalar_numeric_range R1, __math::scalar_numeric_range R2, typename T = std::ranges::range_value_t<R1>>
        requires std::same_as<T, std::ranges::range_value_t<R2>>
    constexpr T inner_product(R1&& r1, R2&& r2) {
        auto res = T(0);
        auto iter1 = std::ranges::begin(r1);
        const auto sent1 = std::ranges::end(r1);
        auto iter2 = std::ranges::begin(r2);
        const auto sent2 = std::ranges::end(r2);
        while (iter1 != sent1 && iter2 != sent2) {
            res += *iter1 * (*iter2);
            ++iter1;
            ++iter2;
        }
        return res;
    }

} // namespace gold::math

#endif // __GOLD_BITS_NUMERIC_MATH_RANGES_HPP
