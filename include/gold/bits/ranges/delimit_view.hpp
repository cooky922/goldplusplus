// <gold/bits/ranges/delimit_view.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RANGES_DELIMIT_VIEW_HPP
#define __GOLD_BITS_RANGES_DELIMIT_VIEW_HPP

#include <ranges>
#include <gold/bits/ranges/range_primitives.hpp>
#include <gold/bits/ranges/view_util.hpp>
#include <gold/bits/ranges/range_adaptor.hpp>

namespace gold::ranges {

    /// ranges::delimit_view
    template <std::ranges::view V, std::move_constructible T>
        requires std::ranges::input_range<V> &&
                 std::is_object_v<T> &&
                 std::indirect_binary_predicate<std::ranges::equal_to, std::ranges::iterator_t<V>, const T*>
    class delimit_view : public std::ranges::view_interface<delimit_view<V, T>> {
      private:
        template <bool Const>
        struct sentinel {
          private:
            using Base = __ranges::maybe_const_t<Const, V>;
            using Sent = std::ranges::sentinel_t<Base>;

            Sent m_end_ = Sent();
            const T* m_val_ = nullptr;

            friend delimit_view;

            constexpr sentinel(Sent e, const T* val)
            : m_end_(std::move(e)), m_val_(val) {}

          public:
            constexpr sentinel() = default;

            constexpr sentinel(sentinel<!Const> s)
                requires Const && std::convertible_to<std::ranges::sentinel_t<V>, Sent>
            : m_end_(std::move(s.m_end_)), m_val_(s.m_val_) {}

            friend constexpr bool operator==(const std::ranges::iterator_t<Base>& x, const sentinel& y)
            { return y.m_end_ == x || *y.m_val_ == *x; }

            template <bool OtherConst = !Const>
                requires std::sentinel_for<Sent, std::ranges::iterator_t<__ranges::maybe_const_t<OtherConst, V>>>
            friend constexpr bool operator==(const std::ranges::iterator_t<__ranges::maybe_const_t<OtherConst, V>> x,
                                             const sentinel& y)
            { return y.m_end_ == x || *y.m_val_ == *x; }
        };

        V m_base_ = V();
        __ranges::box<T> m_val_;

      public:
        constexpr delimit_view()
            requires std::default_initializable<V> &&
                     std::default_initializable<T>
        = default;

        constexpr explicit delimit_view(V base, T value)
        : m_base_(std::move(base)), m_val_(std::move(value)) {}

        constexpr V base() const& requires std::copy_constructible<V>
        { return m_base_; }
        constexpr V base() &&
        { return std::move(m_base_); }

        constexpr auto begin() requires (!__ranges::simple_view<V>)
        { return std::ranges::begin(m_base_); }

        constexpr auto begin() const
            requires std::ranges::range<const V> &&
                     std::indirect_binary_predicate<std::ranges::equal_to, std::ranges::iterator_t<const V>, const T*>
        { return std::ranges::begin(m_base_); }

        constexpr auto end() requires (!__ranges::simple_view<V>)
        { return sentinel<false>(std::ranges::end(m_base_), std::addressof(*m_val_)); }

        constexpr auto end() const
            requires std::ranges::range<const V> &&
                     std::indirect_binary_predicate<std::ranges::equal_to, std::ranges::iterator_t<const V>, const T*>
        { return sentinel<true>(std::ranges::end(m_base_), std::addressof(*m_val_)); }
    };

    template <typename R, typename T>
    delimit_view(R&&, T) -> delimit_view<std::views::all_t<R>, T>;

    namespace views {

        namespace __views {

            template <typename R, typename T>
            concept can_delimit_view =
                requires { delimit_view { std::declval<R>(), std::declval<T>() }; };

            struct delimit_fn {
                static constexpr std::size_t arity = 2;

                template <typename R, typename T, typename E = std::remove_cvref_t<R>>
                    requires (!std::ranges::range<E> && std::input_iterator<E>) ||
                              can_delimit_view<R, T>
                static constexpr auto operator()(R&& r, T&& val) {
                    if constexpr (!std::ranges::range<E> && std::input_iterator<E>)
                        return delimit_view { std::ranges::subrange(std::forward<R>(r), std::unreachable_sentinel),
                                              std::forward<T>(val) };
                    else
                        return delimit_view { std::forward<R>(r), std::forward<T>(val) };
                }
            };

        } // namespace __views

        /// ranges::views::delimit
        inline constexpr range_adaptor<__views::delimit_fn> delimit {};

    } // namespace views

} // namespace gold::ranges

#endif // __GOLD_BITS_RANGES_DELIMIT_VIEW_HPP
