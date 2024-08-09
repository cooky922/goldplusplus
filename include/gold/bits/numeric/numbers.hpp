// <gold/bits/numeric/numbers.hpp> - gold++ library

// Copyright (C) [ 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_NUMERIC_NUMBERS_HPP
#define __GOLD_BITS_NUMERIC_NUMBERS_HPP

#include <concepts>
#include <numbers>

// [gold++.numeric.math.numbers] Mathematical Constants

namespace gold {

    // namespace math::numbers
    namespace math::numbers {}

    // alias namespace numbers
    namespace numbers = math::numbers;

} // namespace gold

namespace gold::math::numbers {

    /// numbers::tau_v
    template <std::floating_point T>
    inline constexpr T tau_v = T(6.283185307179586476925286766559005768L);

    /// numbers::tau
    inline constexpr double tau = tau_v<double>;

    /// numbers::inv_phi_v
    template <std::floating_point T>
    inline constexpr T inv_phi_v = T(0.618033988749894848204586834365638118L);

    /// numbers::inv_phi
    inline constexpr double inv_phi = inv_phi_v<double>;

    /// numbers::silver_ratio_v
    template <std::floating_point T>
    inline constexpr T silver_ratio_v = T(2.4142135623730950488L);

    /// numbers::silver_ratio
    inline constexpr double silver_ratio = silver_ratio_v<double>;

    /// numbers::w1_omega_v
    template <std::floating_point T>
    inline constexpr T w1_omega_v = T(0.56714329040978387299996866221L);

    /// numbers::w1_omega
    inline constexpr double w1_omega = w1_omega_v<double>;

} // namespace gold::math::numbers

#endif // __GOLD_BITS_NUMERIC_NUMBERS_HPP
