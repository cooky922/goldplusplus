// <gold/bits/ranges/slide_view.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RANGES_SLIDE_VIEW_HPP
#define __GOLD_BITS_RANGES_SLIDE_VIEW_HPP

#include <ranges>
#include <gold/bits/ranges/range_primitives.hpp>
#include <gold/bits/ranges/view_util.hpp>
#include <gold/bits/ranges/range_adaptor.hpp>
#include <gold/bits/casts.hpp>

namespace gold::ranges {

    namespace __ranges {

        /// ranges::__ranges::slide_caches_nothing
        template <typename V>
        concept slide_caches_nothing =
               std::ranges::random_access_range<V>
            && std::ranges::sized_range<V>;

        /// ranges::__ranges::slide_caches_last
        template <typename V>
        concept slide_caches_last =
               !__ranges::slide_caches_nothing<V>
            && std::ranges::bidirectional_range<V>
            && std::ranges::common_range<V>;

        /// ranges::__ranges::slide_caches_first
        template <typename V>
        concept slide_caches_first =
               !__ranges::slide_caches_nothing<V>
            && !__ranges::slide_caches_last<V>;

    } // namespace __ranges

    /// ranges::slide_view
    template <std::ranges::forward_range V>
        requires std::ranges::view<V>
    class slide_view : public std::ranges::view_interface<slide_view<V>> {
      private:
        V m_base_;
        std::ranges::range_difference_t<V> m_n_;

        using CachedBegin = __ranges::maybe_present_t<
            __ranges::slide_caches_first<V>,
            __ranges::cached_position<V>
        >;
        using CachedEnd   = __ranges::maybe_present_t<
            __ranges::slide_caches_last<V>,
            __ranges::cached_position<V>
        >;

        [[no_unique_address]] CachedBegin m_cached_begin_ = CachedBegin();
        [[no_unique_address]] CachedEnd m_cached_end_     = CachedEnd();

        class sentinel;

        /// ::iterator
        template <bool Const>
        class iterator {
          private:
            friend slide_view;
            friend iterator<!Const>;
            friend class sentinel;

            using Base     = __ranges::maybe_const_t<Const, V>;
            using LastElem = __ranges::maybe_present_t<__ranges::slide_caches_first<Base>, std::ranges::iterator_t<Base>>;

            std::ranges::iterator_t<Base>       m_current_   = std::ranges::iterator_t<Base>();
            [[no_unique_address]] LastElem m_last_elem_ = LastElem();
            std::ranges::range_difference_t<Base>     m_n_         = 0;

            constexpr iterator(std::ranges::iterator_t<Base> current, std::ranges::range_difference_t<Base> n)
                requires (!__ranges::slide_caches_first<Base>)
            : m_current_(current), m_n_(n) {}

            constexpr iterator(std::ranges::iterator_t<Base> current,
                               std::ranges::iterator_t<Base> last_elem,
                               std::ranges::range_difference_t<Base> n)
                requires __ranges::slide_caches_first<Base>
            : m_current_(current), m_last_elem_(last_elem), m_n_(n) {}

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
            using value_type        = decltype(std::views::counted(m_current_, m_n_));
            using difference_type   = std::ranges::range_difference_t<Base>;

            constexpr iterator() = default;

            constexpr iterator(iterator<!Const> other)
                requires Const
                      && std::convertible_to<std::ranges::iterator_t<V>, std::ranges::iterator_t<Base>>
            : m_current_(std::move(other.m_current_)),
              m_n_(other.m_n_) {}

            constexpr auto operator*() const { return std::views::counted(m_current_, m_n_); }

            constexpr iterator& operator++() {
                ++m_current_;
                if constexpr (__ranges::is_present<LastElem>)
                    ++m_last_elem_;
                return *this;
            }

            constexpr iterator operator++(int) {
                auto temp = *this;
                ++*this;
                return temp;
            }

            constexpr iterator& operator--() requires std::ranges::bidirectional_range<Base> {
                --m_current_;
                if constexpr (__ranges::is_present<LastElem>)
                    --m_last_elem_;
                return *this;
            }

            constexpr iterator operator--(int) requires std::ranges::bidirectional_range<Base> {
                auto temp = *this;
                --*this;
                return temp;
            }

            constexpr iterator& operator+=(difference_type n) requires std::ranges::random_access_range<Base> {
                m_current_ += n;
                if constexpr (__ranges::is_present<LastElem>)
                    m_last_elem_ += n;
                return *this;
            }

            constexpr iterator& operator-=(difference_type n) requires std::ranges::random_access_range<Base> {
                m_current_ -= n;
                if constexpr (__ranges::is_present<LastElem>)
                    m_last_elem_ -= n;
                return *this;
            }

            constexpr auto operator[](difference_type n) requires std::ranges::random_access_range<Base> {
                return std::views::counted(m_current_ + n, m_n_);
            }

            friend constexpr bool operator==(const iterator& lhs, const iterator& rhs) {
                if constexpr (__ranges::is_present<LastElem>)
                    return lhs.m_last_elem_ == rhs.m_last_elem_;
                else
                    return lhs.m_current_ == rhs.m_current_;
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
                      && std::three_way_comparable<std::ranges::iterator_t<Base>>
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

            friend constexpr iterator operator-(const iterator& iter, difference_type n)
                requires std::ranges::random_access_range<Base>
            {
                auto r = iter;
                r -= n;
                return r;
            }

            friend constexpr difference_type operator-(const iterator& lhs, const iterator& rhs)
                requires std::sized_sentinel_for<std::ranges::iterator_t<Base>, std::ranges::iterator_t<Base>>
            {
                if constexpr (__ranges::is_present<LastElem>)
                    return lhs.m_last_elem_ - rhs.m_last_elem_;
                else
                    return lhs.m_current_ - rhs.m_current_;
            }

        };

