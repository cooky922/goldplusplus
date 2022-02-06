// <gold/bits/format/fmt_parse.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FORMAT_PARSE_HPP
#define __GOLD_BITS_FORMAT_PARSE_HPP

#include <gold/bits/format/fmt_error.hpp>
#include <gold/bits/format/fmt_specs.hpp>
#include <bits/iterator_concepts.h>

namespace gold {

    namespace __format {

        /// parse_result_
        template <typename Iter, typename Arg>
        struct parse_result_ {
            Iter iter;
            Arg  value;
        };

        /// parse_fmt_specs_result_
        template <typename Iter>
        struct parse_fmt_specs_result_ {
            Iter iter;
            int width_arg_id, prec_arg_id;
        };

        namespace __detail {

            /// parse_nonnegative_int_
            template <typename Iter>
            constexpr parse_result_<Iter, int> parse_nonnegative_int_(Iter iter) {
                int result_ = 0;
                while (*iter >= '0' && *iter <= '9') {
                    result_ *= 10;
                    result_ += *iter - '0';
                    ++iter;
                }
                return {.iter = iter, .value = result_};
            }

        } // namespace __detail

        /// parse_arg_id_
        //// scans a non-negative number until it scans ':' character
        //// negative result means - unused, or automatic indexing
        template <typename Iter, std::sentinel_for<Iter> Sent>
        constexpr parse_result_<Iter, int> parse_arg_id_(Iter iter, Sent sent) {

            // ${}   - automatic
            // ${:}  - automatic
            // ${2}  - manual
            // ${2:} - manual

            // note: the iterator is first passed on '{'
            if (*iter != '{')
                throw gold::format_error("the arg-id should scan first on '{'");

            // move to the next, whether it lands on numbers, ':', or '}'
            ++iter;

            if (*iter == '}' || *iter == ':') {
                return { .iter = iter, .value = -1 };
            }

            auto [next_iter_, new_value_] = __detail::parse_nonnegative_int_(iter);

            iter = next_iter_;

            // the next character should be either '}' or ':'
            if (!(*iter == '}' || *iter == ':'))
                throw gold::format_error("the arg-id should scan last on '}' or ':'");

            return { .iter = iter, .value = new_value_ };
        }

        /// parse_fill_char_
        template <typename Iter, std::sentinel_for<Iter> Sent>
        constexpr Iter parse_fill_char_(Iter iter, Sent sent, format_specs& specs) {

            if (*iter == '$') {
                ++iter;
                switch (*iter) {
                  case '$':
                    if (*(iter + 1) == '<' || *(iter + 1) == '>' || *(iter + 1) == '^')
                        specs.fill_character = '$';
                    else
                        throw gold::format_error("explicitly specified fill character "
                                                 "must be followed by alignment arrows");
                    break;

                  // for dynamic width, do backward instead
                  case '{':
                    return --iter;

                  default:
                    throw gold::format_error("'$' as fill character must be doubled");
                }

                return ++iter;
            }

            if (*(iter + 1) == '<' || *(iter + 1) == '>' || *(iter + 1) == '^') {
                switch (*iter) {
                  case '{':
                    throw gold::format_error("'{' must not be used as fill character");
                    break;

                  case '}':
                    throw gold::format_error("'}' must not be used as fill character");
                    break;

                  default:
                    specs.fill_character = *iter;
                }

                return ++iter;

            } else
                return iter;
        }

        /// parse_align_
        template <typename Iter, std::sentinel_for<Iter> Sent>
        constexpr Iter parse_align_(Iter iter, Sent sent, format_specs& specs) {
            switch (*iter) {
              case '<':
                specs.alignment = format_specs::alignment_t::left;
                break;
              case '^':
                specs.alignment = format_specs::alignment_t::center;
                break;
              case '>':
                specs.alignment = format_specs::alignment_t::right;
                break;
              default:
                return iter;
            }
            return ++iter;
        }

        /// parse_alternate_form_
        template <typename Iter, std::sentinel_for<Iter> Sent>
        constexpr Iter parse_alternate_form_(Iter iter, Sent sent, format_specs& specs) {
            if (*iter == '#') {
                specs.show_base = true;
                return ++iter;
            }
            return iter;
        }

        /// parse_zero_
        template <typename Iter, std::sentinel_for<Iter> Sent>
        constexpr Iter parse_zero_(Iter iter, Sent sent, format_specs& specs) {
            if (*iter == '0') {
                specs.padding_zero = true;
                return ++iter;
            }
            return iter;
        }

