// <gold/bits/sequence/value_seq.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_SEQUENCE_VALUE_SEQ_HPP
#define __GOLD_BITS_SEQUENCE_VALUE_SEQ_HPP

#include <type_traits>
#include <concepts>
#include <bits/utility.h> // std::integer_sequence
#include <gold/pack>
#include <gold/bits/__util/mini_array.hpp>
#include <gold/bits/algo/min_max.hpp>
#include <gold/bits/non_type.hpp>
#include <gold/bits/type_traits/constant.hpp>
#include <gold/bits/type_traits/conditional.hpp>
#include <gold/bits/sequence/value_fwd.hpp>

namespace gold {

    namespace __value_seq {

        /// __value_seq::is_std_seq
        template <typename>
        inline constexpr bool is_std_seq = false;

        template <std::integral T, T... Ns>
        inline constexpr bool is_std_seq<std::integer_sequence<T, Ns...>> = true;

        /// __value_seq::enable_value_seq
        template <typename T>
        inline constexpr bool enable_value_seq = false;

        template <auto... Ns>
        inline constexpr bool enable_value_seq<value_seq_t<Ns...>> = true;

        template <std::integral T, T... Ns>
        inline constexpr bool enable_value_seq<integer_seq_t<T, Ns...>> = true;

        template <std::floating_point T, T... Ns>
        inline constexpr bool enable_value_seq<floating_point_seq_t<T, Ns...>> = true;

        template <typename Seq>
            requires __value_seq::is_std_seq<Seq>
        inline constexpr bool enable_value_seq<Seq> = true;

        /// __value_seq::qualified_value_seq
        template <typename T>
        concept qualified_value_seq = enable_value_seq<__remove_cvref(T)>;

        /// __value_seq::is_homogeneous
        template <typename T>
        inline constexpr bool is_homogeneous = false;

        template <std::integral T, T... Ns>
        inline constexpr bool is_homogeneous<std::integer_sequence<T, Ns...>> = true;

        template <__value_seq::qualified_value_seq Seq>
            requires requires { typename gold::non_type_t<Seq::template homogeneous<>>; }
        inline constexpr bool is_homogeneous<Seq> = Seq::template homogeneous<>;

        /// __value_seq::is_empty
        template <typename T>
        inline constexpr bool is_empty = false;

        template <std::integral T>
        inline constexpr bool is_empty<std::integer_sequence<T>> = true;

        template <__value_seq::qualified_value_seq Seq>
            requires requires { typename gold::non_type_t<Seq::template empty<>>; }
        inline constexpr bool is_empty<Seq> = Seq::template empty<>;

        /// __value_seq::element_type_for
        template <typename>
        struct element_type_for {};

        template <typename Array>
            requires requires { typename Array::element_type; }
        struct element_type_for<Array> {
            using type = Array::element_type;
        };

        template <typename Array>
            requires (!requires { typename Array::element_type; }) &&
                       requires { typename Array::value_type; }
        struct element_type_for<Array> {
            using type = Array::value_type;
        };

        /// __value_seq::element_type_for_t
        template <typename Array>
        using element_type_for_t = element_type_for<Array>::type;

        /// __value_seq::from_mini_array_impl
        template <auto Array>
        consteval auto from_mini_array_impl() {
            using element_type = __value_seq::element_type_for_t<decltype(Array)>;
            return gold::pack_apply_index<Array.size()>([]<std::size_t... Is> static {
                if constexpr (std::is_integral_v<element_type>)
                    return static_cast<gold::integer_seq_t<element_type, Array[Is]...>*>(nullptr);
                else if constexpr (std::is_floating_point_v<element_type>)
                    return static_cast<gold::floating_point_seq_t<element_type, Array[Is]...>*>(nullptr);
                else
                    return static_cast<value_seq_t<Array[Is]...>*>(nullptr);
            });
        }

        /// __value_seq::from_mini_array
        template <auto Array>
        using from_mini_array = __remove_reference(decltype(*from_mini_array_impl<Array>()));