        /// ::sentinel
        class sentinel {
          private:
            friend slide_view;

            std::ranges::sentinel_t<V> m_sent_ = std::ranges::sentinel_t<V>();

            constexpr explicit sentinel(std::ranges::sentinel_t<V> sent)
            : m_sent_(sent) {}

          public:
            constexpr sentinel() = default;

            friend constexpr bool operator==(const iterator<false>& iter, const sentinel& sent) {
                return iter.m_last_elem_ == sent.m_sent_;
            }

            friend constexpr std::ranges::range_difference_t<V> operator-(const iterator<false>& iter, const sentinel& sent)
                requires std::sized_sentinel_for<std::ranges::sentinel_t<V>, std::ranges::iterator_t<V>>
            {
                return iter.m_last_elem_ - sent.m_sent_;
            }

            friend constexpr std::ranges::range_difference_t<V> operator-(const sentinel& sent, const iterator<false>& iter)
                requires std::sized_sentinel_for<std::ranges::sentinel_t<V>, std::ranges::iterator_t<V>>
            {
                return sent.m_sent_ - iter.m_last_elem_;
            }

        };

      public:
        constexpr explicit slide_view(V base, std::ranges::range_difference_t<V> n)
        : m_base_(std::move(base)), m_n_(n) {}

        constexpr auto begin()
            requires (!__ranges::simple_view<V>)
                  || (!__ranges::slide_caches_nothing<const V>)
        {
            if constexpr (__ranges::slide_caches_first<V>) {
                auto iter = std::ranges::begin(m_base_);

                if (m_cached_begin_._M_has_value())
                    return iterator<false>{std::move(iter), m_cached_begin_._M_get(m_base_), m_n_};

                auto next_iter = std::ranges::next(iter, m_n_ - 1, std::ranges::end(m_base_));
                m_cached_begin_._M_set(m_base_, next_iter);

                return iterator<false>{std::move(iter), std::move(next_iter), m_n_};
            } else {
                return iterator<false>{std::ranges::begin(m_base_), m_n_};
            }
        }

        constexpr auto begin() const
            requires __ranges::slide_caches_nothing<const V>
        {
            return iterator<true>{std::ranges::begin(m_base_), m_n_};
        }

        constexpr auto end()
            requires (!__ranges::simple_view<V>)
                  || (!__ranges::slide_caches_nothing<const V>)
        {
            if constexpr (__ranges::slide_caches_nothing<V>) {
                return iterator<false>{std::ranges::begin(m_base_) + std::ranges::range_difference_t<V>(size()), m_n_};
            } else if constexpr (__ranges::slide_caches_last<V>) {
                auto sent = std::ranges::end(m_base_);

                if (m_cached_end_._M_has_value())
                    return iterator<false>{m_cached_end_._M_get(m_base_), m_n_};

                auto prev_iter = std::ranges::prev(sent, m_n_ - 1, std::ranges::begin(m_base_));
                m_cached_end_._M_set(m_base_, prev_iter);

                return iterator<false>{std::move(prev_iter), m_n_};
            } else if constexpr (std::ranges::common_range<V>) {
                return iterator<false>{std::ranges::end(m_base_), std::ranges::end(m_base_), m_n_};
            } else {
                return sentinel{std::ranges::end(m_base_)};
            }
        }

        constexpr auto end() const
            requires __ranges::slide_caches_nothing<const V>
        {
            return begin() + std::ranges::range_difference_t<const V>(size());
        }

        constexpr auto size() requires std::ranges::sized_range<V> {
            auto sz = std::ranges::distance(m_base_) - m_n_ + 1;
            if (sz < 0)
                sz = 0;
            return gold::to_unsigned(sz);
        }

        constexpr auto size() const requires std::ranges::sized_range<const V> {
            auto sz = std::ranges::distance(m_base_) - m_n_ + 1;
            if (sz < 0)
                sz = 0;
            return gold::to_unsigned(sz);
        }
    };

    template <typename R>
    slide_view(R&&, std::ranges::range_difference_t<R>) -> slide_view<std::views::all_t<R>>;

    namespace views {

        namespace __views {

            /// ranges::views::__views::can_slide_view
            template <typename R, typename D>
            concept can_slide_view = requires {
                slide_view { std::declval<R>(), std::declval<D>() };
            };

            /// ranges::views::__views::slide_fn
            struct slide_fn {

                template <std::ranges::viewable_range R, typename D = std::ranges::range_difference_t<R>>
                    requires can_slide_view<R, D>
                [[nodiscard]] constexpr auto operator()(R&& r, std::type_identity_t<D> n) const {
                    return slide_view { std::forward<R>(r), n };
                }

                inline static constexpr int arity = 2;

                template <typename T>
                inline static constexpr bool has_simple_extra_args = __ranges::integer_like<T>;

            };

        } // namespace __views

        /// ranges::views::slide
        inline constexpr range_adaptor<__views::slide_fn> slide {};

    } // namespace views

} // namespace gold::ranges

#endif // __GOLD_BITS_RANGES_SLIDE_VIEW_HPP
