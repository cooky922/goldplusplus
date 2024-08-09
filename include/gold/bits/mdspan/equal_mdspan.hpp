// <gold/bits/mdspan/equal_mdspan.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_MDSPAN_EQUAL_MDSPAN_HPP
#define __GOLD_BITS_MDSPAN_EQUAL_MDSPAN_HPP

#include <gold/bits/mdspan/mdspan.hpp>
#include <gold/bits/mdspan/index_cartesian_product.hpp>

namespace gold {

    /// equal_mdspan
    template <typename T,  typename U,
              typename E1, typename E2,
              typename L1, typename L2,
              typename A1, typename A2>
        requires std::equality_comparable_with<T, U>
    constexpr bool equal_mdspan(const gold::mdspan<T, E1, L1, A1>& a,
                                const gold::mdspan<U, E2, L2, A2>& b) {
        if constexpr (a.rank() == b.rank()) {
            if constexpr (std::is_same_v<decltype(a), decltype(b)>)
                if (&a == &b)
                    return true;
            if (a.extents() == b.extents()) {
                for (const auto& indices : __mdspan::index_cartesian_product(a.extents()))
                    if (a[indices] != b[indices])
                        return false;
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

} // namespace gold

#endif // __GOLD_BITS_MDSPAN_EQUAL_MDSPAN_HPP
