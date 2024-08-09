// <gold/bits/ranges/from_range.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RANGES_FROM_RANGE_HPP
#define __GOLD_BITS_RANGES_FROM_RANGE_HPP

namespace gold {

    /// from_range_t
    struct from_range_t {
        constexpr explicit from_range_t() noexcept = default;
    };

    /// from_range
    inline constexpr from_range_t from_range {};

} // namespace gold

#endif // __GOLD_BITS_RANGES_FROM_RANGE_HPP
