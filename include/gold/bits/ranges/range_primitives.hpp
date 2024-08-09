// <gold/bits/ranges/range_primitives.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RANGES_RANGE_PRIMITIVES_HPP
#define __GOLD_BITS_RANGES_RANGE_PRIMITIVES_HPP

#include <bits/ranges_base.h>

namespace gold::ranges {

//    /// ranges::iterator_t
//    template <typename R>
//    using iterator_t = std::ranges::iterator_t<R>;
//
//    /// ranges::const_iterator_t
//    template <typename R>
//    using const_iterator_t = gold::const_iterator<iterator_t<R>>;
//
//    /// ranges::sentinel_t
//    template <std::ranges::range R>
//    using sentinel_t = std::ranges::sentinel_t<R>;
//
//    /// ranges::const_sentinel_t
//    template <std::ranges::range R>
//    using const_sentinel_t = gold::const_sentinel<sentinel_t<R>>;
//
//    /// ranges::range_size_t
//    template <std::ranges::sized_range R>
//    using range_size_t = std::ranges::range_size_t<R>;
//
//    /// ranges::range_diff_t
//    template <std::ranges::range R>
//    using range_diff_t = std::ranges::range_difference_t<R>;
//
//    /// ranges::range_val_t
//    template <std::ranges::range R>
//    using range_val_t = std::ranges::range_value_t<R>;
//
//    /// ranges::range_ref_t
//    template <std::ranges::range R>
//    using range_ref_t = std::ranges::range_reference_t<R>;
//
//    /// ranges::range_cref_t
//    template <std::ranges::range R>
//    using range_cref_t = gold::iter_cref_t<iterator_t<R>>;
//
//    /// ranges::range_rref_t
//    template <std::ranges::range R>
//    using range_rref_t = std::ranges::range_rvalue_reference_t<R>;
//
//    /// ranges::range_common_ref_t
//    template <std::ranges::range R>
//    using range_common_ref_t = std::iter_common_reference_t<iterator_t<R>>;
//
//    /// ranges::constant_range
//    template <typename R>
//    concept constant_range = std::ranges::input_range<R>
//        && __iterator::qualified_constant_iterator<iterator_t<R>>;

} // namespace gold::ranges

#endif // __GOLD_BITS_RANGES_RANGE_PRIMITIVES_HPP
