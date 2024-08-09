// <gold/bits/algo/range_ends_with.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ALGO_RANGE_ENDS_WITH_HPP
#define __GOLD_BITS_ALGO_RANGE_ENDS_WITH_HPP

#include <gold/bits/ranges/range_primitives.hpp>
#include <bits/stl_algobase.h>
#include <bits/ranges_algobase.h>

namespace gold::ranges {

    namespace __ranges {

        /// ranges::__ranges::ends_with_fn
        struct ends_with_fn {
            template <std::input_iterator Iter1,
                      std::sentinel_for<Iter1> Sent1,
                      std::input_iterator Iter2,
                      std::sentinel_for<Iter2> Sent2,
                      typename Pred = std::ranges::equal_to,
                      typename Proj1 = std::identity,
                      typename Proj2 = std::identity>
                requires (std::forward_iterator<Iter1> || std::sized_sentinel_for<Sent1, Iter1>)
                      && (std::forward_iterator<Iter2> || std::sized_sentinel_for<Sent2, Iter2>)
                      && std::indirectly_comparable<
                        Iter1, Iter2, Pred, Proj1, Proj2
                      >
            static constexpr bool operator()(Iter1 iter1, Sent1 sent1,
                                      Iter2 iter2, Sent2 sent2,
                                      Pred pred = {},
                                      Proj1 proj1 = {},
                                      Proj2 proj2 = {}) {
                const auto n1 = std::ranges::distance(iter1, sent1);
                const auto n2 = std::ranges::distance(iter2, sent2);
                if (n1 < n2)
                    return false;
                std::ranges::advance(iter1, n1 - n2);
                return std::ranges::equal(
                    std::move(iter1), std::move(sent1),
                    std::move(iter2), std::move(sent2),
                    std::move(pred),
                    std::move(proj1), std::move(proj1)
                );
            }

            template <std::ranges::input_range R1,
                      std::ranges::input_range R2,
                      typename Pred = std::ranges::equal_to,
                      typename Proj1 = std::identity,
                      typename Proj2 = std::identity>
                requires (std::ranges::forward_range<R1> || std::ranges::sized_range<R1>)
                      && (std::ranges::forward_range<R2> || std::ranges::sized_range<R2>)
                      && std::indirectly_comparable<
                        std::ranges::iterator_t<R1>, std::ranges::iterator_t<R2>,
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

    /// ranges::ends_with
    inline constexpr __ranges::ends_with_fn ends_with {};

} // namespace gold::ranges

#endif // __GOLD_BITS_ALGO_RANGE_ENDS_WITH_HPP
