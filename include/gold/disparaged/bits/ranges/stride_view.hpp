// <gold/bits/ranges/stride_view.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RANGES_STRIDE_VIEW_HPP
#define __GOLD_BITS_RANGES_STRIDE_VIEW_HPP

#include <gold/bits/algo/div_ceil.hpp>
#include <gold/bits/ranges/range_primitives.hpp>
#include <gold/bits/ranges/view_util.hpp>
#include <gold/bits/ranges/range_adaptor.hpp>
#include <gold/bits/casts.hpp>

namespace gold::ranges {

    namespace __ranges {

        /// ranges::__ranges::stride_iter_cat_base
        template <typename Base, bool Present = std::ranges::forward_range<Base>>
        struct stride_iter_cat_base {
          private:
            static consteval auto s_iter_cat_() {
                using Cat = std::iterator_traits<ranges::iterator_t<Base>>::iterator_category;
                if constexpr (std::derived_from<Cat, std::random_access_iterator_tag>)
                    return std::random_access_iterator_tag();
                else
                    return Cat();
            }

          public:
            using iterator_category = decltype(s_iter_cat_());
        };


        template <typename Base>
        struct stride_iter_cat_base<Base, false> {};

    } // namespace __ranges

    /// ranges::stride_view
    template <std::ranges::input_range V>
        requires std::ranges::view<V>
    class stride_view : public std::ranges::view_interface<stride_view<V>> {
      private:
        V m_base_ = V();
        ranges::range_diff_t<V> m_stride_ = 1;

