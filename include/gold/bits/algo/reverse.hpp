// <gold/bits/algo/reverse.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ALGO_REVERSE_HPP
#define __GOLD_BITS_ALGO_REVERSE_HPP

#include <bits/move.h>
#include <bits/ranges_base.h>

namespace gold::__algo {

    /// __algo::reverse
    template <typename Iter, typename Sent>
    constexpr void reverse(Iter iter, Sent sent) {
        auto last_2 = std::ranges::next(iter, sent);
        for (auto tail = last_2; iter != tail && iter != --tail; ++iter)
            std::ranges::iter_swap(iter, tail);
    }

    template <typename R>
    constexpr void reverse(R&& r) {
        __algo::reverse(std::ranges::begin(r), std::ranges::end(r));
    }

} // namespace gold::__algo

#endif // __GOLD_BITS_ALGO_REVERSE_HPP
