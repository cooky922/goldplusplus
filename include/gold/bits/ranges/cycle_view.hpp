// <gold/bits/ranges/cycle_view.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RANGES_CYCLE_VIEW_HPP
#define __GOLD_BITS_RANGES_CYCLE_VIEW_HPP

#include <ranges>
#include <gold/bits/ranges/range_primitives.hpp>
#include <gold/bits/ranges/view_util.hpp>
#include <gold/bits/ranges/range_adaptor.hpp>
#include <gold/bits/type_traits/specialization_of.hpp>

namespace gold::ranges {

    namespace __ranges {

        /// ranges::__ranges::unbounded_range
        template <typename T>
        concept unbounded_range = std::ranges::range<T> && std::same_as<std::ranges::sentinel_t<T>, std::unreachable_sentinel_t>;

    } // namespace __ranges

    template <std::ranges::forward_range V, std::semiregular Bound = std::unreachable_sentinel_t>
        requires std::ranges::view<V> &&
                (__ranges::integer_like<Bound> || std::same_as<Bound, std::unreachable_sentinel_t>)
    class cycle_view : public std::ranges::view_interface<cycle_view<V, Bound>> {
      private:
        static constexpr bool Bounded = !std::is_same_v<Bound, std::unreachable_sentinel_t>;
        using CachedEnd = __ranges::maybe_present_t<
            !std::ranges::common_range<V>, __ranges::non_propagating_cache<std::ranges::iterator_t<V>>
        >;

        V m_base_ = V();
        [[no_unique_address]] Bound m_bound_ = Bound();
        [[no_unique_address]] CachedEnd m_cached_end_ = CachedEnd();

        template <bool Const>
        class iterator {
          private:
            friend cycle_view;
            friend iterator<!Const>;

            using Parent   = __ranges::maybe_const_t<Const, cycle_view>;
            using Base     = __ranges::maybe_const_t<Const, V>;
            using BaseIter = std::ranges::iterator_t<Base>;
            using Counter  = gold::conditional_t<Bounded, Bound, std::size_t>;

            Parent* m_parent_ = nullptr;
            BaseIter m_current_ = BaseIter();
            Counter m_n_ = 0;

            constexpr explicit iterator(Parent* parent)
            : m_parent_(parent), m_current_(std::ranges::begin(parent->m_base_)) {}

            constexpr explicit iterator(Parent* parent, BaseIter current)
            : m_parent_(parent), m_current_(std::move(current)) {}

            constexpr explicit iterator(Parent* parent, BaseIter current, Counter n)
            : m_parent_(parent), m_current_(std::move(current)), m_n_(n) {}

            constexpr BaseIter mf_get_begin_() const {
                return std::ranges::begin(m_parent_->m_base_);
            }

            constexpr void mf_set_end_() const {
                if constexpr (!std::ranges::common_range<V>) {
                    if (!m_parent_->m_cached_end_)
                        m_parent_->m_cached_end_ = m_current_;
                }
            }

            template <bool CanBeEmpty = false>
            constexpr std::ranges::sentinel_t<Base> mf_get_end_() const {
                if constexpr (std::ranges::common_range<V>) {
                    return std::ranges::end(m_parent_->m_base_);
                } else {
                    // expect: CanBeEmpty || m_parent_->m_cached_end_
                    if (CanBeEmpty && !m_parent_->m_cached_end_)
                        m_parent_->m_cached_end_ = std::ranges::next(m_current_, std::ranges::end(m_parent_->m_base_));
                    return *m_parent_->m_cached_end_;
                }
            }

            static consteval auto s_iter_concept_() {
                if constexpr (std::ranges::random_access_range<Base>)
                    return std::random_access_iterator_tag();
                else if constexpr (std::ranges::bidirectional_range<Base>)
                    return std::bidirectional_iterator_tag();
                else
                    return std::forward_iterator_tag();
            }

          public:
            using value_type        = std::ranges::range_value_t<Base>;
            using difference_type   = std::ranges::range_difference_t<Base>;
            using iterator_concept  = decltype(s_iter_concept_());
            using iterator_category = std::input_iterator_tag;

