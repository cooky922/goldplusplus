// <gold/bits/tuples/fold.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TUPLES_FOLD_HPP
#define __GOLD_BITS_TUPLES_FOLD_HPP

#include <gold/pack>
#include <gold/bits/tuples/concepts.hpp>
#include <gold/bits/tuples/apply.hpp>

namespace gold::tuples {

    namespace __cpo_access {

        /// tuples::__cpo_access::pack_left_foldable
        template <typename F, typename... Args>
        inline constexpr bool pack_left_foldable =
            requires (F&& f, Args&&... args) {
                (... && __pack::foldable_wrapper<F, Args&&> { f, static_cast<Args&&>(args) });
            };

        /// tuples::__cpo_access::pack_left_foldable_noexcept
        template <typename F, typename... Args>
        inline constexpr bool pack_left_foldable_noexcept =
            requires (F&& f, Args&&... args) {
                { (... && __pack::foldable_wrapper<F, Args&&> { f, static_cast<Args&&>(args) }) } noexcept;
            };

        /// tuples::__cpo_access::left_foldable_impl
        template <typename F, typename Tuple, typename = void,
                  typename IndexSeq = std::make_index_sequence<__tuples::decayed_tuple_size<Tuple>>>
        inline constexpr bool left_foldable_impl = false;

        template <typename F, typename Tuple, std::size_t... Is>
        inline constexpr bool left_foldable_impl<F, Tuple, void, std::index_sequence<Is...>> =
            pack_left_foldable<F, __tuples::get_result_t<Tuple, Is>...>;

        template <typename F, typename Tuple, typename Init, std::size_t... Is>
        inline constexpr bool left_foldable_impl<F, Tuple, Init, std::index_sequence<Is...>> =
            pack_left_foldable<F, Init, __tuples::get_result_t<Tuple, Is>...>;

        /// tuples::__cpo_access::pack_right_foldable
        template <typename F, typename... Args>
        inline constexpr bool pack_right_foldable =
            requires (F&& f, Args&&... args) {
                (__pack::foldable_wrapper<F, Args&&> { f, static_cast<Args&&>(args) } && ...);
            };

        /// tuples::__cpo_access::pack_right_foldable_noexcept
        template <typename F, typename... Args>
        inline constexpr bool pack_right_foldable_noexcept =
            requires (F&& f, Args&&... args) {
                { (__pack::foldable_wrapper<F, Args&&> { f, static_cast<Args&&>(args) } && ...) } noexcept;
            };

        /// tuples::__cpo_access::right_foldable_impl
        template <typename F, typename Tuple, typename = void,
                  typename IndexSeq = std::make_index_sequence<__tuples::decayed_tuple_size<Tuple>>>
        inline constexpr bool right_foldable_impl = false;

        template <typename F, typename Tuple, std::size_t... Is>
        inline constexpr bool right_foldable_impl<F, Tuple, void, std::index_sequence<Is...>> =
            pack_right_foldable<F, __tuples::get_result_t<Tuple, Is>...>;

        template <typename F, typename Tuple, typename Init, std::size_t... Is>
        inline constexpr bool right_foldable_impl<F, Tuple, Init, std::index_sequence<Is...>> =
            pack_right_foldable<F, __tuples::get_result_t<Tuple, Is>..., Init>;

        /// tuples::__cpo_access::left_foldable
        template <typename F, typename Tuple, typename Init>
        concept left_foldable =
            (__tuples::decayed_tuple_size<Tuple> >= 1) &&
             left_foldable_impl<F, Tuple, Init>;

        /// tuples::__cpo_access::left_foldable_first
        template <typename F, typename Tuple>
        concept left_foldable_first =
            (__tuples::decayed_tuple_size<Tuple> >= 2) &&
             left_foldable_impl<F, Tuple>;

        /// tuples::__cpo_access::right_foldable
        template <typename F, typename Tuple, typename Init>
        concept right_foldable =
            (__tuples::decayed_tuple_size<Tuple> >= 1) &&
             right_foldable_impl<F, Tuple, Init>;

        /// tuples::__cpo_access::right_foldable_last
        template <typename F, typename Tuple>
        concept right_foldable_last =
            (__tuples::decayed_tuple_size<Tuple> >= 2) &&
             right_foldable_impl<F, Tuple>;

