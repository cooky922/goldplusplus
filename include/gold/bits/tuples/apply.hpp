// <gold/bits/tuples/apply.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TUPLES_APPLY_HPP
#define __GOLD_BITS_TUPLES_APPLY_HPP

#include <gold/bits/__util/uneval.hpp>
#include <gold/bits/tuples/concepts.hpp>
#include <gold/bits/functional/invoke.hpp>

namespace gold::tuples {

    namespace __cpo_access {

        /// adl apply = deleted
        void apply(auto&&, auto&&) = delete;

        /// tuples::__cpo_access::has_adl_apply
        template <typename F, typename Tuple>
        concept has_adl_apply =
               __concepts::class_or_union_or_enum<std::remove_cvref_t<Tuple>>
            && requires (F&& f, Tuple&& op) { apply(std::forward<F>(f), std::forward<Tuple>(op)); };

        /// tuples::__cpo_access::apply_result_impl
        template <typename F, typename Tuple>
        struct apply_result_impl {
            using type = decltype([]<std::size_t... Is>(std::index_sequence<Is...>) {
                if constexpr (has_adl_apply<std::remove_cvref_t<F>, std::remove_cvref_t<Tuple>>)
                    return __util::uneval<
                        decltype((apply(std::declval<F>(), std::declval<Tuple>())))
                    >;
                else if constexpr (__tuples::empty_tuple_size<std::remove_cvref_t<Tuple>>)
                    return __util::uneval<
                        decltype((gold::invoke(std::declval<F>())))
                    >;
                else
                    return __util::uneval<
                        decltype((gold::invoke(std::declval<F>(), tuples::get<Is>(std::declval<Tuple>())...)))
                    >;
            }(std::make_index_sequence<__tuples::decayed_tuple_size<Tuple>>{})());
        };

        /// tuples::__cpo_access::applicable_using_get
        template <typename F, typename Tuple>
        concept applicable_using_get = []<std::size_t... Is>(std::index_sequence<Is...>) {
            return std::is_invocable_v<F, __tuples::get_result_t<Tuple, Is>...>;
        }(std::make_index_sequence<__tuples::decayed_tuple_size<Tuple>>{});

    } // namespace __cpo_access

    /// tuples::applicable
    template <typename F, typename Tuple>
    concept applicable =
        (__tuples::empty_tuple_size<std::remove_cvref_t<Tuple>> && std::invocable<F>) ||
        (tuples::tuple_like<Tuple> && (__cpo_access::has_adl_apply<F, Tuple> ||
                                       __cpo_access::applicable_using_get<F, Tuple>));

    /// tuples::apply_result
    template <typename F, tuples::tuple_like Tuple>
    using apply_result = __cpo_access::apply_result_impl<F, Tuple>;

    /// tuples::apply_result_t
    template <typename F, tuples::tuple_like Tuple>
    using apply_result_t = apply_result<F, Tuple>::type;

    namespace __cpo_access {

        /// tuples::__cpo_access::apply_fn
        struct apply_fn {
          private:
            template <typename F, typename Tuple>
            static consteval bool s_noexcept_() {
                if constexpr (has_adl_apply<std::remove_cvref_t<F>, std::remove_cvref_t<Tuple>>)
                    return noexcept(apply(std::declval<F>(), std::declval<Tuple>()));
                else if constexpr (__tuples::empty_tuple_size<std::remove_cvref_t<Tuple>>)
                    return std::is_nothrow_invocable_v<F>;
                else
                    return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                        return std::is_nothrow_invocable_v<
                            F, __tuples::get_result_t<Tuple, Is>...
                        >;
                    }(std::make_index_sequence<__tuples::decayed_tuple_size<Tuple>>{});
            }

          public:
            template <typename F, tuples::tuple_like Tuple>
                requires tuples::applicable<F, Tuple>
            static constexpr tuples::apply_result_t<F, Tuple> operator()(F&& f, Tuple&& op) noexcept(s_noexcept_<F, Tuple>()) {
                if constexpr (has_adl_apply<std::remove_cvref_t<F>, std::remove_cvref_t<Tuple>>)
                    return apply(std::forward<F>(f), std::forward<Tuple>(op));
                else if constexpr (__tuples::empty_tuple_size<std::remove_cvref_t<Tuple>>)
                    return gold::invoke(std::forward<F>(f));
                else
                    return [&]<std::size_t... Is>(std::index_sequence<Is...>) -> decltype(auto) {
                        return gold::invoke(std::forward<F>(f), tuples::get<Is>(std::forward<Tuple>(op))...);
                    }(std::make_index_sequence<__tuples::decayed_tuple_size<Tuple>>{});
            }
        };

    } // namespace __cpo_access

    inline namespace __cpo {

        /// tuples::apply
        inline constexpr __cpo_access::apply_fn apply {};

    } // inline namespace __cpo

} // namespace gold::tuples

#endif // __GOLD_BITS_TUPLES_APPLY_HPP
