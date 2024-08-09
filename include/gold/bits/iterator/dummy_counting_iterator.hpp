// <gold/bits/iterator/dummy_counting_iterator.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ITERATOR_DUMMY_COUNTING_ITERATOR_HPP
#define __GOLD_BITS_ITERATOR_DUMMY_COUNTING_ITERATOR_HPP

#include <bits/stl_iterator_base_types.h>

namespace gold {

    //// [gold.ranges.iterator.adaptors]
    /// dummy_counting_iterator
    template <typename T>
    class dummy_counting_iterator {
      public:
        using iterator_category = std::output_iterator_tag;
        using value_type        = void;
        using difference_type   = std::ptrdiff_t;
        using pointer           = void;
        using reference         = void;

      private:
        std::size_t m_count_ { 0 };

      public:
        constexpr dummy_counting_iterator() = default;
        constexpr dummy_counting_iterator& operator=(const T&) noexcept { return *this; }
        constexpr dummy_counting_iterator& operator=(T&&) noexcept { return *this; }

        constexpr dummy_counting_iterator& operator*() noexcept { return *this; }
        constexpr dummy_counting_iterator& operator++() noexcept { ++m_count_; return *this; }
        constexpr dummy_counting_iterator operator++(int) noexcept {
            auto temp = *this;
            ++*this;
            return temp;
        }
        constexpr std::size_t count() const noexcept { return m_count_; }
        constexpr void reset_count() noexcept { m_count_ = 0; }
    };

} // namespace gold

#endif // __GOLD_BITS_ITERATOR_DUMMY_COUNTING_ITERATOR_HPP
