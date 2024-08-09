// <gold/bits/algo/equal.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ALGO_EQUAL_HPP
#define __GOLD_BITS_ALGO_EQUAL_HPP

#include <bits/move.h>
#include <bits/ranges_base.h>

namespace gold::__algo {

    /// __algo::equal
    template <typename R1, typename R2 = R1>
    constexpr bool equal(R1&& r1, R2&& r2) {
        if constexpr (std::ranges::sized_range<R1> && std::ranges::sized_range<R2>) {
            if (std::ranges::size(r1) != std::ranges::size(r2))
                return false;
        }
        auto iter1 = std::ranges::begin(r1);
        auto iter2 = std::ranges::begin(r2);
        auto sent1 = std::ranges::end(r1);
        auto sent2 = std::ranges::end(r2);
        for (; iter1 != sent1 && sent1 != sent2; ++iter1, ++iter2) {
            if (*iter1 != *iter2)
                return false;
        }
        return true;
    }

    /// __algo::equal_n
    template <typename R1, typename R2 = R1>
    constexpr bool equal_n(R1&& r1, R2&& r2, std::size_t n) {
        auto iter1 = std::ranges::begin(r1);
        auto iter2 = std::ranges::begin(r2);
        auto sent1 = std::ranges::end(r1);
        auto sent2 = std::ranges::end(r2);
        for (; n != 0 && iter1 != sent1 && sent1 != sent2; ++iter1, ++iter2, --n) {
            if (*iter1 != *iter2)
                return false;
        }
        return true;
    }

} // namespace gold::__algo

#endif // __GOLD_BITS_ALGO_EQUAL_HPP

