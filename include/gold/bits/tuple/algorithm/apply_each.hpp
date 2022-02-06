// <gold/bits/tuple/algorithm/apply_each.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TUPLE_ALGO_APPLY_EACH_HPP
#define __GOLD_BITS_TUPLE_ALGO_APPLY_EACH_HPP

#include <gold/bits/tuple/algorithm/tuple_zip.hpp>

namespace gold {

    namespace __detail {

        // apply_each_impl_
        template <typename F, typename... Tuple, std::size_t... Is> requires (sizeof...(Tuple) > 0)
        constexpr decltype(auto) apply_each_impl_(std::index_sequence<Is...>, F&& f, Tuple&&... ts) {
            decltype(auto) zipped_ = tuple_zip(std::forward<Tuple>(ts)...);
            using result_t_ = decltype([&]{
                return (std::apply(std::forward<F>(f), std::get<Is>(zipped_)), ...);
            }());

            if constexpr (std::is_void_v<result_t_>) {
                return (
                    std::apply(std::forward<F>(f), std::get<Is>(zipped_)), ...
                );
            } else {
                return std::make_tuple(
                    std::apply(
                        std::forward<F>(f), std::get<Is>(zipped_)
                    )...
                );
            }
        }

    } // namespace __detail

    /// apply_each
    template <typename F, typename... Tuple>
        requires ((qualified_tuple<std::remove_reference_t<
            __detail::maybe_unwrap_ref_wrapper_t_<Tuple>
        >> && ...))
    constexpr decltype(auto) apply_each(F&& f, Tuple&&... ts) {
        using indices_ = std::make_index_sequence<gold::__detail::min_({
            std::tuple_size_v<std::decay_t<__detail::maybe_unwrap_ref_wrapper_t_<Tuple>>>...
        })>;
        return __detail::apply_each_impl_(indices_{}, std::forward<F>(f), std::forward<Tuple>(ts)...);
    }

    /// apply_each_n
    template <std::size_t N, typename F, typename... Tuple>
    requires (
        (qualified_tuple<std::remove_reference_t<__detail::maybe_unwrap_ref_wrapper_t_<Tuple>>> && ...) &&
        (N <= gold::__detail::min_({std::tuple_size_v<std::decay_t<__detail::maybe_unwrap_ref_wrapper_t_<Tuple>>>...}))
    )
    constexpr decltype(auto) apply_each_n(F&& f, Tuple&&... ts) {
        using indices_ = std::make_index_sequence<N>;
        return __detail::apply_each_impl_(indices_{}, std::forward<F>(f), std::forward<Tuple>(ts)...);
    }

} // namespace gold

#endif // __GOLD_BITS_TUPLE_ALGO_APPLY_EACH_HPP
