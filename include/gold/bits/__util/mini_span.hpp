// <gold/bits/__util/mini_span.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_UTIL_MINI_SPAN_HPP
#define __GOLD_BITS_UTIL_MINI_SPAN_HPP

#include <cstddef>

namespace gold::__util {

    /// __util::mini_span
    template <typename T>
    struct mini_span {
        T* elems;
        std::size_t n;

        using element_type   = T;
        using iterator       = element_type*;
        using const_iterator = const element_type*;

        constexpr mini_span() noexcept = default;

        constexpr mini_span(T* elems, std::size_t n) noexcept
        : elems(elems), n(n) {}

        constexpr mini_span(T* iter, T* sent) noexcept
        : elems(iter), n(sent - iter) {}

        template <std::size_t N>
        constexpr mini_span(T (& arr) [N]) noexcept
        : elems(arr), n(N) {}

        constexpr mini_span(const mini_span&) = default;
        constexpr mini_span& operator=(const mini_span&) = default;

        constexpr auto begin() const noexcept { return iterator{elems}; }
        constexpr auto cbegin() const noexcept { return const_iterator{elems}; }
        constexpr auto end() const noexcept { return iterator{elems + n}; }
        constexpr auto cend() const noexcept { return const_iterator{elems + n}; }
        constexpr auto data() noexcept { return begin(); }
        constexpr std::size_t size() const noexcept { return n; }
        constexpr bool empty() const noexcept { return n == 0; }
        constexpr element_type& operator[](std::size_t pos) const noexcept { return elems[pos]; }

        constexpr mini_span first(std::size_t c) const noexcept {
            // pre: 0 < c <= n
            return mini_span {elems, c};
        }

        constexpr mini_span last(std::size_t c) const noexcept {
            // pre: 0 < c <= n
            return mini_span {elems + (n - c), c};
        }

        constexpr mini_span subspan(std::size_t offset) const noexcept {
            return mini_span { elems + offset, static_cast<std::size_t>(n - offset) };
        }

        constexpr mini_span subspan(std::size_t offset, std::size_t c) const noexcept {
            return mini_span { elems + offset, c };
        }

        constexpr bool contains(const T& query) const noexcept {
            for (const auto& e : *this)
                if (e == query)
                    return true;
            return false;
        }
    };

} // namespace gold::__util

#endif // __GOLD_BITS_UTIL_MINI_SPAN_HPP
