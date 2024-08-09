// <gold/bits/void_like.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_VOID_LIKE_HPP
#define __GOLD_BITS_VOID_LIKE_HPP

namespace gold {

    /// void_like
    struct void_like {
        [[gnu::always_inline]] constexpr void_like(auto&&...) noexcept {}
        [[gnu::always_inline]] static constexpr void_like operator()(auto&&...) noexcept { return {}; }
    };

    /// void_like_v
    inline constexpr void_like void_like_v {};

} // namespace gold

#endif // __GOLD_BITS_VOID_LIKE_HPP
