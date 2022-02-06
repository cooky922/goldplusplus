// <gold/bits/concepts/lambda.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_CONCEPTS_LAMBDA_HPP
#define __GOLD_BITS_CONCEPTS_LAMBDA_HPP

#include <gold/bits/typeof/compile_typeof.hpp>

namespace gold {

    namespace __detail {

        template <typename T>
        consteval bool is_anonymous_func_() {
            constexpr auto name_ = gold::__detail::compile_type_of_<T>();

            if (name_.contains("<lambda closure object>")) return false;
            return name_.contains("<lambda(");
        }

    } // namespace __detail

    /// lambda
    template <typename T>
    concept lambda = __detail::is_anonymous_func_<T>();

} // namespace gold

#endif // __GOLD_BITS_CONCEPTS_LAMBDA_HPP

