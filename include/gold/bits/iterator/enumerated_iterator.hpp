// <gold/bits/iterator/enumerated_iterator.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ITERATOR_ENUMERATED_ITERATOR_HPP
#define __GOLD_BITS_ITERATOR_ENUMERATED_ITERATOR_HPP

#include <gold/bits/iterator/iter_primitives.hpp>

namespace gold {

    /// enumerated_iterator
    template <typename Iter>
    class enumerated_iterator {
      public:
        using iterator_category = std::output_iterator_tag;
        using base_type         = Iter;
        using value_type        = std::iter_value_t<Iter>;
        using difference_type   = std::iter_difference_t<Iter>;
        using reference         = std::iter_reference_t<Iter>;
        using pointer           = std::iterator_traits<Iter>::pointer;

      private:
        base_type m_base_;
        std::size_t m_count_ { 0 };

      public:
        constexpr enumerated_iterator() noexcept = default;
        constexpr explicit enumerated_iterator(Iter iter)
        : m_base_(std::move(iter)) {}

        constexpr enumerated_iterator& operator++() noexcept {
            ++m_base_, ++m_count_;
            return *this;
        }

        constexpr enumerated_iterator operator++(int) noexcept {
            auto temp = *this;
            ++*this;
            return temp;
        }

        constexpr reference operator*() const noexcept { return *m_base_; }
        constexpr pointer operator->() const noexcept {
            if constexpr (std::is_pointer_v<Iter>)
                return m_base_;
            return m_base_.operator->();
        }

        constexpr base_type base() const& noexcept { return m_base_; }
        constexpr base_type base() && noexcept { return std::move(m_base_); }

        constexpr std::size_t count() const noexcept { return m_count_; }
        constexpr void reset_count() noexcept { m_count_ = 0; }
    };

    /// deduction guide for 'enumerated_iterator'
    template <typename Iter>
    enumerated_iterator(Iter) -> enumerated_iterator<Iter>;

} // namespace gold

#endif // __GOLD_BITS_ITERATOR_ENUMERATED_ITERATOR_HPP
