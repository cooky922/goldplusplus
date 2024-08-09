// <gold/bits/pack_getter.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_PACK_GETTER_HPP
#define __GOLD_BITS_PACK_GETTER_HPP

#include <bits/utility.h>
#include <gold/bits/void_like.hpp>

namespace gold::__pack {

    /// __pack::indexed
    template <std::size_t, typename T>
    struct indexed { using type = T; };

    /// __pack::indexer
    template <typename, typename...>
    struct indexer;

    template <std::size_t... Is, typename... Ts>
    struct indexer<std::index_sequence<Is...>, Ts...>
    : indexed<Is, Ts> ... {};

    /// [uneval] __pack::select_arg
    template <std::size_t I, typename T>
    auto select_arg(const indexed<I, T>&) -> indexed<I, T>;

    /// __pack::at_back_impl
    template <typename... Ts>
    struct at_back_impl {
        using type = typename decltype((std::type_identity<Ts>{}, ...))::type;
    };

    /// __pack::at_front_impl
    template <typename... Ts>
    struct at_front_impl {
        using type = typename decltype((..., std::type_identity<Ts>{}))::type;
    };

    /// __pack::get_at_impl
    template <typename>
    struct get_at_impl;

    template <std::size_t... Is>
    struct get_at_impl<std::index_sequence<Is...>> {

        template <typename Arg>
        [[gnu::always_inline]]
        static constexpr decltype(auto) at(decltype((Is, gold::void_like{})) ..., Arg&& arg, ...) noexcept {
            return std::forward<Arg>(arg);
        }

    };

} // namespace gold::__pack

#endif // __GOLD_BITS_PACK_GETTER_HPP
