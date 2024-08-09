// <gold/bits/concepts/not_void.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_CONCEPTS_NOT_VOID_HPP
#define __GOLD_BITS_CONCEPTS_NOT_VOID_HPP

#include <type_traits>

namespace gold::__concepts {

    /// __concepts::not_void
    template <typename T>
    concept not_void = !std::is_void_v<T>;

} // namespace gold::__concepts

#endif // __GOLD_BITS_CONCEPTS_NOT_VOID_HPP