        template <bool Const>
        class iterator
        : public __ranges::stride_iter_cat_base<__ranges::maybe_const_t<Const, V>> {
          private:
            friend stride_view;
            friend iterator<!Const>;

            using Parent = __ranges::maybe_const_t<Const, stride_view>;
            using Base   = __ranges::maybe_const_t<Const, V>;

            ranges::iterator_t<Base> m_current_   = ranges::iterator_t<Base>();
            ranges::sentinel_t<Base> m_end_       = ranges::sentinel_t<Base>();
            ranges::range_diff_t<Base> m_stride_  = 0;
            ranges::range_diff_t<Base> m_missing_ = 0;

            constexpr iterator(Parent* parent, ranges::iterator_t<Base> current,
                               ranges::range_diff_t<Base> missing = 0)
            : m_current_(std::move(current)),
              m_end_(std::ranges::end(parent->m_base_)),
              m_stride_(parent->m_stride_),
              m_missing_(missing) {}

            static consteval auto s_iter_concept_() {
                if constexpr (std::ranges::random_access_range<Base>)
                    return std::random_access_iterator_tag();
                else if constexpr (std::ranges::bidirectional_range<Base>)
                    return std::bidirectional_iterator_tag();
                else if constexpr (std::ranges::forward_range<Base>)
                    return std::forward_iterator_tag();
                else
                    return std::input_iterator_tag();
            }

          public:
            using difference_type  = ranges::range_diff_t<Base>;
            using value_type       = ranges::range_val_t<Base>;
            using iterator_concept = decltype(s_iter_concept_());
            // iterator_category may be defined in __ranges::stride_iter_cat_base

            constexpr iterator()
                requires std::default_initializable<ranges::iterator_t<Base>>
            = default;

            constexpr iterator(iterator<!Const> other)
                requires Const && std::convertible_to<ranges::iterator_t<V>, ranges::iterator_t<Base>>
                               && std::convertible_to<ranges::sentinel_t<V>, ranges::sentinel_t<Base>>
            : m_current_(std::move(other.m_current_)),
              m_end_(std::move(other.m_end_)),
              m_stride_(other.m_stride_),
              m_missing_(other.m_missing_) {}

            constexpr ranges::iterator_t<Base> base() && { return std::move(m_current_); }
            constexpr const ranges::iterator_t<Base>& base() const& { return m_current_; }

            constexpr decltype(auto) operator*() const { return *m_current_; }

            constexpr iterator& operator++() {
                // precondition: m_current_ != m_end_
                m_missing_ = std::ranges::advance(m_current_, m_stride_, m_end_);
                return *this;
            }

            constexpr void operator++(int) { ++*this; }

            constexpr iterator operator++(int) requires std::ranges::forward_range<Base> {
                auto temp = *this;
                ++*this;
                return temp;
            }

            constexpr iterator& operator--() requires std::ranges::bidirectional_range<Base> {
                std::ranges::advance(m_current_, m_missing_ - m_stride_);
                m_missing_ = 0;
                return *this;
            }

            constexpr iterator operator--(int) requires std::ranges::bidirectional_range<Base> {
                auto temp = *this;
                --*this;
                return temp;
            }

            constexpr iterator& operator+=(difference_type n) requires std::ranges::random_access_range<Base> {
                // precondition: if n > 0, then ranges::distance(m_current_, m_end_) > m_stride_ * (n - 1) is true
                if (n > 0) {
                    m_missing_ = std::ranges::advance(m_current_, m_stride_ * n, m_end_);
                } else  if (n < 0) {
                    std::ranges::advance(m_current_, m_stride_ * n + m_missing_);
                    m_missing_ = 0;
                }
                return *this;
            }

            constexpr iterator& operator-=(difference_type n) requires std::ranges::random_access_range<Base> {
                return *this += -n;
            }

            friend constexpr bool operator==(const iterator& iter, std::default_sentinel_t) {
                return iter.m_current_ == iter.m_end_;
            }

            friend constexpr bool operator==(const iterator& lhs, const iterator& rhs)
                requires std::equality_comparable<ranges::iterator_t<Base>>
            {
                return lhs.m_current_ = rhs.m_current_;
            }

            friend constexpr bool operator<(const iterator& lhs, const iterator& rhs)
                requires std::ranges::random_access_range<Base>
            {
                return lhs.m_current_ < rhs.m_current_;
            }

            friend constexpr bool operator>(const iterator& lhs, const iterator& rhs)
                requires std::ranges::random_access_range<Base>
            {
                return rhs < lhs;
            }

            friend constexpr bool operator<=(const iterator& lhs, const iterator& rhs)
                requires std::ranges::random_access_range<Base>
            {
                return !(rhs < lhs);
            }

            friend constexpr bool operator>=(const iterator& lhs, const iterator& rhs)
                requires std::ranges::random_access_range<Base>
            {
                return !(lhs < rhs);
            }

            friend constexpr auto operator<=>(const iterator& lhs, const iterator& rhs)
                requires std::ranges::random_access_range<Base>
                      && std::three_way_comparable<ranges::iterator_t<Base>>
            {
                return lhs.m_current_ <=> rhs.m_current_;
            }

            friend constexpr iterator operator+(const iterator& iter, difference_type n)
                requires std::ranges::random_access_range<Base>
            {
                auto r = iter;
                r += n;
                return r;
            }

            friend constexpr iterator operator+(difference_type n, const iterator& iter)
                requires std::ranges::random_access_range<Base>
            {
                return iter + n;
            }

            friend constexpr iterator operator-(const iterator& iter, difference_type n)
                requires std::ranges::random_access_range<Base>
            {
                auto r = iter;
                r -= n;
                return r;
            }

            friend constexpr difference_type operator-(const iterator& lhs, const iterator& rhs)
                requires std::sized_sentinel_for<ranges::iterator_t<Base>, ranges::iterator_t<Base>>
            {
                const auto N = lhs.m_current_ - rhs.m_current_;
                if constexpr (std::ranges::forward_range<Base>) {
                    return (N + lhs.m_missing_ - rhs.m_missing_) / lhs.m_stride_;
                } else {
                    if (N < 0)
                        return -__algo::div_ceil(-N, lhs.m_stride_);
                    else
                        return __algo::div_ceil(N, lhs.m_stride_);
                }

                // note: when Base is input-only, the value of m_missing_ is unreliable
            }

            friend constexpr difference_type operator-(std::default_sentinel_t, const iterator& iter)
                requires std::sized_sentinel_for<ranges::sentinel_t<Base>, ranges::iterator_t<Base>>
            {
                return __algo::div_ceil(iter.m_end_ - iter.m_current_, iter.m_stride_);
            }

            friend constexpr difference_type operator-(const iterator& iter, std::default_sentinel_t sent)
                requires std::sized_sentinel_for<ranges::sentinel_t<Base>, ranges::iterator_t<Base>>
            {
                return -(sent - iter);
            }

            friend constexpr ranges::range_rref_t<Base> iter_move(const iterator& iter)
                noexcept(noexcept(std::ranges::iter_move(iter.m_current_)))
            {
                return std::ranges::iter_move(iter.m_current_);
            }

            friend constexpr void iter_swap(const iterator& lhs, const iterator& rhs)
                noexcept(noexcept(std::ranges::iter_swap(lhs.m_current_, rhs.m_current_)))
                requires std::indirectly_swappable<ranges::iterator_t<Base>>
            {
                std::ranges::iter_swap(lhs.m_current_, rhs.m_current_);
            }

        };

