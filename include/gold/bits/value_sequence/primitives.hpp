// <gold/bits/value_sequence/primitives.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_VALUE_SEQUENCE_PRIMITIVES_HPP
#define __GOLD_BITS_VALUE_SEQUENCE_PRIMITIVES_HPP

#include <bits/utility.h>

namespace gold {

    ///// Homogeneous Data Sequence

    /// homo_value_sequence [general version]
    template <typename T, T... Is>
    struct homo_value_sequence {
        using value_type = T;
        static constexpr std::size_t size() { return sizeof...(Is); }
    };

    /// bool_sequence
    template <bool... Bs>
    using bool_sequence = std::integer_sequence<bool, Bs...>;

    /// char_sequence
    template <char... Cs>
    using char_sequence = std::integer_sequence<char, Cs...>;

    /// int_sequence
    template <int... Is>
    using int_sequence = std::integer_sequence<int, Is...>;

    /// floating point sequence
    template <std::floating_point F, F... Fs>
    struct floating_point_sequence {
        using value_type = F;
        static constexpr std::size_t size() noexcept { return sizeof...(Fs); }
    };

    /// float_sequence
    template <float... Fs>
    using float_sequence = floating_point_sequence<float, Fs...>;

    /// double_sequence
    template <double... Fs>
    using double_sequence = floating_point_sequence<double, Fs...>;

    /// long_double_sequence
    template <long double... Fs>
    using long_double_sequence = floating_point_sequence<long double, Fs...>;

    ///// Heterogeneous Data Sequence

    /// hetero_value_sequence
    template <auto... Is>
    struct hetero_value_sequence {
        static constexpr std::size_t size() { return sizeof...(Is); }
    };

} // namespace gold

#endif // __GOLD_BITS_VALUE_SEQUENCE_PRIMITIVES_HPP
