// <gold/bits/algo/copy.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ALGO_COPY_HPP
#define __GOLD_BITS_ALGO_COPY_HPP

#include <bits/move.h>

namespace gold::__algo {

    /// __algo::copy
    // unconstrained copy algorithm
    template <typename R, typename Out>
    constexpr Out copy(R&& r, Out out) {
        for (auto&& elem : std::forward<R>(r)) {
            if constexpr (requires { out++; })
                *out++ = std::forward<decltype(elem)>(elem);
            else {
                *out = std::forward<decltype(elem)>(elem);
                (void) ++out;
            }
        }
        return out;
    }

    template <typename In, typename Sent, typename Out>
    constexpr Out copy(In in, Sent sent, Out out) {
        for (; in != sent; ) {
            if constexpr (requires { in++; out++; })
                *out++ = *in++;
            else {
                *out = *in;
                (void) ++out, ++in;
            }
        }
        return out;
    }

} // namespace gold::__algo

#endif // __GOLD_BITS_ALGO_COPY_HPP
