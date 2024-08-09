// <gold/bits/format/concepts.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FORMAT_CONCEPTS_HPP
#define __GOLD_BITS_FORMAT_CONCEPTS_HPP

#include <bits/iterator_concepts.h>
#include <gold/bits/concepts/types.hpp>
#include <gold/bits/format/fwd.hpp>
#include <gold/bits/format/error.hpp>
#include <gold/bits/format/format_arg_kind.hpp>
#include <gold/bits/format/format_parse_context.hpp>
#include <gold/bits/format/format_context.hpp>

namespace gold {

    namespace __format {

        /// __format::int_only
        template <typename T>
        concept int_only = std::is_integral_v<T> && !is_char_v<T> && !std::is_same_v<T, bool>;

        /// __format::has_basic_parser
        template <typename T, typename Formatter = gold::formatter<__format::normalize_type<T>>>
        concept has_basic_parser =
            requires (Formatter f, gold::format_parse_context pc) {
                { f.try_parse(pc) } noexcept -> std::same_as<__format::result<typename gold::format_parse_context::iterator>>;
            };

        /// __format::has_parser
        template <typename T, typename Formatter = gold::formatter<__format::normalize_type<T>>>
        concept has_parser =
            has_basic_parser<T> &&
            requires (Formatter f, gold::format_parse_context pc) {
                { f.parse(pc) } -> std::same_as<typename gold::format_parse_context::iterator>;
            };

        /// __format::has_basic_format
        template <typename T, typename Formatter = gold::formatter<__format::normalize_type<T>>>
        concept has_basic_format =
            requires (const Formatter cf, T&& op, gold::format_context fc) {
                { cf.try_format(op, fc) } -> std::same_as<__format::result<typename gold::format_context::iterator>>;
            };

        /// __format::has_format
        template <typename T, typename Formatter = gold::formatter<__format::normalize_type<T>>>
        concept has_format =
            requires (const Formatter cf, T&& op, gold::format_context fc) {
                { cf.format(op, fc) } -> std::same_as<typename gold::format_context::iterator>;
            };

        /// __format::basic_formattable_impl
        template <typename T, typename Formatter = gold::formatter<__format::normalize_type<T>>>
        concept basic_formattable_impl =
            std::semiregular<Formatter> &&
            __format::has_basic_parser<T> &&
            __format::has_basic_format<T>;

        /// __format::advanced_formattable_impl
        template <typename T, typename Formatter = gold::formatter<__format::normalize_type<T>>>
        concept advanced_formattable_impl =
            __format::basic_formattable_impl<T> &&
            __format::has_parser<T> &&
            __format::has_format<T>;

        /// __format::char_output_iterator
        template <typename Out>
        concept char_output_iterator = std::output_iterator<Out, const char&>;

    } // namespace __format

    /// formattable
    template <typename T>
    concept formattable = __format::basic_formattable_impl<std::remove_reference_t<T>>;

} // namespace gold

#endif // __GOLD_BITS_FORMAT_CONCEPTS_HPP
