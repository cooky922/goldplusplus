// <gold/bits/format/format_parse_context.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FORMAT_FORMAT_PARSE_CONTEXT_HPP
#define __GOLD_BITS_FORMAT_FORMAT_PARSE_CONTEXT_HPP

#include <string_view>
#include <gold/bits/format/error.hpp>
#include <gold/bits/format/format_arg_kind.hpp>

namespace gold {

    /// format_parse_context
    class format_parse_context {
      public:
        using char_type      = char;
        using const_iterator = std::string_view::const_iterator;
        using iterator       = const_iterator;

      private:
        enum class indexing_kind { unknown, manual, automatic };
        iterator m_begin_, m_end_;
        indexing_kind m_indexing_;
        std::size_t m_next_arg_id_, m_num_args_;
        __format::arg_kinds<> m_kinds_;

      public:
        constexpr explicit format_parse_context(std::string_view fmt, std::size_t num_args = 0)
        : m_begin_(fmt.begin()), m_end_(fmt.end()), m_indexing_(indexing_kind::unknown),
          m_next_arg_id_(0), m_num_args_(num_args), m_kinds_(__format::make_arg_kinds<>()) {}

        constexpr explicit format_parse_context(std::string_view fmt, __format::arg_kinds<> kinds)
        : m_begin_(fmt.begin()), m_end_(fmt.end()), m_indexing_(indexing_kind::unknown),
          m_next_arg_id_(0), m_num_args_(kinds.size()), m_kinds_(kinds) {}

        constexpr format_parse_context(const format_parse_context&) = delete;
        constexpr format_parse_context& operator=(const format_parse_context&) = delete;

        constexpr const_iterator begin() const noexcept { return m_begin_; }
        constexpr const_iterator end() const noexcept { return m_end_; }
        constexpr void advance_to(const_iterator iter) { m_begin_ = iter; }
        constexpr void advance_next() { ++m_begin_; }
        constexpr void advance_to_end() { m_begin_ = m_end_; }

        // for automatic indexing check
        constexpr __format::result<std::size_t> try_next_arg_id() noexcept {
            if (m_indexing_ == indexing_kind::manual)
                return __format::error_t(__fmt_error_code::mixing_manual_in_automatic_indexing);
            if (m_indexing_ == indexing_kind::unknown)
                m_indexing_ = indexing_kind::automatic;
            if (__builtin_is_constant_evaluated() && m_next_arg_id_ >= m_num_args_)
                return __format::error_t(__fmt_error_code::arg_index_outside_valid_range);
            return m_next_arg_id_++;
        }

        // for manual indexing check
        constexpr __format::result<void> try_check_arg_id(std::size_t id) noexcept {
            if (m_indexing_ == indexing_kind::automatic)
                return __format::error_t(__fmt_error_code::mixing_automatic_in_manual_indexing);
            if (m_indexing_ == indexing_kind::unknown)
                m_indexing_ = indexing_kind::manual;
            if (__builtin_is_constant_evaluated() && id >= m_num_args_)
                return __format::error_t(__fmt_error_code::arg_index_outside_valid_range);
            return {};
        }

        // for checking the type of arguments
        template <typename... Ts>
        constexpr __format::result<void> try_check_dynamic_spec(std::size_t id) noexcept {
            if consteval {
                // only the checking scanner can do this
                if (m_kinds_.size() != 0) {
                    constexpr auto query_kinds = __format::make_arg_kinds<Ts...>();
                    if (id >= m_num_args_)
                        return __format::error_t(__fmt_error_code::arg_index_outside_valid_range);
                    if (!query_kinds.packed_values.contains(m_kinds_[id]))
                        return __format::error_t(__fmt_error_code::mismatched_types_from_arguments);
                }
            }
            return {};
        }

        constexpr __format::result<void> try_check_dynamic_spec_integral(std::size_t id) noexcept {
            if consteval {
                // only the checking scanner can do this
                if (m_kinds_.size() != 0) {
                    if (id >= m_num_args_)
                        return __format::error_t(__fmt_error_code::arg_index_outside_valid_range);
                    gold::uint8_t underlying = static_cast<gold::uint8_t>(m_kinds_[id]);
                    if (underlying < 3 || underlying > 8)
                        return __format::error_t(__fmt_error_code::mismatched_types_from_arguments);
                }
            }
            return {};
        }

        constexpr __format::result<void> try_check_dynamic_spec_string(std::size_t id) noexcept {
            if consteval {
                // only the checking scanner can do this
                if (m_kinds_.size() != 0) {
                    if (id >= m_num_args_)
                        return __format::error_t(__fmt_error_code::arg_index_outside_valid_range);
                    gold::uint8_t underlying = static_cast<gold::uint8_t>(m_kinds_[id]);
                    if (underlying < 15 || underlying > 16)
                        return __format::error_t(__fmt_error_code::mismatched_types_from_arguments);
                }
            }
            return {};
        }

    };

} // namespace gold

#endif // __GOLD_BITS_FORMAT_FORMAT_PARSE_CONTEXT_HPP
