// <gold/bits/format/format_to_n_result.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FORMAT_FORMAT_TO_N_RESULT_HPP
#define __GOLD_BITS_FORMAT_FORMAT_TO_N_RESULT_HPP

#include <bits/iterator_concepts.h>

namespace gold {

    /// format_to_n_result
    template <typename Out>
    struct format_to_n_result {
        Out out;
        std::iter_difference_t<Out> size;
    };

} // namespace gold

#endif // __GOLD_BITS_FORMAT_FORMAT_TO_N_RESULT_HPP
