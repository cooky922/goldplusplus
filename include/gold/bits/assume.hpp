// <gold/bits/assume.hpp> - gold++ library

// Copyright (C) [ 2021 - 2025 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ASSUME_HPP
#define __GOLD_BITS_ASSUME_HPP

namespace gold {

    namespace __utility {

        /// __utility::assumption_failure
        struct assumption_failure {};

    } // namespace __utility

    /// assume
    [[gnu::always_inline]] inline constexpr void assume(bool b) {
        if consteval {
            if (!b)
                throw __utility::assumption_failure();
        } else {
            if (!b) [[unlikely]]
                #ifndef NDEBUG
                __builtin_trap();
                #else
                __builtin_unreachable();
                #endif // NDEBUG
        }
    }

} // namespace gold

#endif // __GOLD_BITS_UNREACHABLE_HPP
