
// <gold/bits/format/error.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FORMAT_ERROR_HPP
#define __GOLD_BITS_FORMAT_ERROR_HPP

#include <bits/exception.h>
#include <gold/expected>

namespace gold {

    /// format_error
    class format_error : public std::exception {
      private:
        const char* m_str_;

      public:
        explicit format_error(const char* what) : m_str_(what) {}

        const char* what() const noexcept override { return m_str_; }
    };

    namespace __format {

        /// __format::error_code
        enum class error_code {
            none,
            mixing_manual_in_automatic_indexing,
            mixing_automatic_in_manual_indexing,
            arg_index_outside_valid_range,
            mismatched_types_from_arguments,
            unmatched_left_brace_in_fmt_string,
            unmatched_right_brace_in_fmt_string,
            invalid_arg_id_in_fmt_string,
            failed_to_parse_fmt_spec,
            invalid_width_or_precision_in_fmt_spec,
            zero_width_in_fmt_string,
            missing_precision_after_dot_in_fmt_string,
            missing_separator_field_after_tilde_in_fmt_string,
            invalid_string_in_dynamic_spec,
            invalid_options_for_bool,
            invalid_options_for_char,
            invalid_options_for_ordering,
            integer_not_representable_as_character,
            clear_brackets_and_string_type_cannot_be_used_together,
            clear_brackets_and_debug_string_type_cannot_be_used_together,
            non_pair_like_underlying_used_mapped_type,
            custom_separator_and_string_type_cannot_be_used_together,
            custom_separator_and_debug_string_type_cannot_be_used_together,
            custom_separator_and_mapped_type_cannot_be_used_together,
            non_character_underlying_used_string_type,
            non_character_underlying_used_debug_string_type,
            underlying_fmt_spec_and_string_type_cannot_be_used_together,
            underlying_fmt_spec_and_debug_string_type_cannot_be_used_together,
            unknown
        };

        /// __format::error_t
        using error_t = gold::unexpected<__format::error_code>;

        /// __format::result
        template <typename T>
        using result = gold::expected<T, __format::error_code>;

    } // namespace __format

    namespace __fmt_error_code {
        using enum __format::error_code;
    }

    namespace __fmt_error {

        /// __fmt_error::compile_time_error
        void compile_time_error ();

        /// __fmt_error::on_error
        template <std::size_t N>
        [[noreturn]] constexpr void on_error(const char (& s)[N]) {
            if consteval { compile_time_error(); }
            else { throw format_error(s); }
        }

        /// __fmt_error::mixing_manual_in_automatic_indexing
        [[noreturn]] inline void mixing_manual_in_automatic_indexing() {
            if not consteval { throw format_error("format error: mixing manual in automatic indexing"); }
        }

        /// __fmt_error::mixing_automatic_in_manual_indexing
        [[noreturn]] inline void mixing_automatic_in_manual_indexing() {
            if not consteval { throw format_error("format error: mixing automatic in manual indexing"); }
        }

        /// __fmt_error::arg_index_outside_valid_range
        [[noreturn]] inline void arg_index_outside_valid_range() {
            if not consteval { throw format_error("format error: arg index outside valid range"); }
        }

        /// __fmt_error::mismatched_types_from_arguments
        [[noreturn]] inline void mismatched_types_from_arguments() {
            if not consteval { throw format_error("format error: mismatched types from arguments"); }
        }

        /// __fmt_error::unmatched_left_brace_in_fmt_string
        [[noreturn]] inline void unmatched_left_brace_in_fmt_string() {
            if not consteval { throw format_error("format error: unmatched left brace in format string"); }
        }

        /// __fmt_error::unmatched_right_brace_in_fmt_string
        [[noreturn]] inline void unmatched_right_brace_in_fmt_string() {
            if not consteval { throw format_error("format error: unmatched right brace in format string"); }
        }

        /// __fmt_error::invalid_arg_id_in_fmt_string
        [[noreturn]] inline void invalid_arg_id_in_fmt_string() {
            if not consteval { throw format_error("format error: invalid arg id in format string"); }
        }

        /// __fmt_error::failed_to_parse_fmt_spec
        [[noreturn]] inline void failed_to_parse_fmt_spec() {
            if not consteval { throw format_error("format error: failed to parse format spec"); }
        }

        /// __fmt_error::invalid_width_or_precision_in_fmt_spec
        [[noreturn]] inline void invalid_width_or_precision_in_fmt_spec() {
            if not consteval { throw format_error("format error: invalid width or precision in format spec"); }
        }

