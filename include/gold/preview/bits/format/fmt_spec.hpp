// <gold/preview/bits/format/fmt_spec.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: this is only a prototype
// note: this is an internal header

#pragma once
#ifndef __GOLD_PREVIEW_BITS_FMT_SPEC_HPP
#define __GOLD_PREVIEW_BITS_FMT_SPEC_HPP

#include <gold/preview/bits/format/fmt_error.hpp>
#include <gold/preview/bits/format/fmt_arg.hpp>
#include <gold/preview/bits/format/fmt_string.hpp>
#include <concepts>

namespace gold::preview {

    namespace __format::__detail {

        /// flags_
        struct flags_ {

            /// alignment_
            enum class alignment_ : std::uint8_t {
                default_, left_, center_, right_
            };

            /// sign_
            enum class sign_ : std::uint8_t {
                default_, minus_, plus_, space_
            };

            /// m_alignment_
            alignment_ m_alignment_ { alignment_::default_ };

            /// m_sign_
            sign_ m_sign_ { sign_::default_ };

            /// m_alternate_form_
            bool m_alternate_form_ : 1 { false };

            /// m_zero_padding_
            bool m_zero_padding_ : 1 { false };

            /// m_locale_specific_form_
            bool m_locale_specific_form_ : 1 { false };

            /// type_
            enum class type_ : std::uint8_t {
                default_,
                string_,
                binary_lower_case_,
                binary_upper_case_,
                octal_,
                decimal_,
                hexadecimal_lower_case_,
                hexadecimal_upper_case_,
                pointer_,
                char_,
                float_hexadecimal_lower_case_,
                float_hexadecimal_upper_case_,
                scientific_lower_case_,
                scientific_upper_case_,
                fixed_lower_case_,
                fixed_upper_case_,
                general_lower_case_,
                general_upper_case_
            };

            /// m_type_
            type_ m_type_ { type_::default_ };
        };

        /// parse_alignment_
        template <typename CharT>
        constexpr bool parse_alignment_(CharT c, flags_& flags) noexcept {
            switch (c) {
              case CharT('<'):
                flags.m_alignment_ = flags_::alignment_::left_;
                return true;

              case CharT('^'):
                flags.m_alignment_ = flags_::alignment_::center_;
                return true;

              case CharT('>'):
                flags.m_alignment_ = flags_::alignment_::right_;
                return true;
            }
            return false;
        }

        /// parser_fill_align_ [ incomplete ]
        template <typename T>
        class parser_fill_align_;

        /// parse_sign_
        template <typename CharT>
        constexpr const CharT* parse_sign_(const CharT* begin_, flags_& flags) noexcept {
            switch (*begin_) {
              case CharT('-'):
                flags.m_sign_ = flags_::sign_::minus_;
                break;
              case CharT('+'):
                flags.m_sign_ = flags_::sign_::plus_;
                break;
              case CharT(' '):
                flags.m_sign_ = flags_::sign_::space_;
                break;
              default:
                return begin_;
            }
            return begin_ + 1;
        }

        /// parse_alternate_form_
        template <typename CharT>
        constexpr const CharT* parse_alternate_form_(const CharT* begin_, flags_& flags) noexcept {
            if (*begin_ == CharT('#')) {
                flags.m_alternate_form_ = true;
                ++begin_;
            }
            return begin_;
        }

        /// parse_zero_padding_
        template <typename CharT>
        constexpr const CharT* parse_zero_padding_(const CharT* begin_, flags_& flags) noexcept {
            if (*begin_ == CharT('0')) {
                flags.m_zero_padding_ = true;
                ++begin_;
            }
            return begin_;
        }

        /// parse_arg_id_

        /// substitude_arg_id_

        /// parser_width_
        class parser_width_;

        /// parser_precision
        class parser_precision_;

        /// parse_locale_specific_form_
        template <typename CharT>
        constexpr const CharT* parse_locale_specific_form_(const CharT* begin_, flags_& flags) noexcept {
            if (*begin_ == CharT('L')) {
                flags.m_locale_specific_form_ = true;
                ++begin_;
            }
            return begin_;
        }

        /// parse_type_
        template <typename CharT>
        constexpr const CharT* parse_type_(const CharT* begin_, flags_& flags) noexcept {
            switch (*begin_) {
              case 'A':
                flags.m_type_ = flags_::type_::float_hexadecimal_upper_case_;
                break;
              case 'B':
                flags.m_type_ = flags_::type_::binary_upper_case_;
                break;
              case 'E':
                flags.m_type_ = flags_::type_::scientific_upper_case_;
                break;
              case 'F':
                flags.m_type_ = flags_::type_::fixed_upper_case_;
                break;
              case 'G':
                flags.m_type_ = flags_::type_::general_upper_case_;
                break;
              case 'X':
                flags.m_type_ = flags_::type_::hexadecimal_upper_case_;
                break;
              case 'a':
                flags.m_type_ = flags_::type_::float_hexadecimal_lower_case_;
                break;
              case 'b':
                flags.m_type_ = flags_::type_::binary_lower_case_;
                break;
              case 'c':
                flags.m_type_ = flags_::type_::char_;
                break;
              case 'd':
                flags.m_type_ = flags_::type_::decimal_;
                break;
              case 'e':
                flags.m_type_ = flags_::type_::scientific_lower_case_;
                break;
              case 'f':
                flags.m_type_ = flags_::type_::fixed_lower_case_;
                break;
              case 'g':
                flags.m_type_ = flags_::type_::general_lower_case_;
                break;
              case 'o':
                flags.m_type_ = flags_::type_::octal_;
                break;
              case 'p':
                flags.m_type_ = flags_::type_::pointer_;
                break;
              case 's':
                flags.m_type_ = flags_::type_::string_;
                break;
              case 'x':
                flags.m_type_ = flags_::type_::hexadecimal_lower_case_;
                break;
              default:
                return begin_;
            }
            return ++begin_;
        }

        /// parser_string_
        template <typename CharT>
        class parser_string_;

        /// parser_integral_
        template <typename CharT>
        class parser_integral_;

    } // namespace __format::__detail

} // namespace gold::preview


#endif // __GOLD_PREVIEW_BITS_FMT_SPEC_HPP
