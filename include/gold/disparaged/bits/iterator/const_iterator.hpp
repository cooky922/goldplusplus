// <gold/bits/iterator/const_iterator.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ITERATOR_CONST_ITERATOR_HPP
#define __GOLD_BITS_ITERATOR_CONST_ITERATOR_HPP

#include <compare>
#include <gold/bits/concepts/types.hpp>
#include <gold/bits/iterator/iter_primitives.hpp>
#include <gold/bits/type_traits/specialization_of.hpp>

namespace gold {

    /// basic_const_iterator [fwd]
    template <std::input_iterator>
    class basic_const_iterator;

    namespace __iterator {

        /// __iterator::not_a_const_iterator
        template <typename Iter>
        concept not_a_const_iterator = !gold::is_specialization_of_v<Iter, basic_const_iterator>();

        /// __iterator::qualified_constant_iterator
        template <typename Iter>
        concept qualified_constant_iterator = std::input_iterator<Iter>
            && std::same_as<iter_cref_t<Iter>, iter_ref_t<Iter>>;

        /// __iterator::basic_const_iter_cat_base
        template <bool, typename>
        struct basic_const_iter_cat_base {};

        template <typename Iter>
        struct basic_const_iter_cat_base<true, Iter> {
            using iterator_category = std::iterator_traits<Iter>::iterator_category;
        };

        /// __iterator::has_arrow
        template <typename T>
        concept has_arrow = std::is_pointer_v<T> || requires (T op) { op.operator->(); };

    } // namespace __iterator

