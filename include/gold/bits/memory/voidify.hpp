// <gold/bits/memory/voidify.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_MEMORY_VOIDIFY_HPP
#define __GOLD_BITS_MEMORY_VOIDIFY_HPP

namespace gold {

    /// voidify
    template <typename T>
    constexpr void* voidify(T& from) noexcept {
        return const_cast<void*>(static_cast<const volatile void*>(__builtin_addressof(from)));
    }

    template <typename T>
    constexpr void voidify(const T&&) = delete;


} // namespace gold

#endif // __GOLD_BITS_MEMORY_VOIDIFY_HPP
