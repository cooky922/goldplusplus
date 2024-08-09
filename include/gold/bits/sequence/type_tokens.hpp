// <gold/bits/sequence/type_tokens.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_SEQUENCE_TYPE_TOKENS_HPP
#define __GOLD_BITS_SEQUENCE_TYPE_TOKENS_HPP

#include <cstddef>
#include <compare>

namespace gold {

    /// index_token_t
    template <std::size_t I>
    struct index_token_t {

        /// ctor
        consteval explicit index_token_t() noexcept = default;

        /// ::value
        inline static constexpr std::size_t value = I;

        /// ::operator==
        template <std::size_t J>
        friend consteval bool operator==(index_token_t, index_token_t<J>) {
            return I == J;
        }

        /// ::operator<=>
        template <std::size_t J>
        friend consteval std::strong_ordering operator<=>(index_token_t, index_token_t<J>) {
            return I <=> J;
        }
    };

    /// index_token
    template <std::size_t I>
    inline constexpr index_token_t<I> index_token {};

    /// sentinel_token_t
    struct sentinel_token_t {

        /// ctor
        consteval explicit sentinel_token_t() noexcept = default;

        /// ::operator==
        friend consteval bool operator==(sentinel_token_t, sentinel_token_t) {
            return true;
        }

        template <std::size_t I>
        friend consteval bool operator==(sentinel_token_t, index_token_t<I>) {
            return false;
        }

        /// ::operator<=>
        template <std::size_t I>
        friend consteval std::partial_ordering operator<=>(sentinel_token_t, index_token_t<I>) {
            return std::partial_ordering::unordered;
        }
    };

    /// sentinel_token
    inline constexpr sentinel_token_t sentinel_token {};

} // namespace gold

#endif // __GOLD_BITS_SEQUENCE_TYPE_TOKENS_HPP
