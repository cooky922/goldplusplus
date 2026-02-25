// <gold/bits/algo/contains.hpp> - gold++ library

// Copyright (C) [ 2021 - 2025 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ALGO_CONTAINS_HPP
#define __GOLD_BITS_ALGO_CONTAINS_HPP

#include <initializer_list>

namespace gold::__algo {

    /// __algo::contains
    // note: unconstrained algorithm
    template <typename R, typename T>
    constexpr bool contains(const R& r, const T& op) noexcept {
        for (const auto& elem : r)
            if (op == elem)
                return true;
        return false;
    }

    template <typename T>
    constexpr bool contains(std::initializer_list<T> il, const T& op) noexcept {
        for (const auto& elem : il)
            if (op == elem)
                return true;
        return false;
    }

    /// __algo::string_contains
    // accepts an array representing char array
    // and expected to have null terminator at the end
    template <typename T, std::size_t N>
    constexpr bool string_contains(const T (& str)[N], const T& op) noexcept {
        for (std::size_t i = 0; i < N - 1; ++i)
            if (op == str[i])
                return true;
        return false;
    }

} // namespace gold::__algo

#endif // __GOLD_BITS_ALGO_CONTAINS_HPP
