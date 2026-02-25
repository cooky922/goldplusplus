// <gold/bits/ranges/elements_view.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RANGES_ELEMENTS_VIEW_HPP
#define __GOLD_BITS_RANGES_ELEMENTS_VIEW_HPP

#include <ranges>
#include <gold/bits/casts.hpp>
#include <gold/bits/tuples/get.hpp>
#include <gold/bits/type_traits/specialization_of.hpp>
#include <gold/bits/ranges/range_adaptor.hpp>
#include <gold/bits/ranges/zip_view.hpp>

namespace gold::ranges::views {

    namespace __views {

        /// ranges::views::__views::is_zip_view
        template <typename T>
        concept is_zip_view = gold::is_specialization_of_v<std::remove_cvref_t<T>, gold::ranges::zip_view>();

        /// ranges::views::__views::can_elements_view
        template <std::size_t N, typename R>
        concept can_elements_view = requires { std::ranges::elements_view<std::views::all_t<R>, N> { std::declval<R>() }; };

        /// ranges::views::__views::elements_fn
        template <std::size_t N>
        struct elements_fn {

            template <std::ranges::viewable_range R>
                requires is_zip_view<R>
                      || can_elements_view<N, R>
            constexpr auto operator()(R&& r) noexcept {
                if constexpr (is_zip_view<R>)
                    return tuples::get<N>(gold::forward_like<R>(r.m_views_));
                else
                    return std::ranges::elements_view<std::views::all_t<R>, N>{
                        std::forward<R>(r)
                    };
            }

        };

    } // namespace __views

    /// ranges::views::elements
    template <std::size_t N>
    inline constexpr range_adaptor_closure<__views::elements_fn<N>> elements {};

} // namespace gold::ranges::views

#endif // __GOLD_BITS_RANGES_ELEMENTS_VIEW_HPP
