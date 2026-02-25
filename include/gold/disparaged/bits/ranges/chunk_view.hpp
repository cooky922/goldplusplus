// <gold/bits/ranges/chunk_view.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RANGES_CHUNK_VIEW_HPP
#define __GOLD_BITS_RANGES_CHUNK_VIEW_HPP

#include <ranges>
#include <gold/bits/algo/div_ceil.hpp>
#include <gold/bits/algo/min_max.hpp>
#include <gold/bits/ranges/range_primitives.hpp>
#include <gold/bits/ranges/view_util.hpp>
#include <gold/bits/ranges/range_adaptor.hpp>
#include <gold/bits/casts.hpp>

namespace gold::ranges {

    /// ranges::chunk_view
    template <std::ranges::view V>
        requires std::ranges::input_range<V>
    class chunk_view : public std::ranges::view_interface<chunk_view<V>> {
      private:
        V m_base_;
        ranges::range_diff_t<V> m_n_;
        ranges::range_diff_t<V> m_rem_ = 0;

        __ranges::non_propagating_cache<ranges::iterator_t<V>> m_current_;

        /// ::outer_iterator [fwd]
        class outer_iterator;

        /// ::inner_iterator
        class inner_iterator {
          private:
            friend class outer_iterator::value_type;

            chunk_view* m_parent_;

            constexpr explicit inner_iterator(chunk_view& parent)
            : m_parent_(std::addressof(parent)) {}

          public:
            using iterator_concept = std::input_iterator_tag;
            using difference_type  = ranges::range_diff_t<V>;
            using value_type       = ranges::range_val_t<V>;

            constexpr inner_iterator(inner_iterator&&) = default;
            constexpr inner_iterator& operator=(inner_iterator&&) = default;

            constexpr const ranges::iterator_t<V>& base() const& { return *m_parent_->m_current_; }

            constexpr ranges::range_ref_t<V> operator*() const { return **m_parent_->m_current_; }

            constexpr inner_iterator& operator++() {
                ++*m_parent_->m_current_;
                if (*m_parent_->m_current_ == std::ranges::end(m_parent_->m_base_))
                    m_parent_->m_rem_ = 0;
                else
                    --m_parent_->m_rem_;
                return *this;
            }

            constexpr void operator++(int) { ++*this; }

            friend constexpr bool operator==(const inner_iterator& iter, std::default_sentinel_t) {
                return iter.m_parent_->m_rem_ == 0;
            }

            friend constexpr difference_type operator-(std::default_sentinel_t, const inner_iterator& iter)
                requires std::sized_sentinel_for<ranges::sentinel_t<V>, ranges::iterator_t<V>>
            {
                return __algo::min_element({iter.m_parent_->m_rem_,
                                        std::ranges::end(iter.m_parent_->m_base_) - *iter.m_parent_->m_current_});
            }

            friend constexpr difference_type operator-(const inner_iterator& iter, std::default_sentinel_t sent)
                requires std::sized_sentinel_for<ranges::sentinel_t<V>, ranges::iterator_t<V>>
            {
                return -(sent - iter);
            }
        };

        /// ::outer_iterator
        class outer_iterator {
          private:
            friend chunk_view;

            chunk_view* m_parent_;

            constexpr explicit outer_iterator(chunk_view& parent)
            : m_parent_(std::addressof(parent)) {}

          public:
            using iterator_concept = std::input_iterator_tag;
            using difference_type  = ranges::range_diff_t<V>;

            /// ::value_type
            struct value_type {
              private:
                friend outer_iterator;

                chunk_view* m_parent_;

                constexpr explicit value_type(chunk_view& parent)
                : m_parent_(std::addressof(parent)) {}

              public:
                constexpr inner_iterator begin() const noexcept {
                    return inner_iterator(*m_parent_);
                }

                constexpr std::default_sentinel_t end() const noexcept {
                    return std::default_sentinel;
                }

                constexpr auto size() const requires std::sized_sentinel_for<ranges::sentinel_t<V>, ranges::iterator_t<V>> {
                    return gold::to_unsigned(__algo::min_element({
                        m_parent_->m_rem_,
                        std::ranges::end(m_parent_->m_base_) - *m_parent_->m_current_
                    }));
                }
            };

            constexpr outer_iterator(outer_iterator&&) = default;
            constexpr outer_iterator& operator=(outer_iterator&&) = default;

            constexpr value_type operator*() const { return value_type(*m_parent_); }

            constexpr outer_iterator& operator++() {
                std::ranges::advance(*m_parent_->m_current_, m_parent_->m_rem_, std::ranges::end(m_parent_->m_base_));
                m_parent_->m_rem_ = m_parent_->m_n_;
                return *this;
            }

            constexpr void operator++(int) { ++*this; }

            friend constexpr bool operator==(const outer_iterator& iter, std::default_sentinel_t) {
                return *iter.m_parent_->m_current_ == std::ranges::end(iter.m_parent_->m_base_)
                     && iter.m_parent_->m_rem_ != 0;
            }

