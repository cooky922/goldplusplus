// <gold/bits/algo/contains.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ALGO_CONTAINS_HPP
#define __GOLD_BItS_ALGO_CONTAINS_HPP

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

} // namespace gold::__algo

#endif // __GOLD_BITS_ALGO_CONTAINS_HPP
