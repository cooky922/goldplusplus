// <gold/bits/concepts/lambda.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_CONCEPTS_LAMBDA_HPP
#define __GOLD_BITS_CONCEPTS_LAMBDA_HPP

#include <concepts>
#include <gold/bits/type_name/type_name_c.hpp>

namespace gold {

    namespace __concepts {

        /// __concepts::is_lambda
        template <typename T>
        consteval bool is_lambda() {
            constexpr auto name = gold::type_name<T>();
            if (name.contains("<lambda closure object>"))
                return false;
            return name.contains("<lambda(");
        }

    } // namespace __concepts

    /// lambda
    // TODO: create more constrained lambda
    template <typename T>
    concept lambda = __concepts::is_lambda<T>();

    /// generic_lambda
    template <typename T>
    concept generic_lambda = lambda<T> && !requires { &T::operator(); };

    /// stateless_lambda
    template <typename T>
    concept stateless_lambda = lambda<T> && std::default_initializable<T>;

} // namespace gold

#endif // __GOLD_BITS_CONCEPTS_LAMBDA_HPP

