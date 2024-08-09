// <gold/bits/ranges/move_each_view.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RANGES_MOVE_EACH_VIEW_HPP
#define __GOLD_BITS_RANGES_MOVE_EACH_VIEW_HPP

#include <gold/bits/ranges/range_primitives.hpp>
#include <gold/bits/ranges/view_util.hpp>
#include <gold/bits/ranges/range_adaptor.hpp>

namespace gold::ranges {

    /// ranges::move_each_view
    template <std::ranges::input_range V>
        requires std::ranges::view<V>
    class move_each_view : public std::ranges::view_interface<move_each_view<V>> {
      private:
        V m_base_ = V();

      public:
        constexpr move_each_view() requires std::default_initializable<V> = default;

        constexpr explicit move_each_view(V r)
        : m_base_(std::move(r)) {}

        constexpr V base() const& requires std::copy_constructible<V> { return m_base_; }
        constexpr V base() && { return std::move(m_base_); }

        constexpr auto begin() requires (!__ranges::simple_view<V>) {
            return std::move_iterator(std::ranges::begin(m_base_));
        }
        constexpr auto begin() const requires std::ranges::range<const V> {
            return std::move_iterator(std::ranges::begin(m_base_));
        }

        constexpr auto end() requires (!__ranges::simple_view<V>) {
            if constexpr (std::ranges::common_range<V> || std::input_iterator<std::ranges::sentinel_t<V>>)
                return std::move_iterator(std::ranges::end(m_base_));
            else
                return std::move_sentinel(std::ranges::end(m_base_));
        }

        constexpr auto end() const requires std::ranges::range<const V> {
            if constexpr (std::ranges::common_range<V> || std::input_iterator<std::ranges::sentinel_t<const V>>)
                return std::move_iterator(std::ranges::end(m_base_));
            else
                return std::move_sentinel(std::ranges::end(m_base_));
        }

        constexpr auto size() requires std::ranges::sized_range<V> {
            return std::ranges::size(m_base_);
        }
        constexpr auto size() const requires std::ranges::sized_range<const V> {
            return std::ranges::size(m_base_);
        }

    };

    template <typename R>
    move_each_view(R&&) -> move_each_view<std::views::all_t<R>>;

    namespace views {

        namespace __views {

            /// ranges::views::__views::can_move_each_move
            template <typename R>
            concept can_move_each_view = requires {
                move_each_view { std::declval<R>() };
            };

            /// ranges::views::__views::move_each_fn
            struct move_each_fn {

                template <std::ranges::viewable_range R>
                    requires std::same_as<std::ranges::range_rvalue_reference_t<R>, std::ranges::range_reference_t<R>>
                          || can_move_each_view<R>
                [[nodiscard]] constexpr auto operator()(R&& r) const {
                    if constexpr (std::same_as<std::ranges::range_rvalue_reference_t<R>, std::ranges::range_reference_t<R>>)
                        return std::views::all(std::forward<R>(r));
                    else
                        return move_each_view { std::forward<R>(r) };
                }

            };

        } // namespace __views

        /// ranges::views::move_each
        inline constexpr range_adaptor_closure<__views::move_each_fn> move_each {};

    } // namespace views

} // namespace gold::ranges

namespace std::ranges {
    template <typename T>
    inline constexpr bool enable_borrowed_range<gold::ranges::move_each_view<T>> =
        enable_borrowed_range<T>;
}

#endif // __GOLD_BITS_RANGES_MOVE_EACH_VIEW_HPP
