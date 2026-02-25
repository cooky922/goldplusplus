// <gold/preview/bits/format/fmt_args.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: this is only a prototype
// note: this is an internal header

#pragma once
#ifndef __GOLD_PREVIEW_BITS_FMT_ARGS_HPP
#define __GOLD_PREVIEW_BITS_FMT_ARGS_HPP

#include <gold/preview/bits/format/fmt_fwd.hpp>
#include <cstddef>

namespace gold::preview {

    /// basic_format_args
    template <typename Context>
    class basic_format_args {
      private:
        std::size_t m_size_ { 0 };
        const basic_format_arg<Context>* m_data_ { nullptr };

      public:
        /// default constructor
        basic_format_args() noexcept = default;

        /// more constructors ...
        template <typename... Args>
        basic_format_args(const __detail::format_arg_store_<Context, Args...>& store) noexcept
        : m_size_(sizeof...(Args)), m_data_(store.args_.data()) {}

        /// get
        basic_format_arg<Context> get(std::size_t id) const noexcept {
            return id < m_size_ ? m_data_[id] : basic_format_arg<Context>{};
        }

        /// size_ [ not intended to be exposed in public ]
        std::size_t size_() const noexcept { return m_size_; }

    };

} // namespace gold::preview

#endif // __GOLD_PREVIEW_BITS_FMT_ARGS_HPP
