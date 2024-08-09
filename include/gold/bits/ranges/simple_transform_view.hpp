// <gold/bits/ranges/simple_transform_view.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RANGES_SIMPLE_TRANSFORM_VIEW_HPP
#define __GOLD_BITS_RANGES_SIMPLE_TRANSFORM_VIEW_HPP

#include <bits/views_all.h>
#include <gold/bits/ranges/range_primitives.hpp>
#include <gold/bits/functional/invoke.hpp>

namespace gold::ranges::__ranges {

    /// gold::ranges::__ranges::simple_transform_view
    template <typename V, typename F>
    struct simple_transform_view {
        V base = V();
        [[no_unique_address]] F fun  = F();

        struct iterator {
            simple_transform_view* parent;
            std::ranges::iterator_t<V>  current;

            using value_type = std::ranges::range_value_t<V>;
            using difference_type = std::ranges::range_difference_t<V>;
            using iterator_concept = std::input_iterator_tag;

            constexpr decltype(auto) operator*() const {
                return gold::invoke(parent->fun, *current);
            }

            constexpr iterator& operator++() { ++current; }
            constexpr void operator++(int) { ++*this; }

            constexpr bool operator==(const iterator& other) const noexcept {
                return current == other.current;
            }

            constexpr bool operator==(std::default_sentinel_t) const noexcept {
                return current == std::ranges::end(parent->base);
            }
        };

        constexpr auto begin() {
            return iterator { this, std::ranges::begin(base) };
        }

        constexpr auto end() requires std::ranges::common_range<V> {
            return iterator { this, std::ranges::end(base) };
        }

        static constexpr std::default_sentinel_t end() noexcept {
            return std::default_sentinel;
        }

        constexpr auto size() const requires std::ranges::sized_range<V> {
            return std::ranges::size(base);
        }
    };

    template <typename R, typename F>
    simple_transform_view(R&&, F) -> simple_transform_view<std::views::all_t<R>, F>;

} // namespace gold::ranges::__ranges

#endif // __GOLD_BITS_RANGES_SIMPLE_TRANSFORM_VIEW_HPP
