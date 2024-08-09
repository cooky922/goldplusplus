// <gold/bits/algo/synth3way.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ALGO_SYNTH3WAY_HPP
#define __GOLD_BITS_ALGO_SYNTH3WAY_HPP

#include <compare>

namespace gold::__algo {

    /// __algo::synth3way
    inline constexpr auto synth3way = std::__detail::__synth3way;

    /// __algo::synth3way_t
    template <typename T, typename U = T>
    using synth3way_t = std::__detail::__synth3way_t<T, U>;

} // namespace gold::__algo

#endif // __GOLD_BITS_ALGO_SYNTH3WAY_HPP
