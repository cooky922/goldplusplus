// <gold/bits/memory/uninitialized.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_MEMORY_UNINITIALIZED_HPP
#define __GOLD_BITS_MEMORY_UNINITIALIZED_HPP

#include <type_traits>

namespace gold {

    /// uninitialized
    // manual lifetime of objects
    template <typename T>
    union uninitialized {
        T value;

        // this makes the type non-trivial must still
        // trivially copyable if its underlying type is too
        constexpr uninitialized() {}
        constexpr ~uninitialized() {}

        constexpr uninitialized(const uninitialized&)
            requires std::is_trivially_copyable_v<T>
        = default;

        constexpr uninitialized& operator=(const uninitialized&)
            requires std::is_trivially_copyable_v<T>
        = default;
    };

} // namespace gold

#endif // __GOLD_BITS_MEMORY_UNINITIALIZED_HPP

