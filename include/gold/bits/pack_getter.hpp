// <gold/bits/pack_getter.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_PACK_GETTER_HPP
#define __GOLD_BITS_PACK_GETTER_HPP

#include <bits/utility.h>
#include <gold/bits/void_like.hpp>

namespace gold::__pack {

    template <std::size_t, typename T>
    struct indexed_ { using type = T; };

    template <typename, typename...>
    struct indexer_;

    template <std::size_t... Is, typename... Ts>
    struct indexer_<std::index_sequence<Is...>, Ts...>
    : indexed_<Is, Ts> ... {};

    template <std::size_t I, typename T>
    auto select_(const indexed_<I, T>&) -> indexed_<I, T>;

    template <typename... Ts>
    struct pack_at_back_impl_ {
        using type = typename decltype((std::type_identity<Ts>{}, ...))::type;
    };

    template <typename... Ts>
    struct pack_at_front_impl_ {
        using type = typename decltype((..., std::type_identity<Ts>{}))::type;
    };

    /// get_at_impl
    template <typename>
    struct get_at_impl;

    template <std::size_t... Is>
    struct get_at_impl<std::index_sequence<Is...>> {

        template <typename Arg>
        static constexpr decltype(auto) at(decltype((Is, void_like{})) ..., Arg&& arg, ...) {
            return std::forward<Arg>(arg);
        }

        template <typename Arg>
        constexpr decltype(auto) operator()(decltype((Is, void_like{})) ..., Arg&& arg, ...) {
            return std::forward<Arg>(arg);
        }
    };

} // namespace gold::__pack

#endif // __GOLD_BITS_PACK_GETTER_HPP
