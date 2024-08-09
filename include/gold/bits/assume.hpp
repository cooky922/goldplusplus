// <gold/bits/assume.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ASSUME_HPP
#define __GOLD_BITS_ASSUME_HPP

namespace gold {

    namespace __utility {

        /// __utility::assume_failure
        inline void assume_failure() {}

    } // namespace __utility

    /// assume
    [[gnu::always_inline]] inline constexpr void assume(bool b) noexcept {
        if consteval {
            if (!b)
                __utility::assume_failure();
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
