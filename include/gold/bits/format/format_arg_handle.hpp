// <gold/bits/format/format_arg_handle.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FORMAT_FORMAT_ARG_HANDLE_HPP
#define __GOLD_BITS_FORMAT_FORMAT_ARG_HANDLE_HPP

#include <gold/bits/__util/cast_from_vptr.hpp>
#include <gold/bits/format/fwd.hpp>
#include <gold/bits/format/format_args.hpp>
#include <gold/bits/format/format_parse_context.hpp>
#include <gold/bits/format/format_context.hpp>

namespace gold {

    /// format_arg::handle::handle(T&)
    template <typename T>
    constexpr format_arg::handle::handle(T& op) noexcept
    : ptr(const_cast<void*>(static_cast<const void*>(__builtin_addressof(op)))),
      fn([](void* op, format_parse_context& pc, format_context& fc) -> __format::result<void> {
        using Td = __remove_cv(T);
        gold::formatter<Td> f;
        __GOLDM_FMT_TRY(f.try_parse(pc), { pc.advance_to(try_result); });
        T& val = *static_cast<T*>(op);
        __GOLDM_FMT_TRY((f.try_format(val, fc)), { fc.advance_to(try_result); });
        return {};
    }) {}

} // namespace gold

#endif // __GOLD_BITS_FORMAT_FORMAT_ARG_HANDLE_HPP