            constexpr iterator() = default;
            constexpr iterator(iterator<!Const> other)
                requires Const && std::convertible_to<std::ranges::iterator_t<V>, BaseIter>
            : m_parent_(other.m_parent_),
              m_current_(std::move(other.m_current_)),
              m_n_(other.m_n_) {}

            constexpr std::ranges::range_reference_t<Base> operator*() const { return *m_current_; }

            constexpr iterator& operator++() {
                if (++m_current_ == std::ranges::end(m_parent_->m_base_)) {
                    m_current_ = mf_get_begin_();
                    // expect: m_n_ < bound
                    ++m_n_;
                }
                return *this;
            }

            constexpr iterator operator++(int) {
                auto temp = *this;
                ++*this;
                return temp;
            }

            constexpr iterator& operator--() requires std::ranges::bidirectional_range<Base> {
                if (m_current_ == std::ranges::begin(m_parent_->m_base_)) {
                    // expect: m_n_ > 0
                    --m_n_;
                    m_current_ = mf_get_end_();
                }
                --m_current_;
                return *this;
            }

            constexpr iterator operator--(int) requires std::ranges::bidirectional_range<Base> {
                auto temp = *this;
                --*this;
                return temp;
            }

            constexpr iterator& operator+=(difference_type n) requires std::ranges::random_access_range<Base> {
                const auto first = std::ranges::begin(m_parent_->m_base_);
                const auto last  = mf_get_end_<true>();

                const auto dist  = last - first;
                const auto current_dist = m_current_ - first;
                const auto offset = (current_dist + n) % dist;
                m_n_ += (current_dist + n) / dist;
                // expect: m_n_ >= 0

                m_current_ = first + difference_type(offset < 0 ? offset + dist : offset);
                return *this;
            }

            constexpr iterator& operator-=(difference_type n) requires std::ranges::random_access_range<Base> {
                return *this += -n;
            }

            constexpr std::ranges::range_reference_t<Base> operator[](difference_type n) const
                requires std::ranges::random_access_range<Base>
            {
                return *(*this + n);
            }

            template <bool OtherConst>
            friend constexpr bool operator==(const iterator& lhs, const iterator<OtherConst>& rhs) noexcept {
                return lhs.m_current_ == rhs.m_current_ && lhs.m_n_ == rhs.m_n_;
            }

            template <bool OtherConst>
            friend constexpr auto operator<=>(const iterator& lhs, const iterator<OtherConst>& rhs) noexcept {
                if (auto cmp = lhs.m_n_ <=> rhs.m_n_; cmp != 0)
                    return cmp;
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

            template <bool OtherConst>
            friend constexpr difference_type operator-(const iterator& lhs, const iterator<OtherConst>& rhs)
                requires std::sized_sentinel_for<std::ranges::sentinel_t<Base>, std::ranges::iterator_t<Base>>
                      && std::sized_sentinel_for<std::ranges::iterator_t<Base>, std::ranges::iterator_t<Base>>
            {
                const auto first = lhs.mf_get_begin_();
                const auto last  = lhs.template mf_get_end_<true>();
                const auto dist  = last - first;
                return (lhs.m_n_ - rhs.m_n_) * dist + (lhs.m_current_ - rhs.m_current_);
            }
        };

      public:
        constexpr cycle_view() requires std::default_initializable<V> = default;

        constexpr cycle_view(V base)
        : m_base_(std::move(base)) {}

        constexpr cycle_view(V base, Bound bound)
        : m_base_(std::move(base)), m_bound_(std::move(bound)) {}

        constexpr V base() const& requires std::copy_constructible<V> { return m_base_; }
        constexpr V base() && { return std::move(m_base_); }

        constexpr iterator<false> begin()
            requires (!__ranges::simple_view<V> || !std::ranges::common_range<V>)
        {
            return iterator<false>{this};
        }


        constexpr iterator<true> begin() const requires std::ranges::common_range<const V> {
            return iterator<true>{this};
        }

