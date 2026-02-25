// <gold/bits/tuples/aggregate.hpp> - gold++ library

// Copyright (C) [ 2025 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TUPLES_AGGREGATE_HPP
#define __GOLD_BITS_TUPLES_AGGREGATE_HPP

#include <gold/bits/concepts/destructurable_type.hpp>
#include <gold/bits/functional/invoke.hpp>

namespace gold::tuples {

    /// tuples::destructurable_aggregate_type
    template <typename T>
    concept destructurable_aggregate_type =
        gold::destructurable_type<T> &&
        std::is_aggregate_v<std::remove_cvref_t<T>>;

    /// tuples::aggregate_field_count[_v]
    template <tuples::destructurable_aggregate_type T>
    inline constexpr std::size_t aggregate_field_count_v =
        __builtin_structured_binding_size(std::remove_cvref_t<T>);

    template <tuples::destructurable_aggregate_type T>
    struct aggregate_field_count :
        std::integral_constant<std::size_t, aggregate_field_count_v<T>> {};

    namespace __tuples {

        template <std::size_t I>
        struct aggregate_get_fn {
            template <tuples::destructurable_aggregate_type T>
                requires (I < tuples::aggregate_field_count_v<T>)
            static constexpr decltype(auto) operator()(T&& t) noexcept {
                auto&& [...elems] = std::forward<T>(t);
                return std::forward<decltype(elems...[I])>(elems...[I]);
            }
        };

        struct aggregate_apply_fn {
            // TODO: add invocation constraint
            // TODO: add noexcept specification
            template <tuples::destructurable_aggregate_type T, typename F>
            static constexpr decltype(auto) operator()(T&& t, F&& f) {
                auto&& [...elems] = std::forward<T>(t);
                return gold::invoke(std::forward<F>(f), std::forward<decltype(elems)>(elems)...);
            }
        };

    } // namespace __tuples

    /// tuples::aggregate_get
    template <std::size_t I>
    inline constexpr __tuples::aggregate_get_fn<I> aggregate_get {};

    /// tuples::aggregate_apply
    inline constexpr __tuples::aggregate_apply_fn aggregate_apply {};

} // namespace gold::tuples

#endif // __GOLD_BITS_TUPLES_AGGREGATE_HPP
