// <gold/bits/format/format_context.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FORMAT_FORMAT_CONTEXT_HPP
#define __GOLD_BITS_FORMAT_FORMAT_CONTEXT_HPP

#include <gold/bits/format/fwd.hpp>
#include <gold/bits/format/format_args.hpp>
#include <gold/bits/format/sink.hpp>

namespace gold {

    /// format_context
    class format_context {
      public:
        using iterator = __format::sink_iterator;

      private:
        const format_args* m_args_;
        iterator           m_out_;

        constexpr format_context(const format_args* args, iterator out) noexcept
        : m_args_(args), m_out_(std::move(out)) {}

        friend constexpr format_context __format::create_format_context(const format_args*, iterator) noexcept;
        friend constexpr format_context __format::create_artificial_format_context(format_context&, iterator) noexcept;

      public:
        format_context() = default;
        ~format_context() = default;

        [[nodiscard]] constexpr iterator out() { return std::move(m_out_); }
        constexpr void advance_to(iterator it) { m_out_ = std::move(it); }

        constexpr format_arg arg(std::size_t id) const noexcept {
            return m_args_ ? m_args_->at(id) : format_arg();
        }
    };

    namespace __format {

        /// __format::create_format_context
        constexpr format_context create_format_context(const format_args* args, __format::sink_iterator out) noexcept {
            return format_context(args, std::move(out));
        }

        /// __format::create_artificial_format_context
        constexpr format_context create_artificial_format_context(format_context& fc, __format::sink_iterator out) noexcept {
            return format_context(fc.m_args_, std::move(out));
        }

    } // namespace __format

} // namespace gold

#endif // __GOLD_BITS_FORMAT_FORMAT_CONTEXT_HPP
