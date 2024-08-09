// <gold/bits/algo/range_lex_compare.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ALGO_RANGE_LEX_COMPARE_HPP
#define __GOLD_BITS_ALGO_RANGE_LEX_COMPARE_HPP

#include <compare>
#include <bits/refwrap.h>
#include <gold/bits/concepts/types.hpp>
#include <gold/bits/functional/invoke.hpp>
#include <gold/bits/iterator/iter_primitives.hpp>
#include <gold/bits/ranges/range_primitives.hpp>

namespace gold::ranges {

    namespace __ranges {

        /// ranges::__ranges::indirectly_binary_invocable
        template <typename F, typename Iter1, typename Iter2>
        concept indirectly_binary_invocable =
            std::indirectly_readable<Iter1> &&
            std::indirectly_readable<Iter2> &&
            std::invocable<F&, std::iter_value_t<Iter1>&, std::iter_value_t<Iter2>&> &&
            std::invocable<F&, std::iter_value_t<Iter1>&, std::iter_reference_t<Iter2>> &&
            std::invocable<F&, std::iter_reference_t<Iter1>, std::iter_value_t<Iter2>&> &&
            std::invocable<F&, std::iter_reference_t<Iter1>, std::iter_reference_t<Iter2>> &&
            std::invocable<F&, std::iter_common_reference_t<Iter1>, std::iter_common_reference_t<Iter2>> &&
            std::common_reference_with<
                std::invoke_result_t<F&, std::iter_value_t<Iter1>&, std::iter_value_t<Iter2>&>,
                std::invoke_result_t<F&, std::iter_reference_t<Iter1>, std::iter_reference_t<Iter2>>
            > &&
            std::common_reference_with<
                std::invoke_result_t<F&, std::iter_reference_t<Iter1>, std::iter_value_t<Iter2>&>,
                std::invoke_result_t<F&, std::iter_value_t<Iter1>&, std::iter_reference_t<Iter2>>
            >;

        /// ranges::__ranges::indirectly_three_way_comparable
        template <typename F, typename Iter1, typename Iter2 = Iter1>
        concept indirectly_three_way_comparable =
            std::indirectly_readable<Iter1> &&
            std::indirectly_readable<Iter2> &&
            std::copy_constructible<F> &&
            indirectly_binary_invocable<F&, Iter1, Iter2> &&
            gold::ordering_type<std::decay_t<std::indirect_result_t<F&, Iter1, Iter2>>>;

        /// ranges::__ranges::lexicographical_compare_three_way_fn
        struct lexicographical_compare_three_way_fn {
            template <std::input_iterator Iter1,
                      std::sentinel_for<Iter1> Sent1,
                      std::input_iterator Iter2,
                      std::sentinel_for<Iter2> Sent2,
                      typename Proj1 = std::identity,
                      typename Proj2 = std::identity,
                      indirectly_three_way_comparable<
                        std::projected<Iter1, Proj1>,
                        std::projected<Iter2, Proj2>
                      > Comp = std::compare_three_way,
                      typename R = std::indirect_result_t<Comp, Iter1, Iter2>>
            static constexpr R operator()(Iter1 iter1, Sent1 sent1,
                                   Iter2 iter2, Sent2 sent2,
                                   Comp comp = {},
                                   Proj1 proj1 = {},
                                   Proj2 proj2 = {}) {
                bool exhaust1 = (iter1 == sent1);
                bool exhaust2 = (iter2 == sent2);
                for (; !exhaust1 && !exhaust2; exhaust1 = (++iter1 == sent1), exhaust2 = (++iter2 == sent2))
                    if (auto c = gold::invoke(comp, gold::invoke(proj1, *iter1),
                                                    gold::invoke(proj2, *iter2)); c != 0)
                        return c;
                return !exhaust1 ? std::strong_ordering::greater :
                       !exhaust2 ? std::strong_ordering::less :
                                   std::strong_ordering::equal;
            }

            template <std::ranges::input_range R1,
                      std::ranges::input_range R2,
                      typename Proj1 = std::identity,
                      typename Proj2 = std::identity,
                      indirectly_three_way_comparable<
                        std::projected<std::ranges::iterator_t<R1>, Proj1>,
                        std::projected<std::ranges::iterator_t<R2>, Proj2>
                      > Comp = std::compare_three_way,
                      typename R = std::indirect_result_t<Comp,
                        std::ranges::iterator_t<R1>,
                        std::ranges::iterator_t<R2>
                      >>
            static constexpr R operator()(R1&& r1, R2&& r2,
                                   Comp comp = {},
                                   Proj1 proj1 = {},
                                   Proj2 proj2 = {}) {
                return operator()(std::ranges::begin(r1), std::ranges::end(r1),
                                  std::ranges::begin(r2), std::ranges::end(r2),
                                  std::ref(comp), std::ref(proj1), std::ref(proj2));
            }
        };

    } // namespace __ranges

    /// ranges::lexicographical_compare_three_way
    inline constexpr __ranges::lexicographical_compare_three_way_fn lexicographical_compare_three_way {};

} // namespace gold::ranges

#endif // __GOLD_BITS_ALGO_RANGE_LEX_COMPARE_HPP