        /// __value_seq::partial_sum_impl_fn
        template <typename T, T... Ns>
        consteval auto partial_sum_impl_fn() {
            constexpr std::size_t Nm = sizeof...(Ns);
            __util::mini_array<__remove_cv(T), Nm> vals { Ns ... };
            __util::mini_array<__remove_cv(T), Nm> partial_sums {};
            if constexpr (Nm != 0) {
                T running_sum = vals[0];
                for (std::size_t i = 1; i < Nm; ++i) {
                    partial_sums[i - 1] = running_sum;
                    running_sum        += vals[i];
                }
                partial_sums[Nm - 1] = running_sum;
            }
            return partial_sums;
        }

        /// __value_seq::partial_sum_impl
        template <typename T, T... Ns>
        using partial_sum_impl = from_mini_array<partial_sum_impl_fn<T, Ns...>()>;

    } // namespace __value_seq

    /// value_sequence
    template <typename T>
    concept value_sequence = __value_seq::qualified_value_seq<T>;

    /// value_seq_t
    template <auto... Ns>
    struct value_seq_t {
      private:
        /// ::self [private]
        using self = value_seq_t;

      public:
        /// ::homogeneous<>
        template <typename... Ts>
            requires (sizeof...(Ts) == 0)
        static constexpr bool homogeneous = (__is_same(__remove_cvref(decltype(gold::pack_get<0>(Ns...))), __remove_cvref(decltype(Ns))) && ...);

        /// ::size<>
        template <typename... Ts>
            requires (sizeof...(Ts) == 0)
        static constexpr std::size_t size = sizeof...(Ns);

        /// ::empty<>
        template <typename... Ts>
            requires (sizeof...(Ts) == 0)
        static constexpr bool empty = (self::size<> == 0);

        /// ::at
        template <std::size_t I>
            requires (I < self::size<>)
        static constexpr auto at = gold::pack_get<I>(Ns...);

        /// ::front<>
        template <typename... Ts>
            requires (sizeof...(Ts) == 0)
                  && (self::size<> >= 0)
        static constexpr auto front = self::at<0>;

        /// ::back<>
        template <typename... Ts>
            requires (sizeof...(Ts) == 0)
                  && (self::size<> >= 0)
        static constexpr auto back = self::at<self::size<> - 1>;

        /// ::unpack_t
        template <template <auto...> typename Temp>
        using unpack_t = Temp<Ns...>;

        /// ::offset_t
        template <auto O>
            requires requires { ((O + Ns), ...); }
        using offset_t = value_seq_t<Ns + O...>;

        /// ::offset
        template <auto O>
            requires requires { typename offset_t<O>; }
        static constexpr auto offset = offset_t<O> {};

        /// ::scale_t
        template <auto O>
            requires requires { ((O * Ns), ...); }
        using scale_t = value_seq_t<Ns * O...>;

        /// ::scale
        template <auto O>
            requires requires { typename scale_t<O>; }
        static constexpr auto scale = scale_t<O> {};

        /// ::concat_with_t
        template <__value_seq::qualified_value_seq... Seqs>
        using concat_with_t = decltype((std::declval<self>() + ... + std::declval<Seqs>()));

        /// ::fill_t
        template <auto N>
        using fill_t = value_seq_t<gold::conditional_t<true, gold::non_type_t<N>, gold::non_type_t<Ns>>::value...>;

        /// ::fill
        template <auto N>
        static constexpr auto fill = fill_t<N> {};

        /// ::equals
        template <typename Seq>
        static constexpr bool equals = (self{} == Seq{});

        /// ::sum<>
        template <typename... Ts>
            requires (sizeof...(Ts) == 0)
                   && requires { (Ns + ... + 0); }
        static constexpr auto sum = (Ns + ... + 0);

        /// ::product<>
        template <typename... Ts>
            requires (sizeof...(Ts) == 0)
                   && requires { (Ns * ... * 1); }
        static constexpr auto product = (Ns * ... * 1);

        /// ::min_element<>
        template <typename... Ts>
            requires (sizeof...(Ts) == 0)
                  && requires { typename std::common_type_t<decltype(Ns)...>; }
        static constexpr auto min_element = __algo::min_element<
            std::common_type_t<decltype(Ns)...>
        >({Ns...});

        /// ::max_element<>
        template <typename... Ts>
            requires (sizeof...(Ts) == 0)
                  && requires { typename std::common_type_t<decltype(Ns)...>; }
        static constexpr auto max_element = __algo::max_element<
            std::common_type_t<decltype(Ns)...>
        >({Ns...});

