// <gold/bits/type_name/template_arg_names.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TYPE_NAME_TEMPLATE_ARG_NAMES_HPP
#define __GOLD_BITS_TYPE_NAME_TEMPLATE_ARG_NAMES_HPP

#include <array>
#include <source_location>
#include <string_view>

namespace gold {

    /// raw_tt_arg_names
    template <typename... Ts>
    inline constexpr auto raw_tt_arg_names = []<typename...> consteval {
        std::string_view&& func_name_ = std::source_location::current().function_name();
        return std::string_view { func_name_.begin() + 44, func_name_.end() - 1 };
    }.template operator()<Ts...>();

    /// raw_tnt_arg_names
    template <auto... Is>
    inline constexpr auto raw_tnt_arg_names = []<auto...> consteval {
        std::string_view&& func_name_ = std::source_location::current().function_name();
        return std::string_view { func_name_.begin() + 39, func_name_.end() - 1 };
    }.template operator()<Is...>();

    /// raw_ttt_arg_names
    template <template <typename...> typename... Ts>
    inline constexpr auto raw_ttt_arg_names = []<template <typename...> typename...> consteval {
        std::string_view&& func_name_ = std::source_location::current().function_name();
        return std::string_view { func_name_.begin() + 44, func_name_.end() - 1 };
    }.template operator()<Ts...>();

    /// raw_tntt_arg_names
    template <template <auto...> typename... Is>
    inline constexpr auto raw_tntt_arg_names = []<template <auto...> typename...> consteval {
        std::string_view&& func_name_ = std::source_location::current().function_name();
        return std::string_view { func_name_.begin() + 44, func_name_.end() - 1 };
    }.template operator()<Is...>();

    namespace __detail {

        template <std::size_t... Is>
        constexpr auto str_as_array_(std::string_view str, std::index_sequence<Is...>) {
            return std::array{ str[Is]..., '\0' };
        }

        template <typename... Ts>
        struct tt_arg_names_holder_ {
            inline static constexpr auto value = [] consteval {
                constexpr auto& temp_ = raw_tt_arg_names<Ts...>;
                return str_as_array_(
                    temp_, std::make_index_sequence<temp_.size()>{}
                );
            }();
        };

        template <auto... Is>
        struct tnt_arg_names_holder_ {
            inline static constexpr auto value = [] consteval {
                constexpr auto& temp_ = raw_tnt_arg_names<Is...>;
                return str_as_array_(
                    temp_, std::make_index_sequence<temp_.size()>{}
                );
            }();
        };

        template <template <typename...> typename... Ts>
        struct ttt_arg_names_holder_ {
            inline static constexpr auto value = [] consteval {
                constexpr auto& temp_ = raw_ttt_arg_names<Ts...>;
                return str_as_array_(
                    temp_, std::make_index_sequence<temp_.size()>{}
                );
            }();
        };

        template <template <auto...> typename... Ts>
        struct tntt_arg_names_holder_ {
            inline static constexpr auto value = [] consteval {
                constexpr auto& temp_ = raw_tntt_arg_names<Ts...>;
                return str_as_array_(
                    temp_, std::make_index_sequence<temp_.size()>{}
                );
            }();
        };

    }

    /// template_arg_names [for template type arguments]
    template <typename... Ts>
        requires (sizeof...(Ts) != 0)
    consteval auto template_arg_names() {
        constexpr auto& name_ = __detail::tt_arg_names_holder_<Ts...>::value;
        return std::string_view { name_.data(), name_.size() - 1 };
    }

    /// template_arg_names [for template type arguments (with func arguments)]
    template <typename... Ts>
        requires (sizeof...(Ts) != 0)
    constexpr auto template_arg_names(Ts&&... args) { // needed to be constexpr
        constexpr auto& name_ = __detail::tt_arg_names_holder_<decltype(args)...>::value;
        return std::string_view { name_.data(), name_.size() - 1 };
    }

    /// template_arg_names [for template non-type arguments]
    template <auto... Is>
        requires (sizeof...(Is) != 0)
    consteval auto template_arg_names() {
        constexpr auto& name_ = __detail::tnt_arg_names_holder_<Is...>::value;
        return std::string_view { name_.data(), name_.size() - 1 };
    }

    /// template_arg_names [for template type-template arguments]
    template <template <typename...> typename... Ts>
        requires (sizeof...(Ts) != 0)
    consteval auto template_arg_names() {
        constexpr auto& name_ = __detail::ttt_arg_names_holder_<Ts...>::value;
        return std::string_view { name_.data(), name_.size() - 1 };
    }

    /// template_arg_names [for template non-type-template arguments]
    template <template <auto...> typename... Ts>
        requires (sizeof...(Ts) != 0)
    consteval auto template_arg_names() {
        constexpr auto& name_ = __detail::tntt_arg_names_holder_<Ts...>::value;
        return std::string_view { name_.data(), name_.size() - 1 };
    }

    /// template_arg_names [empty case]
    template <auto... Is>
        requires (sizeof...(Is) == 0)
    consteval auto template_arg_names() {
        return std::string_view{"{}"};
    }

} // namespace gold

#endif // __GOLD_BITS_TYPE_NAME_TEMPLATE_ARG_NAMES_HPP
