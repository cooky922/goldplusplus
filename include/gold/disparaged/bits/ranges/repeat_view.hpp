// <gold/bits/ranges/repeat_view.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RANGES_REPEAT_VIEW_HPP
#define __GOLD_BITS_RANGES_REPEAT_VIEW_HPP

#include <tuple>
#include <gold/bits/casts.hpp>
#include <gold/bits/ranges/view_util.hpp>

namespace gold::ranges {

    /// repeat_view
    template <std::move_constructible W, std::semiregular Bound = std::unreachable_sentinel_t>
        requires (std::is_object_v<W>
               && std::same_as<W, std::remove_cv_t<W>>
               && (__ranges::integer_like<Bound> || std::same_as<Bound, std::unreachable_sentinel_t>))
    class repeat_view : public std::ranges::view_interface<repeat_view<W, Bound>> {
      private:
        inline static constexpr bool Bounded = !std::same_as<Bound, std::unreachable_sentinel_t>;

        [[no_unique_address]] __ranges::box<W> m_value_ = W();
        [[no_unique_address]] Bound m_bound_ = Bound();

        class iterator {
          private:
            using index_type = gold::conditional_t<Bounded, Bound, std::ptrdiff_t>;

            const W* m_value_;
            index_type m_current_ = index_type();

          public:
            using iterator_concept = std::random_access_iterator_tag;
            using iterator_category = std::random_access_iterator_tag;
            using value_type = W;
            using difference_type = index_type;

            constexpr iterator() = default;

            constexpr explicit iterator(const W* value, index_type b = index_type())
            : m_value_(value), m_current_(b) {}

            constexpr const W& operator*() const noexcept { return *m_value_; }

            constexpr iterator& operator++() {
                ++m_current_;
                return *this;
            }

            constexpr iterator operator++(int) {
                auto temp = *this;
                ++*this;
                return temp;
            }

            constexpr iterator& operator--() {
                --m_current_;
                return *this;
            }

            constexpr iterator operator--(int) {
                auto temp = *this;
                --*this;
                return temp;
            }

            constexpr iterator& operator+=(difference_type n) {
                m_current_ += n;
                return *this;
            }

            constexpr iterator& operator-=(difference_type n) {
                m_current_ -= n;
                return *this;
            }

            constexpr const W& operator[](difference_type) const noexcept { return *m_value_; }

            friend constexpr bool operator==(const iterator& x, const iterator& y) {
                return x.m_current_ == y.m_current_;
            }

            friend constexpr auto operator<=>(const iterator& x, const iterator& y) {
                return x.m_current_ <=> y.m_current_;
            }

            friend constexpr iterator operator+(iterator i, difference_type n) {
                return iterator { i.m_value_, i.m_current_ + n };
            }

            friend constexpr iterator operator+(difference_type n, iterator i) {
                return iterator { i.m_value_, i.m_current_ + n };
            }

            friend constexpr iterator operator-(iterator i, difference_type n) {
                return iterator { i.m_value_, i.m_current_ - n };
            }

            friend constexpr difference_type operator-(const iterator& x, const iterator& y) {
                return x.m_current_ - y.m_current_;
            }
        };

      public:
        constexpr repeat_view() requires std::default_initializable<W> = default;

        constexpr explicit repeat_view(const W& value, Bound bound = Bound())
            requires std::copy_constructible<W>
        : m_value_(value), m_bound_(bound) {}

        constexpr explicit repeat_view(W&& value, Bound bound = Bound())
        : m_value_(std::move(value)), m_bound_(bound) {}

        template <typename... WArgs, typename... BoundArgs>
          requires std::constructible_from<W, WArgs...>
                && std::constructible_from<Bound, BoundArgs...>
        constexpr explicit repeat_view(std::piecewise_construct_t,
                                       std::tuple<WArgs...> value_args,
                                       std::tuple<BoundArgs...> bound_args = std::tuple<>{})
        : m_value_(std::make_from_tuple<W>(value_args)),
          m_bound_(std::make_from_tuple<Bound>(bound_args)) {}

        constexpr iterator begin() const { return iterator { std::addressof(*m_value_) }; }

        constexpr iterator end() const requires Bounded {
            return iterator { std::addressof(*m_value_), m_bound_ };
        }

        static constexpr std::unreachable_sentinel_t end() noexcept { return std::unreachable_sentinel; }

        constexpr auto size() const requires Bounded {
            return gold::to_unsigned(m_bound_);
        }
    };

    template <typename W, typename Bound>
    repeat_view(W, Bound) -> repeat_view<W, Bound>;

    namespace views {

        namespace __views {

            /// ranges::views::__views::can_repeat_view
            template <typename T, typename U = void>
            concept can_repeat_view = [] {
                if constexpr (std::is_void_v<U>)
                    return requires { repeat_view { std::declval<T>() }; };
                else
                    return requires { repeat_view { std::declval<T>(), std::declval<U>() }; };
            }();

            /// ranges::views::__views::repeat_fn
            struct repeat_fn {

                template <typename T>
                    requires can_repeat_view<T>
                [[nodiscard]] constexpr auto operator() (T&& t) const noexcept {
                    return repeat_view { std::forward<T>(t) };
                }

                template <typename T, typename U>
                    requires can_repeat_view<T, U>
                [[nodiscard]] constexpr auto operator() (T&& t, U&& u) const noexcept {
                    return repeat_view { std::forward<T>(t), std::forward<U>(u) };
                }

            };

        } // namespace __views

        /// ranges::views::repeat
        inline constexpr __views::repeat_fn repeat {};

    } // namespace views


} // namespace gold::ranges

#endif // __GOLD_BITS_RANGES_REPEAT_VIEW_HPP
