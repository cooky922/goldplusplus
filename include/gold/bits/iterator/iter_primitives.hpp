// <gold/bits/iterator/iter_primitives.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ITERATOR_ITER_PRIMITIVES_HPP
#define __GOLD_BITS_ITERATOR_ITER_PRIMITIVES_HPP

#include <bits/iterator_concepts.h>

namespace gold {

    //// [gold.ranges.iterator.primitives]
    /// indirect_value_result_t
    template <typename F, std::indirectly_readable... Iters>
        requires std::invocable<F, std::iter_value_t<Iters>&...>
    using indirect_value_result_t = std::invoke_result_t<F, std::iter_value_t<Iters>&...>;

    /// projected_value
    template <std::indirectly_readable Iter, std::indirectly_regular_unary_invocable<Iter> Proj>
    struct projected_value {
        using value_type = std::remove_cvref_t<indirect_value_result_t<Proj&, Iter>>;
        indirect_value_result_t<Proj&, Iter> operator*() const; // not defined
    };

} // namespace gold

namespace std {

    /// specialized std::incrementable_traits for gold::projected_value
    template <std::weakly_incrementable Iter, typename Proj>
    struct incrementable_traits<gold::projected_value<Iter, Proj>> {
        using difference_type = std::iter_difference_t<Iter>;
    };

} // namespace std

#endif // __GOLD_BITS_ITERATOR_ITER_PRIMITIVES_HPP
