// <gold/bits/ranges/enumerate_view.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RANGES_ENUMERATE_VIEW_HPP
#define __GOLD_BITS_RANGES_ENUMERATE_VIEW_HPP

#include <gold/bits/tuples/fwd.hpp>
#include <gold/bits/ranges/range_primitives.hpp>
#include <gold/bits/ranges/view_util.hpp>
#include <gold/bits/ranges/range_adaptor.hpp>

namespace gold::ranges {

    /// ranges::enumerate_result [fwd]
    template <typename, typename>
    struct enumerate_result;

} // namespace gold::ranges

namespace gold::tuples {

    template <typename Index, typename Value>
    struct tuple_size<ranges::enumerate_result<Index, Value>> {
        inline static constexpr std::size_t value = 2;
    };

    template <std::size_t I, typename Index, typename Value>
    struct tuple_element<I, ranges::enumerate_result<Index, Value>> {
        using type = gold::conditional_t<I == 0, Index, Value>;
    };
}

namespace gold::ranges {

    /// ranges::enumerate_result
    template <typename Index, typename Value>
    struct enumerate_result {
        Index index;
        Value value;

        template <std::size_t I, typename T>
            requires (I == 0 || I == 1)
                  && (std::same_as<std::remove_cvref_t<T>, enumerate_result>)
        friend constexpr decltype(auto) get(T&& op) noexcept {
            if constexpr (I == 0)
                return op.index;
            else
                return op.value;
        }
    };

    /// ranges::enumerate_view
    template <std::ranges::input_range V>
        requires std::ranges::view<V>
    class enumerate_view : public std::ranges::view_interface<enumerate_view<V>> {
      private:
        V m_base_ = V();

        template <bool Const>
        class iterator {
          private:
            using Base = __ranges::maybe_const_t<Const, V>;
            using index_type = gold::conditional_t<
                std::ranges::sized_range<Base>,
                std::ranges::range_size_t<Base>,
                std::make_unsigned_t<std::ranges::range_difference_t<Base>>
            >;

            friend class iterator<!Const>;

            std::ranges::iterator_t<Base> m_current_ = std::ranges::iterator_t<Base>();
            index_type m_pos_ = 0;

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
            using reference  = enumerate_result<index_type, std::ranges::range_reference_t<Base>>;
            using value_type = enumerate_result<index_type, std::ranges::range_value_t<Base>>;
            using difference_type = std::ranges::range_difference_t<Base>;
            using iterator_category = std::input_iterator_tag;
            using iterator_concept = decltype(s_iter_concept_());

            constexpr iterator() = default;

            constexpr explicit iterator(std::ranges::iterator_t<Base> current, std::ranges::range_difference_t<Base> pos)
            : m_current_(std::move(current)), m_pos_(static_cast<index_type>(pos)) {}

            constexpr iterator(iterator<!Const> other)
                requires Const && std::convertible_to<std::ranges::iterator_t<V>, std::ranges::iterator_t<Base>>
            : m_current_(std::move(other.m_current_)), m_pos_(other.m_pos_) {}

            constexpr std::ranges::iterator_t<Base> base() const&
                requires std::copyable<std::ranges::iterator_t<Base>> { return m_current_; }

            constexpr std::ranges::iterator_t<Base> base() && { return std::move(m_current_); }

            constexpr decltype(auto) operator*() const {
                return reference{m_pos_, *m_current_};
            }

            constexpr iterator& operator++() {
                ++m_pos_;
                ++m_current_;
                return *this;
            }

            constexpr void operator++(int) requires (!std::ranges::forward_range<Base>) { ++*this; }

            constexpr iterator operator++(int) requires std::ranges::forward_range<Base> {
                auto temp = *this;
                ++*this;
                return temp;
            }

            constexpr iterator& operator--() requires std::ranges::bidirectional_range<Base> {
                --m_pos_;
                --m_current_;
                return *this;
            }

            constexpr iterator operator--(int) requires std::ranges::bidirectional_range<Base> {
                auto temp = *this;
                --*this;
                return temp;
            }

            constexpr iterator& operator+=(difference_type x)
                requires std::ranges::random_access_range<Base>
            {
                m_pos_ += x;
                m_current_ += x;
                return *this;
            }

            constexpr iterator& operator-=(difference_type x)
                requires std::ranges::random_access_range<Base>
            {
                m_pos_ -= x;
                m_current_ -= x;
                return *this;
            }

            constexpr decltype(auto) operator[](difference_type n) const
                requires std::ranges::random_access_range<Base>
            {
                return reference{static_cast<difference_type>(m_pos_ + n), *(m_current_ + n)};
            }

            friend constexpr bool operator==(const iterator& x, const iterator& y)
                requires std::equality_comparable<std::ranges::iterator_t<Base>>
            {
                return x.m_current_ == y.m_current_;
            }

            friend constexpr bool operator<(const iterator& x, const iterator& y)
                requires std::ranges::random_access_range<Base>
            {
                return x.m_current_ < y.m_current_;
            }

            friend constexpr bool operator>(const iterator& x, const iterator& y)
                requires std::ranges::random_access_range<Base>
            {
                return y < x;
            }

