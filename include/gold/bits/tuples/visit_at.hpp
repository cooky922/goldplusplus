// <gold/bits/tuples/visit_at.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TUPLES_VISIT_AT_HPP
#define __GOLD_BITS_TUPLES_VISIT_AT_HPP

#include <gold/bits/tuples/concepts.hpp>
#include <gold/bits/tuples/get.hpp>
#include <gold/bits/functional/invoke.hpp>
#include <gold/bits/sequence/make_integer_seq.hpp>
#include <gold/bits/algo/with_index.hpp>

namespace gold::tuples {

    namespace __cpo_access {

        template <typename Tuple, typename F, typename = gold::make_index_seq_t<__tuples::decayed_tuple_size<Tuple>>>
        inline constexpr bool visitable_with_impl = false;

        template <typename Tuple, typename F, std::size_t... Is>
        inline constexpr bool visitable_with_impl<Tuple, F, gold::index_seq_t<Is...>> =
            (std::is_invocable_v<F, __tuples::get_result_t<Tuple, Is>> && ...);

        struct visit_at_fn {

            // runtime index
            template <tuples::tuple_like Tuple, typename F>
                requires visitable_with_impl<Tuple, F>
            static constexpr decltype(auto) operator()(std::size_t i, Tuple&& tup, F&& f) { // TODO: add noexcept
                return __algo::visit_with_index<tuples::size(tup)>(i, [&]<std::size_t I> -> decltype(auto) {
                    return gold::invoke(std::forward<F>(f), tuples::get<I>(std::forward<Tuple>(tup)));
                });
            }

            // compile-time index
            template <tuples::tuple_like Tuple, typename F, std::integral auto I>
                requires (I >= 0) && std::invocable<F, __tuples::get_result_t<Tuple, I>>
            static constexpr decltype(auto) operator()(std::integral_constant<decltype(I), I>, Tuple&& tup, F&& f) {
                return gold::invoke(std::forward<F>(f), tuples::get<I>(std::forward<Tuple>(tup)));
            }

        };

    } // namespace __cpo_access

    inline namespace __cpo {

        /// tuples::visit_at
        inline constexpr __cpo_access::visit_at_fn visit_at {};

    }

    /// tuples::visitable_with
    template <typename Tuple, typename F>
    concept visitable_with = __cpo_access::visitable_with_impl<Tuple, F>;

    /// tuples::visitor_for
    template <typename F, typename Tuple>
    concept visitor_for = visitable_with<Tuple, F>;

} // namespace gold::tuples

#endif // __GOLD_BITS_TUPLES_VISIT_AT_HPP
