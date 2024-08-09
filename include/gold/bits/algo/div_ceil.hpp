// <gold/bits/algo/div_ceil.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ALGO_DIV_CEIL_HPP
#define __GOLD_BITS_ALGO_DIV_CEIL_HPP

namespace gold::__algo {

    /// __algo::div_ceil
    template <typename I>
    constexpr I div_ceil(I num, I denom) noexcept {
        I r = num / denom;
        if (num % denom)
            ++r;
        return r;
    }

} // namespace gold::__algo

#endif // __GOLD_BITS_ALGO_DIV_CEIL_HPP
