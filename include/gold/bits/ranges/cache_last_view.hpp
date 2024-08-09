// <gold/bits/ranges/cache_last_view.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RANGES_CACHE_LAST_VIEW_HPP
#define __GOLD_BITS_RANGES_CACHE_LAST_VIEW_HPP

#include <ranges>
#include <gold/bits/ranges/range_primitives.hpp>
#include <gold/bits/ranges/view_util.hpp>
#include <gold/bits/ranges/range_adaptor.hpp>

namespace gold::ranges {

    namespace __ranges {

        /// ranges::__ranges::as_lvalue
        template <typename T>
        constexpr T& as_lvalue(T&& op) noexcept {
            return static_cast<T&>(op);
        }

    } // namespace __ranges

    /// ranges::cache_last_view
    template <std::ranges::input_range V>
        requires std::ranges::view<V>
    class cache_last_view : public std::ranges::view_interface<cache_last_view<V>> {
      private:
        using CacheT = gold::conditional_t<
            std::is_reference_v<std::ranges::range_reference_t<V>>,
            std::add_pointer_t<std::ranges::range_reference_t<V>>,
            std::ranges::range_reference_t<V>
        >;

        V m_base_ = V();
        __ranges::non_propagating_cache<CacheT> m_cache_;

        class iterator {
          private:
            cache_last_view* m_parent_;
            std::ranges::iterator_t<V> m_current_;

            constexpr explicit iterator(cache_last_view& parent)
            : m_current_(std::ranges::begin(parent.m_base_)),
              m_parent_(std::addressof(parent)) {}

            friend cache_last_view;

          public:
            using difference_type = std::ranges::range_difference_t<V>;
            using value_type      = std::ranges::range_value_t<V>;
            using iterator_concept = std::input_iterator_tag;

            constexpr iterator(iterator&&) = default;
            constexpr iterator& operator=(iterator&&) = default;

            constexpr std::ranges::iterator_t<V> base() &&
            { return std::move(m_current_); }
            constexpr const std::ranges::iterator_t<V>& base() const& noexcept
            { return m_current_; }

            constexpr std::ranges::range_reference_t<V>& operator*() const {
                if constexpr (std::is_reference_v<std::ranges::range_reference_t<V>>) {
                    if (!m_parent_->m_cache_)
                        m_parent_->m_cache_ = std::addressof(__ranges::as_lvalue(*m_current_));
                    return **m_parent_->m_cache_;
                } else {
                    if (!m_parent_->m_cache_)
                        m_parent_->m_cache_._M_emplace_deref(m_current_);
                    return *m_parent_->m_cache_;
                }
            }

            constexpr iterator& operator++() {
                ++m_current_;
                m_parent_->m_cache_._M_reset();
                return *this;
            }

            constexpr void operator++(int) {
                ++*this;
            }

            friend constexpr std::ranges::range_rvalue_reference_t<V> iter_move(const iterator& iter)
                noexcept(noexcept(std::ranges::iter_move(iter.m_current_)))
            { return std::ranges::iter_move(iter.m_current_); }

            friend constexpr void iter_swap(const iterator& lhs, const iterator& rhs)
                noexcept(noexcept(std::ranges::iter_swap(lhs.m_current_, rhs.m_current_)))
                requires std::indirectly_swappable<std::ranges::iterator_t<V>>
            { std::ranges::iter_swap(lhs.m_current_, rhs.m_current_); }
        };

        class sentinel {
          private:
            std::ranges::sentinel_t<V> m_end_;

            constexpr explicit sentinel(cache_last_view& parent)
            : m_end_(std::ranges::end(parent.m_base_)) {}

            friend cache_last_view;

          public:
            constexpr sentinel() = default;
            constexpr std::ranges::sentinel_t<V> base() const { return m_end_; }
            friend constexpr bool operator==(const iterator& lhs, const sentinel& rhs)
            { return lhs.m_current_ == rhs.m_end_; }
        };

      public:
        constexpr cache_last_view() requires std::default_initializable<V> = default;
        constexpr explicit cache_last_view(V base)
        : m_base_(std::move(base)) {}

        constexpr V base() const & requires std::copy_constructible<V> { return m_base_; }
        constexpr V base() && { return std::move(m_base_); }

        constexpr auto begin() { return iterator(*this); }
        constexpr auto end() { return sentinel(*this); }

        constexpr auto size() requires std::ranges::sized_range<V>
        { return std::ranges::size(m_base_); }
        constexpr auto size() const requires std::ranges::sized_range<const V>
        { return std::ranges::size(m_base_); }
    };

    template <typename R>
    cache_last_view(R&&) -> cache_last_view<std::views::all_t<R>>;

    namespace views {

        namespace __views {

            /// ranges::views::__views::can_cache_last_view
            template <typename T>
            concept can_cache_last_view = requires { cache_last_view { std::declval<T>() }; };

            /// ranges::views::__views::cache_last_fn
            struct cache_last_fn {

                template <std::ranges::viewable_range R>
                    requires can_cache_last_view<R>
                static constexpr auto operator()(R&& r) {
                    return cache_last_view { std::forward<R>(r) };
                }

            };

        } // namespace __views

        /// ranges::views::cache_last
        inline constexpr range_adaptor_closure<__views::cache_last_fn> cache_last {};

    } // namespace views

} // namespace gold::ranges

#endif // __GOLD_BITS_RANGES_CACHE_LAST_VIEW_HPP
