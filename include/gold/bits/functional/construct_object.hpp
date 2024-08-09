// <gold/bits/functional/construct_object.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FUNCTIONAL_CONSTRUCT_OBJECT_HPP
#define __GOLD_BITS_FUNCTIONAL_CONSTRUCT_OBJECT_HPP

#include <bits/move.h>

namespace gold {

    namespace __functional {

        /// __functional::construct_object_fn
        template <typename To>
        struct construct_object_fn {

            template <typename... Args>
            static To operator()(Args&&... args)
                noexcept(noexcept(To(std::forward<Args>(args)...)))
                requires requires { To(std::forward<Args>(args)...); }
            { return To(std::forward<Args>(args)...); }

        };

    } // namespace __functional

    /// construct_object
    template <typename To>
    inline constexpr __functional::construct_object_fn<To> construct_object {};

} // namespace gold

#endif // __GOLD_BITS_FUNCTIONAL_CONSTRUCT_OBJECT_HPP
