// <gold/bits/__util/mini_array.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_UTIL_MINI_ARRAY_HPP
#define __GOLD_BITS_UTIL_MINI_ARRAY_HPP

#include <cstddef>
#include <gold/bits/__util/empty_t.hpp>

namespace gold::__util {

    /// __util::mini_array
    template <typename T, std::size_t N>
    struct mini_array {
        T elems[N];

        using element_type   = T;
        using value_type     = __remove_cv(T);
        using iterator       = value_type*;
        using const_iterator = const value_type*;
        inline static constexpr std::size_t extent = N;

        constexpr auto begin() noexcept { return iterator{elems}; }
        constexpr auto begin() const noexcept { return const_iterator{elems}; }
        constexpr auto end() noexcept { return iterator{elems + N}; }
        constexpr auto end() const noexcept { return const_iterator{elems + N}; }
        constexpr auto data() noexcept { return begin(); }
        constexpr auto data() const noexcept { return begin(); }
        static constexpr std::size_t size() noexcept { return N; }
        constexpr value_type& operator[](std::size_t pos) noexcept { return elems[pos]; }
        constexpr const value_type& operator[](std::size_t pos) const noexcept { return elems[pos]; }

        constexpr bool operator==(const mini_array&) const noexcept = default;
    };

    /// __util::mini_array
    template <typename T>
    struct mini_array<T, 0> {
        [[no_unique_address]] __util::empty_t elems;

        using element_type   = T;
        using value_type     = __remove_cv(T);
        using iterator       = const value_type*;
        using const_iterator = iterator;
        inline static constexpr std::size_t extent = 0;

        constexpr mini_array() noexcept = default;
        constexpr mini_array(const mini_array&) noexcept = default;
        constexpr mini_array& operator=(const mini_array&) noexcept = default;

        static constexpr auto begin() noexcept { return const_iterator{nullptr}; }
        static constexpr auto end() noexcept { return const_iterator{nullptr}; }
        static constexpr auto data() noexcept { return begin(); }
        static constexpr std::size_t size() noexcept { return 0; }
        static constexpr value_type& operator[](std::size_t) noexcept { __builtin_unreachable(); }

        constexpr bool operator==(const mini_array&) const noexcept = default;
    };

    template <typename T, typename... Ts>
        requires (__is_same(T, Ts) && ...)
    mini_array(T, Ts...) -> mini_array<T, sizeof...(Ts) + 1>;

} // namespace gold::__util

#endif // __GOLD_BITS_UTIL_MINI_ARRAY_HPP
