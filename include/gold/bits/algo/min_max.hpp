// <gold/bits/algo/min_max.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ALGO_MIN_MAX_HPP
#define __GOLD_BITS_ALGO_MIN_MAX_HPP

#include <initializer_list>

namespace gold::__algo {

    /// __algo::min
    template <typename T>
    constexpr auto min(const T& lhs, const T& rhs) {
        return lhs < rhs ? lhs : rhs;
    }

    /// __algo::max
    template <typename T>
    constexpr auto max(const T& lhs, const T& rhs) {
        return lhs > rhs ? lhs : rhs;
    }

    /// __algo::min_max_result
    template <typename T>
    struct min_max_result {
        T min;
        T max;
    };

    /// __algo::min_max
    template <typename T>
    constexpr min_max_result<T> min_max(const T& lhs, const T& rhs) {
        using return_type = min_max_result<T>;
        return lhs < rhs ? return_type{.min = lhs, .max = rhs}
                         : return_type{.min = rhs, .max = lhs};
    }

    /// __algo::min_element
    template <typename T>
    constexpr auto min_element(std::initializer_list<T> il) {
        auto iter = il.begin();
        auto sent = il.end();
        T result = *iter++;
        while (iter != sent) {
            if (*iter < result)
                result = *iter;
            ++iter;
        }
        return result;
    }

    // overload for custom comparator
    template <typename T, typename Compare>
    constexpr auto min_element(std::initializer_list<T> il, Compare compare) {
        auto iter = il.begin();
        auto sent = il.end();
        T result = *iter++;
        while (iter != sent) {
            if (compare(*iter, result))
                result = *iter;
            ++iter;
        }
        return result;
    }

    /// __algo::max_element
    template <typename T>
    constexpr auto max_element(std::initializer_list<T> il) {
        auto iter = il.begin();
        auto sent = il.end();
        T result = *iter++;
        while (iter != sent) {
            if (*iter > result)
                result = *iter;
            ++iter;
        }
        return result;
    }

    // overload for custom comparator
    template <typename T, typename Compare>
    constexpr auto max_element(std::initializer_list<T> il, Compare compare) {
        return min_element(il, compare);
    }

    /// __algo::min_max_element
    template <typename T>
    constexpr min_max_result<T> min_max_element(std::initializer_list<T> il) {
        using return_type = min_max_result<T>;
        auto iter = il.begin();
        auto sent = il.end();
        T min_result = *iter;
        T max_result = *iter;
        ++iter;
        while (iter != sent) {
            if (*iter < min_result)
                min_result = *iter;
            if (*iter > max_result)
                max_result = *iter;
            ++iter;
        }
        return return_type { .min = min_result, .max = max_result };
    }

} // namespace gold::__algo

#endif // __GOLD_BITS_ALGO_MIN_MAX_HPP
