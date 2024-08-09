// <gold/bits/format/scanner.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FORMAT_SCANNER_HPP
#define __GOLD_BITS_FORMAT_SCANNER_HPP

#include <gold/bits/algo/with_index.hpp>
#include <gold/bits/format/fwd.hpp>
#include <gold/bits/format/error.hpp>
#include <gold/bits/format/format_parse_context.hpp>
#include <gold/bits/format/format_context.hpp>
#include <gold/bits/format/format_args.hpp>
#include <gold/bits/format/format_arg_handle.hpp>

namespace gold::__format {

    /// __format::scanner
    // Abstract base class defining an interface for scanning format strings.
    // Scan the characters in a format string, dividing it up into strings of
    // ordinary characters, escape sequences, and replacement fields.
    // Call virtual functions for derived classes to parse format-specifiers
    // or write formatted output.
    struct scanner {
        using iterator = gold::format_parse_context::iterator;

        gold::format_parse_context m_pc_;

        constexpr explicit scanner(std::string_view sv, std::size_t n = - 1)
        : m_pc_(sv, n) {}

        constexpr explicit scanner(std::string_view sv, __format::arg_kinds<> kinds)
        : m_pc_(sv, kinds) {}

        constexpr iterator begin() const noexcept { return m_pc_.begin(); }
        constexpr iterator end() const noexcept { return m_pc_.end(); }

        constexpr std::string_view fmt_string() const noexcept { return {begin(), end()}; }

        constexpr __format::result<void> try_scan() {
            std::string_view fmt = fmt_string();

            // special case when fmt is "{}"
            if (fmt.size() == 2 && fmt[0] == '{' && fmt[1] == '}') {
                m_pc_.advance_next();
                std::size_t arg_id = 0;
                __GOLDM_FMT_TRY(m_pc_.try_next_arg_id(), { arg_id = try_result; });
                __GOLDM_FMT_TRYV(this->format_arg(arg_id), {});
                return {};
            }

            std::size_t lbr = fmt.find('{');
            std::size_t rbr = fmt.find('}');

            while (fmt.size()) {
                auto cmp = lbr <=> rbr;

                // has not found replacement field or escape characters
                // lbr and rbr are both npos
                if (cmp == 0) {
                    on_chars(end());
                    m_pc_.advance_to_end();
                    return {};
                }
                // '{' comes before '}'
                else if (cmp < 0) {
                    // [error] if '{' has no '}'
                    // since rbr is npos or
                    // if it lacks another '{' to be considered as
                    // an escape character
                    if (lbr + 1 == fmt.size() ||
                       (rbr == fmt.npos && fmt[lbr + 1] != '{'))
                        return __format::error_t(__fmt_error_code::unmatched_left_brace_in_fmt_string);

                    // in these cases below, '}' is found in fmt string

                    // check if '{{' is found
                    const bool is_escape = fmt[lbr + 1] == '{';
                    iterator last = begin() + lbr + int(is_escape);
                    on_chars(last);
                    m_pc_.advance_to(last + 1);
                    fmt = fmt_string();
                    if (is_escape) {
                        if (rbr != fmt.npos)
                            rbr -= lbr + 2;
                        lbr = fmt.find('{');
                    } else {
                        __GOLDM_FMT_TRYV(on_replacement_field(), {});
                        fmt = fmt_string();
                        lbr = fmt.find('{');
                        rbr = fmt.find('}');
                    }
                }
                // '}' comes before '{'
                else {
                    if (++rbr == fmt.size() || fmt[rbr] != '}')
                        return __format::error_t(__fmt_error_code::unmatched_right_brace_in_fmt_string);

                    iterator last = begin() + rbr;
                    on_chars(last);
                    m_pc_.advance_to(last + 1);
                    fmt = fmt_string();

                    if (lbr != fmt.npos)
                        lbr -= rbr + 1;
                    rbr = fmt.find('}');
                }
            }

            return {};
        }

        constexpr void scan() {
            if (auto scan_result = try_scan(); !scan_result)
                __fmt_error::on_error_with(scan_result.error());
        }

