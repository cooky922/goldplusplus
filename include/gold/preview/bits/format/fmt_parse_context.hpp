// <gold/preview/bits/format/fmt_parse_context.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: this is only a prototype
// note: this is an internal header

#pragma once
#ifndef __GOLD_PREVIEW_BITS_FMT_PARSE_CONTEXT_HPP
#define __GOLD_PREVIEW_BITS_FMT_PARSE_CONTEXT_HPP

#include <gold/preview/bits/format/fmt_fwd.hpp>
#include <gold/preview/bits/format/fmt_error.hpp>
#include <type_traits>
#include <string_view>

namespace gold::preview {

    /// basic_format_parse_context
    template <typename CharT>
    class basic_format_parse_context {
      public:
        /// type alias
        using char_type      = CharT;
        using const_iterator = typename std::basic_string_view<CharT>::const_iterator;
        using iterator       = const_iterator;

      private:
        /// data members
        iterator m_begin_;
        iterator m_end_;
        enum class indexing_ { unknown_, manual_, automatic_ };
        indexing_ m_indexing_;

        std::size_t m_next_arg_id_;
        std::size_t m_num_args_;

      public:

        /// explicit constructor
        explicit constexpr basic_format_parse_context(std::basic_string_view<CharT> fmt,
                                                      std::size_t num_args = 0) noexcept
        : m_begin_(fmt.begin()),
          m_end_(fmt.end()),
          m_indexing_(indexing_::unknown_),
          m_next_arg_id_(0),
          m_num_args_(num_args) {}

        /// deleted copy constructor
        constexpr basic_format_parse_context(const basic_format_parse_context&) = delete;

        /// deleted copy assignment
        constexpr basic_format_parse_context& operator=(const basic_format_parse_context&) = delete;

        /// iterator begin
        constexpr iterator begin() const noexcept { return m_begin_; }

        /// iterator end
        constexpr iterator end() const noexcept { return m_end_; }

        /// advance_to
        constexpr void advance_to(const_iterator it) { m_begin_ = it; }

        /// next_arg_id
        constexpr std::size_t next_arg_id() {
            if (m_indexing_ == indexing_::manual_)
                throw gold::preview::format_error("Using automatic argument numbering in manual "
                                         "argument numbering mode");

            if (m_indexing_ == indexing_::unknown_)
                m_indexing_ = indexing_::automatic_;

            return m_next_arg_id_++;
        }

        /// check_arg_id
        constexpr void check_arg_id(std::size_t id) {
            if (m_indexing_ == indexing_::automatic_)
                throw gold::preview::format_error("Using manual argument numbering in automatic "
                                         "argument numbering mode");

            if (m_indexing_ == indexing_::unknown_)
                m_indexing_ = indexing_::manual_;

            if (std::is_constant_evaluated() && id >= m_num_args_)
                throw gold::preview::format_error("Argument index outside the valid range");
        }
    };

    /// format_parse_context
    using format_parse_context = basic_format_parse_context<char>;

    /// wformat_parse_context
    using wformat_parse_context = basic_format_parse_context<wchar_t>;

} // namespace gold::preview

#endif // __GOLD_PREVIEW_BITS_FMT_PARSE_CONTEXT_HPP
