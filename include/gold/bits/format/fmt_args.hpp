// <gold/bits/format/fmt_args.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FORMAT_ARGS_HPP
#define __GOLD_BITS_FORMAT_ARGS_HPP

#include <gold/bits/format/formatter.hpp>

#include <tuple>

namespace gold {

    namespace __format {

        template <typename T>
        struct formatter_wrapper {
            T base;
            [[no_unique_address]] gold::formatter<T> formatter {};
        };

    } // namespace __format

    /// format_args
    template <typename... Args>
        requires (formattable<Args> && ...)
    class format_args {
      private:

        /// stored arguments
        std::tuple<__format::formatter_wrapper<Args>...> m_args_;

      public:
        /// variadic constructor
        template <typename... Us>
        constexpr format_args(Us&&... args)
        : m_args_(std::forward<Us>(args)...) {}

        constexpr format_args(const format_args&) = default;
        constexpr format_args(format_args&&) = default;

        /// returned size
        constexpr std::size_t size() const noexcept { return sizeof...(Args); }

        /// visit_arg_at
        template <typename F>
        constexpr void visit_arg_at(std::size_t pos, F&& f) {
            [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                [](const auto&...){}(
                    (pos == Is && ((void) std::forward<F>(f)(std::get<Is>(m_args_).base), false)) ...
                );
            }(std::make_index_sequence<sizeof...(Args)>{});
        }

        template <typename F>
        constexpr void visit_arg_at(std::size_t pos, F&& f) const {
            [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                [](const auto&...){}(
                    (pos == Is && ((void) std::forward<F>(f)(std::get<Is>(m_args_).base), false)) ...
                );
            }(std::make_index_sequence<sizeof...(Args)>{});
        }

        /// visit_formatter_at
        template <typename F>
        constexpr void visit_formatter_at(std::size_t pos, F&& f) {
            [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                [](const auto&...){}(
                    (pos == Is && ((void) std::forward<F>(f)(std::get<Is>(m_args_).formatter), false)) ...
                );
            }(std::make_index_sequence<sizeof...(Args)>{});
        }

        template <typename F>
        constexpr void visit_formatter_at(std::size_t pos, F&& f) const {
            [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                [](const auto&...){}(
                    (pos == Is && ((void) std::forward<F>(f)(std::get<Is>(m_args_).formatter), false)) ...
                );
            }(std::make_index_sequence<sizeof...(Args)>{});
        }

        /// visit_at
        template <typename F>
        constexpr void visit_at(std::size_t pos, F&& f) {
            [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                [](const auto&...){}(
                    (pos == Is && ((void) std::forward<F>(f)(std::get<Is>(m_args_)), false)) ...
                );
            }(std::make_index_sequence<sizeof...(Args)>{});
        }

        template <typename F>
        constexpr void visit_at(std::size_t pos, F&& f) const {
            [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                [](const auto&...){}(
                    (pos == Is && ((void) std::forward<F>(f)(std::get<Is>(m_args_)), false)) ...
                );
            }(std::make_index_sequence<sizeof...(Args)>{});
        }

        /// base
        constexpr auto base() const {
            return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                return std::tie(std::get<Is>(m_args_).base...);
            }(std::make_index_sequence<sizeof...(Args)>{});
        }

        /// operator
        constexpr operator std::tuple<__format::formatter_wrapper<Args>...>() { return m_args_; }
    };

    namespace __format {

        /// decay_t_
        template <typename T, typename U = std::remove_cvref_t<T>>
        using decay_t_ = std::conditional_t<
            (std::is_pointer_v<T> && !gold::char_type<std::remove_cvref_t<std::remove_pointer_t<T>>>),
            const void*,
            std::decay_t<T>
        >;

        /// decay_t_
        template <typename T>
        struct decay_ : std::type_identity<decay_t_<T>> {};

//        /// decay_
//        template <typename T>
//        struct decay_ {
//            using type = std::conditional_t<
//                (std::is_pointer_v<T> && !gold::char_type<std::remove_cvref_t<std::remove_pointer_t<T>>>),
//                const void*,
//                std::decay_t<T>
//            >;
//        };

    } // namespace __format

    /// deduction guide for format_args
    template <typename... Args>
    format_args(Args...) -> format_args<__format::decay_t_<Args>...>;

    /// make_format_args
    template <typename... Args>
    constexpr format_args<__format::decay_t_<Args>...> make_format_args(Args&&... args) noexcept {
        return { std::forward<Args>(args)... };
    }

} // namespace gold

#endif // __GOLD_BITS_FORMAT_ARGS_HPP
