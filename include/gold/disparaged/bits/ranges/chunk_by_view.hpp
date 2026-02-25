// <gold/bits/ranges/chunk_by_view.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RANGES_CHUNK_BY_VIEW_HPP
#define __GOLD_BITS_RANGES_CHUNK_BY_VIEW_HPP

#include <ranges>
#include <gold/bits/algo/min_max.hpp>
#include <gold/bits/ranges/range_primitives.hpp>
#include <gold/bits/ranges/view_util.hpp>
#include <gold/bits/ranges/range_adaptor.hpp>
#include <gold/bits/casts.hpp>
#include <bits/not_fn.h>

namespace gold::ranges {

    /// ranges::chunk_by_view
    template <std::ranges::forward_range V,
              std::indirect_binary_predicate<ranges::iterator_t<V>, ranges::iterator_t<V>> Pred>
        requires std::ranges::view<V> && std::is_object_v<V>
    class chunk_by_view : public std::ranges::view_interface<chunk_by_view<V, Pred>> {
      private:
        V m_base_ = V();
        [[no_unique_address]] __ranges::box<Pred> m_pred_ = Pred();
        [[no_unique_address]] __ranges::cached_position<V> m_cached_begin_;

        /// ::iterator
        class iterator {
          private:
            chunk_by_view* m_parent_ = nullptr;
            ranges::iterator_t<V> m_current_ = ranges::iterator_t<V>();
            ranges::iterator_t<V> m_next_    = ranges::iterator_t<V>();

            friend chunk_by_view;

            constexpr iterator(chunk_by_view& parent, ranges::iterator_t<V> current, ranges::iterator_t<V> next)
            : m_parent_(std::addressof(parent)),
              m_current_(current),
              m_next_(next) {}

            static consteval auto s_iter_concept_() {
                if constexpr (std::ranges::bidirectional_range<V>)
                    return std::bidirectional_iterator_tag();
                else
                    return std::forward_iterator_tag();
            }

          public:
            using value_type        = std::ranges::subrange<ranges::iterator_t<V>>;
            using difference_type   = ranges::range_diff_t<V>;
            using iterator_category = std::input_iterator_tag;
            using iterator_concept  = decltype(s_iter_concept_());

            constexpr iterator() = default;

            constexpr value_type operator*() const {
                // precondition: m_current_ != m_next_
                return std::ranges::subrange(m_current_, m_next_);
            }

            constexpr iterator& operator++() {
                // precondition: m_current_ != m_next_
                m_current_ = m_next_;
                m_next_    = m_parent_->mf_find_next_(m_current_);
                return *this;
            }

            constexpr iterator operator++(int) {
                auto temp = *this;
                ++*this;
                return temp;
            }

            constexpr iterator& operator--() requires std::ranges::bidirectional_range<V> {
                m_next_    = m_current_;
                m_current_ = m_parent_->mf_find_prev_(m_next_);
                return *this;
            }

            constexpr iterator operator--(int) requires std::ranges::bidirectional_range<V> {
                auto temp = *this;
                --*this;
                return temp;
            }

            friend constexpr bool operator==(const iterator& lhs, const iterator& rhs) {
                return lhs.m_current_ == rhs.m_current_;
            }

            friend constexpr bool operator==(const iterator& iter, std::default_sentinel_t) {
                return iter.m_current_ == iter.m_next_;
            }

        };

        constexpr ranges::iterator_t<V> mf_find_next_(ranges::iterator_t<V> current) {
            // precondition: m_pred_.has_value() is true
            return std::ranges::next(
                std::ranges::adjacent_find(
                    current,
                    std::ranges::end(m_base_),
                    std::not_fn(std::ref(*m_pred_))
                ),
                1,
                std::ranges::end(m_base_)
            );
        }

        constexpr ranges::iterator_t<V> mf_find_prev_(ranges::iterator_t<V> current)
            requires std::ranges::bidirectional_range<V>
        {
            // precondition: current != std::ranges::begin(m_base_)
            //               and m_pred_.has_value() is true
            std::ranges::reverse_view rev { std::ranges::subrange(std::ranges::begin(m_base_)), current };
            return std::ranges::prev(
                std::ranges::adjacent_find(rev, std::not_fn(std::ref(*m_pred_))).base(),
                1,
                std::ranges::begin(m_base_)
            );
        }

      public:
        constexpr chunk_by_view()
            requires std::default_initializable<V>
                  && std::default_initializable<Pred>
        = default;

        constexpr explicit chunk_by_view(V base, Pred pred)
        : m_base_(std::move(base)),
          m_pred_(std::move(pred)) {}

        constexpr V base() const& requires std::copy_constructible<V> { return m_base_; }
        constexpr V base() && { return std::move(m_base_); }

        constexpr const Pred& pred() const { return *m_pred_; }

        constexpr iterator begin() {
            // precondtion: m_pred_.has_value() is true

            auto iter = std::ranges::begin(m_base_);

            if (m_cached_begin_._M_has_value())
                return iterator{*this, std::move(iter), m_cached_begin_._M_get(m_base_)};

            auto next_iter = mf_find_next_(iter);
            m_cached_begin_._M_set(m_base_, next_iter);

            return iterator{*this, std::move(iter), std::move(next_iter)};
        }

        constexpr auto end() {
            if constexpr (std::ranges::common_range<V>) {
                auto sent = std::ranges::end(m_base_);
                return iterator{*this, sent, sent};
            } else {
                return std::default_sentinel;
            }
        }
    };

    template <typename R, typename Pred>
    chunk_by_view(R&&, Pred) -> chunk_by_view<std::views::all_t<R>, Pred>;

    namespace views {

        namespace __views {

            /// ranges::views::__views::can_chunk_by_view
            template <typename R, typename F>
            concept can_chunk_by_view = requires {
                chunk_by_view { std::declval<R>(), std::declval<F>() };
            };

            /// ranges::views::__views::chunk_by_fn
            struct chunk_by_fn {

                template <std::ranges::viewable_range R, typename F>
                    requires can_chunk_by_view<R, F>
                [[nodiscard]] constexpr auto operator()(R&& r, F&& f) const {
                    return chunk_by_view { std::forward<R>(r), std::forward<F>(f) };
                }

                inline static constexpr int arity = 2;

                inline static constexpr bool has_simple_extra_args = true;

            };

        } // namespace __views

        /// ranges::views::chunk_by
        inline constexpr range_adaptor<__views::chunk_by_fn> chunk_by {};

    } // namespace views

} // namespace gold::ranges

#endif // __GOLD_BITS_RANGES_CHUNK_BY_VIEW_HPP