        /// parse_width_
        template <typename Iter, std::sentinel_for<Iter> Sent>
        constexpr parse_result_<Iter, int> parse_width_(Iter iter, Sent sent, format_specs& specs) {
            /// either '$' or digit
            if (*iter == '$' && *(iter + 1) == '{') {
                iter += 2;
                [[maybe_unused]] auto [next_iter_, arg_id_] = __detail::parse_nonnegative_int_(iter);
                iter = next_iter_;
                specs.width = -2;
                if (*iter != '}')
                    throw gold::format_error("'}' is missing to terminate dynamic width spec parsing");
                return { ++iter, arg_id_ };

            } else {
                auto [next_iter_, new_value_] = __detail::parse_nonnegative_int_(iter);
                iter = next_iter_;
                specs.width = new_value_;
                return { iter, -1 };
            }
        }

        /// parse_precision_
        template <typename Iter, std::sentinel_for<Iter> Sent>
        constexpr parse_result_<Iter, int> parse_precision_(Iter iter, Sent sent, format_specs& specs) {
            if (*iter == '.') {
                ++iter;
                /// either '$' or digit
                if (*iter == '$' && *(iter + 1) == '{') {
                    iter += 2;
                    [[maybe_unused]] auto [next_iter_, arg_id_] = __detail::parse_nonnegative_int_(iter);
                    iter = next_iter_;
                    specs.precision = -2;
                    if (*iter != '}')
                        throw gold::format_error("'}' is missing to terminate dynamic width spec parsing");
                    return { ++iter, arg_id_ };

                } else {
                    auto [next_iter_, new_value_] = __detail::parse_nonnegative_int_(iter);
                    iter = next_iter_;
                    specs.precision = new_value_;
                }
            }

            return { iter, -1 };
        }

        /// parse_sign_
        template <typename Iter, std::sentinel_for<Iter> Sent>
        constexpr Iter parse_sign_(Iter iter, Sent sent, format_specs& specs) {
            switch (*iter) {
              case '-':
                specs.sign = format_specs::sign_t::minus;
                break;
              case '+':
                specs.sign = format_specs::sign_t::plus;
                break;
              case ' ':
                specs.sign = format_specs::sign_t::space;
                break;
              default:;
                return iter;
            }
            return ++iter;
        }

        /// parse_type_
        template <typename Iter, std::sentinel_for<Iter> Sent>
        constexpr Iter parse_type_(Iter iter, Sent sent, format_specs& specs) {

            switch (*iter) {
              case 'a':
                specs.type = format_specs::type_t::float_hex_lower;
                break;
              case 'A':
                specs.type = format_specs::type_t::float_hex_upper;
                break;
              case 'b':
                specs.type = format_specs::type_t::binary_lower;
                break;
              case 'B':
                specs.type = format_specs::type_t::binary_upper;
                break;
              case 'c':
                specs.type = format_specs::type_t::character;
                break;
              case 'C':
                specs.type = format_specs::type_t::character_quoted;
                break;
              case 'd':
                specs.type = format_specs::type_t::decimal;
                break;
              case 'e':
                specs.type = format_specs::type_t::scientific_lower;
                break;
              case 'E':
                specs.type = format_specs::type_t::scientific_upper;
                break;
              case 'f':
                specs.type = format_specs::type_t::fixed_lower;
                break;
              case 'F':
                specs.type = format_specs::type_t::fixed_upper;
                break;
              case 'g':
                specs.type = format_specs::type_t::general_lower;
                break;
              case 'G':
                specs.type = format_specs::type_t::general_upper;
                break;
              case 'o':
                specs.type = format_specs::type_t::octal;
                break;
              case 'p':
                specs.type = format_specs::type_t::pointer;
                break;
              case 's':
                specs.type = format_specs::type_t::string;
                break;
              case 'S':
                specs.type = format_specs::type_t::string_quoted;
                break;
              case 'x':
                specs.type = format_specs::type_t::hex_lower;
                break;
              case 'X':
                specs.type = format_specs::type_t::hex_upper;
                break;
              default:
                return iter;
            }
            return ++iter;
        }
    } // namespace __format

} // namespace gold

#endif // __GOLD_BITS_FORMAT_PARSE_HPP
