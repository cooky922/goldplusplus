// <gold/bits/mdspan/fwd.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_MDSPAN_FWD_HPP
#define __GOLD_BITS_MDSPAN_FWD_HPP

#include <cstddef>

namespace gold {

    /// extents [fwd]
    template <typename, std::size_t...>
    class extents;

    /// layout_left
    // column-major multidimensional array layout mapping
    // leftmost extent has stride 1
    struct layout_left {
        template <typename Extents>
        class mapping;
    };

    /// layout_left_padded
    // a layout which behaves like layout_left
    // except that its mapping which has a
    // padding stride 'stride(1)' can be greater
    // than or equal to 'extent(0)'
    template <std::size_t PaddingValue = std::size_t(-1)>
    struct layout_left_padded {
        template <typename Extents>
        class mapping;
    };

    /// layout_right
    // row-major multidimensional array layout mapping
    // rightmost extent has stride 1
    // this is the default layout mapping for gold::mdspan
    struct layout_right {
        template <typename Extents>
        class mapping;
    };

    /// layout_right_padded
    // a layout which behaves like layout_right
    // except that its mapping which has a
    // padding stride 'stride(n - 2)' can be greater
    // than or equal to 'extent(n - 1)'
    template <std::size_t PaddingValue = std::size_t(-1)>
    struct layout_right_padded {
        template <typename Extents>
        class mapping;
    };


    /// layout_stride
    // layout mapping with user-defined strides
    struct layout_stride {
        template <typename Extents>
        class mapping;
    };

    /// default_accessor [fwd]
    template <typename>
    struct default_accessor;

    /// mdspan [fwd]
    template <typename T, typename Extents,
              typename LayoutPolicy   = layout_right,
              typename AccessorPolicy = default_accessor<T>>
    class mdspan;

} // namespace gold

#endif // __GOLD_BITS_MDSPAN_FWD_HPP
