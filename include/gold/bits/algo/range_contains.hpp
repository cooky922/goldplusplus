// <gold/bits/algo/range_contains.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ALGO_RANGE_CONTAINS_HPP
#define __GOLD_BITS_ALGO_RANGE_CONTAINS_HPP

#include <gold/bits/ranges/range_primitives.hpp>
#include <bits/ranges_util.h>

namespace gold::ranges {

    namespace __ranges {

        /// ranges::__ranges::contains_fn
        struct contains_fn {
            template <std::input_iterator Iter,
                      std::sentinel_for<Iter> Sent,
                      typename T,
                      typename Proj = std::identity>
                requires std::indirect_binary_predicate<
                    std::ranges::equal_to,
                    std::projected<Iter, Proj>,
                    const T*
                >
            static constexpr bool operator()(Iter iter, Sent sent, const T& val, Proj proj = {}) {
                return std::ranges::find(iter, sent, val, proj) != sent;
            }

            template <std::ranges::input_range R,
                      typename T,
                      typename Proj = std::identity>
                requires std::indirect_binary_predicate<
                    std::ranges::equal_to,
                    std::projected<std::ranges::iterator_t<R>, Proj>,
                    const T*
                >
            static constexpr bool operator()(R&& r, const T& val, Proj proj = {}) {
                return operator()(std::ranges::begin(r),
                                  std::ranges::end(r),
                                  val,
                                  std::move(proj));
            }
        };

        /// ranges::__ranges::contains_subrange_fn
        struct contains_subrange_fn {
            template <std::forward_iterator Iter1,
                      std::sentinel_for<Iter1> Sent1,
                      std::forward_iterator Iter2,
                      std::sentinel_for<Iter2> Sent2,
                      class Pred = std::ranges::equal_to,
                      class Proj1 = std::identity,
                      class Proj2 = std::identity>
                requires std::indirectly_comparable<Iter1, Iter2, Pred, Proj1, Proj2>
            static constexpr bool operator()(Iter1 iter1, Sent1 sent1,
                                      Iter2 iter2, Sent2 sent2,
                                      Pred pred = {},
                                      Proj1 proj1 = {},
                                      Proj2 proj2 = {}) {
                return std::ranges::search(
                    iter1, sent1, iter2, sent2,
                    pred, proj1, proj2
                ).empty() == false;
            }

            template <std::ranges::forward_range R1, std::ranges::forward_range R2,
                      class Pred = std::ranges::equal_to,
                      class Proj1 = std::identity,
                      class Proj2 = std::identity>
                requires std::indirectly_comparable<std::ranges::iterator_t<R1>,
                                                    std::ranges::iterator_t<R2>,
                                                    Pred, Proj1, Proj2>
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

    /// ranges::contains
    inline constexpr __ranges::contains_fn contains {};

    /// ranges::contains_subrange
    inline constexpr __ranges::contains_subrange_fn contains_subrange {};

} // namespace gold::ranges

#endif // __GOLD_BITS_ALGO_RANGE_CONTAINS_HPP