            friend constexpr bool operator<=(const iterator& x, const iterator& y)
                requires std::ranges::random_access_range<Base>
            {
                return !(y < x);
            }

            friend constexpr bool operator>=(const iterator& x, const iterator& y)
                requires std::ranges::random_access_range<Base>
            {
                return !(x < y);
            }

            friend constexpr auto operator<=>(const iterator& x, const iterator& y)
                requires std::ranges::random_access_range<Base>
                      && std::three_way_comparable<std::ranges::iterator_t<Base>>
            {
                return x.m_current_ <=> y.m_current_;
            }

            friend constexpr iterator operator+(const iterator& x, difference_type y)
                requires std::ranges::random_access_range<Base>
            {
                return iterator{x} += y;
            }


            friend constexpr iterator operator+(difference_type x, const iterator& y)
                requires std::ranges::random_access_range<Base>
            {
                return y + x;
            }

            friend constexpr iterator operator-(const iterator& x, difference_type y)
                requires std::ranges::random_access_range<Base>
            {
                return iterator{x} -= y;
            }

            friend constexpr difference_type operator-(const iterator& x, const iterator& y)
                requires std::ranges::random_access_range<Base>
            {
                return y.m_current_ - x.m_current_;
            }

        };

        template <bool Const>
        class sentinel {
          private:
            using Base = __ranges::maybe_const_t<Const, V>;

            friend class sentinel<!Const>;

            std::ranges::sentinel_t<Base> m_sent_ = std::ranges::sentinel_t<Base>();

          public:
            constexpr sentinel() = default;

            constexpr explicit sentinel(std::ranges::sentinel_t<Base> sent)
            : m_sent_(sent) {}

            constexpr sentinel(sentinel<!Const> other)
                requires Const && std::convertible_to<std::ranges::sentinel_t<V>, std::ranges::sentinel_t<Base>>
            : m_sent_(other.m_sent_) {}

            constexpr std::ranges::sentinel_t<Base> base() const { return m_sent_; }

            friend constexpr bool operator==(const iterator<Const>& x, const sentinel& y) {
                return x.m_current_ == y.m_sent_;
            }

            friend constexpr std::ranges::range_difference_t<Base> operator-(const iterator<Const>& x, const sentinel& y)
                requires std::sized_sentinel_for<std::ranges::sentinel_t<Base>, std::ranges::iterator_t<Base>>
            {
                return x.m_current_ - y.m_sent_;
            }

            friend constexpr std::ranges::range_difference_t<Base> operator-(const sentinel& x, const iterator<Const>& y)
                requires std::sized_sentinel_for<std::ranges::sentinel_t<Base>, std::ranges::iterator_t<Base>>
            {
                return x.m_sent_ - y.m_current_;
            }
        };

      public:
        constexpr enumerate_view() requires std::default_initializable<V> = default;

        constexpr enumerate_view(V base) : m_base_(std::move(base)) {}

        constexpr auto begin() requires (!__ranges::simple_view<V>) {
            return iterator<false>(std::ranges::begin(m_base_), 0);
        }

        constexpr auto begin() const requires __ranges::simple_view<V> {
            return iterator<true>(std::ranges::begin(m_base_), 0);
        }

        constexpr auto end() {
            if constexpr (std::ranges::common_range<V> && std::ranges::sized_range<V>)
                return iterator<false>{std::ranges::end(m_base_), std::ranges::range_difference_t<V>(size())};
            else
                return sentinel<false>{std::ranges::end(m_base_)};
        }

        constexpr auto end() const requires std::ranges::range<const V> {
            if constexpr (std::ranges::common_range<const V> && std::ranges::sized_range<V>)
                return iterator<true>{std::ranges::end(m_base_), std::ranges::range_difference_t<V>(size())};
            else
                return sentinel<true>{std::ranges::end(m_base_)};
        }

        constexpr auto size() requires std::ranges::sized_range<V> {
            return std::ranges::size(m_base_);
        }

        constexpr auto size() const requires std::ranges::sized_range<const V> {
            return std::ranges::size(m_base_);
        }

        constexpr V base() const& requires std::copy_constructible<V> {
            return m_base_;
        }

        constexpr V base() && {
            return std::move(m_base_);
        }
    };

    template <typename R>
    enumerate_view(R&&) -> enumerate_view<std::views::all_t<R>>;

    namespace views {

        namespace __views {

            /// ranges::views::__views::can_enumerate_view
            template <typename R>
            concept can_enumerate_view = requires {
                enumerate_view { std::declval<R>() };
            };

            /// ranges::views::__views::enumerate_fn
            struct enumerate_fn {

                template <std::ranges::viewable_range R>
                    requires can_enumerate_view<R>
                [[nodiscard]] constexpr auto operator()(R&& r) const {
                    return enumerate_view { std::forward<R>(r) };
                }

            };

        } // namespace __views

        /// ranges::views::enumerate
        inline constexpr range_adaptor_closure<__views::enumerate_fn> enumerate {};

    } // namespace views

} // namespace gold::ranges

#endif // __GOLD_BITS_RANGES_ENUMERATE_VIEW_HPP
