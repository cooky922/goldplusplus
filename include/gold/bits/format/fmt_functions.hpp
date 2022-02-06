// <gold/bits/format/fmt_functions.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FORMAT_FUNCTIONS_HPP
#define __GOLD_BITS_FORMAT_FUNCTIONS_HPP

#include <gold/bits/format/fmt_error.hpp>
#include <gold/bits/format/fmt_parse.hpp>
#include <gold/bits/format/fmt_args.hpp>
#include <gold/bits/raw_buffer.hpp>
#include <gold/iterator>

#include <utility>
#include <bits/stl_algo.h>

namespace gold {

    namespace __format {

        /// parse_fmt_specs_
        template <typename Iter, std::sentinel_for<Iter> Sent, typename T>
        constexpr parse_fmt_specs_result_<Iter> parse_fmt_specs_(Iter iter, Sent sent, gold::formatter<T>& fmt, format_specs& specs) {

            parse_fmt_specs_result_<Iter> result;
            parse_result_<Iter, int> func_result;

            /// parse the fill character
            iter = parse_fill_char_(iter, sent, specs);

            /// parse the alignment
            iter = parse_align_(iter, sent, specs);

            /// parse the sign
            iter = parse_sign_(iter, sent, specs);

            /// parse the '#' for alternate form
            iter = parse_alternate_form_(iter, sent, specs);

            /// parse width
            func_result = parse_width_(iter, sent, specs);
            result.width_arg_id = func_result.value;
            iter = func_result.iter;

            /// parse precision - currently has no dynamic replacement
            func_result = parse_precision_(iter, sent, specs);
            result.prec_arg_id = func_result.value;
            iter = func_result.iter;

            /// parse type flags [ format specifier ]
            if constexpr (gold::custom_formattable<T>)
                iter = fmt.parse_to(iter, sent, specs);
            else
                iter = parse_type_(iter, sent, specs);

            result.iter = iter;

            return result;
        }

        /// format_each_to_
        // write the aligned version (padded) of each format
        template <gold::__format::char_output_iterator Out, typename T>
            requires formattable_to<__format::decay_t_<T>, Out>
        constexpr Out format_each_to_(Out out, T&& item, gold::formatter<__format::decay_t_<T>>& formatter_, format_specs specs = {}) {

            // if width is -1
            if (specs.width == -1)
                return formatter_.format_to(out, std::forward<T>(item), std::move(specs));

            // fmt width
            const int fmt_width_ = [&]{
                gold::counting_iterator<char> iter_;
                iter_ = formatter_.format_to(iter_, std::forward<T>(item), std::move(specs));
                return iter_.count();
            }();

            // setting up the functions
            auto on_left_aligned_ = [&]{
                out = formatter_.format_to(out, std::forward<T>(item), std::move(specs));
                if (std::cmp_less(fmt_width_, specs.width)) {
                    int right_width_ = specs.width - fmt_width_;
                    while (right_width_-- > 0)
                        *out++ = specs.fill_character;
                }
                return out;
            };

            auto on_center_aligned_ = [&]{
                int fmt_left_width_  = (specs.width - fmt_width_) / 2;
                int fmt_right_width_ = specs.width - fmt_width_ - fmt_left_width_;

                if (std::cmp_less(fmt_width_, specs.width)) {
                    while (fmt_left_width_-- > 0)
                        *out++ = specs.fill_character;
                    out = formatter_.format_to(out, std::forward<T>(item), std::move(specs));
                    while (fmt_right_width_-- > 0)
                        *out++ = specs.fill_character;
                    return out;
                } else {
                    return formatter_.format_to(out, std::forward<T>(item), std::move(specs));
                }
            };

            auto on_right_aligned_ = [&]{
                if (std::cmp_less(fmt_width_, specs.width)) {
                    std::size_t offset_ = specs.width - fmt_width_;
                    while (offset_-- > 0)
                        *out++ = specs.fill_character;
                }
                return formatter_.format_to(out, std::forward<T>(item), std::move(specs));
            };

            // choosing the alignment
            switch (specs.alignment) {
              case format_specs::alignment_t::left:
                return on_left_aligned_();
              case format_specs::alignment_t::center:
                return on_center_aligned_();
              case format_specs::alignment_t::right:
                return on_right_aligned_();
              case format_specs::alignment_t::default_opt:
              default:
                if constexpr (gold::qualified_numeric<std::remove_cvref_t<T>>)
                    return on_right_aligned_();
                else
                    return on_left_aligned_();
            }

            return out;
        }

    } // namespace __format

