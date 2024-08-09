// <gold/bits/expected/utils.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_EXPECTED_UTILS_HPP
#define __GOLD_BITS_EXPECTED_UTILS_HPP

#include <bits/utility.h>

namespace gold {

    /// unexpect_t
    struct unexpect_t {
        constexpr explicit unexpect_t() = default;
    };

    /// unexpect
    inline constexpr unexpect_t unexpect {};

    namespace __expected {

        /// __expected::is_unexpected_v
        template <typename>
        inline constexpr bool is_unexpected_v = false;

        /// __expected::is_expected_v
        template <typename>
        inline constexpr bool is_expected_v = false;

        /// __expected::can_be_unexpected
        template <typename T>
        concept can_be_unexpected =
                 std::is_object_v<T>
            && (!std::is_array_v<T>)
            && (!std::is_const_v<T>)
            && (!std::is_volatile_v<T>)
            && (!std::is_same_v<std::remove_cv_t<T>, std::in_place_t>)
            && (!std::is_same_v<std::remove_cv_t<T>, unexpect_t>)
            && (!is_unexpected_v<T>)
            && (!is_expected_v<T>);

        /// __expected::can_be_expected_value
        template <typename T>
        concept can_be_expected_value =
               (!std::is_reference_v<T>)
            && (!std::is_function_v<T>)
            && (!std::is_same_v<std::remove_cv_t<T>, std::in_place_t>)
            && (!std::is_same_v<std::remove_cv_t<T>, unexpect_t>)
            && (!is_unexpected_v<std::remove_cv_t<T>>);

        /// __expected::can_be_expected
        template <typename T, typename E>
        concept can_be_expected =
               can_be_unexpected<E>
            && can_be_expected_value<T>;

    } // namespace __expected

} // namespace gold

#endif // __GOLD_BITS_EXPECTED_UTILS_HPP
