// <gold/bits/algo/range_is_uniqued.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ALGO_RANGE_IS_UNIQUED_HPP
#define __GOLD_BITS_ALGO_RANGE_IS_UNIQUED_HPP

#include <gold/bits/ranges/range_primitives.hpp>
#include <bits/ranges_util.h>

namespace gold::ranges {

    namespace __ranges {

        /// ranges::__ranges::is_uniqued_fn
        struct is_uniqued_fn {
            template <std::forward_iterator I, std::sentinel_for<I> S, typename Proj = std::identity,
                      std::indirect_binary_predicate<
                        std::projected<I, Proj>, std::projected<I, Proj>
                      > Pred = std::ranges::equal_to>
            static constexpr bool operator()(I first, S last, Pred pred = {}, Proj proj = {}) {
                return std::ranges::adjacent_find(first, last, pred, proj) == last;
            }

            template <std::ranges::forward_range R, typename Proj = std::identity,
                      std::indirect_binary_predicate<
                        std::projected<std::ranges::iterator_t<R>, Proj>,
                        std::projected<std::ranges::iterator_t<R>, Proj>
                      > Pred = std::ranges::equal_to>
            static constexpr bool operator()(R&& r, Pred pred = {}, Proj proj = {}) {
                return operator()(std::ranges::begin(r), std::ranges::end(r),
                                  std::move(pred),
                                  std::move(proj));
            }
        };

    } // namespace __ranges

    /// ranges::is_uniqued
    inline constexpr __ranges::is_uniqued_fn is_uniqued {};

} // namespace gold::ranges

#endif // __GOLD_BITS_ALGO_RANGE_CONTAINS_HPP