    /// vformat_to
    template <gold::__format::char_output_iterator Out, typename... Args>
        requires (formattable_to<Args, Out> && ...)
    constexpr Out vformat_to(Out out, std::string_view fmt, format_args<Args...> fmt_args_) {

        auto iter_fmt_ = fmt.begin();
        bool replacement_field_state_ = false;
        format_specs specs_;
        specs_.fill_character = ' ';

        int arg_id_ = 0, width_arg_id_ = 0, prec_arg_id_ = 0;

        // loop begin
        while (iter_fmt_ != fmt.end()) {

            /// find '$'
            if (*iter_fmt_ == '$') {
                ++iter_fmt_;

                switch (*iter_fmt_) {

                  // escape sequence
                  case '$':
                    break;

                  // replacement field state is set to true
                  case '{':
                    replacement_field_state_ = true;
                    break;

                  default:
                    if (iter_fmt_ == fmt.end())
                        throw gold::format_error("Special character '$' ends early");
                }
            }

            /// replacement field state - 1 loop per state only
            if (replacement_field_state_) {

                /// check if the tuple is empty
                if constexpr (sizeof...(Args) == 0)
                    throw gold::format_error("Format string requests arguments but it is empty");

                /// parse_arg_id
                auto [next_iter_, arg_id_result_] = __format::parse_arg_id_(iter_fmt_, fmt.end());

                if (arg_id_result_ != -1) {
                    arg_id_ = arg_id_result_;

                    if (std::cmp_greater_equal(arg_id_, sizeof...(Args)))
                        throw gold::format_error("Argument ID requests number that is not less than the format argument size");
                }

                iter_fmt_ = next_iter_;

                /// ':' and '}' should be handled
                if (*iter_fmt_ == ':') {

                    ++iter_fmt_;

                    /// parse default specs
                    __format::parse_fmt_specs_result_<decltype(iter_fmt_)> parse_result_;

                    fmt_args_.visit_formatter_at(arg_id_, [&]<typename T>(T& arg) {
                        parse_result_ = __format::parse_fmt_specs_(iter_fmt_, fmt.end(), arg, specs_);
                    });

                    iter_fmt_ = parse_result_.iter;

                    if (parse_result_.width_arg_id != -1) {
                        width_arg_id_ = parse_result_.width_arg_id;
                    } else {
                        ++width_arg_id_;
                    }

                    if (parse_result_.prec_arg_id != -1) {
                        prec_arg_id_ = parse_result_.prec_arg_id;
                    } else {
                        ++prec_arg_id_;
                    }
                }

                if (specs_.width == -2) {
                    fmt_args_.visit_arg_at(width_arg_id_, [&]<typename T>(const T& arg) {
                        if constexpr (__format::__detail::is_ints_only_v_<T>) {
                            if (arg >= 0)
                                specs_.width = static_cast<int>(arg);
                            else
                                throw gold::format_error("Dynamic width arg-id should be non-negative");
                        } else {
                            throw gold::format_error("Dynamic width arg-id should be integral");
                        }
                    });
                }

                if (specs_.precision == -2) {
                    fmt_args_.visit_arg_at(prec_arg_id_, [&]<typename T>(const T& arg) {
                        if constexpr (__format::__detail::is_ints_only_v_<T>) {
                            if (arg >= 0)
                                specs_.precision = static_cast<int>(arg);
                            else
                                throw gold::format_error("Dynamic precision arg-id should be non-negative");
                        } else {
                            throw gold::format_error("Dynamic precision arg-id should be integral");
                        }
                    });
                }

                fmt_args_.visit_at(arg_id_, [&]<typename T>(T& arg) {
                    out = __format::format_each_to_(out, arg.base, arg.formatter, specs_);
                });

                if (arg_id_result_ == -1)
                    ++arg_id_;

                if (*iter_fmt_ != '}')
                    throw gold::format_error("Unmatched '}' after specification");

                replacement_field_state_ = false;
                specs_.reset();

            } else {
                *out++ = *iter_fmt_;
            }

            ++iter_fmt_;
        }

        return out;
    }