        /// __fmt_error::zero_width_in_fmt_string
        [[noreturn]] inline void zero_width_in_fmt_string() {
            if not consteval { throw format_error("format error: zero width in format string"); }
        }

        /// __fmt_error::missing_precision_after_dot_in_fmt_string
        [[noreturn]] inline void missing_precision_after_dot_in_fmt_string() {
            if not consteval { throw format_error("format error: missing precision after '.' in format string"); }
        }

        /// __fmt_error::on_error_with
        [[noreturn]] constexpr void on_error_with(__format::error_code code) {
            switch (code) {
              case __fmt_error_code::mixing_manual_in_automatic_indexing:
                mixing_manual_in_automatic_indexing();

              case __fmt_error_code::mixing_automatic_in_manual_indexing:
                mixing_automatic_in_manual_indexing();

              case __fmt_error_code::arg_index_outside_valid_range:
                arg_index_outside_valid_range();

              case __fmt_error_code::mismatched_types_from_arguments:
                mismatched_types_from_arguments();

              case __fmt_error_code::unmatched_left_brace_in_fmt_string:
                unmatched_left_brace_in_fmt_string();

              case __fmt_error_code::unmatched_right_brace_in_fmt_string:
                unmatched_right_brace_in_fmt_string();

              case __fmt_error_code::invalid_arg_id_in_fmt_string:
                invalid_arg_id_in_fmt_string();

              case __fmt_error_code::failed_to_parse_fmt_spec:
                failed_to_parse_fmt_spec();

              case __fmt_error_code::invalid_width_or_precision_in_fmt_spec:
                invalid_width_or_precision_in_fmt_spec();

              case __fmt_error_code::zero_width_in_fmt_string:
                zero_width_in_fmt_string();

              case __fmt_error_code::missing_precision_after_dot_in_fmt_string:
                missing_precision_after_dot_in_fmt_string();

              case __fmt_error_code::missing_separator_field_after_tilde_in_fmt_string:
                on_error("format error: missing '{' and '}' after '~' for specifying separator");

              case __fmt_error_code::invalid_string_in_dynamic_spec:
                on_error("format error: invalid string in dynamic spec");

              case __fmt_error_code::invalid_options_for_bool:
                on_error("format error: format string contains invalid options for 'bool'");

              case __fmt_error_code::invalid_options_for_char:
                on_error("format error: format string contains invalid options for 'char'");

              case __fmt_error_code::invalid_options_for_ordering:
                on_error("format error: format string contains invalid options for ordering type");

              case __fmt_error_code::integer_not_representable_as_character:
                on_error("format error: the provided integer is not representable as character");

              case __fmt_error_code::clear_brackets_and_string_type_cannot_be_used_together:
                on_error("format error: 'n' option and type 's' cannot be used together");

              case __fmt_error_code::clear_brackets_and_debug_string_type_cannot_be_used_together:
                on_error("format error: 'n' option and type '?s' cannot be used together");

              case __fmt_error_code::non_pair_like_underlying_used_mapped_type:
                on_error("format error: type 'm' requires a pair-like object");

              case __fmt_error_code::custom_separator_and_string_type_cannot_be_used_together:
                on_error("format error: custom separator and type 's' cannot be used together");

              case __fmt_error_code::custom_separator_and_debug_string_type_cannot_be_used_together:
                on_error("format error: custom separator and type '?s' cannot be used together");

              case __fmt_error_code::custom_separator_and_mapped_type_cannot_be_used_together:
                on_error("format error: custom separator and type 'm' cannot be used together");

              case __fmt_error_code::non_character_underlying_used_string_type:
                on_error("format error: type 's' requires character type");

              case __fmt_error_code::non_character_underlying_used_debug_string_type:
                on_error("format error: type '?s' requires character type");

              case __fmt_error_code::underlying_fmt_spec_and_string_type_cannot_be_used_together:
                on_error("format error: type 's' and underlying format-spec cannot be used together");

              case __fmt_error_code::underlying_fmt_spec_and_debug_string_type_cannot_be_used_together:
                on_error("format error: type '?s' and underlying format-spec cannot be used together");

              case __fmt_error_code::unknown:
                on_error("format error");

              default:
                __builtin_unreachable();
            }
        }

    } // namespace __fmt_error

    /// try_format_result
    template <typename T>
    using try_format_result = __format::result<T>;

    /// format_error_code
    using format_error_code = __format::error_code;

} // namespace gold

#endif // __GOLD_BITS_FORMAT_ERROR_HPP
