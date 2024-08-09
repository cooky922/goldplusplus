// <gold/bits/concepts/lambda.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_CONCEPTS_LAMBDA_HPP
#define __GOLD_BITS_CONCEPTS_LAMBDA_HPP

#include <concepts>
#include <gold/bits/type_name/type_name_c.hpp>

namespace gold {

    namespace __concepts {

        /// __concepts::get_template_name_of
        consteval std::string_view get_template_name_of(std::string_view info) {
            const std::size_t bound_left  = info.find_first_of('<');
            const std::size_t bound_right = info.find_last_of('>');
            if (bound_left  == std::string_view::npos ||
                bound_right == std::string_view::npos ||
                info[bound_left - 1] == ':')
                return "";
            return info.substr(0, bound_left);
        }

        /// __concepts::get_raw_template_args_of
        consteval std::string_view get_raw_template_args_of(std::string_view info) {
            if (__concepts::get_template_name_of(info).empty())
                return "";
            const std::size_t bound_left  = info.find_first_of('<');
            const std::size_t bound_right = info.find_last_of('>');
            return info.substr(bound_left + 1, bound_right - bound_left - 1);
        }

        /// __concepts::is_lambda
        template <typename T>
        consteval bool is_lambda() {
            std::string_view name = gold::type_name<T>();
            if (!name.contains("<lambda(") || !name.ends_with(")>"))
                return false;
            return __concepts::get_raw_template_args_of(name).empty();
        }

    } // namespace __concepts

    /// closure_type
    template <typename T>
    concept closure_type = __concepts::is_lambda<T>();

    /// generic_closure_type
    template <typename T>
    concept generic_closure_type = closure_type<T> && !requires { &T::operator(); };

    /// stateless_closure_type
    template <typename T>
    concept stateless_closure_type = closure_type<T> && std::default_initializable<T>;

} // namespace gold

#endif // __GOLD_BITS_CONCEPTS_LAMBDA_HPP

