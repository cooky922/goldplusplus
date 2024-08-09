// <gold/bits/ranges/view_util.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RANGES_VIEW_UTIL_HPP
#define __GOLD_BITS_RANGES_VIEW_UTIL_HPP 1

#if __has_include(<bits/views_all.h>)
#include <bits/views_all.h>
#else
#include <ranges>
#endif
#include <gold/bits/memory/ops.hpp>
#include <gold/bits/type_traits/conditional.hpp>

namespace gold::ranges::__ranges {

    /// ranges::__ranges::boxable
    template <typename T>
    concept boxable = std::ranges::__detail::__boxable<T>;

    /// ranges::__ranges::box
    template <boxable T>
    using box = std::ranges::__detail::__box<T>;

    /// ranges::__ranges::non_propagating_cache
    template <typename T>
    using non_propagating_cache = std::ranges::__detail::__non_propagating_cache<T>;

    /// ranges::__ranges::cached_position
    template <typename R>
    using cached_position = std::ranges::__detail::_CachedPosition<R>;

    /// ranges::__ranges::simple_view
    template <typename T>
    concept simple_view = std::ranges::__detail::__simple_view<T>;

    /// ranges::__ranges::maybe_const_t
    template <bool B, typename T>
    using maybe_const_t = gold::conditional_t<B, const T, T>;

    /// ranges::__ranges::maybe_present_t
    template <bool B, typename T>
    using maybe_present_t = std::ranges::__detail::__maybe_present_t<B, T>;

    /// ranges::__ranges::is_absent_v
    template <typename>
    inline constexpr bool is_absent_v = false;

    template <typename T, int Disc>
    inline constexpr bool is_absent_v<std::ranges::__detail::_Absent<T, Disc>> = true;

    /// ranges::__ranges::is_present
    template <typename T>
    concept is_present = !is_absent_v<T> && !std::is_void_v<T>;

    /// ranges::__ranges::can_reference
    template <typename T>
    concept can_reference = std::__detail::__can_reference<T>;

    /// ranges::__ranges::integer_like
    template <typename T>
    concept integer_like = std::ranges::__detail::__is_integer_like<T>;

} // namespace gold::ranges::__ranges

namespace gold {

    namespace ranges::views {} // namespace ranges::views

    /// namespace alias views
    namespace views = ranges::views;

} // namespace gold

#endif // __GOLD_BITS_RANGES_VIEW_UTIL_HPP
