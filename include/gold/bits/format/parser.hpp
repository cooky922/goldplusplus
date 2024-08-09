// <gold/bits/format/parser.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FORMAT_PARSER_HPP
#define __GOLD_BITS_FORMAT_PARSER_HPP

#include <charconv>
#include <gold/bits/format/fwd.hpp>
#include <gold/bits/format/error.hpp>
#include <gold/bits/format/format_args.hpp>
#include <gold/bits/format/format_parse_context.hpp>
#include <gold/bits/format/format_context.hpp>

namespace gold::__format {

    /// __format::try_string_from_arg
    constexpr __format::result<std::string_view> try_string_from_arg(gold::format_arg op) {
        return gold::visit_format_arg([]<typename T>(T& arg) -> __format::result<std::string_view> {
            if constexpr (__is_same(__util::empty_t, T))
                return __format::error_t(__fmt_error_code::invalid_arg_id_in_fmt_string);
            else if constexpr (__is_same(const char*, T) ||
                               __is_same(std::string_view, T))
                return arg;
            else
                return __format::error_t(__fmt_error_code::invalid_string_in_dynamic_spec);
        }, op);
    }

    /// __format::try_int_from_arg
    constexpr __format::result<std::size_t> try_int_from_arg(gold::format_arg op) {
        return gold::visit_format_arg([]<typename T>(T& arg) -> __format::result<std::size_t> {
            if constexpr (__is_same(__util::empty_t, T))
                return __format::error_t(__fmt_error_code::invalid_arg_id_in_fmt_string);
            else if constexpr (sizeof(T) <= sizeof(long long)) {
                if constexpr (std::unsigned_integral<T>)
                    return arg;
                else if constexpr (std::signed_integral<T>)
                    if (arg >= 0)
                        return arg;
            }
            return __format::error_t(__fmt_error_code::invalid_width_or_precision_in_fmt_spec);
        }, op);
    }

    namespace __specs {

        /// __format::__specs::align_kind
        enum class align_kind : gold::uint8_t {
            _default,
            _left,   // '<'
            _center, // '^'
            _right   // '>'
        };

        // [meaning] __format::__specs::align_kind::_left
        //      | + forces the field to be aligned to the start
        //      |   of the available space
        //      | + default for non-numeric, non-pointer

        // [meaning] __format::__specs::align_kind::_right
        //      | + forces the field to be aligned to the end
        //      |   of the available space
        //      | + default for numeric types

        // [meaning] __format::__specs::align_kind::_center
        //      | + forces the field to be centered within the
        //      |   available space by inserting floor(n / 2) chars
        //      |   before and ceil(n / 2) chars after the value,
        //      |   where n is the total number of fill characters
        //      |   to insert


        /// __format::__specs::sign_kind
        enum class sign_kind : gold::uint8_t {
            _default,
            _minus,   // '-'
            _plus,    // '+'
            _space    // ' '
        };

        // [meaning] __format::__specs::sign_kind
        //      | + only available for numeric types or
        //      |   when integral presentation type is specified

        /// __format::__specs::wp_kind
        enum class wp_kind : gold::uint8_t {
            _none,
            _value,
            _from_arg
        };

        // [meaning] __format::__specs::wp_kind
        //      | + width or precision kind

        // [meaning] __format::__specs::wp_kind::_none
        //      | + no width / prec specified

        // [meaning] __format::__specs::wp_kind::_value
        //      | + fixed width / prec specified

        // [meaning] __format::__specs::wp_kind::_from_arg
        //      | + use a formatting argument for width / prec

        /// __format::__specs::pres_kind
        enum class pres_kind : gold::uint8_t {
            _default,
            _string,           // 's'
            _binary,           // 'b'
            _binary_upper,     // 'B'
            _octal,            // 'o'
            _decimal,          // 'd'
            _hex,              // 'x'
            _hex_upper,        // 'X'
            _float_hex,        // 'a'
            _float_hex_upper,  // 'A'
            _pointer,          // 'p'
            _pointer_upper,    // 'P'
            _char,             // 'c'
            _scientific,       // 'e'
            _scientific_upper, // 'E'
            _fixed,            // 'f'
            _fixed_upper,      // 'F'
            _general,          // 'g'
            _general_upper,    // 'G'
            _escape            // '?'
        };

        // [meaning] __format::__specs::pres_kind
        //      | + presentation types

        /// __format::__specs::spec_base
        struct spec_base {
            align_kind     alignment  : 2 = align_kind::_default;
            wp_kind        width_kind : 2 {};
            gold::uint16_t width {};
            char fill_char = ' ';

            using iterator = std::string_view::iterator;

            static constexpr align_kind get_align(char c) noexcept {
                switch (c) {
                    case '<':
                        return align_kind::_left;
                    case '>':
                        return align_kind::_right;
                    case '^':
                        return align_kind::_center;
                    default:
                        return align_kind::_default;
                }
            }