        consteval value_seq_t() noexcept = default;

        template <auto... Os>
        friend consteval auto operator+(value_seq_t, value_seq_t<Os...>) {
            return value_seq_t<Ns..., Os...>{};
        }

        template <auto N>
        friend consteval auto operator*(constant_t<N>, value_seq_t) {
            constexpr std::size_t M = static_cast<std::size_t>(N);
            if constexpr (M == 0)
                return value_seq_t<> {};
            else if constexpr (M == 1)
                return value_seq_t {};
            else
                return []<std::size_t... Is>(std::index_sequence<Is...>) {
                    return (value_seq_t {} + ... + (Is, value_seq_t{}));
                }(std::make_index_sequence<M - 1>{});
        }

        template <auto N>
        friend consteval auto operator*(value_seq_t, constant_t<N>) {
            return constant<N> * value_seq_t {};
        }

        friend consteval bool operator==(value_seq_t, value_seq_t) noexcept { return true; }

        template <auto... Os>
        friend consteval bool operator==(value_seq_t, value_seq_t<Os...>) noexcept { return false; }
    };

    /// value_seq
    template <auto... Ns>
    inline constexpr value_seq_t<Ns...> value_seq {};

    /// concat_value_seq_t
    template <__value_seq::qualified_value_seq... Seqs>
    using concat_value_seq_t = value_seq_t<>::concat_with_t<Seqs...>;

    /// concat_value_seq
    template <__value_seq::qualified_value_seq... Seqs>
    inline constexpr concat_value_seq_t<Seqs...> concat_value_seq {};

    /// integer_seq_t
    template <std::integral T, T... Ns>
    struct integer_seq_t : value_seq_t<Ns...> {
        using element_type = T;

        using value_seq_t<Ns...>::value_seq_t;

        consteval integer_seq_t(std::integer_sequence<T, Ns...>) noexcept {}

        /// ::homogeneous<> [override]
        template <typename... Ts>
            requires (sizeof...(Ts) == 0)
        static constexpr bool homogeneous = true;

        /// ::partial_sum_t<>
        template <typename... Ts>
             requires (sizeof...(Ts) == 0)
        using partial_sum_t = __value_seq::partial_sum_impl<T, Ns...>;

        /// ::partial_sum<>
        template <typename... Ts>
             requires (sizeof...(Ts) == 0)
        static constexpr auto partial_sum = partial_sum_t <> {};

        /// operator[]
        static constexpr T operator[](std::size_t i) noexcept {
            static constexpr T internal_array[sizeof...(Ns)] { Ns ... };
            return internal_array[i];
        }
    };

    /// integer_seq
    template <std::integral T, T... Ns>
    inline constexpr integer_seq_t<T, Ns...> integer_seq {};

    /// index_seq
    template <std::size_t... Is>
    inline constexpr index_seq_t<Is...> index_seq {};

    /// int_seq
    template <int... Is>
    inline constexpr int_seq_t<Is...> int_seq {};

    /// char_seq
    template <char... Is>
    inline constexpr char_seq_t<Is...> char_seq {};

    /// bool_seq
    template <bool... Is>
    inline constexpr bool_seq_t<Is...> bool_seq {};

    /// floating_point_seq_t
    template <std::floating_point T, T... Fs>
    struct floating_point_seq_t : value_seq_t<Fs...> {
        using element_type = T;

        /// ::homogeneous<> [override]
        template <typename... Ts>
            requires (sizeof...(Ts) == 0)
        static constexpr bool homogeneous = true;

        /// operator[]
        static constexpr T operator[](std::size_t i) noexcept {
            static constexpr T internal_array[sizeof...(Fs)] { Fs ... };
            return internal_array[i];
        }
    };

    /// floating_point_seq
    template <std::floating_point T, T... Fs>
    inline constexpr floating_point_seq_t<T, Fs...> floating_point_seq {};

    /// float_seq
    template <float... Fs>
    inline constexpr float_seq_t<Fs...> float_seq {};

} // namespace gold

#endif // __GOLD_BITS_SEQUENCE_VALUE_SEQ_HPP
