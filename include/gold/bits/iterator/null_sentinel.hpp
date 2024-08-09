// <gold/bits/iterator/null_sentinel.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ITERATOR_NULL_SENTINEL_HPP
#define __GOLD_BITS_ITERATOR_NULL_SENTINEL_HPP

#include <bits/iterator_concepts.h>

namespace gold {

    /// null_sentinel_t
    struct null_sentinel_t {
        constexpr null_sentinel_t base() const noexcept { return {}; }

        template <std::input_iterator Iter>
            requires std::default_initializable<std::iter_value_t<Iter>> &&
                     std::equality_comparable_with<std::iter_reference_t<Iter>, std::iter_value_t<Iter>>
        friend constexpr bool operator==(Iter iter, null_sentinel_t) {
            return *iter == std::iter_value_t<Iter> {};
        }
    };

    /// null_sentinel
    inline constexpr null_sentinel_t null_sentinel {};

} // namespace gold

#endif // __GOLD_BITS_ITERATOR_NULL_SENTINEL_HPP
