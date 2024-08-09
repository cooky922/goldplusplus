// <gold/bits/sequence/make_integer_seq.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_SEQUENCE_MAKE_INTEGER_SEQ_HPP
#define __GOLD_BITS_SEQUENCE_MAKE_INTEGER_SEQ_HPP

#include <gold/bits/sequence/value_seq.hpp>

namespace gold {

    /// make_integer_seq_t
    template <std::integral T, T N>
    using make_integer_seq_t = integer_seq_t<T, __integer_pack(N)...>;

    /// make_integer_seq
    template <std::integral T, T N>
    inline constexpr auto make_integer_seq = make_integer_seq_t<T, N>{};

    /// make_index_seq_t
    template <std::size_t N>
    using make_index_seq_t = make_integer_seq_t<std::size_t, N>;

    /// make_index_seq
    template <std::size_t N>
    inline constexpr auto make_index_seq = make_index_seq_t<N>{};

    /// index_seq_for_t
    template <typename... Ts>
    using index_seq_for_t = make_index_seq_t<sizeof...(Ts)>;

    /// index_seq_for
    template <typename... Ts>
    inline constexpr auto index_seq_for = index_seq_for_t<Ts...>{};

    namespace __value_seq {

        /// __value_seq::make_consecutive_integer_seq_impl
        template <typename T, auto Begin, auto Count, auto Stride>
        consteval auto make_consecutive_integer_seq_impl() {
            return []<T... Is>(gold::integer_seq_t<T, Is...>) {
                return gold::integer_seq<T, (Begin + Stride * Is)...>;
            }(gold::make_integer_seq<T, Count>);
        }

        /// __value_seq::make_interval_integer_seq_impl
        template <typename T, auto Begin, auto End, auto Stride>
        consteval auto make_interval_integer_seq_impl() {
            constexpr auto Count = [] {
                constexpr bool zero_count = (Stride > 0 && Begin >= End)
                                         || (Stride <= 0 && End >= Begin);
                if constexpr (zero_count)
                    return 0;
                constexpr auto low  = Stride > 0 ? Begin : End;
                constexpr auto high = Stride > 0 ? End : Begin;
                return (high - low - 1) / (Stride > 0 ? Stride : -Stride) + 1;
            }();
            return make_consecutive_integer_seq_impl<T, Begin, Count, Stride>();
        }

        /// __value_seq::make_repeat_integer_seq_impl
        template <typename T, std::size_t N, T Op>
        consteval auto make_repeat_integer_seq_impl() {
            return []<std::size_t... Is>(gold::index_seq_t<Is...>*) {
                return gold::integer_seq<T, (Is, Op) ...>;
            }(static_cast<gold::index_seq_t<__integer_pack(N)...>*>(nullptr));
        }

        /// __value_seq::will_reach_negative
        consteval bool will_reach_negative(auto Begin, auto Count, auto Stride) {
            if (Begin < 0) // check if the Begin is negative
                return true;
            for (decltype(Count) i = 0; i < Count; ++i)
                if ((Begin + Stride * i) < 0)
                    return true;
            return false;
        }

        /// __value_seq::viable_reverse_seq
        template <typename T, T N>
        concept viable_reverse_seq = std::integral<T> && (N >= 0);

        /// __value_seq::viable_consecutive_seq
        template <typename T, auto Begin, auto Count, auto Stride>
        concept viable_consecutive_seq = std::integral<T>
            && requires {
                { Begin  } -> std::convertible_to<T>;
                { Count  } -> std::integral;
                { Stride } -> std::integral;
            }
            && (Count <= 2000); // ideal compiler limit of generating 2000 elements

        /// __value_seq::viable_consecutive_index_seq
        template <auto Begin, auto Count, auto Stride>
        concept viable_consecutive_index_seq =
               viable_consecutive_seq<std::size_t, Begin, Count, Stride>
           && !will_reach_negative(Begin, Count, Stride);

        /// __value_seq::viable_interval_seq
        template <typename T, auto Begin, auto End, auto Stride>
        concept viable_interval_seq = std::integral<T>
            && requires {
                { Begin  } -> std::integral;
                { End    } -> std::integral;
                { Stride } -> std::integral;
            }
            && ((Stride > 0 && Begin < End) || (Stride < 0 && Begin > End))
            && (Stride != 0);

    } // namespace __value_seq