            friend constexpr difference_type operator-(std::default_sentinel_t, const outer_iterator& iter)
                requires std::sized_sentinel_for<ranges::sentinel_t<V>, ranges::iterator_t<V>>
            {
                const auto dist = std::ranges::end(iter.m_parent_->m_base_) - *iter.m_parent_->m_current_;
                if (dist < iter.m_parent_->m_rem_)
                    return dist == 0 ? 0 : 1;
                return __algo::div_ceil(dist - iter.m_parent_->m_rem_, iter.m_parent_->m_n_) + 1;
            }

            friend constexpr difference_type operator-(const outer_iterator& iter, std::default_sentinel_t sent)
                requires std::sized_sentinel_for<ranges::sentinel_t<V>, ranges::iterator_t<V>>
            {
                return -(sent - iter);
            }
        };

      public:
        constexpr explicit chunk_view(V base, ranges::range_diff_t<V> n)
        : m_base_(std::move(base)), m_n_(n) {}

        constexpr V base() const& requires std::copy_constructible<V> { return m_base_; }
        constexpr V base() && { return std::move(m_base_); }

        constexpr outer_iterator begin() {
            m_current_ = std::ranges::begin(m_base_);
            m_rem_ = m_n_;
            return outer_iterator(*this);
        }

        constexpr std::default_sentinel_t end() const noexcept {
            return std::default_sentinel;
        }

        constexpr auto size() requires std::ranges::sized_range<V> {
            return gold::to_unsigned(__algo::div_ceil(std::ranges::distance(m_base_), m_n_));
        }

        constexpr auto size() const requires std::ranges::sized_range<const V> {
            return gold::to_unsigned(__algo::div_ceil(std::ranges::distance(m_base_), m_n_));
        }
    };

    template <std::ranges::view V>
        requires std::ranges::forward_range<V>
    class chunk_view<V> : public std::ranges::view_interface<chunk_view<V>> {
      private:
        V m_base_;
        ranges::range_diff_t<V> m_n_;

        /// ::iterator
        template <bool Const>
        class iterator {
          private:
            friend chunk_view;
            friend iterator<!Const>;

            using Parent = __ranges::maybe_const_t<Const, chunk_view>;
            using Base   = __ranges::maybe_const_t<Const, V>;

            ranges::iterator_t<Base> m_current_ = ranges::iterator_t<Base>();
            ranges::sentinel_t<Base> m_end_     = ranges::sentinel_t<Base>();
            ranges::range_diff_t<Base> m_n_ = 0;
            ranges::range_diff_t<Base> m_missing_ = 0;

            constexpr iterator(Parent* parent, ranges::iterator_t<Base> current,
                               ranges::range_diff_t<Base> missing = 0)
            : m_current_(current),
              m_end_(std::ranges::end(parent->m_base_)),
              m_n_(parent->m_n_),
              m_missing_(missing) {}

            static consteval auto s_iter_concept_() {
                if constexpr (std::ranges::random_access_range<Base>)
                    return std::random_access_iterator_tag();
                else if constexpr (std::ranges::bidirectional_range<Base>)
                    return std::bidirectional_iterator_tag();
                else
                    return std::forward_iterator_tag();
            }

          public:
            using iterator_category = std::input_iterator_tag;
            using iterator_concept  = decltype(s_iter_concept_());
            using value_type        = decltype(std::views::take(std::ranges::subrange(m_current_, m_end_), m_n_));
            using difference_type   = ranges::range_diff_t<Base>;

            constexpr iterator() = default;
            constexpr iterator(iterator<!Const> other)
                requires Const && std::convertible_to<ranges::iterator_t<V>, ranges::iterator_t<Base>>
                               && std::convertible_to<ranges::sentinel_t<V>, ranges::sentinel_t<Base>>
            : m_current_(std::move(other.m_current_)),
              m_end_(std::move(other.m_end_)),
              m_n_(other.m_n_),
              m_missing_(other.m_missing_) {}

            constexpr ranges::iterator_t<Base> base() const { return m_current_; }

            constexpr value_type operator*() const {
                return std::views::take(std::ranges::subrange(m_current_, m_end_), m_n_);
            }

            constexpr iterator& operator++() {
                m_missing_ = std::ranges::advance(m_current_, m_n_, m_end_);
                return *this;
            }

            constexpr iterator operator++(int) {
                auto temp = *this;
                ++*this;
                return temp;
            }

            constexpr iterator& operator--() requires std::ranges::bidirectional_range<Base> {
                std::ranges::advance(m_current_, m_missing_ - m_n_);
                m_missing_ = 0;
                return *this;
            }

            constexpr iterator operator--(int) requires std::ranges::bidirectional_range<Base> {
                auto temp = *this;
                --*this;
                return temp;
            }