      public:
        // precondition: stride > 0
        constexpr explicit stride_view(V base, ranges::range_diff_t<V> stride)
        : m_base_(std::move(base)), m_stride_(stride) {}

        constexpr V base() const& requires std::copy_constructible<V> { return m_base_; }
        constexpr V base() && { return std::move(m_base_); }

        constexpr ranges::range_diff_t<V> stride() const noexcept { return m_stride_; }

        constexpr auto begin() requires (!__ranges::simple_view<V>) {
            return iterator<false>(this, std::ranges::begin(m_base_));
        }

        constexpr auto begin() const requires std::ranges::range<const V> {
            return iterator<true>(this, std::ranges::begin(m_base_));
        }

        constexpr auto end() requires (!__ranges::simple_view<V>) {
            if constexpr (std::ranges::common_range<V>
                       && std::ranges::sized_range<V>
                       && std::ranges::forward_range<V>) {
                auto missing = (m_stride_ - std::ranges::distance(m_base_) % m_stride_) % m_stride_;
                return iterator<false>(this, std::ranges::end(m_base_), missing);
            } else if constexpr (std::ranges::common_range<V>
                             && !std::ranges::bidirectional_range<V>) {
                return iterator<false>(this, std::ranges::end(m_base_));
            } else {
                return std::default_sentinel;
            }
        }

        constexpr auto end() const requires std::ranges::range<const V> {
            if constexpr (std::ranges::common_range<const V>
                       && std::ranges::sized_range<const V>
                       && std::ranges::forward_range<const V>) {
                auto missing = (m_stride_ - std::ranges::distance(m_base_) % m_stride_) % m_stride_;
                return iterator<true>(this, std::ranges::end(m_base_), missing);
            } else if constexpr (std::ranges::common_range<const V>
                             && !std::ranges::bidirectional_range<const V>) {
                return iterator<true>(this, std::ranges::end(m_base_));
            } else {
                return std::default_sentinel;
            }
        }

        constexpr auto size() requires std::ranges::sized_range<V> {
            return gold::to_unsigned(__algo::div_ceil(std::ranges::distance(m_base_), m_stride_));
        }

        constexpr auto size() const requires std::ranges::sized_range<const V> {
            return gold::to_unsigned(__algo::div_ceil(std::ranges::distance(m_base_), m_stride_));
        }

    };

    template <typename R>
    stride_view(R&&, ranges::range_diff_t<R>) -> stride_view<std::views::all_t<R>>;

    namespace views {

        namespace __views {

            /// ranges::views::__views::can_stride_view
            template <typename R, typename D>
            concept can_stride_view = requires {
                stride_view { std::declval<R>(), std::declval<D>() };
            };

            /// ranges::views::__views::stride_fn
            struct stride_fn {

                template <std::ranges::viewable_range R, typename D = ranges::range_diff_t<R>>
                    requires can_stride_view<R, D>
                [[nodiscard]] constexpr auto operator()(R&& r, std::type_identity_t<D> n) const {
                    return stride_view { std::forward<R>(r), n };
                }

                inline static constexpr int arity = 2;

                template <typename T>
                inline static constexpr bool has_simple_extra_args = __ranges::integer_like<T>;

            };

        } // namespace __views

        /// ranges::views::stride
        inline constexpr range_adaptor<__views::stride_fn> stride {};

    } // namespace views

} // namespace gold::ranges

namespace std::ranges {

    /// ranges::enable_borrowed_range<gold::ranges::stride_view>
    template <typename V>
    inline constexpr bool enable_borrowed_range<gold::ranges::stride_view<V>> = enable_borrowed_range<V>;

} // namespace std::ranges

#endif // __GOLD_BITS_RANGES_STRIDE_VIEW_HPP