    /// basic_const_iterator
    template <std::input_iterator Iter>
    class basic_const_iterator
    : public __iterator::basic_const_iter_cat_base<std::forward_iterator<Iter>, Iter> {
      private:
        Iter m_iter_ = Iter();

        static consteval auto s_iter_concept_() {
            if constexpr (std::contiguous_iterator<Iter>)
                return std::contiguous_iterator_tag();
            else if constexpr (std::random_access_iterator<Iter>)
                return std::random_access_iterator_tag();
            else if constexpr (std::bidirectional_iterator<Iter>)
                return std::bidirectional_iterator_tag();
            else if constexpr (std::forward_iterator<Iter>)
                return std::forward_iterator_tag();
            else
                return std::input_iterator_tag();
        }

      public:
        using iterator_concept = decltype(s_iter_concept_());
        // iterator_category may be defined in __iterator::basic_const_iter_cat_base
        using value_type      = iter_val_t<Iter>;
        using reference       = iter_cref_t<Iter>;
        using pointer         = std::add_pointer_t<reference>;
        using difference_type = iter_diff_t<Iter>;

        constexpr basic_const_iterator() requires std::default_initializable<Iter> = default;

        constexpr basic_const_iterator(Iter iter)
        : m_iter_(std::move(iter)) {}

        template <std::convertible_to<Iter> UIter>
        constexpr basic_const_iterator(basic_const_iterator<UIter> iter)
        : m_iter_(std::move(iter.m_iter_)) {}

        template <std::convertible_to<Iter> UIter>
        constexpr basic_const_iterator(UIter&& iter)
        : m_iter_(std::forward<UIter>(iter)) {}

        constexpr const Iter& base() const& noexcept { return m_iter_; }
        constexpr Iter base() && noexcept { return std::move(m_iter_); }

        constexpr reference operator*() const { return static_cast<reference>(*m_iter_); }

        constexpr pointer operator->() const
            requires __iterator::has_arrow<Iter>
                  || std::is_lvalue_reference_v<iter_ref_t<Iter>>
                  && std::same_as<std::remove_cvref_t<iter_ref_t<Iter>>, value_type>
        {
            if constexpr (__iterator::has_arrow<Iter>)
                return std::to_address(m_iter_);
            else
                return std::addressof(*m_iter_);
        }

        constexpr basic_const_iterator& operator++() {
            ++m_iter_;
            return *this;
        }

        constexpr void operator++(int) { ++m_iter_; }

        constexpr basic_const_iterator operator++(int) requires std::forward_iterator<Iter> {
            auto temp = *this;
            ++*this;
            return temp;
        }

        constexpr basic_const_iterator& operator--() requires std::bidirectional_iterator<Iter> {
            --m_iter_;
            return *this;
        }

        constexpr basic_const_iterator operator--(int) requires std::bidirectional_iterator<Iter> {
            auto temp = *this;
            ++*this;
            return temp;
        }

        constexpr basic_const_iterator& operator+=(difference_type n) requires std::random_access_iterator<Iter> {
            m_iter_ += n;
            return *this;
        }

        constexpr basic_const_iterator& operator-=(difference_type n) requires std::random_access_iterator<Iter> {
            m_iter_ -= n;
            return *this;
        }

        constexpr reference operator[](difference_type n) const requires std::random_access_iterator<Iter> {
            return static_cast<reference>(m_iter_[n]);
        }

        template <std::sentinel_for<Iter> Sent>
        constexpr bool operator==(const Sent& sent) const {
            return m_iter_ == sent;
        }

        constexpr bool operator<(const basic_const_iterator& other) const
            requires std::random_access_iterator<Iter>
        {
            return m_iter_ < other.m_iter_;
        }

        constexpr bool operator<=(const basic_const_iterator& other) const
            requires std::random_access_iterator<Iter>
        {
            return m_iter_ <= other.m_iter_;
        }

        constexpr bool operator>(const basic_const_iterator& other) const
            requires std::random_access_iterator<Iter>
        {
            return m_iter_ > other.m_iter_;
        }

        constexpr bool operator>=(const basic_const_iterator& other) const
            requires std::random_access_iterator<Iter>
        {
            return m_iter_ >= other.m_iter_;
        }

        constexpr auto operator<=>(const basic_const_iterator& other) const
            requires std::random_access_iterator<Iter>
                  && std::three_way_comparable<Iter>
        {
            return m_iter_ <=> other.m_iter_;
        }

        template <__concepts::different_from<basic_const_iterator> UIter>
        constexpr bool operator<(const UIter& other) const
            requires std::random_access_iterator<Iter>
                  && std::totally_ordered_with<Iter, UIter>
        {
            return m_iter_ < other;
        }

        template <__concepts::different_from<basic_const_iterator> UIter>
        constexpr bool operator<=(const UIter& other) const
            requires std::random_access_iterator<Iter>
                  && std::totally_ordered_with<Iter, UIter>
        {
            return m_iter_ <= other;
        }

        template <__concepts::different_from<basic_const_iterator> UIter>
        constexpr bool operator>(const UIter& other) const
            requires std::random_access_iterator<Iter>
                  && std::totally_ordered_with<Iter, UIter>
        {
            return m_iter_ > other;
        }

        template <__concepts::different_from<basic_const_iterator> UIter>
        constexpr bool operator>=(const UIter& other) const
            requires std::random_access_iterator<Iter>
                  && std::totally_ordered_with<Iter, UIter>
        {
            return m_iter_ >= other;
        }

        template <__concepts::different_from<basic_const_iterator> UIter>
        constexpr auto operator<=>(const UIter& other) const
            requires std::random_access_iterator<Iter>
                  && std::totally_ordered_with<Iter, UIter>
                  && std::three_way_comparable_with<Iter, UIter>
        {
            return m_iter_ <=> other;
        }

        template <__iterator::not_a_const_iterator UIter>
        friend constexpr bool operator<(const UIter& lhs, const basic_const_iterator& other)
            requires std::random_access_iterator<Iter>
                  && std::totally_ordered_with<Iter, UIter>
        {
            return lhs < other.m_iter_;
        }

        template <__iterator::not_a_const_iterator UIter>
        friend constexpr bool operator<=(const UIter& lhs, const basic_const_iterator& other)
            requires std::random_access_iterator<Iter>
                  && std::totally_ordered_with<Iter, UIter>
        {
            return lhs <= other.m_iter_;
        }

        template <__iterator::not_a_const_iterator UIter>
        friend constexpr bool operator>(const UIter& lhs, const basic_const_iterator& other)
            requires std::random_access_iterator<Iter>
                  && std::totally_ordered_with<Iter, UIter>
        {
            return lhs > other.m_iter_;
        }

        template <__iterator::not_a_const_iterator UIter>
        friend constexpr bool operator>=(const UIter& lhs, const basic_const_iterator& other)
            requires std::random_access_iterator<Iter>
                  && std::totally_ordered_with<Iter, UIter>
        {
            return lhs >= other.m_iter_;
        }


        friend constexpr basic_const_iterator operator+(const basic_const_iterator& iter,
                                                        difference_type n)
            requires std::random_access_iterator<Iter> {
            return basic_const_iterator(iter.m_iter_ + n);
        }

        friend constexpr basic_const_iterator operator+(difference_type n,
                                                        const basic_const_iterator& iter)
            requires std::random_access_iterator<Iter> {
            return basic_const_iterator(iter.m_iter_ + n);
        }

        friend constexpr basic_const_iterator operator-(const basic_const_iterator& iter,
                                                        difference_type n)
            requires std::random_access_iterator<Iter> {
            return basic_const_iterator(iter.m_iter_ - n);
        }

        template <std::sized_sentinel_for<Iter> Sent>
        constexpr difference_type operator-(const Sent& sent) const {
            return m_iter_ - sent;
        }

        template <__iterator::not_a_const_iterator Sent>
            requires std::sized_sentinel_for<Sent, Iter>
        friend constexpr difference_type operator-(const Sent& sent, const basic_const_iterator& iter) {
            return sent - iter.m_iter_;
        }

    };

} // namespace gold