        constexpr virtual __format::result<void> format_arg(std::size_t) {
            return {};
        }

        constexpr virtual void on_chars(iterator) {}

        constexpr __format::result<void> on_replacement_field() {
            // starts after '{'
            auto next = begin();
            std::size_t id;

            if (*next == '}') {
                __GOLDM_FMT_TRY(m_pc_.try_next_arg_id(), { id = try_result; });
            } else if (*next == ':') {
                __GOLDM_FMT_TRY(m_pc_.try_next_arg_id(), { id = try_result; });
                ++next;
                m_pc_.advance_next();
            } else {
                // expected to have a digit
                auto [i, ptr] = __parse::parse_arg_id(begin(), end());
                if (!ptr || !(*ptr == '}' || *ptr == ':'))
                    return __format::error_t(__fmt_error_code::invalid_arg_id_in_fmt_string);
                __GOLDM_FMT_TRYV(m_pc_.try_check_arg_id(id = i), {});
                if (*ptr == ':') {
                    m_pc_.advance_to(++ptr);
                } else {
                    m_pc_.advance_to(ptr);
                }
            }

            __GOLDM_FMT_TRYV(this->format_arg(id), {});

            next = m_pc_.begin();
            if (next == m_pc_.end() || *next != '}')
                return __format::error_t(__fmt_error_code::unmatched_right_brace_in_fmt_string);

            // move past '}'
            m_pc_.advance_next();

            return {};
        }
    };

    /// __format::formatting_scanner
    struct formatting_scanner : scanner {
        format_context& m_fc_;

        constexpr formatting_scanner(format_context& fc, std::string_view s)
        : scanner(s), m_fc_(fc) {}

        using iterator = scanner::iterator;

        constexpr void on_chars(iterator last) override {
            std::string_view str (this->begin(), last);
            m_fc_.advance_to(__format::write(m_fc_.out(), str));
        }

        constexpr __format::result<void> format_arg(std::size_t id) override {
            using handle  = gold::format_arg::handle;
            return gold::visit_format_arg([this](auto& arg) -> __format::result<void> {
                using Type      = __remove_reference(decltype(arg));
                using Formatter = gold::formatter<Type>;
                if constexpr (__is_same(__util::empty_t, Type))
                    return __format::error_t(__fmt_error_code::invalid_arg_id_in_fmt_string);
                else if constexpr (__is_same(handle, Type))
                    return arg.try_format(this->m_pc_, this->m_fc_);
                else if constexpr (__is_constructible(Formatter)) {
                    Formatter f;
                    __GOLDM_FMT_TRY(f.try_parse(this->m_pc_), { this->m_pc_.advance_to(try_result); });
                    __GOLDM_FMT_TRY((f.try_format(arg, this->m_fc_)), { this->m_fc_.advance_to(try_result); });
                }
                return {};
            }, m_fc_.arg(id));
        }
    };

    /// __format::checking_scanner
    template <typename... Args>
    struct checking_scanner : scanner {

        constexpr checking_scanner(std::string_view s)
        : scanner(s, __format::make_arg_kinds<Args...>()) {}

        constexpr __format::result<void> format_arg(std::size_t id) override
        {
            __format::error_code ec = __fmt_error_code::none;
            if constexpr (sizeof...(Args) != 0) {
                gold::__algo::visit_with_index<sizeof...(Args)>(id, [&]<std::size_t I> {
                    if (ec == __fmt_error_code::none) {
                        using Formatter = gold::formatter<__format::normalize_type<__type_pack_element<I, Args...>>>;
                        Formatter f;
                        if (auto parse_result = f.try_parse(this->m_pc_); !parse_result)
                            ec = parse_result.error();
                        else
                            this->m_pc_.advance_to(*parse_result);
                    }
                });
            }
            if (ec != __fmt_error_code::none)
                return __format::error_t(ec);
            return {};
        }
    };

} // namespace gold::__format

#endif // __GOLD_BITS_FORMAT_SCANNER_HPP
