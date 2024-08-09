// <gold/bits/numeric/interval.hpp> - gold++ library

// Copyright (C) [ 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_NUMERIC_INTERVAL_HPP
#define __GOLD_BITS_NUMERIC_INTERVAL_HPP

#include <gold/bits/__util/type_identity.hpp>

namespace gold::math {

    // note: they are all bound intervals
    // - there is no infinity as an endpoint

    /// math::closed_interval
    // represents closed interval [first, last]
    template <typename T>
    struct closed_interval {
        T first;
        T last;

        template <typename U>
        constexpr operator closed_interval<U>() const noexcept {
            return closed_interval<U>(static_cast<U>(first), static_cast<U>(last));
        }

        constexpr bool includes(const T& op) const noexcept {
            return first <= op && op <= last;
        }

        constexpr T length() const noexcept {
            return last - first;
        }

        // pre: first < last
    };

    /// math::unit_interval
    inline constexpr closed_interval unit_interval (0.0, 1.0);

    /// math::open_interval
    // represents open interval (first, last)
    template <typename T>
    struct open_interval {
        T first;
        T last;

        template <typename U>
        constexpr operator open_interval<U>() const noexcept {
            return open_interval<U>(static_cast<U>(first), static_cast<U>(last));
        }

        constexpr bool includes(const T& op) const noexcept {
            return first < op && op < last;
        }

        constexpr T length() const noexcept {
            return last - first;
        }

        // pre: first < last
    };

    /// math::left_open_interval
    // represents half left open interval (first, last]
    template <typename T>
    struct left_open_interval {
        T first;
        T last;

        template <typename U>
        constexpr operator left_open_interval<U>() const noexcept {
            return left_open_interval<U>(static_cast<U>(first), static_cast<U>(last));
        }

        constexpr bool includes(const T& op) const noexcept {
            return first < op && op <= last;
        }

        constexpr T length() const noexcept {
            return last - first;
        }

        // pre: first < last
    };

    /// math::right_open_interval
    // represents half right open interval [first, last)
    template <typename T>
    struct right_open_interval {
        T first;
        T last;

        template <typename U>
        constexpr operator right_open_interval<U>() const noexcept {
            return right_open_interval<U>(static_cast<U>(first), static_cast<U>(last));
        }

        constexpr bool includes(const T& op) const noexcept {
            return first <= op && op < last;
        }

        constexpr T length() const noexcept {
            return last - first;
        }

        // pre: first < last
    };

    /// math::transmute_value
    template <typename From, typename To = From>
    constexpr To transmute_value(__util::non_deduced_t<From> val,
                                   math::closed_interval<From> interval_from,
                                   math::closed_interval<To>   interval_to) noexcept {
        const math::closed_interval<To> interval_from_u = interval_from;
        const To length_from = interval_from_u.length();
        const To length_to   = interval_to.length();
        return (static_cast<To>(val) - interval_from_u.first)
              * length_to / length_from + interval_to.first;
    }

    /// math::normalize_value
    template <typename To = double, typename From>
    constexpr To normalize_value(__util::non_deduced_t<From> val,
                                 math::closed_interval<From> interval_from) noexcept {
        return transmute_value(val, interval_from, static_cast<math::closed_interval<To>>(math::unit_interval));
    }

} // namespace gold

#endif // __GOLD_BITS_NUMERIC_INTERVAL_HPP
