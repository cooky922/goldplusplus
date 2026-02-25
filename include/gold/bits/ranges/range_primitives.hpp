// <gold/bits/ranges/range_primitives.hpp> - gold++ library

// Copyright (C) [ 2021 - 2025 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RANGES_RANGE_PRIMITIVES_HPP
#define __GOLD_BITS_RANGES_RANGE_PRIMITIVES_HPP

#include <bits/ranges_base.h>
#include <gold/bits/constant_arg.hpp>

namespace gold::ranges {

    /// ranges::infinite_range
    // a range that is unbounded
    template <typename R>
    concept infinite_range =
        std::ranges::range<R> &&
        std::same_as<std::ranges::sentinel_t<R>, std::unreachable_sentinel_t>;

    /// ranges::nested_range
    template <typename R>
    concept nested_range =
        std::ranges::range<R> &&
        std::ranges::range<std::ranges::range_reference_t<R>>;

    /// ranges::statically_sized_range
    template <typename R>
    concept statically_sized_range =
        std::ranges::sized_range<R> &&
        requires (R& r) {
            { gold::constant_arg_t<std::ranges::size(r)>::value };
        };

    /// ranges::const_iterable_range
    // ex: auto a = views::iota(0, 10);
    //     static_assert(const_iterable_range<decltype(a)>);
    //     auto b = return_generator();
    //     static_assert(!const_iterable_range<decltype(b)>);
    template <typename R>
    concept const_iterable_range =
        std::ranges::range<R> && std::ranges::range<const R>;

} // namespace gold::ranges

#endif // __GOLD_BITS_RANGES_RANGE_PRIMITIVES_HPP
