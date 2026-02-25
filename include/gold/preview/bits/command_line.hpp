// <gold/preview/bits/command_line.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: this is only a prototype
// note: this is an internal header

#pragma once
#ifndef __GOLD_PREVIEW_BITS_COMMAND_LINE_HPP
#define __GOLD_PREVIEW_BITS_COMMAND_LINE_HPP

#include <cstdint>
#include <cstddef>

namespace gold::preview {

    namespace graphics { class window_t; } // namespace graphics

    /// command_line
    class command_line {
      private:
        friend graphics::window_t;

        int* m_arg_count_;
        char** m_args_;

      public:
        /// type alias
        using value_type = char*;
        using iterator   = char**;

        /// constructors
        constexpr command_line(int& argc, char** argv)
        : m_arg_count_(&argc), m_args_(argv) {}

        constexpr command_line(char** argv, int& argc)
        : command_line(argc, argv) {}

        /// size
        constexpr std::size_t size() const noexcept { return *m_arg_count_; }

        /// data
        constexpr char** data() const noexcept { return m_args_; }

        /// operator[]
        constexpr char* operator[](std::size_t pos) const noexcept {
            return m_args_[pos];
        }

        /// begin
        constexpr auto begin() const noexcept { return iterator { m_args_ }; }

        /// end
        constexpr auto end() const noexcept { return iterator { m_args_ + *m_arg_count_ }; }

    }; // class command_line

} // namespace gold::preview

#endif // __GOLD_PREVIEW_BITS_COMMAND_LINE_HPP
