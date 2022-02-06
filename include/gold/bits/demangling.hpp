// <gold/bits/demangling.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_DEMANGLING_HPP
#define __GOLD_BITS_DEMANGLING_HPP

#include <string>

namespace gold {

    /// demangle
    std::string demangle(std::string_view) noexcept;

} // namespace gold

#endif // __GOLD_BITS_DEMANGLING_HPP