        constexpr iterator<false> end()
            requires Bounded && (!__ranges::simple_view<V> || !std::ranges::common_range<V>)
        {
            return iterator<false>{this, std::ranges::begin(m_base_), m_bound_};
        }

        constexpr iterator<true> end() const requires Bounded && std::ranges::common_range<const V> {
            return iterator<true>{this, std::ranges::begin(m_base_), m_bound_};
        }

        constexpr std::unreachable_sentinel_t end() const noexcept {
            return std::unreachable_sentinel;
        }

        constexpr auto size() noexcept requires Bounded && std::ranges::sized_range<V> {
            return std::ranges::size(m_base_) * m_bound_;
        }

        constexpr auto size() const noexcept requires Bounded && std::ranges::sized_range<const V> {
            return std::ranges::size(m_base_) * m_bound_;
        }
    };

    template <typename R, typename Bound = std::unreachable_sentinel_t>
    cycle_view(R&&, Bound = {}) -> cycle_view<std::views::all_t<R>, Bound>;

    namespace views {

        namespace __views {

            /// ranges::views::__views::can_cycle_view
            template <typename T>
            concept can_cycle_view = requires { cycle_view { std::declval<T>() }; };

            /// ranges::views::__views::can_cycle_n_view
            template <typename T, typename B>
            concept can_cycle_n_view = requires { cycle_view { std::declval<T>(), std::declval<B>() }; };

            /// ranges::views::__views::cycle_fn
            struct cycle_fn {

                template <std::ranges::viewable_range R>
                    requires (gold::is_specialization_of_v<std::remove_cvref_t<R>, std::ranges::empty_view>())
                          || (gold::is_specialization_of_v<std::remove_cvref_t<R>, std::ranges::single_view>())
                          || __ranges::unbounded_range<R>
                          || can_cycle_view<R>
                static constexpr auto operator()(R&& r) {
                    if constexpr (gold::is_specialization_of_v<std::remove_cvref_t<R>, std::ranges::empty_view>())
                        return std::forward<R>(r);
                    else if constexpr (gold::is_specialization_of_v<std::remove_cvref_t<R>, std::ranges::single_view>())
                        return std::ranges::repeat_view { *r.data() };
                    else if constexpr (__ranges::unbounded_range<R>)
                        return std::views::all(std::forward<R>(r));
                    else
                        return cycle_view { std::forward<R>(r) };
                }

            };

            /// ranges::views::__views::cycle_n_fn
            struct cycle_n_fn {

                static constexpr std::size_t arity = 2;
                static constexpr bool has_simple_extra_args = true;

                template <std::ranges::viewable_range R, typename Bound>
                    requires (gold::is_specialization_of_v<std::remove_cvref_t<R>, std::ranges::empty_view>())
                          || (gold::is_specialization_of_v<std::remove_cvref_t<R>, std::ranges::single_view>())
                          || __ranges::unbounded_range<R>
                          || can_cycle_n_view<R, Bound>
                static constexpr auto operator()(R&& r, Bound&& bound) {
                    if constexpr (gold::is_specialization_of_v<std::remove_cvref_t<R>, std::ranges::empty_view>())
                        return std::forward<R>(r);
                    else if constexpr (gold::is_specialization_of_v<std::remove_cvref_t<R>, std::ranges::single_view>())
                        return std::ranges::repeat_view { *r.data(), std::forward<Bound>(bound) };
                    else if constexpr (__ranges::unbounded_range<R>)
                        return std::views::all(std::forward<R>(r));
                    else
                        return cycle_view { std::forward<R>(r), std::forward<Bound>(bound) };
                }

            };

        } // namespace __views

        /// ranges::views::cycle
        inline constexpr range_adaptor_closure<__views::cycle_fn> cycle {};

        /// ranges::views::cycle_n
        inline constexpr range_adaptor<__views::cycle_n_fn> cycle_n {};


    } // namespace views

} // gold::ranges

#endif // __GOLD_BITS_RANGES_CYCLE_VIEW_HPP
