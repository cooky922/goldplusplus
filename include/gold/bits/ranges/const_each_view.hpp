// <gold/bits/ranges/const_each_view.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RANGES_CONST_EACH_VIEW_HPP
#define __GOLD_BITS_RANGES_CONST_EACH_VIEW_HPP

#if __has_include(<bits/views_empty.h>)
#include <bits/views_empty.h>
#else
#include <ranges>
#endif
#include <gold/bits/casts.hpp>
#include <gold/bits/ranges/range_primitives.hpp>
#include <gold/bits/ranges/view_util.hpp>
#include <gold/bits/ranges/range_adaptor.hpp>
#include <gold/bits/type_traits/specialization_of.hpp>

namespace std { template <typename, std::size_t> class span; } /// fwd only

namespace gold::ranges {

    /// ranges::const_each_view
    template <std::ranges::input_range V>
        requires std::ranges::view<V>
    class const_each_view : public std::ranges::view_interface<const_each_view<V>> {
      private:
        V m_base_ = V();

      public:
        constexpr const_each_view() requires std::default_initializable<V> = default;

        constexpr explicit const_each_view(V r)
        : m_base_(std::move(r)) {}

        constexpr V base() const& requires std::copy_constructible<V> { return m_base_; }
        constexpr V base() && { return std::move(m_base_); }

        constexpr auto begin() requires (!__ranges::simple_view<V>) {
            return std::ranges::cbegin(m_base_);
        }
        constexpr auto begin() const requires std::ranges::range<const V> {
            return std::ranges::cbegin(m_base_);
        }

        constexpr auto end() requires (!__ranges::simple_view<V>) {
            return std::ranges::cend(m_base_);
        }

        constexpr auto end() const requires std::ranges::range<const V> {
            return std::ranges::cend(m_base_);
        }

        constexpr auto size() requires std::ranges::sized_range<V> {
            return std::ranges::size(m_base_);
        }
        constexpr auto size() const requires std::ranges::sized_range<const V> {
            return std::ranges::size(m_base_);
        }

    };

    template <typename R>
    const_each_view(R&&) -> const_each_view<std::views::all_t<R>>;

    namespace views {

        namespace __views {

            /// ranges::views::__views::can_const_each_view
            template <typename R>
            concept can_const_each_view = requires {
                const_each_view { std::declval<R>() };
            };

            /// ranges::views::__views::is_ref_view
            template <typename T>
            inline constexpr bool is_ref_view = false;

            template <typename R>
            inline constexpr bool is_ref_view<std::ranges::ref_view<R>> = true;

            /// ranges::views::__views::is_ref_view_and_constant_range
            template <typename R>
            concept is_ref_view_and_constant_range =
                   is_ref_view<R>
                && std::ranges::constant_range<const __remove_reference(decltype(std::declval<R>().base()))>;

            /// ranges::views::__views::is_empty_view
            template <typename T>
            inline constexpr bool is_empty_view = false;

            template <typename T>
            inline constexpr bool is_empty_view<std::ranges::empty_view<T>> = true;

            /// ranges::views::__views::empty_view_underlying_type
            template <typename R>
            using empty_view_underlying_type = __remove_reference(decltype(*std::declval<R>().data()));

            /// ranges::views::__views::const_each_fn
            struct const_each_fn {

                template <std::ranges::viewable_range R, typename U = std::remove_cvref_t<R>>
                    requires std::ranges::constant_range<std::views::all_t<R>>
                         || is_empty_view<U>
                         || (gold::is_specialization_of_v<U, std::span>())
                         || is_ref_view_and_constant_range<U>
                         || (std::is_lvalue_reference_v<R>
                            &&  std::ranges::constant_range<const U>
                            && !std::ranges::view<U>)
                         || can_const_each_view<R>
                [[nodiscard]] constexpr auto operator()(R&& r) const {
                    if constexpr (std::ranges::constant_range<std::views::all_t<R>>)
                        return std::views::all(std::forward<R>(r));
                    else if constexpr (is_empty_view<U>)
                        return auto(std::views::empty<const empty_view_underlying_type<U>>);
                    else if constexpr (gold::is_specialization_of_v<U, std::span>())
                        return std::span<const typename U::element_type, U::extent>(std::forward<R>(r));
                    else if constexpr (is_ref_view_and_constant_range<U>)
                        return std::ranges::ref_view(gold::as_const(r.base()));
                    else if constexpr (std::is_lvalue_reference_v<R>
                                    && std::ranges::constant_range<const U>
                                    && !std::ranges::view<U>)
                        return std::ranges::ref_view(static_cast<const U&>(r));
                    else
                        return const_each_view { std::forward<R>(r) };
                }

            };

        } // namespace __views

        /// ranges::views::const_each
        inline constexpr range_adaptor_closure<__views::const_each_fn> const_each {};

    } // namespace views

} // namespace gold::ranges

namespace std::ranges {
    template <typename T>
    inline constexpr bool enable_borrowed_range<gold::ranges::const_each_view<T>> =
        enable_borrowed_range<T>;
}

#endif // __GOLD_BITS_RANGES_CONST_EACH_VIEW_HPP
