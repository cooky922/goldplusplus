// <gold/bits/concepts/scalar_numeric_type.hpp> - gold++ library

// Copyright (C) [ 2025 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_CONCEPTS_SCALAR_NUMERIC_TYPE_HPP
#define __GOLD_BITS_CONCEPTS_SCALAR_NUMERIC_TYPE_HPP

#include <gold/bits/type_traits/is_scalar_numeric_type_v.hpp>

namespace gold {

    /// scalar_numeric_type
    template <typename T>
    concept scalar_numeric_type = is_scalar_numeric_type_v<T>;

} // namespace gold

#endif // __GOLD_BITS_CONCEPTS_SCALAR_NUMERIC_TYPE_HPP
