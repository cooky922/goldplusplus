// <gold/bits/value_sequence/factory.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_VALUE_SEQUENCE_FACTORY_HPP
#define __GOLD_BITS_VALUE_SEQUENCE_FACTORY_HPP

#include <gold/bits/value_sequence/integer_seq_impl.hpp>

namespace gold {

    /// make_consecutive_integer_sequence
    template <typename T, auto Begin, auto Count, auto Stride = 1>
    requires __requirements::make_consecutive_sequence_requires_<T, Begin, Count, Stride>
    using make_consecutive_integer_sequence
    = decltype(__detail::make_consecutive_integer_sequence_impl_<T, Begin, Count, Stride>());

    /// make_consecutive_index_sequence
    template <auto Begin, auto Count, auto Stride = 1>
    requires __requirements::make_consecutive_index_sequence_requires_<Begin, Count, Stride>
    using make_consecutive_index_sequence
    = make_consecutive_integer_sequence<std::size_t, Begin, Count, Stride>;

    /// make_consecutive_int_sequence
    template <auto Begin, auto Count, auto Stride = 1>
    requires __requirements::make_consecutive_sequence_requires_<int, Begin, Count, Stride>
    using make_consecutive_int_sequence
    = make_consecutive_integer_sequence<int, Begin, Count, Stride>;

    /// make_reverse_integer_sequence
    template <typename T, T N>
    requires __requirements::make_reverse_sequence_requires_<T, N>
    using make_reverse_integer_sequence
    = make_consecutive_integer_sequence<T, N - 1, N, -1>;

    /// make_reverse_index_sequence
    template <std::size_t N>
    requires __requirements::make_reverse_sequence_requires_<std::size_t, N>
    using make_reverse_index_sequence
    = make_reverse_integer_sequence<std::size_t, N>;

    /// make_reverse_int_sequence
    template <int N>
    requires __requirements::make_reverse_sequence_requires_<int, N>
    using make_reverse_int_sequence
    = make_reverse_integer_sequence<int, N>;

    /// reverse_index_sequence_for
    template <typename... Ts>
    using reverse_index_sequence_for
    = make_reverse_index_sequence<sizeof...(Ts)>;

    /// make_interval_integer_sequence
    template <typename T, auto Begin, auto End, auto Stride = (Begin < End ? 1 : -1)>
    requires __requirements::make_interval_sequence_requires_<T, Begin, End, Stride>
    using make_interval_integer_sequence
    = decltype(__detail::make_interval_integer_sequence_impl_<T, Begin, End, Stride>());

    /// make_interval_index_sequence
    template <auto Begin, auto End, auto Stride = (Begin < End ? 1 : -1)>
    requires __requirements::make_interval_sequence_requires_<std::size_t, Begin, End, Stride>
    using make_interval_index_sequence
    = make_interval_integer_sequence<std::size_t, Begin, End, Stride>;

    /// make_interval_int_sequence
    template <auto Begin, auto End, auto Stride = (Begin < End ? 1 : -1)>
    requires __requirements::make_interval_sequence_requires_<int, Begin, End, Stride>
    using make_interval_int_sequence
    = make_interval_integer_sequence<int, Begin, End, Stride>;

} // namespace gold

#endif // __GOLD_BITS_VALUE_SEQUENCE_FACTORY_HPP
