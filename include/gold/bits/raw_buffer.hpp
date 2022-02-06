// <gold/bits/raw_buffer.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RAW_BUFFER_HPP
#define __GOLD_BITS_RAW_BUFFER_HPP

/// note: this header is for internal only
/// usually used for compile-time

#include <bits/ranges_algobase.h>

namespace gold::__util {

    /// raw_buffer
    template <typename T>
    class raw_buffer {
      public:
        /// value_type type alias
        using value_type = T;
        /// difference type alias
        using difference_type = std::ptrdiff_t;

      private:
        std::size_t m_cap_ { 0 }, m_size_ { 0 };
        T* m_data_ { nullptr };

      public:
        /// explicit constructors
        explicit constexpr raw_buffer(const std::size_t size) noexcept
        : m_cap_(size), m_size_(0), m_data_(new T[size]) {}

        explicit constexpr raw_buffer(const std::size_t size, const T& value) noexcept
        : raw_buffer(size) {
            setfill(value);
        }

        // noted here: data parameter must be in an heap-allocated storage
        constexpr raw_buffer(T* data, const std::size_t size)
        : m_cap_(size), m_size_(size), m_data_(data) {}

        /// destructor
        constexpr ~raw_buffer() {
            if (m_data_ != nullptr)
                delete[] m_data_;
        }

        /// iterator support
        constexpr T* begin() noexcept { return m_data_; }
        constexpr T* end() noexcept { return m_data_ + m_size_; }
        constexpr std::size_t size() const noexcept { return m_size_; }
        constexpr std::size_t capacity() const noexcept { return m_cap_; }
        constexpr void clear() noexcept { m_size_ = 0; }
        constexpr bool empty() const noexcept { return m_size_ == 0; }

        /// default_initialize
        constexpr void default_initialize() noexcept {
            for (std::size_t i = 0; i < m_cap_; ++i)
                m_data_[i] = T{};
        }

        /// setfill
        constexpr void setfill(const T& value) {
            for (std::size_t i = 0; i < m_cap_; ++i)
                m_data_[i] = value;
        }

        constexpr void reserve(const std::size_t cap) {
            if (cap > m_cap_) {
                auto temp = new T[cap];
                std::ranges::copy_n(m_data_, m_size_, temp);
                delete[] m_data_;
                m_cap_ = cap;
                m_data_ = std::move(temp);
            }
        }

        constexpr void resize(const std::size_t count) {
            reserve(count);
            m_size_ = count <= m_cap_ ? count : m_cap_;
        }

        constexpr void shrink_to_fit() {
            if (m_cap_ >= m_size_) {
                auto temp = new T[m_size_];
                std::ranges::copy_n(m_data_, m_size_, temp);
                delete[] m_data_;
                m_cap_ = m_size_;
                m_data_ = std::move(temp);
            }
        }

        constexpr T* data() { return m_data_; }

        constexpr void push_back(const T& value) {
            reserve(m_size_ + 1);
            m_data_[m_size_++] = value;
        }

    };

} // namespace gold::__util

#endif // __GOLD_BITS_RAW_BUFFER_HPP