    /// make_consecutive_integer_seq_t
    template <std::integral T, auto Begin, auto Count, auto Stride = 1>
        requires __value_seq::viable_consecutive_seq<T, Begin, Count, Stride>
    using make_consecutive_integer_seq_t = decltype(__value_seq::make_consecutive_integer_seq_impl<T, Begin, Count, Stride>());

    /// make_consecutive_integer_seq
    template <std::integral T, auto Begin, auto Count, auto Stride = 1>
        requires __value_seq::viable_consecutive_seq<T, Begin, Count, Stride>
    inline constexpr make_consecutive_integer_seq_t<T, Begin, Count, Stride> make_consecutive_integer_seq {};

    /// make_consecutive_index_seq_t
    template <auto Begin, auto Count, auto Stride = 1>
        requires __value_seq::viable_consecutive_index_seq<Begin, Count, Stride>
    using make_consecutive_index_seq_t = make_consecutive_integer_seq_t<std::size_t, Begin, Count, Stride>;

    /// make_consecutive_index_seq
    template <auto Begin, auto Count, auto Stride = 1>
        requires __value_seq::viable_consecutive_index_seq<Begin, Count, Stride>
    inline constexpr make_consecutive_index_seq_t<Begin, Count, Stride> make_consecutive_index_seq {};

    /// make_reverse_integer_seq_t
    template <std::integral T, T N>
        requires __value_seq::viable_reverse_seq<T, N>
    using make_reverse_integer_seq_t = make_consecutive_integer_seq_t<T, N - 1, N, -1>;

    /// make_reverse_integer_seq
    template <std::integral T, T N>
        requires __value_seq::viable_reverse_seq<T, N>
    inline constexpr make_reverse_integer_seq_t<T, N> make_reverse_integer_seq {};

    /// make_reverse_index_seq_t
    template <std::size_t N>
        requires __value_seq::viable_reverse_seq<std::size_t, N>
    using make_reverse_index_seq_t = make_reverse_integer_seq_t<std::size_t, N>;

    /// make_reverse_index_seq
    template <std::size_t N>
        requires __value_seq::viable_reverse_seq<std::size_t, N>
    inline constexpr make_reverse_index_seq_t<N> make_reverse_index_seq {};

    /// make_interval_integer_seq_t
    template <typename T, auto Begin, auto End, auto Stride = (Begin < End ? 1 : -1)>
        requires __value_seq::viable_interval_seq<T, Begin, End, Stride>
    using make_interval_integer_seq_t = decltype(__value_seq::make_interval_integer_seq_impl<T, Begin, End, Stride>());

    /// make_interval_integer_seq
    template <typename T, auto Begin, auto End, auto Stride = (Begin < End ? 1 : -1)>
        requires __value_seq::viable_interval_seq<T, Begin, End, Stride>
    inline constexpr make_interval_integer_seq_t<T, Begin, End, Stride> make_interval_integer_seq {};

    /// make_interval_index_seq_t
    template <auto Begin, auto End, auto Stride = (Begin < End ? 1 : -1)>
        requires __value_seq::viable_interval_seq<std::size_t, Begin, End, Stride>
    using make_interval_index_seq_t = make_interval_integer_seq_t<std::size_t, Begin, End, Stride>;

    /// make_interval_index_seq
    template <auto Begin, auto End, auto Stride = (Begin < End ? 1 : -1)>
        requires __value_seq::viable_interval_seq<std::size_t, Begin, End, Stride>
    inline constexpr make_interval_index_seq_t<Begin, End, Stride> make_interval_index_seq {};

    /// make_repeat_integer_seq_t
    template <typename T, std::size_t N, T Op>
    using make_repeat_integer_seq_t = decltype(__value_seq::make_repeat_integer_seq_impl<T, N, Op>());

    /// make_repeat_integer_seq
    template <typename T, std::size_t N, T Op>
    inline constexpr make_repeat_integer_seq_t<T, N, Op> make_repeat_integer_seq {};

    /// make_repeat_index_seq_t
    template <std::size_t N, std::size_t Op>
    using make_repeat_index_seq_t = make_repeat_integer_seq_t<std::size_t, N, Op>;

    /// make_repeat_index_seq
    template <std::size_t N, std::size_t Op>
    inline constexpr make_repeat_index_seq_t<N, Op> make_repeat_index_seq {};

} // namespace gold

#endif // __GOLD_BITS_SEQUENCE_MAKE_INTEGER_SEQ_HPP
