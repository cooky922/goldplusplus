// <gold/bits/algo/move_range.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ALGO_MOVE_RANGE_HPP
#define __GOLD_BITS_ALGO_MOVE_RANGE_HPP

#include <bits/iterator_concepts.h>

namespace gold::__algo {

    /// __algo::move_range
    template <typename Iter, typename Sent, typename Out>
    constexpr Out move_range(Iter iter, Sent sent, Out out) {
        for (; iter != sent; ++iter, ++out)
            *out = std::ranges::iter_move(iter);
        return out;
    }

} // namespace gold::__algo

#endif // __GOLD_BITS_ALGO_MOVE_RANGE_HPP
