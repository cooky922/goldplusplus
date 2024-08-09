// <gold/bits/algo/range_find_last.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ALGO_RANGE_FIND_LAST_HPP
#define __GOLD_BITS_ALGO_RANGE_FIND_LAST_HPP

#include <bits/ranges_util.h>
#include <bits/refwrap.h>
#include <gold/bits/functional/invoke.hpp>
#include <gold/bits/ranges/range_primitives.hpp>

namespace gold::ranges {

    namespace __ranges {

        /// ranges::__ranges::find_last_fn
        struct find_last_fn {

            template <std::forward_iterator I, std::sentinel_for<I> S,
                      typename T, typename Proj = std::identity>
                requires std::indirect_binary_predicate<std::ranges::equal_to, std::projected<I, Proj>, const T*>
            static constexpr std::ranges::subrange<I> operator()(I first, S last, const T& value, Proj proj = {}) {
                if constexpr (std::bidirectional_iterator<I> && (std::assignable_from<I&, S> || std::sized_sentinel_for<S, I>)) {
                    I tail = std::ranges::next(first, last);
                    for (; first != tail;) {
                        if (gold::invoke(proj, *--tail) == value)
                            return {tail, last};
                    }
                    return {last, last};
                } else {
                    I found = std::ranges::find(first, last, value, proj);
                    if (found == last)
                        return {found, found};
                    first = found;
                    for (;;) {
                        first = std::ranges::find(std::ranges::next(first), last, value, proj);
                        if (first == last)
                            return {found, first};
                        found = first;
                    }
                }
            }

            template <std::ranges::forward_range R,
                      typename T, typename Proj = std::identity>
                requires std::indirect_binary_predicate<
                    std::ranges::equal_to,
                    std::projected<std::ranges::iterator_t<R>, Proj>,
                    const T*
                >
            static constexpr std::ranges::borrowed_subrange_t<R> operator()(R&& r, const T& value, Proj proj = {}) {
                return operator()(std::ranges::begin(r), std::ranges::end(r), value, std::ref(proj));
            }

        };

        /// ranges::__ranges::find_last_if_fn
        struct find_last_if_fn {

            template <std::forward_iterator I, std::sentinel_for<I> S,
                      typename Proj = std::identity,
                      std::indirect_unary_predicate<std::projected<I, Proj>> Pred>
            static constexpr std::ranges::subrange<I> operator()(I first, S last, Pred pred, Proj proj = {}) {
                if constexpr (std::bidirectional_iterator<I> && (std::assignable_from<I&, S> || std::sized_sentinel_for<S, I>)) {
                    I tail = std::ranges::next(first, last);
                    for (; first != tail;) {
                        if (gold::invoke(pred, gold::invoke(proj, *--tail)))
                            return {tail, last};
                    }
                    return {last, last};
                } else {
                    I found = std::ranges::find_if(first, last, pred, proj);
                    if (found == last)
                        return {found, found};
                    first = found;
                    for (;;) {
                        first = std::ranges::find_if(std::ranges::next(first), last, pred, proj);
                        if (first == last)
                            return {found, first};
                        found = first;
                    }
                }
            }

            template <std::ranges::forward_range R,
                      typename Proj = std::identity,
                      std::indirect_unary_predicate<std::projected<std::ranges::iterator_t<R>, Proj>> Pred>
            static constexpr std::ranges::borrowed_subrange_t<R> operator()(R&& r, Pred pred, Proj proj = {}) {
                return operator()(std::ranges::begin(r), std::ranges::end(r), std::ref(pred), std::ref(proj));
            }

        };

        /// ranges::__ranges::find_last_if_not_fn
        struct find_last_if_not_fn {

            template <std::forward_iterator I, std::sentinel_for<I> S,
                      typename Proj = std::identity,
                      std::indirect_unary_predicate<std::projected<I, Proj>> Pred>
            static constexpr std::ranges::subrange<I> operator()(I first, S last, Pred pred, Proj proj = {}) {
                if constexpr (std::bidirectional_iterator<I> && (std::assignable_from<I&, S> || std::sized_sentinel_for<S, I>)) {
                    I tail = std::ranges::next(first, last);
                    for (; first != tail;) {
                        if (!gold::invoke(pred, gold::invoke(proj, *--tail)))
                            return {tail, last};
                    }
                    return {last, last};
                } else {
                    I found = std::ranges::find_if_not(first, last, pred, proj);
                    if (found == last)
                        return {found, found};
                    first = found;
                    for (;;) {
                        first = std::ranges::find_if_not(std::ranges::next(first), last, pred, proj);
                        if (first == last)
                            return {found, first};
                        found = first;
                    }
                }
            }

            template <std::ranges::forward_range R,
                      typename Proj = std::identity,
                      std::indirect_unary_predicate<std::projected<std::ranges::iterator_t<R>, Proj>> Pred>
            static constexpr std::ranges::borrowed_subrange_t<R> operator()(R&& r, Pred pred, Proj proj = {}) {
                return operator()(std::ranges::begin(r), std::ranges::end(r), std::ref(pred), std::ref(proj));
            }

        };

    } // namespace __ranges

    /// ranges::find_last
    inline constexpr __ranges::find_last_fn find_last {};

    /// ranges::find_last_if
    inline constexpr __ranges::find_last_if_fn find_last_if {};

    /// ranges::find_last_if_not
    inline constexpr __ranges::find_last_if_not_fn find_last_if_not {};

} // namespaceg gold::ranges

#endif // __GOLD_BITS_ALGO_RANGE_FIND_LAST_HPP
