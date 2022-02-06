// <gold/bits/value_sequence/integer_seq_impl.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_VALUE_SEQUENCE_INTEGER_IMPL_HPP
#define __GOLD_BITS_VALUE_SEQUENCE_INTEGER_IMPL_HPP

#include <utility>
#include <concepts>

namespace gold {

    namespace __detail {

        template <typename T, auto Begin, auto Stride, T... Is>
        consteval auto make_consecutive_integer_sequence_helper_impl_(std::integer_sequence<T, Is...>) {
            auto evaluate_ = [](const T& I) consteval -> T { return Begin + Stride * I; };
            return std::integer_sequence<T, evaluate_(Is)...>{};
        }

        template <typename T, auto Begin, auto Count, auto Stride>
        consteval auto make_consecutive_integer_sequence_impl_() {
            return make_consecutive_integer_sequence_helper_impl_<T, Begin, Stride>(std::make_integer_sequence<T, Count>{});
        }

        template <typename T, auto Begin, auto End, auto Stride>
        consteval auto make_interval_integer_sequence_impl_() {

            constexpr auto Count_ = [] consteval {
                if (
                    (Stride > 0 && Begin >= End) ||
                    (Stride <= 0 && End >= Begin)
                )   return 0;

                constexpr auto lo = Stride > 0 ? Begin : End;
                constexpr auto hi = Stride > 0 ? End : Begin;

                return (hi - lo - 1) / (Stride > 0 ? Stride : -Stride) + 1;
            }();

            return make_consecutive_integer_sequence_impl_<T, Begin, Count_, Stride>();
        }

        consteval bool will_reach_negative_(const auto& begin_, const auto& count_, const auto& stride_) {
            // check first if the begin is negative
            if (begin_ < 0) return true;

            using type_ = std::decay_t<decltype(count_)>;

            for (type_ i = 0; i < count_; ++i) {
                if ((begin_ + i * stride_) < 0) return true;
            }

            return false;
        }

    } // namespace __detail

    namespace __requirements {

        // integer_
        template <typename T>
        concept integer_ = std::integral<T>;

        // positive_
        template <auto N>
        concept positive_ = (N > 0);

        // positive_or_zero_
        template <auto N>
        concept positive_or_zero_ = (N >= 0);

        // non_zero_
        template <auto N>
        concept non_zero_ = (N != 0);

        // count_limit_
        template <auto C, auto L>
        concept count_limit_ = (C <= L);

        // make_reverse_sequence_requires_
        template <typename T, T N>
        concept make_reverse_sequence_requires_
        = integer_<T> && positive_or_zero_<N>;

        // make_consecutive_sequence_requires_
        template <typename T, auto Begin, auto Count, auto Stride>
        concept make_consecutive_sequence_requires_
        =  integer_<T>
        && (integer_<decltype(Begin)> && integer_<decltype(Count)> && integer_<decltype(Stride)>)
        && std::convertible_to<decltype(Begin), T>
        && count_limit_<Count, 2'000>;

        // make_consecutive_index_sequence_requires_
        template <auto Begin, auto Count, auto Stride>
        concept make_consecutive_index_sequence_requires_
        =  make_consecutive_sequence_requires_<std::size_t, Begin, Count, Stride>
        && !__detail::will_reach_negative_(Begin, Count, Stride);

        // make_interval_sequence_requires_
        template <typename T, auto Begin, auto End, auto Stride>
        concept make_interval_sequence_requires_
        =  integer_<T>
        && (integer_<decltype(Begin)> && integer_<decltype(End)> && integer_<decltype(Stride)>)
        && ((Stride > 0 && Begin < End) || (Stride < 0 && Begin > End))
        && non_zero_<Stride>;

    } // namespace __requirements

} // namespace gold

#endif // __GOLD_BITS_VALUE_SEQUENCE_INTEGER_IMPL_HPP
