// <gold/bits/tuples/apply_each.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TUPLES_APPLY_EACH_HPP
#define __GOLD_BITS_TUPLES_APPLY_EACH_HPP

#include <tuple>
#include <gold/bits/algo/min_max.hpp>
#include <gold/bits/tuples/get.hpp>
#include <gold/bits/tuples/concepts.hpp>
#include <gold/bits/functional/invoke.hpp>

namespace gold::tuples {

    namespace __cpo_access {

        /// tuples::__cpo_access::invoke_at
        template <std::size_t I, typename F, tuples::tuple_like... Tuples>
        constexpr decltype(auto) invoke_at(F&& f, Tuples&&... tuples)
            noexcept(std::is_nothrow_invocable_v<F, __tuples::get_result_t<Tuples, I>...>)
        {
            return gold::invoke(std::forward<F>(f), tuples::get<I>(std::forward<Tuples>(tuples))...);
        }

        /// tuples::__cpo_access::invocable_at
        template <std::size_t I, typename F, typename... Tuples>
        concept invocable_at = std::invocable<F, __tuples::get_result_t<Tuples, I>...>;

        /// tuples::__cpo_access::can_apply_each_impl_v
        template <typename, typename, typename...>
        inline constexpr bool can_apply_each_impl_v = false;

        template <std::size_t... Is, typename F, typename... Tuples>
        inline constexpr bool can_apply_each_impl_v<std::index_sequence<Is...>, F, Tuples...> =
            (invocable_at<Is, F, Tuples...> && ...);

        /// tuples::__cpo_access::can_apply_each_impl
        template <std::size_t Min, typename F, typename... Tuples>
        concept can_apply_each_impl =
            ((Min == 0 || sizeof...(Tuples) == 0) && std::invocable<F>)
         || (can_apply_each_impl_v<std::make_index_sequence<Min>, F, Tuples...>);

        /// tuples::__cpo_access::can_apply_each
        template <typename F, typename... Tuples>
        concept can_apply_each = (tuples::tuple_like<Tuples> && ...)
             && can_apply_each_impl<
                    (sizeof...(Tuples) == 0 ? 0 : __algo::min_element<std::size_t>({__tuples::decayed_tuple_size<Tuples>...})),
                    F, Tuples...>;

        /// tuples::__cpo_access::apply_each_fn
        struct apply_each_fn {
          private:
            template <typename F, typename... Tuples>
            static consteval bool s_noexcept_() {
                if constexpr (sizeof...(Tuples) == 0)
                    return std::is_nothrow_invocable_v<F>;
                else {
                    constexpr std::size_t min_size = __algo::min_element<std::size_t>({__tuples::decayed_tuple_size<Tuples>...});
                    if constexpr (min_size == 0)
                        return std::is_nothrow_invocable_v<F>;
                    else
                        return []<std::size_t... Is>(std::index_sequence<Is...>) {
                            return (noexcept(__cpo_access::invoke_at<Is>(std::declval<F>(), std::declval<Tuples>()...)) && ...);
                        }(std::make_index_sequence<min_size>{});
                }
            }

          public:
            template <typename F, tuples::tuple_like... Tuples>
                requires can_apply_each<F, Tuples...>
            static constexpr decltype(auto) operator()(F&& f, Tuples&&... tuples) noexcept(s_noexcept_<F, Tuples...>()) {
                if constexpr (sizeof...(Tuples) == 0)
                    return gold::invoke(std::forward<F>(f));
                else {
                    constexpr std::size_t min_size = __algo::min_element<std::size_t>({__tuples::decayed_tuple_size<Tuples>...});
                    if constexpr (min_size == 0)
                        return gold::invoke(std::forward<F>(f));
                    else
                        return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                            constexpr bool has_void = std::disjunction_v<
                                std::is_void<decltype(__cpo_access::invoke_at<Is>(std::forward<F>(f), std::forward<Tuples>(tuples)...))> ...
                            >;
                            if constexpr (has_void)
                                (__cpo_access::invoke_at<Is>(std::forward<F>(f), std::forward<Tuples>(tuples)...), ...);
                            else
                                return std::tuple<
                                    decltype(__cpo_access::invoke_at<Is>(std::forward<F>(f), std::forward<Tuples>(tuples)...))...
                                > { __cpo_access::invoke_at<Is>(std::forward<F>(f), std::forward<Tuples>(tuples)...) ... };
                        }(std::make_index_sequence<min_size>{});
                }
            }

        };

    } // namespace __cpo_access

    inline namespace __cpo {

        /// tuples::apply_each
        inline constexpr __cpo_access::apply_each_fn apply_each {};

    } // inline namespace __cpo

    /// tuples::apply_each_result
    template <typename, typename...>
    struct apply_each_result {};

    template <typename F, typename... Tuples>
        requires __cpo_access::can_apply_each<F, Tuples...>
    struct apply_each_result<F, Tuples...> {
        using type = decltype(tuples::apply_each(std::declval<F>(), std::declval<Tuples>()...));
    };

    /// tuples::apply_each_result_t
    template <typename F, tuples::tuple_like... Tuples>
        requires __cpo_access::can_apply_each<F, Tuples...>
    using apply_each_result_t = apply_each_result<F, Tuples...>::type;

} // namespace gold::tuples

#endif // __GOLD_BITS_TUPLES_APPLY_EACH_HPP
