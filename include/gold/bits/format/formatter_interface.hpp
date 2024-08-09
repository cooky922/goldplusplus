// <gold/bits/format/formatter_interface.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FORMAT_FORMATTER_INTERFACE_HPP
#define __GOLD_BITS_FORMAT_FORMATTER_INTERFACE_HPP

#include <gold/bits/format/fwd.hpp>
#include <gold/bits/format/error.hpp>
#include <gold/bits/format/concepts.hpp>

// TODO:

//namespace gold {
//
//    /// formatter_interface
//    template <typename D>
//        requires std::is_class_v<D> &&
//                 __format::basic_formattable_impl<D>
//    struct formatter_interface {
//      private:
//        constexpr D& mf_derived_() noexcept {
//            return static_cast<D&>(*this);
//        }
//
//        constexpr const D& mf_derived_() const noexcept {
//            return static_cast<const D&>(*this);
//        }
//
//      public:
//        // provide member 'parse' or
//        constexpr auto parse(format_parse_context& pc)
//            requires (!requires { mf_derived_().D::parse(pc); })
//        {
//
//        }
//
//    };
//
//} // namespace gold

#endif // __GOLD_BITS_FORMAT_FORMATTER_INTERFACE_HPP
