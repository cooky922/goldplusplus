// <gold/bits/algo/rotate.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ALGO_ROTATE_HPP
#define __GOLD_BITS_ALGO_ROTATE_HPP

#include <gold/bits/algo/reverse.hpp>

namespace gold::__algo {

    /// __algo::rotate
    // only defined for random_access_iterator as for now
    template <typename Iter, typename Sent>
    constexpr void rotate(Iter iter, Iter middle, Sent sent) {
        if (iter == middle || middle == sent)
            return;
        __algo::reverse(iter, middle);
        auto last_it = std::ranges::next(iter, sent);
        __algo::reverse(middle, last_it);
        __algo::reverse(iter, last_it);
    }

} // namespace gold::__algo

#endif // __GOLD_BITS_ALGO_ROTATE_HPP