/// std::common_type spec. for gold::basic_const_iterator
template <typename T, std::common_with<T> U>
struct std::common_type<gold::basic_const_iterator<T>, U> {
    using type = gold::basic_const_iterator<std::common_type<T, U>>;
};

template <typename T, std::common_with<T> U>
struct std::common_type<U, gold::basic_const_iterator<T>> {
    using type = gold::basic_const_iterator<std::common_type<T, U>>;
};

template <typename T, std::common_with<T> U>
struct std::common_type<gold::basic_const_iterator<U>, gold::basic_const_iterator<T>> {
    using type = gold::basic_const_iterator<std::common_type<T, U>>;
};

namespace gold {

    namespace __iterator {

        /// __iterator::swallowed_bool
        struct swallowed_bool { consteval swallowed_bool(bool) {} };

        /// __iterator::const_iterator_impl
        template <bool IsConstIter>
        struct const_iterator_impl {
            template <__iterator::swallowed_bool>
            struct inner {
                template <typename Iter>
                using type = Iter;
            };
        };

        template <>
        struct const_iterator_impl<false> {
            template <bool IsPointer>
            struct inner {
                template <typename Iter>
                using type = const gold::iter_val_t<Iter>*;
            };
        };

        template <>
        struct const_iterator_impl<false>::inner<false> {
            template <typename Iter>
            using type = gold::basic_const_iterator<Iter>;
        };

    } // namespace __iterator

    /// const_iterator
    template <std::input_iterator Iter>
    using const_iterator = __iterator::const_iterator_impl<__iterator::qualified_constant_iterator<Iter>>
                                     ::template inner<std::is_pointer_v<Iter>>
                                     ::template type<Iter>;

    namespace __iterator {

        /// __iterator::const_sentinel_impl
        template <bool IsInputIterator>
        struct const_sentinel_impl {
            template <typename Sent>
            using type = gold::const_iterator<Sent>;
        };

        template <>
        struct const_sentinel_impl<false> {
            template <typename Sent>
            using type = Sent;
        };

    } // namespace __iterator

    /// const_sentinel
    template <typename Sent>
    using const_sentinel = __iterator::const_sentinel_impl<std::input_iterator<Sent>>
                                     ::template type<Sent>;

    /// make_const_iterator
    template <std::input_iterator Iter>
    constexpr const_iterator<Iter> make_const_iterator(Iter iter) {
        return iter;
    }

    /// make_const_sentinel
    template <typename Sent>
    constexpr const_sentinel<Sent> make_const_sentinel(Sent sent) {
        return sent;
    }

} // namespace gold

#endif // __GOLD_BITS_ITERATOR_CONST_ITERATOR_HPP