            constexpr bool has_width() const noexcept {
                return static_cast<gold::uint8_t>(width_kind) != static_cast<gold::uint8_t>(wp_kind::_none);
            }

            constexpr __format::result<std::size_t> try_get_width(format_context&) const noexcept;
            constexpr iterator parse_fill_and_align(iterator, iterator) noexcept;
            static constexpr __format::result<iterator> try_parse_width_or_precision(iterator, iterator,
                                                                                     gold::uint16_t&,
                                                                                     bool&,
                                                                                     format_parse_context&) noexcept;
            constexpr __format::result<iterator> try_parse_width(iterator, iterator, format_parse_context&) noexcept;
        };

        /// __format::__specs::std_spec
        struct std_spec : spec_base {
            sign_kind  sign      : 2 = sign_kind::_default;
            pres_kind  pres      : 5 = pres_kind::_default;
            bool       alt       : 1 = false;
            bool       zero_fill : 1 = false;
            wp_kind    prec_kind : 2 {};
            gold::uint16_t prec {};

            using iterator = spec_base::iterator;

            static constexpr sign_kind get_sign(char c) noexcept {
                switch (c) {
                    case '+':
                        return sign_kind::_plus;
                    case '-':
                        return sign_kind::_minus;
                    case ' ':
                        return sign_kind::_space;
                    default:
                        return sign_kind::_default;
                }
            }

            constexpr bool has_precision() const noexcept {
                return static_cast<gold::uint8_t>(prec_kind) != static_cast<gold::uint8_t>(wp_kind::_none);
            }

            constexpr __format::result<std::size_t> try_get_precision(format_context&) const noexcept;
            constexpr iterator parse_sign(iterator, iterator) noexcept;
            constexpr iterator parse_alternate_form(iterator, iterator) noexcept;
            constexpr iterator parse_zero_fill(iterator, iterator) noexcept;
            constexpr __format::result<iterator> try_parse_precision(iterator, iterator, format_parse_context&) noexcept;
        };

    } // namespace __specs

    namespace __parse {

        // in parsing, it is recommended to use std::expected or std::optional
        // as error handling to avoid costy and potentially-expensive exceptions

        // [:format-string:]
        //      | sequence of [:replacement-field:], [:escape-seq:],
        //      | characters other than '{' and '}'

        // [:escape-seq:]
        //      | '{{' and '}}' which are replaced with '{' and '}'
        //        respectively in the output

        // [:replacement-field:]
        //      | '{' [:arg-id (opt):] ':' [:format-spec (opt):] '}'

        // [:arg-id:]
        //      | index of argument in Args... whose value is used
        //        for formatting
        //      | if it is ommited, the arguments are used in order
        //      | all manual, all automatic (mixing manual and automatic is error)

        // [:format-spec:]
        // - varies across different types
        //      | [:fill-and-align (opt):] [:sign (opt):]
        //        '#' (opt) '0' (opt) [:width (opt):] [:prec (opt):] [:type (opt):]

        /// __format::__parse::parse_number_result
        struct parse_number_result {
            unsigned short value;
            const char*    ptr;
        };

        /// __format::__parse::parse_integer
        constexpr parse_number_result parse_integer(const char* first,
                                                    const char* last) {
            // pre: first != last
            if (first == last)
                __builtin_unreachable();

            const auto start = first;
            unsigned short value = 0;
            // TODO: replace with homemade function
            if (std::__detail::__from_chars_alnum<true>(first, last, value, 10)
             && first != start) [[likely]]
                return {value, first};

            return {0, nullptr};
        }

        /// __format::__parse::parse_arg_id
        constexpr parse_number_result parse_arg_id(const char* first,
                                                   const char* last) {
            // pre: first != last
            if (first == last)
                __builtin_unreachable();

            if (*first == '0')
                return {0, first + 1}; // no leading zeroes allowed
                                       // so '0...' == 0

            if ('1' <= *first && *first <= '9') {
                const unsigned short id = *first - '0';
                const auto next = first + 1;
                // optimize for most likely case of single digit arg-id
                if (next == last || !('0' <= *next && *next <= '9'))
                    return {id, next};
                else
                    return __parse::parse_integer(first, last);
            }

            return {0, nullptr};
        }

        /// __format::__parse::is_digit
        constexpr bool is_digit(char c) {
            return '0' <= c && c <= '9';
        }

        /// __format::__parse::is_xdigit
        constexpr bool is_xdigit(char c) {
            return is_digit(c) || ('a' <= c && c <= 'f')
                               || ('A' <= c && c <= 'F');
        }

        /// __format::__parse::to_bool
        constexpr bool to_bool(auto op) {
            return static_cast<bool>(static_cast<gold::uint8_t>(op));
        }

    } // namespace __parse

    // pre: first != last
    constexpr auto __specs::spec_base::parse_fill_and_align(iterator first, iterator last) noexcept -> iterator {
        if (*first != '{') {
            if (last - first >= 2) {
                if (__specs::align_kind align_op = get_align(first[1]); __parse::to_bool(align_op)) {
                    fill_char = *first;
                    alignment = align_op;
                    return first + 2;
                }
            }
            if (__specs::align_kind align_op = get_align(first[0]); __parse::to_bool(align_op)) {
                fill_char = ' ';
                alignment = align_op;
                return first + 1;
            }
        }
        return first;
    }

