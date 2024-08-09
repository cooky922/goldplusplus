// <gold/bits/functional/convert_as.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FUNCTIONAL_CONVERT_AS_HPP
#define __GOLD_BITS_FUNCTIONAL_CONVERT_AS_HPP

#include <bits/move.h>

namespace gold {

    namespace __functional {

        /// __functional::convert_as_fn
        template <typename To>
        struct convert_as_fn {

            template <typename From>
            static To operator()(From&& from)
                noexcept(noexcept(static_cast<To>(std::forward<From>(from))))
                requires requires { static_cast<To>(std::forward<From>(from)); }
            { return static_cast<To>(std::forward<From>(from)); }

        };

    } // namespace __functional

    /// convert_as
    template <typename To>
    inline constexpr __functional::convert_as_fn<To> convert_as {};

} // namespace gold

#endif // __GOLD_BITS_FUNCTIONAL_CONVERT_AS_HPP
