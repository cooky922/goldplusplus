// <gold/bits/typeof/compile_typeof.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_COMPILE_TYPEOF_HPP
#define __GOLD_BITS_COMPILE_TYPEOF_HPP

#include <array>
#include <source_location>
#include <string_view>
#include <bits/utility.h>

namespace gold {

    namespace __detail {

        template <std::size_t... Is>
        constexpr auto substr_as_arr_(std::string_view str_, std::index_sequence<Is...>) {
            return std::array{str_[Is]...};
        }

        /// func_name_
        template <typename T>
        consteval auto func_name_() {
            const auto& loc_ = std::source_location::current();
            return loc_.function_name();
        }

        template <typename T>
        consteval auto compile_type_of_impl_() {
            #ifdef __cpp_lib_source_location
                constexpr std::string_view function_ = func_name_<T>();
            #else
                #error Your compiler must at least support standard source location library.
            #endif

            #if defined (__clang__)
                constexpr std::string_view prefix_ = "[T = ";
                constexpr std::string_view suffix_ = "]";
            #elif defined (__GNUC__)
                constexpr std::string_view prefix_ = "[with T = ";
                constexpr std::string_view suffix_ = "]";
            #elif defined (_MSC_VER)
                constexpr std::string_view prefix_ = "func_name<";
                constexpr std::string_view suffix_ = ">(void)";
            #else
                #error Unsupported compiler
            #endif

            constexpr auto start_ = function_.find(prefix_) + prefix_.size();
            constexpr auto end_   = function_.rfind(suffix_);

            static_assert(start_ < end_);

            constexpr auto name_ = function_.substr(start_, (end_ - start_));
            return substr_as_arr_(name_, std::make_index_sequence<name_.size()>{});
        }

        template <typename T> struct type_name_holder_ {
            inline static constexpr auto value = compile_type_of_impl_<T>();
        };

        /// compile_type_of_
        template <typename T>
        constexpr auto compile_type_of_(T&& arg) {
            constexpr auto& value_ = type_name_holder_<decltype(arg)>::value;
            return std::string_view{ value_.data(), value_.size() };
        }

        template <typename T>
        constexpr auto compile_type_of_() {
            constexpr auto& value_ = type_name_holder_<T>::value;
            return std::string_view{ value_.data(), value_.size() };
        }

    } // namespace __detail

} // namespace gold

#endif // __GOLD_BITS_COMPILE_TYPEOF_HPP
