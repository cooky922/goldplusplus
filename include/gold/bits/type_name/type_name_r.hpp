// <gold/bits/type_name/type_name_r.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TYPE_NAME_R_HPP
#define __GOLD_BITS_TYPE_NAME_R_HPP

#include <string>
#include <typeinfo>
#include <gold/demangling>

namespace gold {

    /// type_name_r - runtime type name query
    // source: type_info
    template <typename T>
    auto type_name_r(const T& arg) noexcept {
        return gold::demangle(typeid(arg).name());
    }

    template <typename T>
    auto type_name_r() noexcept {
        return gold::demangle(typeid(T).name());
    }

} // namespace gold

#endif // __GOLD_BITS_TYPE_NAME_R_HPP
