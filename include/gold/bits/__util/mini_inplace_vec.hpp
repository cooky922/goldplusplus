// <gold/bits/__util/mini_inplace_vec.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_UTIL_MINI_INPLACE_VEC_HPP
#define __GOLD_BITS_UTIL_MINI_INPLACE_VEC_HPP

#include <cstddef>
#include <gold/bits/memory/ops.hpp>

namespace gold::__util {

    /// __util::mini_inplace_vec
    // only used for trivial types
    template <typename T, std::size_t N>
    struct mini_inplace_vec {
        using value_type      = T;
        using reference       = T&;
        using const_reference = const T&;
        using pointer         = T*;
        using const_pointer   = const T*;
        using size_type       = std::size_t;
        using iterator        = pointer;
        using const_iterator  = const_pointer;

        // expos only
        T m_data_[N];
        std::size_t m_size_ {0};

        constexpr mini_inplace_vec() noexcept = default;
        constexpr ~mini_inplace_vec() noexcept
        { clear(); }

        constexpr mini_inplace_vec(const mini_inplace_vec& other)
            noexcept(std::is_nothrow_copy_constructible_v<T>)
        { for (auto&& elem : other) try_emplace_back(elem); }

        constexpr mini_inplace_vec(mini_inplace_vec&& other)
            noexcept(std::is_nothrow_move_constructible_v<T>)
        { for (auto&& elem : other) try_emplace_back(std::move(elem)); }

        constexpr mini_inplace_vec& operator=(const mini_inplace_vec& other)
            noexcept(std::is_nothrow_copy_constructible_v<T>)
        {
            clear();
            for (auto&& elem : other) try_emplace_back(elem);
            return *this;
        }

        constexpr mini_inplace_vec& operator=(mini_inplace_vec&& other)
            noexcept(std::is_nothrow_move_constructible_v<T>)
        {
            clear();
            for (auto&& elem : other) try_emplace_back(std::move(elem));
            return *this;
        }

        constexpr void swap(mini_inplace_vec& other)
            noexcept(std::is_nothrow_swappable_v<T>)
        {
            auto temp = std::move(other);
            other     = std::move(*this);
            *this     = std::move(temp);
        }

        //// iterator access
        constexpr iterator begin() noexcept
        { return data(); }

        constexpr const_iterator begin() const noexcept
        { return data(); }

        constexpr const_iterator cbegin() const noexcept
        { return data(); }

        constexpr iterator end() noexcept
        { return begin() + size(); }

        constexpr const_iterator end() const noexcept
        { return begin() + size(); }

        constexpr const_iterator cend() const noexcept
        { return cbegin() + size(); }

        //// data and element access
        constexpr pointer data() noexcept
        { return m_data_; }

        constexpr const_pointer data() const noexcept
        { return m_data_; }

        constexpr size_type size() const noexcept
        { return m_size_; }

        constexpr bool empty() const noexcept
        { return capacity() == 0 || size() == 0; }

        constexpr bool full() const noexcept
        { return size() == capacity(); }

        static consteval size_type capacity() noexcept
        { return N; }

        static consteval size_type max_size() noexcept
        { return N; }

        constexpr reference operator[](size_type n)
        { return data()[n]; }

        constexpr const_reference operator[](size_type n) const
        { return data()[n]; }

        constexpr reference front()
        { return data()[0]; }

        constexpr const_reference front() const
        { return data()[0]; }

        constexpr reference back()
        { return data()[size() - 1]; }

        constexpr const_reference back() const
        { return data()[size() - 1]; }

        template <typename... Args>
        constexpr T* try_emplace_back(Args&&... args) noexcept {
            if (full())
                return nullptr;
            T* result = gold::construct_at(data() + size(), std::forward<Args>(args)...);
            ++m_size_;
            return result;
        }

        constexpr T* try_push_back(const T& val) noexcept
        { return try_emplace_back(val); }

        constexpr T* try_push_back(T&& val) noexcept
        { return try_emplace_back(std::move(val)); }

        constexpr bool try_pop_back() noexcept {
            if (empty())
                return false;
            gold::destroy_at(data() + size() - 1);
            --m_size_;
            return true;
        }

        constexpr void clear() noexcept {
            for (std::size_t i = 0; i < size(); ++i)
                gold::destroy_at(data() + i);
            m_size_ = 0;
        }
    };

} // namespace gold::__util

#endif // __GOLD_BITS_UTIL_MINI_INPLACE_VEC_HPP