    /// format_to
    template <gold::__format::char_output_iterator Out, typename... Args>
        requires (formattable_to<__format::decay_t_<Args>, Out> && ...)
    constexpr Out format_to(Out out, std::string_view fmt, Args&&... args) {
        return vformat_to<Out, __format::decay_t_<Args>...>(out, fmt, make_format_args(std::forward<Args>(args)...));
    }

    /// vformat
    template <typename... Args>
        requires (formattable_to<Args, std::back_insert_iterator<std::string>> && ...)
    constexpr std::string vformat(std::string_view fmt, format_args<Args...> fmt_args) {
        std::string result;
        vformat_to(std::back_inserter(result), fmt, fmt_args);
        return result;
    }

    /// format
    template <typename... Args>
        requires (formattable_to<__format::decay_t_<Args>, std::back_insert_iterator<std::string>> && ...)
    constexpr std::string format(std::string_view fmt, Args&&... args) {
        return vformat<__format::decay_t_<Args>...>(fmt, make_format_args(std::forward<Args>(args)...));
    }

    /// formatted_size
    template <typename... Args>
        requires (formattable_to<__format::decay_t_<Args>, gold::counting_iterator<char>> && ...)
    constexpr std::size_t formatted_size(std::string_view fmt, Args&&... args) {
        gold::counting_iterator<char> iter_;
        iter_ = vformat_to<
            gold::counting_iterator<char>,
            __format::decay_t_<Args>...
        >(iter_, fmt, make_format_args(std::forward<Args>(args)...));
        return iter_.count();
    }

    /// format_to_c
    template <typename Container, typename... Args>
        requires (formattable_to<__format::decay_t_<Args>, std::back_insert_iterator<std::string>> && ...)
        && requires (Container cont, std::size_t n) {
            cont.reserve(n);
            std::back_inserter(cont);
        }
    constexpr Container format_to_c(std::string_view fmt, Args&&... args) {
        Container result;
        result.reserve(formatted_size(fmt, std::forward<Args>(args)...));
        format_to(std::back_inserter(result), fmt, std::forward<Args>(args)...);
        return result;
    }

    /// format_to_n_result
    template <typename Out>
    struct format_to_n_result {
        Out out;
        std::iter_difference_t<Out> size;
    };

    namespace __format {



    } // namepace __format

    /// format_to_n
    template <typename Out, typename... Args>
        requires (formattable_to<__format::decay_t_<Args>, Out> && ...)
    constexpr format_to_n_result<Out> format_to_n(Out out, std::iter_difference_t<Out> n, std::string_view fmt, Args&&... args) {
        gold::__util::raw_buffer<char> buffer_ (128);
        buffer_.setfill('\0');
        auto buf_ = vformat_to(gold::coop_counting_iterator(buffer_.data()), fmt, make_format_args(std::forward<Args>(args)...));
        std::iter_difference_t<Out> s_ = buf_.count();
        std::iter_difference_t<Out> m_ = std::clamp(n, std::iter_difference_t<Out>(0), s_);
        out = std::ranges::copy_n(buffer_.begin(), m_, out).out;
        return {.out = out, .size = s_};
    }

} // namespace gold

#endif // __GOLD_BITS_FORMAT_FUNCTIONS_HPP
