// <gold/bits/memory/voidify.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_MEMORY_VOIDIFY_HPP
#define __GOLD_BITS_MEMORY_VOIDIFY_HPP

#include <bits/move.h>

namespace gold {

    /// voidify
    template <typename T>
    constexpr void* voidify(T& from) {
        return const_cast<void*>(
            static_cast<const volatile void*>(std::addressof(from))
        );
    }

} // namespace gold

#endif // __GOLD_BITS_MEMORY_VOIDIFY_HPP
