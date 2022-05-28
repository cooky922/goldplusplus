// <gold/bits/discard_unused.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_DISCARD_UNUSED_HPP
#define __GOLD_BITS_DISCARD_UNUSED_HPP

namespace gold {

    /// discard_unused
    [[gnu::always_inline]] inline
    constexpr void discard_unused(...) noexcept {}

    template <typename...>
    [[gnu::always_inline]] inline
    constexpr void discard_unused() noexcept {}

} // namespace gold

#endif // __GOLD_BITS_DISCARD_UNUSED_HPP
