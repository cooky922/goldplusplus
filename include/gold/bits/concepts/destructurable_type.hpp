// <gold/bits/concepts/destructurable_type.hpp> - gold++ library

// Copyright (C) [ 2025 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_CONCEPTS_DESTRUCTURABLE_HPP
#define __GOLD_BITS_CONCEPTS_DESTRUCTURABLE_HPP

#include <type_traits>

namespace gold {

    namespace __concepts {

        template <typename T>
        inline constexpr bool is_destructurable_impl =
            requires { __builtin_structured_binding_size(T); };

    } // namespace __concepts

    /// destructurable_type
    template <typename T>
    concept destructurable_type = __concepts::is_destructurable_impl<std::remove_cvref_t<T>>;

} // namespace gold

#endif // __GOLD_BITS_CONCEPTS_DESTRUCTURABLE_HPP
