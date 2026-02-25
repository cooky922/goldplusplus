// <gold/preview/bits/format/fmt_context.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: this is only a prototype
// note: this is an internal header

#pragma once
#ifndef __GOLD_PREVIEW_BITS_FMT_CONTEXT_HPP
#define __GOLD_PREVIEW_BITS_FMT_CONTEXT_HPP

#include <gold/preview/bits/format/fmt_args.hpp>

#include <bits/iterator_concepts.h>
#include <concepts>
#include <iterator>
#include <string>

#include <locale>
#include <optional>

namespace gold::preview {

    /// basic_format_context [ forward decl ]
    template <typename OutIter, typename CharT>
        requires std::output_iterator<OutIter, const CharT&>
    class basic_format_context;

    namespace __detail {

        /// format_context_create_
        template  <typename OutIter, typename CharT>
        basic_format_context<OutIter, CharT> format_context_create_(
            OutIter out,
            basic_format_args<basic_format_context<OutIter, CharT>> args,
            std::optional<std::locale>&& loc = std::nullopt
        ) {
            return gold::preview::basic_format_context(std::move(out), args, std::move(loc));
        }

    }

    /// basic_format_context
    template <typename OutIter, typename CharT>
        requires std::output_iterator<OutIter, const CharT&>
    class basic_format_context {
      public:
        /// iterator
        using iterator = OutIter;

        /// char_type
        using char_type = CharT;

        /// formatter_type
        template <typename T>
        using formatter_type = formatter<T, CharT>;

      private:
        /// m_out_
        iterator m_out_;

        /// m_args_;
        basic_format_args<basic_format_context> m_args_;

        /// m_loc_
        std::optional<std::locale> m_loc_;

      public:
        /// deleted copy constructor
        basic_format_context(const basic_format_context&) = delete;

        /// deleted copy assignment
        basic_format_context& operator=(const basic_format_context&) = delete;

        /// arg
        basic_format_arg<basic_format_context> arg(std::size_t id) const {
            return m_args_.get(id);
        }

        /// locale
        std::locale locale() {
            if (!m_loc_)
                m_loc_ = std::locale{};
            return *m_loc_;
        }

        /// out
        iterator out() { return m_out_; }

        /// advance_to
        void advance_to(iterator it) { m_out_ = it; }

      private:
        /// friend function
        template <typename OutIter_, typename CharT_>
        friend basic_format_context<OutIter_, CharT_> __detail::format_context_create_(
            OutIter_, basic_format_args<basic_format_context<OutIter_, CharT_>>,
            std::optional<std::locale>&&
        );

        /// explicit private constructor
        explicit basic_format_context(OutIter out,
                                      basic_format_args<basic_format_context<OutIter, CharT>> args,
                                      std::optional<std::locale>&& loc)
        : m_out_(out), m_args_(args), m_loc_(loc) {}
    };

    /// format_context
    using format_context = basic_format_context<std::back_insert_iterator<std::string>, char>;

    /// wformat_context
    using wformat_context = basic_format_context<std::back_insert_iterator<std::wstring>, wchar_t>;

    /// format_args
    using format_args = basic_format_args<format_context>;

    /// wformat_args
    using wformat_args = basic_format_args<wformat_context>;

    /// format_args_t
    template <typename OutIter, typename CharT>
    using format_args_t = basic_format_args<basic_format_context<OutIter, CharT>>;

} // namespace gold::preview

#endif // __GOLD_PREVIEW_BITS_FMT_CONTEXT_HPP
