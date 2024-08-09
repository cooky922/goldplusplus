// <gold/bits/algo/range_iota.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ALGO_RANGE_IOTA_HPP
#define __GOLD_BITS_ALGO_RANGE_IOTA_HPP

#include <bits/ranges_base.h>
#include <bits/iterator_concepts.h>
#include <gold/bits/casts.hpp>

namespace gold::ranges {

    /// ranges::out_value_result
    template <typename O, typename T>
    struct out_value_result {
        [[no_unique_address]] O out;
        [[no_unique_address]] T value;

        template <typename O2, typename T2>
            requires std::convertible_to<const O&, O2> && std::convertible_to<const T&, T2>
        constexpr operator out_value_result<O2, T2>() const& {
            return {out, value};
        }

        template <typename O2, typename T2>
            requires std::convertible_to<O, O2> && std::convertible_to<T, T2>
        constexpr operator out_value_result<O2, T2>() && {
            return {std::move(out), std::move(value)};
        }

    };

    /// ranges::iota_result
    template <typename O, typename T>
    using iota_result = out_value_result<O, T>;

    namespace __ranges {

        /// ranges::__ranges::iota_fn
        struct iota_fn {

            template <std::input_or_output_iterator Out,
                      std::sentinel_for<Out> Sent,
                      std::weakly_incrementable T>
                requires std::indirectly_writable<Out, const T&>
            static constexpr iota_result<Out, T> operator()(Out out, Sent sent, T value) {
                while (out != sent) {
                    *out = gold::as_const(value);
                    ++out, ++value;
                }
                return {std::move(out), std::move(value)};
            }

            template <std::weakly_incrementable T, std::ranges::output_range<const T&> R>
            static constexpr iota_result<std::ranges::borrowed_iterator_t<R>, T>
            operator()(R&& r, T value) {
                return operator()(std::ranges::begin(r),
                                  std::ranges::end(r),
                                  std::move(value));
            }

        };

    } // namespace __ranges

    /// ranges::iota
    inline constexpr __ranges::iota_fn iota {};

} // namespace gold::ranges

#endif // __GOLD_BITS_ALGO_RANGE_IOTA_HPP
