// <gold/preview/bits/format/fmt_error.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: this is only a prototype
// note: this is an internal header

#pragma once
#ifndef __GOLD_PREVIEW_BITS_FMT_ERROR_HPP
#define __GOLD_PREVIEW_BITS_FMT_ERROR_HPP

#include <stdexcept>
#include <string>

namespace gold::preview {

    /// format_error
    class format_error : public std::runtime_error {
      public:
        explicit format_error(const std::string& s)
        : std::runtime_error(s) {}

        explicit format_error(const char* s)
        : std::runtime_error(s) {}

        virtual ~format_error() noexcept = default;
    };

} // namespace gold::preview

#endif // __GOLD_PREVIEW_BITS_FMT_ERROR_HPP
