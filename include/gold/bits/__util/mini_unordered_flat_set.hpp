// <gold/bits/__util/mini_unordered_flat_set.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_UTIL_MINI_UNORDERED_FLAT_SET_HPP
#define __GOLD_BITS_UTIL_MINI_UNORDERED_FLAT_SET_HPP

#include <cstddef>
#include <gold/bits/algo/contains.hpp>

namespace gold::__util {

    // for incomparable values
    template <typename T, typename Container>
    struct mini_unordered_flat_set {
        using value_type     = T;
        using container_type = Container;

        [[no_unique_address]] Container m_cont_;

        constexpr auto begin(this auto& self) noexcept
        { return self.m_cont_.begin(); }

        constexpr auto cbegin() const noexcept
        { return m_cont_.cbegin(); }

        constexpr auto end(this auto& self) noexcept
        { return self.m_cont_.end(); }

        constexpr auto cend() const noexcept
        { return m_cont_.cend(); }

        // true  -> the key doesn't exist yet
        // false -> the key already exists
        constexpr bool try_push(const T& key) {
            if (__algo::contains(m_cont_, key))
                return false;
            if constexpr (requires { m_cont_.push_back(key); })
                m_cont_.push_back(key);
            else if constexpr (requires { m_cont_.try_push_back(key); })
                m_cont_.try_push_back(key);
            else
                static_assert(false, "no push back operation");
            return true;
        }

        constexpr bool try_push(T&& key) {
            if (__algo::contains(m_cont_, key))
                return false;
            if constexpr (requires { m_cont_.push_back(std::move(key)); })
                m_cont_.push_back(std::move(key));
            else if constexpr (requires { m_cont_.try_push_back(std::move(key)); })
                m_cont_.try_push_back(std::move(key));
            else
                static_assert(false, "no push back operation");
            return true;
        }

        constexpr bool try_pop() {
            if constexpr (requires { m_cont_.try_pop_back(); }) {
                return m_cont_.try_pop_back();
            } else if constexpr (requires { m_cont_.pop_back(); }) {
                m_cont_.pop_back();
                return true;
            }
        }

        constexpr bool empty() const noexcept
            requires requires { m_cont_.empty(); }
        { return m_cont_.empty(); }

        constexpr std::size_t size() const noexcept
            requires requires { m_cont_.size(); }
        { return m_cont_.size(); }

    };

} // namespace gold::__util

#endif // __GOLD_BITS_UTIL_MINI_UNORDERED_FLAT_SET_HPP