            constexpr iterator& operator+=(difference_type n) requires std::ranges::random_access_range<Base> {
                if (n > 0) {
                    m_missing_ = std::ranges::advance(m_current_, m_n_ * n, m_end_);
                } else if (n < 0) {
                    std::ranges::advance(m_current_, m_n_ * n + m_missing_);
                    m_missing_ = 0;
                }
                return *this;
            }

            constexpr iterator& operator-=(difference_type n) requires std::ranges::random_access_range<Base> {
                return *this += -n;
            }

            constexpr value_type operator[](difference_type n) const requires std::ranges::random_access_range<Base> {
                return *(*this + n);
            }

            friend constexpr bool operator==(const iterator& lhs, const iterator& rhs) {
                return lhs.m_current_ == rhs.m_current_;
            }

            friend constexpr bool operator==(const iterator& lhs, std::default_sentinel_t) {
                return lhs.m_current_ == lhs.m_end_;
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
                return (lhs.m_current_ - rhs.m_current_ + lhs.m_missing_ - rhs.m_missing_) / lhs.m_n_;
            }

            friend constexpr difference_type operator-(std::default_sentinel_t, const iterator& iter)
                requires std::sized_sentinel_for<ranges::sentinel_t<Base>, ranges::iterator_t<Base>>
            {
                return __algo::div_ceil(iter.m_end_ - iter.m_current_, iter.m_n_);
            }

            friend constexpr difference_type operator-(const iterator& iter, std::default_sentinel_t sent)
                requires std::sized_sentinel_for<ranges::sentinel_t<Base>, ranges::iterator_t<Base>>
            {
                return -(sent - iter);
            }

        };

      public:
        constexpr explicit chunk_view(V base, ranges::range_diff_t<V> n)
        : m_base_(std::move(base)), m_n_(n) {}

        constexpr V base() const& requires std::copy_constructible<V> { return m_base_; }
        constexpr V base() && { return std::move(m_base_); }

        constexpr auto begin() requires (!__ranges::simple_view<V>) {
            return iterator<false>{this, std::ranges::begin(m_base_)};
        }

        constexpr auto begin() const requires std::ranges::forward_range<const V> {
            return iterator<true>{this, std::ranges::begin(m_base_)};
        }

        constexpr auto end() requires (!__ranges::simple_view<V>) {
            if constexpr (std::ranges::common_range<V> && std::ranges::sized_range<V>) {
                auto missing = (m_n_ - std::ranges::distance(m_base_) % m_n_) % m_n_;
                return iterator<false>{this, std::ranges::end(m_base_), missing};
            } else if constexpr (std::ranges::common_range<V> && !std::ranges::bidirectional_range<V>) {
                return iterator<false>{this, std::ranges::end(m_base_)};
            } else {
                return std::default_sentinel;
            }
        }

        constexpr auto end() const requires std::ranges::forward_range<const V> {
            if constexpr (std::ranges::common_range<const V> && std::ranges::sized_range<const V>) {
                auto missing = (m_n_ - std::ranges::distance(m_base_) % m_n_) % m_n_;
                return iterator<true>{this, std::ranges::end(m_base_), missing};
            } else if constexpr (std::ranges::common_range<const V> && !std::ranges::bidirectional_range<const V>) {
                return iterator<true>{this, std::ranges::end(m_base_)};
            } else {
                return std::default_sentinel;
            }
        }

        constexpr auto size() requires std::ranges::sized_range<V> {
            return gold::to_unsigned(__algo::div_ceil(std::ranges::distance(m_base_), m_n_));
        }

        constexpr auto size() const requires std::ranges::sized_range<const V> {
            return gold::to_unsigned(__algo::div_ceil(std::ranges::distance(m_base_), m_n_));
        }

    };

    template <typename R>
    chunk_view(R&&, ranges::range_diff_t<R>) -> chunk_view<std::views::all_t<R>>;

    namespace views {

        namespace __views {

            /// ranges::views::__views::can_chunk_view
            template <typename R, typename D>
            concept can_chunk_view = requires {
                chunk_view { std::declval<R>(), std::declval<D>() };
            };

            /// ranges::views::__views::chunk_fn
            struct chunk_fn {

                template <std::ranges::viewable_range R, typename D = ranges::range_diff_t<R>>
                    requires can_chunk_view<R, D>
                [[nodiscard]] constexpr auto operator()(R&& r, std::type_identity_t<D> n) const {
                    return chunk_view { std::forward<R>(r), n };
                }

                inline static constexpr int arity = 2;

                template <typename T>
                inline static constexpr bool has_simple_extra_args = __ranges::integer_like<T>;

            };

        } // namespace __views

        /// ranges::views::chunk
        inline constexpr range_adaptor<__views::chunk_fn> chunk {};

    } // namespace views

} // namespace gold::ranges

#endif // __GOLD_BITS_RANGES_CHUNK_VIEW_HPP
