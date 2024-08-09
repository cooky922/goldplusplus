// <gold/bits/tuples/tuple_size.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TUPLES_TUPLE_SIZE_HPP
#define __GOLD_BITS_TUPLES_TUPLE_SIZE_HPP

#include <type_traits>
#include <concepts>
#include <gold/bits/concepts/types.hpp>
#include <bits/utility.h>

namespace gold::tuples {

    /// tuples::tuple_size
    template <typename T>
    struct tuple_size {};

    template <typename T>
    struct tuple_size<const T> : tuple_size<T> {};

    /// tuples::tuple_size<RawArray>
    template <typename T, std::size_t N>
    struct tuple_size<T[N]> {
        inline static constexpr std::size_t value = N;
    };

    /// tuples::tuple_size_v
    // for universal metaprogramming, use tuples::tuple_size_v
    // instead of using tuples::tuple_size<T>::value
    template <typename T>
        requires (gold::complete_type<std::tuple_size<T>>
              && requires { std::tuple_size<T>::value; })
              || requires { tuple_size<T>::value; }
    inline constexpr std::size_t tuple_size_v = [] {
        if constexpr (requires { tuple_size<T>::value; })
            return tuple_size<T>::value;
        else
            return std::tuple_size<T>::value;
    }();

    namespace __tuples {

        /// tuples::__tuples::viable_tuple_size
        template <typename T>
        concept viable_tuple_size = requires { tuples::tuple_size_v<T>; };

        /// tuples::__tuples::decayed_tuple_size_
        template <typename T>
        inline constexpr std::size_t decayed_tuple_size = tuples::tuple_size_v<std::remove_cvref_t<T>>;

    } // namespace __cpo_access

} // namespace gold::tuples

namespace std {

    /// tuple_size
    template <typename T>
        requires requires {
            { gold::tuples::tuple_size<T>::value } -> std::convertible_to<std::size_t>;
        }
    struct tuple_size<T>
    : std::integral_constant<std::size_t, gold::tuples::tuple_size<T>::value> {};

} // namespace std

#endif // __GOLD_BITS_TUPLES_TUPLE_SIZE_HPP
