// <gold/bits/type_name/type_name_c.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TYPE_NAME_C_HPP
#define __GOLD_BITS_TYPE_NAME_C_HPP

#include <array>
#include <source_location>
#include <string_view>
#include <bits/utility.h>

namespace gold {

    namespace __type_name {

        /// __type_name::sv_as_array
        template <std::size_t... Is>
        constexpr auto sv_as_array(std::string_view sv, std::index_sequence<Is...>) {
            return std::array { sv[Is] ... };
        }

        /// __type_name::function_name
        template <typename T>
        consteval auto function_name() {
            const auto& loc = std::source_location::current();
            return loc.function_name();
        }

        /// __type_name::type_name_c_impl
        template <typename T>
        consteval auto type_name_c_impl() {
            constexpr std::string_view fname = function_name<T>();

            #if defined (__clang__)
                constexpr std::string_view prefix = "[T = ";
                constexpr std::string_view suffix = "]";
            #elif defined (__GNUC__)
                constexpr std::string_view prefix = "[with T = ";
                constexpr std::string_view suffix = "]";
            #elif defined (_MSC_VER)
                constexpr std::string_view prefix = "function_name<";
                constexpr std::string_view suffix = ">(void)";
            #else
                #error Unsupported compiler
            #endif

            constexpr auto start = fname.find(prefix) + prefix.size();
            constexpr auto last  = fname.rfind(suffix);

            static_assert(start < last);

            constexpr auto name  = fname.substr(start, last - start);

            return sv_as_array(name, std::make_index_sequence<name.size()>{});
        }

        /// __type_name::type_name_array_holder
        template <typename T>
        struct type_name_array_holder {
            inline static constexpr auto value = type_name_c_impl<T>();
        };

    } // namespace __type_name

    /// type_name_as_array
    template <typename T>
    consteval auto type_name_as_array() noexcept {
        return __type_name::type_name_array_holder<T>::value;
    }

    /// type_name - compile type name query
    // source: source_location
    template <typename T>
    consteval auto type_name() noexcept {
        constexpr auto& name = __type_name::type_name_array_holder<T>::value;
        return std::string_view { name.data(), name.size() };
    }

    template <typename T>
    constexpr auto type_name(T&&) noexcept {
        constexpr auto& name = __type_name::type_name_array_holder<T&&>::value;
        return std::string_view { name.data(), name.size() };
    }

    /// type_name_no_cvref
    template <typename T>
    constexpr auto type_name_no_cvref(const T&) noexcept {
        return type_name<T>();
    }

    /// type_name_no_collapse
    template <typename T>
    constexpr auto type_name_no_collapse(T&&) noexcept {
        return type_name<T>();
    }

} // namespace gold

#endif // __GOLD_BITS_TYPE_NAME_C_HPP
