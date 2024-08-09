// <gold/bits/coroutine/yieldable.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once

#ifndef __GOLD_BITS_CORO_YIELDABLE_HPP
#define __GOLD_BITS_CORO_YIELDABLE_HPP

#include <gold/bits/coroutine/promise_allocator.hpp>
#include <gold/bits/functional/invoke.hpp>
#include <gold/bits/ranges/range_adaptor.hpp>
#if __has_include(<bits/views_all.h>)
#include <bits/views_all.h>
#else
#include <ranges>
#endif

namespace gold::ranges {

    /// yieldable_view
    template <std::ranges::range R, typename Alloc = std::allocator<std::byte>>
    struct yieldable_view {
        [[no_unique_address]] R range;
        [[no_unique_address]] Alloc allocator = Alloc();

        constexpr auto begin() const { return std::ranges::begin(this->range); }
        constexpr auto end() const { return std::ranges::end(this->range); }
        constexpr auto size() const requires std::ranges::sized_range<std::remove_cvref_t<R>> {
            return std::ranges::size(this->range);
        }
    };

    template <typename R, typename Alloc = std::allocator<std::byte>>
    yieldable_view(R&&, Alloc = Alloc()) -> yieldable_view<R&&, Alloc>;

    namespace __ranges {

        /// ranges::__ranges::to_yieldable_fn
        struct to_yieldable_fn {

            template <std::ranges::viewable_range R>
            constexpr auto operator()(R&& r) const noexcept {
                return yieldable_view<R&&>{std::forward<R>(r)};
            }

        };


    } // namespace __ranges

    /// to_yieldable
    inline constexpr range_adaptor_closure<__ranges::to_yieldable_fn> to_yieldable {};

} // namespace gold::ranges

namespace std::ranges {

    template <typename R, typename Alloc>
    inline constexpr bool enable_view<gold::ranges::yieldable_view<R, Alloc>> = true;

} // namespace std::ranges

#endif // __GOLD_BITS_CORO_YIELDABLE_HPP