    constexpr auto __specs::std_spec::parse_sign(iterator first, iterator) noexcept -> iterator {
        if (__specs::sign_kind sign_op = get_sign(*first); __parse::to_bool(sign_op)) {
            sign = sign_op;
            return first + 1;
        }
        return first;
    }

    constexpr auto __specs::std_spec::parse_alternate_form(iterator first, iterator) noexcept -> iterator {
        if (*first == '#') {
            alt = true;
            ++first;
        }
        return first;
    }

    constexpr auto __specs::std_spec::parse_zero_fill(iterator first, iterator) noexcept -> iterator {
        if (*first == '0') {
            zero_fill = true;
            ++first;
        }
        return first;
    }

    constexpr auto __specs::spec_base::try_parse_width_or_precision(iterator first, iterator last,
                                                                    gold::uint16_t& val,
                                                                    bool& arg_id,
                                                                    format_parse_context& pc) noexcept
                                                                    -> __format::result<iterator> {
        if (__parse::is_digit(*first)) {
            auto [v, ptr] = __parse::parse_integer(first, last);
            if (!ptr)
                return __format::error_t(__fmt_error_code::invalid_width_or_precision_in_fmt_spec);
            first = ptr;
            val = v;
        } else if (*first == '{') {
            arg_id = true;
            ++first;
            if (first == last)
                return __format::error_t(__fmt_error_code::unmatched_left_brace_in_fmt_string);
            if (*first == '}') {
                __GOLDM_FMT_TRY(pc.try_next_arg_id(), { val = try_result; });
                __GOLDM_FMT_TRYV(pc.try_check_dynamic_spec_integral(val), {});
            } else {
                auto [v, ptr] = __parse::parse_arg_id(first, last);
                if (ptr == nullptr || ptr == last || *ptr != '}')
                    return __format::error_t(__fmt_error_code::invalid_arg_id_in_fmt_string);
                first = ptr;
                __GOLDM_FMT_TRYV(pc.try_check_arg_id(v), {});
                __GOLDM_FMT_TRYV(pc.try_check_dynamic_spec_integral(v), {});
                val = v;
            }
            ++first; // past the '}'
        }
        return first;
    }

    // pre: first != last
    constexpr auto __specs::spec_base::try_parse_width(iterator first, iterator last, format_parse_context& pc) noexcept
        -> __format::result<iterator>
    {
        bool arg_id = false;
        if (*first == '0')
            return __format::error_t(__fmt_error_code::zero_width_in_fmt_string);

        iterator next;
        __GOLDM_FMT_TRY((try_parse_width_or_precision(first, last, width, arg_id, pc)), { next = try_result; });
        if (next != first)
            width_kind = arg_id ? wp_kind::_from_arg : wp_kind::_value;
        return next;
    }

    // pre: first != last
    constexpr auto __specs::std_spec::try_parse_precision(iterator first, iterator last, format_parse_context& pc) noexcept
        -> __format::result<iterator>
    {
        if (first[0] != '.')
            return first;

        iterator next = ++first;
        bool arg_id = false;

        if (next != last)
            __GOLDM_FMT_TRY((try_parse_width_or_precision(first, last, prec, arg_id, pc)), { next = try_result; });

        if (next == first)
            return __format::error_t(__fmt_error_code::missing_precision_after_dot_in_fmt_string);

        prec_kind = arg_id ? wp_kind::_from_arg : wp_kind::_value;
        return next;
    }

    constexpr __format::result<std::size_t> __specs::spec_base::try_get_width(format_context& ctx) const noexcept {
        std::size_t width_op = 0;
        if (static_cast<gold::uint8_t>(width_kind) == static_cast<gold::uint8_t>(wp_kind::_value))
            width_op = width;
        else if (static_cast<gold::uint8_t>(width_kind) == static_cast<gold::uint8_t>(wp_kind::_from_arg))
            __GOLDM_FMT_TRY(try_int_from_arg(ctx.arg(width)), { width_op = try_result; });
        return width_op;
    }

    constexpr __format::result<std::size_t> __specs::std_spec::try_get_precision(format_context& ctx) const noexcept {
        std::size_t prec_op = 0;
        if (static_cast<gold::uint8_t>(prec_kind) == static_cast<gold::uint8_t>(wp_kind::_value))
            prec_op = prec;
        else if (static_cast<gold::uint8_t>(prec_kind) == static_cast<gold::uint8_t>(wp_kind::_from_arg))
            __GOLDM_FMT_TRY(try_int_from_arg(ctx.arg(prec)), { prec_op = try_result; });
        return prec_op;
    }

} // namespace gold::__format

#endif // __GOLD_BITS_FORMAT_PARSER_HPP
