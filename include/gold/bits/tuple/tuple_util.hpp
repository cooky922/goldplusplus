// <gold/bits/tuple/tuple_util.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TUPLE_UTIL_HPP
#define __GOLD_BITS_TUPLE_UTIL_HPP

#include <gold/bits/tuple/tuple_concepts.hpp>
#include <bits/refwrap.h>

namespace gold::__detail {

    template <typename>
    inline constexpr bool is_ref_wrapper_v_ = false;

    template <typename T>
    inline constexpr bool is_ref_wrapper_v_<std::reference_wrapper<T>> = true;

    template <typename T>
    struct is_ref_wrapper_ : std::bool_constant<is_ref_wrapper_v_<T>> {};

    template <typename T>
    using maybe_unwrap_ref_wrapper_t_ = std::conditional_t<
        is_ref_wrapper_v_<T>,
        std::unwrap_reference_t<T>,
        T
    >;

    template <typename T>
    struct maybe_unwrap_ref_wrapper_ {
        using type = maybe_unwrap_ref_wrapper_t_<T>;
    };

} // namespace gold::__detail

#endif // __GOLD_BITS_TUPLE_UTIL_HPP
