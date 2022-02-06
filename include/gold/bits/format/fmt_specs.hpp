// <gold/bits/format/fmt_specs.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FORMAT_SPECS_HPP
#define __GOLD_BITS_FORMAT_SPECS_HPP

#include <cstdint>

namespace gold {

    /// format_specs
    struct format_specs {

        enum class alignment_t : std::uint8_t {
            default_opt, left, center, right
        };

        /// alignment
        alignment_t alignment { alignment_t::default_opt };


        enum class sign_t : std::uint8_t {
            default_opt, minus, plus, space
        };

        /// sign
        sign_t sign { sign_t::default_opt };

        enum class type_t : std::uint8_t {
            default_opt,
            string,             // s
            string_quoted,      // S
            binary_lower,       // b
            binary_upper,       // B
            octal,              // o
            decimal,            // d
            hex_lower,          // x
            hex_upper,          // X
            float_hex_lower,    // a
            float_hex_upper,    // A
            pointer,            // p
            character,          // c
            character_quoted,   // C
            scientific_lower,   // e
            scientific_upper,   // E
            fixed_lower,        // f
            fixed_upper,        // F
            general_lower,      // g
            general_upper,      // G
        };

        /// type
        type_t type { type_t::default_opt };

        /// padding_zero
        bool padding_zero : 1 { false };

        /// show_base
        bool show_base : 1 { false };

        // for width and precisions:
        // -1 means default
        // -2 means dynamic (taken from formatting arguments)
        /// width
        int width { -1 };

        /// precision
        int precision { -1 };

        /// fill_character
        char fill_character { ' ' };

        /// reset
        constexpr void reset() {
            alignment      = alignment_t::default_opt;
            sign           = sign_t::default_opt;
            type           = type_t::default_opt;
            padding_zero   = false;
            padding_zero   = false;
            precision      = width = -1;
            fill_character = ' ';
        }

    };

} // namespace gold

#endif // __GOLD_BITS_FORMAT_SPECS_HPP
