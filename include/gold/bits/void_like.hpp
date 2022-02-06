// <gold/bits/void_like.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_VOID_LIKE_HPP
#define __GOLD_BITS_VOID_LIKE_HPP

namespace gold {

    /// void_like
    struct void_like {
        constexpr void_like(...) {}
        constexpr void_like operator()(...) const noexcept { return {}; }
    };

    /// void_like_v
    inline constexpr void_like void_like_v {};

} // namespace gold

#endif // __GOLD_BITS_VOID_LIKE_HPP
