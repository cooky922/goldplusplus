// <gold/bits/__util/uneval.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_UTIL_UNEVAL_HPP
#define __GOLD_BITS_UTIL_UNEVAL_HPP

namespace gold::__util {

    /// __util::uneval
    // note: this variable can be used as a probable substitute for std::declval
    //       and can also be used as a type selection return value
    template <typename T>
    inline constexpr auto (*uneval)() -> T = nullptr;

    template <typename T>
    inline constexpr auto (*uneval<T[]>)() -> T(&&)[] = nullptr;

    template <typename T, auto N>
    inline constexpr auto (*uneval<T[N]>)() -> T(&&)[N] = nullptr;

    template <typename R, typename... Args>
    inline constexpr auto (*uneval<R(Args...)>)() -> R(&)(Args...) = nullptr;

    template <typename R, typename... Args>
    inline constexpr auto (*uneval<R(Args......)>)() -> R(&)(Args......) = nullptr;

    // note: it doesn't work well with pure functions that has cv and ref qualifiers

    /// __util::declval
    template <typename T>
    inline constexpr auto declval = uneval<T&&>;

    template <>
    inline constexpr auto declval<void> = uneval<void>;

} // namespace gold::__util

#endif // __GOLD_BITS_UTIL_UNEVAL_HPP
