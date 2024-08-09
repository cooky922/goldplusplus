// <gold/bits/tuples/fwd.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TUPLES_FWD_HPP
#define __GOLD_BITS_TUPLES_FWD_HPP

#include <cstddef>

namespace gold::tuples {

    /// tuples::tuple_size [fwd]
    template <typename>
    struct tuple_size;

    /// tuples::tuple_element [fwd]
    template <std::size_t, typename>
    struct tuple_element;

    /// tuples::enable_array_like
    template <typename>
    inline constexpr bool enable_array_like = false;

} // namespace gold::tuples

#endif // __GOLD_BITS_TUPLES_FWD_HPP
