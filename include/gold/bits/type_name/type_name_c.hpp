// <gold/bits/type_name/type_name_c.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TYPE_NAME_C_HPP
#define __GOLD_BITS_TYPE_NAME_C_HPP

#include <source_location>
#include <string_view>
#include <gold/bits/__util/mini_array.hpp>

namespace gold {

    /// value_seq_t [fwd]
    template <auto...>
    struct value_seq_t;

    namespace __type_name {

        /// __type_name::sv_as_array
        template <auto... Is>
        consteval auto sv_as_array(std::string_view sv, gold::value_seq_t<Is...>*) {
            return __util::mini_array { sv[std::size_t(Is)] ... };
        }

        /// __type_name::function_name
        template <typename T>
        consteval auto function_name() {
            const auto& loc = std::source_location::current();
            return loc.function_name();
        }

        /// __type_name::type_name_c_impl
        // WORKAROUND: add 'typename = decltype([]{})'
        //             so that the mangled name of '[]{}' and '[] static {}'
        //             won't clash
        //             CONS: generates more unnecessary template instantiations
        template <typename T, typename = decltype([]{})>
        consteval auto& type_name_c_impl() {
            static constexpr std::string_view fname = function_name<T>();

            #if defined (__clang__)
                static constexpr std::string_view prefix = "[T = ";
                static constexpr std::string_view suffix = "]";
            #elif defined (__GNUC__)
                static constexpr std::string_view prefix = "[with T = ";
                static constexpr std::string_view suffix = "]";
            #elif defined (_MSC_VER)
                static constexpr std::string_view prefix = "function_name<";
                static constexpr std::string_view suffix = ">(void)";
            #else
                #error Unsupported compiler
            #endif

            constexpr auto start = fname.find(prefix) + prefix.size();
            constexpr auto last  = fname.rfind(suffix);

            // static_assert(start < last);

            constexpr auto name  = fname.substr(start, last - start);
            static constexpr auto name_holder = sv_as_array(name, static_cast<gold::value_seq_t<__integer_pack(name.size())...>*>(nullptr));

            return name_holder;
        }

    } // namespace __type_name

    /// type_name_as_array
    template <typename T>
    consteval auto type_name_as_array() noexcept {
        return __type_name::type_name_c_impl<T>();
    }

    /// type_name - compile type name query
    // source: source_location
    template <typename T>
    consteval auto type_name() noexcept {
        constexpr auto& name = __type_name::type_name_c_impl<T>();
        return std::string_view { name.data(), name.size() };
    }

    template <typename T>
    constexpr auto type_name(T&&) noexcept {
        constexpr auto& name = __type_name::type_name_c_impl<T&&>();
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
