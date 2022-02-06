// <gold/bits/format/fmt_error.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FORMAT_ERROR_HPP
#define __GOLD_BITS_FORMAT_ERROR_HPP

#include <stdexcept>

namespace gold {

    /// format_error
    class format_error : public std::runtime_error {
      public:
        explicit format_error(const char* s)
        : std::runtime_error(s) {}

        virtual ~format_error() noexcept = default;
    };

} // namespace gold

#endif // __GOLD_BITS_FORMAT_ERROR_HPP
