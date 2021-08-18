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
        struct is_ref_wrapper_ : std::false_type {};

        template <typename T>
        struct is_ref_wrapper_<std::reference_wrapper<T>> : std::true_type {};

        template <typename T>
        inline constexpr bool is_ref_wrapper_v_ = is_ref_wrapper_<T>::value;

        template <typename T>
        struct maybe_unwrap_ref_wrapper_ {
            using type = std::conditional_t<is_ref_wrapper_v_<T>, std::unwrap_reference_t<T>, T>;
        };

        template <typename T>
        using maybe_unwrap_ref_wrapper_t_ = typename maybe_unwrap_ref_wrapper_<T>::type;

} // namespace gold::__detail

#endif // __GOLD_BITS_TUPLE_UTIL_HPP
