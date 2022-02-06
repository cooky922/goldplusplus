// <gold/bits/typeof/runtime_typeof.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RUNTIME_TYPEOF_HPP
#define __GOLD_BITS_RUNTIME_TYPEOF_HPP

#include <string>
#include <typeinfo>
#include <gold/bits/demangling.hpp>

namespace gold::__detail {

    /// type_of_
    template <typename T>
    std::string type_of_(const T& arg) {
        return gold::demangle(typeid(arg).name());
    }

    template <typename T>
    std::string type_of_() noexcept {
        return gold::demangle(typeid(T).name());
    }

} // namespace gold::__detail

#endif // __GOLD_BITS_RUNTIME_TYPEOF_HPP
