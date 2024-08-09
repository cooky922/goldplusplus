// <gold/bits/concepts/class_or_union_or_enum.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_CONCEPTS_CLASS_OR_UNION_OR_ENUM_HPP
#define __GOLD_BITS_CONCEPTS_CLASS_OR_UNION_OR_ENUM_HPP

#include <type_traits>

namespace gold::__concepts {

    /// __concepts::class_or_union
    template <typename T>
    concept class_or_union = std::is_class_v<T> || std::is_union_v<T>;

    /// __concepts::class_or_union_or_enum
    template <typename T>
    concept class_or_union_or_enum = __concepts::class_or_union<T> || std::is_enum_v<T>;

} // namespace gold::__concepts

#endif // __GOLD_BITS_CONCEPTS_CLASS_OR_UNION_OR_ENUM_HPP
