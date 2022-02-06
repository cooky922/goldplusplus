// <gold/bits/value_sequence/primitives.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_VALUE_SEQUENCE_PRIMITIVES_HPP
#define __GOLD_BITS_VALUE_SEQUENCE_PRIMITIVES_HPP

#include <concepts>
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

    ///// Variable Templates
    /// homo_value_sequence_v
    template <typename T, T... Is>
    inline constexpr homo_value_sequence<T, Is...> homo_value_sequence_v {};

    /// integer_sequence_v
    template <typename T, T... Is>
    inline constexpr std::integer_sequence<T, Is...> integer_sequence_v {};

    /// index_sequence_v
    template <std::size_t... Is>
    inline constexpr std::index_sequence<Is...> index_sequence_v {};

    /// int_sequence_v
    template <int... Is>
    inline constexpr int_sequence<Is...> int_sequence_v {};

    /// bool_sequence_v
    template <bool... Is>
    inline constexpr bool_sequence<Is...> bool_sequence_v {};

    /// char_sequence_v
    template <char... Is>
    inline constexpr char_sequence<Is...> char_sequence_v {};

    /// floating_point_sequence_v
    template <typename T, T... Is>
    inline constexpr floating_point_sequence<T, Is...> floating_point_sequence_v {};

    /// float_sequence_v
    template <float... Is>
    inline constexpr float_sequence<Is...> float_sequence_v {};

    /// double_sequence_v
    template <double... Is>
    inline constexpr double_sequence<Is...> double_sequence_v {};

    /// long_double_sequence_v
    template <long double... Is>
    inline constexpr long_double_sequence<Is...> long_double_sequence_v {};

    /// hetero_value_sequenve_v
    template <auto... Is>
    inline constexpr hetero_value_sequence<Is...> hetero_value_sequence_v {};

    /// is_value_sequence_v
    template <typename>
    inline constexpr bool is_value_sequence_v = false;

    template <auto... Is>
    inline constexpr bool is_value_sequence_v<hetero_value_sequence<Is...>> = true;

    template <typename T, T... Is>
    inline constexpr bool is_value_sequence_v<homo_value_sequence<T, Is...>> = true;

    template <typename T, T... Is>
    inline constexpr bool is_value_sequence_v<std::integer_sequence<T, Is...>> = true;

    template <typename T, T... Is>
    inline constexpr bool is_value_sequence_v<floating_point_sequence<T, Is...>> = true;

} // namespace gold

#endif // __GOLD_BITS_VALUE_SEQUENCE_PRIMITIVES_HPP