        /// tuples::__cpo_access::fold_left_first_fn
        struct fold_left_first_fn {
          private:
            template <typename F, typename Tuple>
            static constexpr bool s_noexcept_() {
                return []<std::size_t... Is>(std::index_sequence<Is...>) {
                    return pack_left_foldable_noexcept<F, __tuples::get_result_t<Tuple, Is>...>;
                }(std::make_index_sequence<__tuples::decayed_tuple_size<Tuple>>{});
            }

          public:
            template <typename F, tuples::tuple_like Tuple>
                requires left_foldable_first<F, Tuple>
            static constexpr decltype(auto) operator()(F&& f, Tuple&& op) noexcept(s_noexcept_<F, Tuple>()) {
                return tuples::apply([&]<typename... Args>(Args&&... args) -> decltype(auto) {
                    return gold::pack_fold_left(std::forward<F>(f), std::forward<Args>(args)...);
                }, std::forward<Tuple>(op));
            }

        };

        /// tuples::__cpo_access::fold_right_last_fn
        struct fold_right_last_fn {
          private:
            template <typename F, typename Tuple>
            static constexpr bool s_noexcept_() {
                return []<std::size_t... Is>(std::index_sequence<Is...>) {
                    return pack_right_foldable_noexcept<F, __tuples::get_result_t<Tuple, Is>...>;
                }(std::make_index_sequence<__tuples::decayed_tuple_size<Tuple>>{});
            }

          public:
            template <typename F, tuples::tuple_like Tuple>
                requires right_foldable_last<F, Tuple>
            static constexpr decltype(auto) operator()(F&& f, Tuple&& op) noexcept(s_noexcept_<F, Tuple>()) {
                return tuples::apply([&]<typename... Args>(Args&&... args) -> decltype(auto) {
                    return gold::pack_fold_right(std::forward<F>(f), std::forward<Args>(args)...);
                }, std::forward<Tuple>(op));
            }

        };

        /// tuples::__cpo_access::fold_left_fn
        struct fold_left_fn {
          private:
            template <typename F, typename Tuple, typename Init>
            static constexpr bool s_noexcept_() {
                return []<std::size_t... Is>(std::index_sequence<Is...>) {
                    return pack_left_foldable_noexcept<F, Init, __tuples::get_result_t<Tuple, Is>...>;
                }(std::make_index_sequence<__tuples::decayed_tuple_size<Tuple>>{});
            }

          public:
            template <typename F, tuples::tuple_like Tuple, std::move_constructible Init>
                requires left_foldable<F, Tuple, Init>
            static constexpr decltype(auto) operator()(F&& f, Tuple&& op, Init init) noexcept(s_noexcept_<F, Tuple, Init>()) {
                return tuples::apply([&]<typename... Args>(Args&&... args) -> decltype(auto) {
                    return gold::pack_fold_left(std::forward<F>(f), std::move(init), std::forward<Args>(args)...);
                }, std::forward<Tuple>(op));
            }

        };

        /// tuples::__cpo_access::fold_right_fn
        struct fold_right_fn {
          private:
            template <typename F, typename Tuple, typename Init>
            static constexpr bool s_noexcept_() {
                return []<std::size_t... Is>(std::index_sequence<Is...>) {
                    return pack_right_foldable_noexcept<F, __tuples::get_result_t<Tuple, Is>..., Init>;
                }(std::make_index_sequence<__tuples::decayed_tuple_size<Tuple>>{});
            }

          public:
            template <typename F, tuples::tuple_like Tuple, std::move_constructible Init>
                requires right_foldable<F, Tuple, Init>
            static constexpr decltype(auto) operator()(F&& f, Tuple&& op, Init init) noexcept(s_noexcept_<F, Tuple, Init>()) {
                return tuples::apply([&]<typename... Args>(Args&&... args) -> decltype(auto) {
                    return gold::pack_fold_right(std::forward<F>(f), std::forward<Args>(args)..., std::move(init));
                }, std::forward<Tuple>(op));
            }

        };

    } // namespace __cpo_access

    inline namespace __cpo {

        /// tuples::fold_left_first
        inline constexpr __cpo_access::fold_left_first_fn fold_left_first {};

        /// tuples::fold_right_last
        inline constexpr __cpo_access::fold_right_last_fn fold_right_last {};

        /// tuples::fold_left
        inline constexpr __cpo_access::fold_left_fn fold_left {};

        /// tuples::fold_right
        inline constexpr __cpo_access::fold_right_fn fold_right {};

    } // inline namespace __cpo

} // namespace gold::tuples

#endif // __GOLD_BITS_TUPLES_FOLD_HPP
