// <gold/bits/sequence/make_value_seq_from.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_SEQUENCE_MAKE_VALUE_SEQ_FROM_HPP
#define __GOLD_BITS_SEQUENCE_MAKE_VALUE_SEQ_FROM_HPP

#include <gold/bits/sequence/value_seq.hpp>
#include <array>

namespace gold {

    /// make_array_from_sequence
    template <gold::value_sequence Seq>
        requires (!__value_seq::is_empty<Seq>) &&
                   __value_seq::is_homogeneous<Seq>
    consteval auto make_array_from_sequence() {
        if constexpr (__value_seq::is_std_seq<Seq>) {
            return []<auto... Ns>(std::integer_sequence<typename Seq::value_type, Ns...>) {
                return std::array { Ns... };
            }(Seq{});
        } else {
            return []<auto... Ns>(gold::value_seq_t<Ns...>) {
                return std::array { Ns... };
            }(Seq{});
        }
    }

    template <gold::value_sequence auto Seq>
    consteval auto make_array_from_sequence() {
        return gold::make_array_from_sequence<decltype(Seq)>();
    }

    /// make_sequence_from_array
    template <auto Array>
    consteval gold::value_sequence auto make_sequence_from_array() {
        using T = __remove_reference(decltype(*__value_seq::from_mini_array_impl<Array>()));
        return T{};
    }

} // namespace gold

#endif // __GOLD_BITS_SEQUENCE_MAKE_VALUE_SEQ_FROM_HPP
