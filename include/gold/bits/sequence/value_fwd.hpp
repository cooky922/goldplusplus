// <gold/bits/sequence/value_fwd.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_SEQUENCE_VALUE_FWD_HPP
#define __GOLD_BITS_SEQUENCE_VALUE_FWD_HPP

#include <concepts>

namespace gold {

    /// value_seq_t [fwd]
    template <auto...>
    struct value_seq_t;

    /// integer_seq_t [fwd]
    template <std::integral T, T...>
    struct integer_seq_t;

    /// index_seq_t
    template <std::size_t... Is>
    using index_seq_t = integer_seq_t<std::size_t, Is...>;

    /// int_seq_t
    template <int... Is>
    using int_seq_t = integer_seq_t<int, Is...>;

    /// char_seq_t
    template <char... Is>
    using char_seq_t = integer_seq_t<char, Is...>;

    /// bool_seq_t
    template <bool... Is>
    using bool_seq_t = integer_seq_t<bool, Is...>;

    /// floating_point_seq_t [fwd]
    template <std::floating_point T, T...>
    struct floating_point_seq_t;

    /// float_seq_t
    template <float... Fs>
    using float_seq_t = floating_point_seq_t<float, Fs...>;

} // namespace gold

#endif // __GOLD_BITS_SEQUENCE_VALUE_FWD_HPP
