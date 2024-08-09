// <gold/bits/algo/range_starts_with.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ALGO_RANGE_STARTS_WITH_HPP
#define __GOLD_BITS_ALGO_RANGE_STARTS_WITH_HPP

#include <gold/bits/ranges/range_primitives.hpp>
#include <bits/ranges_util.h>

namespace gold::ranges {

    namespace __ranges {

        /// ranges::__ranges::starts_with_fn
        struct starts_with_fn {
            template <std::input_iterator Iter1,
                      std::sentinel_for<Iter1> Sent1,
                      std::input_iterator Iter2,
                      std::sentinel_for<Iter2> Sent2,
                      typename Pred = std::ranges::equal_to,
                      typename Proj1 = std::identity,
                      typename Proj2 = std::identity>
                requires std::indirectly_comparable<
                    Iter1, Iter2, Pred, Proj1, Proj2
                >
            static constexpr bool operator()(Iter1 iter1, Sent1 sent1,
                                      Iter2 iter2, Sent2 sent2,
                                      Pred pred = {},
                                      Proj1 proj1 = {},
                                      Proj2 proj2 = {}) {
                return std::ranges::mismatch(
                    std::move(iter1), sent1,
                    std::move(iter2), sent2,
                    std::move(pred), std::move(proj1), std::move(proj2)
                ).in2 == sent2;
            }

            template <std::ranges::input_range R1,
                      std::ranges::input_range R2,
                      typename Pred = std::ranges::equal_to,
                      typename Proj1 = std::identity,
                      typename Proj2 = std::identity>
                requires std::indirectly_comparable<
                    std::ranges::iterator_t<R1>,
                    std::ranges::iterator_t<R2>,
                    Pred, Proj1, Proj2
                >
            static constexpr bool operator()(R1&& r1, R2&& r2,
                                      Pred pred = {},
                                      Proj1 proj1 = {},
                                      Proj2 proj2 = {}) {
                return operator()(std::ranges::begin(r1), std::ranges::end(r1),
                                  std::ranges::begin(r2), std::ranges::end(r2),
                                  std::move(pred), std::move(proj1), std::move(proj2));
            }
        };

    } // namespace __ranges

    /// ranges::starts_with
    inline constexpr __ranges::starts_with_fn starts_with {};

} // namespace gold::ranges

#endif // __GOLD_BITS_ALGO_RANGE_STARTS_WITH_HPP
