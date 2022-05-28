// <gold/bits/any/bad_any_access.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_BAD_ANY_ACCESS_HPP
#define __GOLD_BITS_BAD_ANY_ACCESS_HPP

#include <bits/exception.h>             // std::exception
#include <string_view>

namespace gold {

    //// [gold.any.exception]
    /// bad_any_access
    class bad_any_access : public std::exception {
      private:
        std::string_view msg;

      public:
        bad_any_access(std::string_view msg)
        : msg(msg) {}

        const char* what() const noexcept override { return msg.data(); }
    };

    namespace __bad_any_access {

        /// __bad_any_access::bad_cast
        void bad_cast() {}

        /// __bad_any_access::unimplemented_equal_operator
        void unimplemented_equal_operator() {}

        /// __bad_any_access::unimplemented_spaceship_operator
        void unimplemented_spaceship_operator() {}

    } // namespace __bad_any_access

} // namespace gold

#endif // __GOLD_BITS_BAD_ANY_ACCESS_HPP
