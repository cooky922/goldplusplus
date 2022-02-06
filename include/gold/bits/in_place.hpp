// <gold/bits/in_place.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_IN_PLACE_HPP
#define __GOLD_BITS_IN_PLACE_HPP

#include <cstddef>

namespace gold {

    /// in_place_braced_t
    struct in_place_braced_t {
        constexpr explicit in_place_braced_t() = default;
    };

    /// in_place_braced
    inline constexpr in_place_braced_t in_place_braced {};

    /// in_place_type_braced_t
    template <typename>
    struct in_place_type_braced_t {
        constexpr explicit in_place_type_braced_t() = default;
    };

    /// in_place_type_braced
    template <typename T>
    inline constexpr in_place_type_braced_t<T> in_place_type_braced {};

    /// in_place_index_braced_t
    template <std::size_t>
    struct in_place_index_braced_t {
        constexpr explicit in_place_index_braced_t() = default;
    };

    /// in_place_index_braced
    template <std::size_t I>
    inline constexpr in_place_index_braced_t<I> in_place_index_braced {};

    /// in_place_value_t
    template <auto N>
    struct in_place_value_t {
        constexpr explicit in_place_value_t() = default;
    };

    /// in_place_value
    template <auto N>
    inline constexpr in_place_value_t<N> in_place_value {};

    /// in_place_value_braced_t
    template <auto N>
    struct in_place_value_braced_t {
        constexpr explicit in_place_value_braced_t() = default;
    };

    /// in_place_value_braced
    template <auto N>
    inline constexpr in_place_value_braced_t<N> in_place_value_braced {};


} // namespace gold

#endif // __GOLD_BITS_IN_PLACE_HPP
