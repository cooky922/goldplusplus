// <gold/bits/algo/with_index.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ALGO_WITH_INDEX_HPP
#define __GOLD_BITS_ALGO_WITH_INDEX_HPP

#include <cstddef>
#include <bits/move.h>
#include <gold/bits/assume.hpp>

namespace gold::__algo {

    namespace __with_index {

        // maybe with different approach using jump table
        #define __GOLD_MACRO_WITH_INDEX_CASE(I) \
            case I: return std::forward<F>(f).template operator()<I + K>()

        #define __GOLD_MACRO_WITH_INDEX_DEFAULT_CASE() \
            default: return std::forward<F>(f).template operator()<K>()

        #define __GOLD_MACRO_MAKE_WITH_INDEX(I, Cases) \
            template <std::size_t K> \
            inline constexpr auto with_index_impl<I, K> = []<typename F>(std::size_t i, F&& f) { \
                switch (i) { \
                    Cases \
                } \
            }

        /// __type_trait::with_index_impl
        template <std::size_t N, std::size_t K = 0>
        inline constexpr auto with_index_impl = []<typename F>(std::size_t i, F&& f) {
            switch (i) {
                __GOLD_MACRO_WITH_INDEX_CASE(19);
                __GOLD_MACRO_WITH_INDEX_CASE(18);
                __GOLD_MACRO_WITH_INDEX_CASE(17);
                __GOLD_MACRO_WITH_INDEX_CASE(16);
                __GOLD_MACRO_WITH_INDEX_CASE(15);
                __GOLD_MACRO_WITH_INDEX_CASE(14);
                __GOLD_MACRO_WITH_INDEX_CASE(13);
                __GOLD_MACRO_WITH_INDEX_CASE(12);
                __GOLD_MACRO_WITH_INDEX_CASE(11);
                __GOLD_MACRO_WITH_INDEX_CASE(10);
                __GOLD_MACRO_WITH_INDEX_CASE(9);
                __GOLD_MACRO_WITH_INDEX_CASE(8);
                __GOLD_MACRO_WITH_INDEX_CASE(7);
                __GOLD_MACRO_WITH_INDEX_CASE(6);
                __GOLD_MACRO_WITH_INDEX_CASE(5);
                __GOLD_MACRO_WITH_INDEX_CASE(4);
                __GOLD_MACRO_WITH_INDEX_CASE(3);
                __GOLD_MACRO_WITH_INDEX_CASE(2);
                __GOLD_MACRO_WITH_INDEX_CASE(1);
                __GOLD_MACRO_WITH_INDEX_DEFAULT_CASE();
            }
            return with_index_impl<N - 20, K + 20>(i - 20, std::forward<F>(f));
        };

        __GOLD_MACRO_MAKE_WITH_INDEX(20,
            __GOLD_MACRO_WITH_INDEX_CASE(19);
            __GOLD_MACRO_WITH_INDEX_CASE(18);
            __GOLD_MACRO_WITH_INDEX_CASE(17);
            __GOLD_MACRO_WITH_INDEX_CASE(16);
            __GOLD_MACRO_WITH_INDEX_CASE(15);
            __GOLD_MACRO_WITH_INDEX_CASE(14);
            __GOLD_MACRO_WITH_INDEX_CASE(13);
            __GOLD_MACRO_WITH_INDEX_CASE(12);
            __GOLD_MACRO_WITH_INDEX_CASE(11);
            __GOLD_MACRO_WITH_INDEX_CASE(10);
            __GOLD_MACRO_WITH_INDEX_CASE(9);
            __GOLD_MACRO_WITH_INDEX_CASE(8);
            __GOLD_MACRO_WITH_INDEX_CASE(7);
            __GOLD_MACRO_WITH_INDEX_CASE(6);
            __GOLD_MACRO_WITH_INDEX_CASE(5);
            __GOLD_MACRO_WITH_INDEX_CASE(4);
            __GOLD_MACRO_WITH_INDEX_CASE(3);
            __GOLD_MACRO_WITH_INDEX_CASE(2);
            __GOLD_MACRO_WITH_INDEX_CASE(1);
            __GOLD_MACRO_WITH_INDEX_DEFAULT_CASE();
        );

        __GOLD_MACRO_MAKE_WITH_INDEX(19,
            __GOLD_MACRO_WITH_INDEX_CASE(18);
            __GOLD_MACRO_WITH_INDEX_CASE(17);
            __GOLD_MACRO_WITH_INDEX_CASE(16);
            __GOLD_MACRO_WITH_INDEX_CASE(15);
            __GOLD_MACRO_WITH_INDEX_CASE(14);
            __GOLD_MACRO_WITH_INDEX_CASE(13);
            __GOLD_MACRO_WITH_INDEX_CASE(12);
            __GOLD_MACRO_WITH_INDEX_CASE(11);
            __GOLD_MACRO_WITH_INDEX_CASE(10);
            __GOLD_MACRO_WITH_INDEX_CASE(9);
            __GOLD_MACRO_WITH_INDEX_CASE(8);
            __GOLD_MACRO_WITH_INDEX_CASE(7);
            __GOLD_MACRO_WITH_INDEX_CASE(6);
            __GOLD_MACRO_WITH_INDEX_CASE(5);
            __GOLD_MACRO_WITH_INDEX_CASE(4);
            __GOLD_MACRO_WITH_INDEX_CASE(3);
            __GOLD_MACRO_WITH_INDEX_CASE(2);
            __GOLD_MACRO_WITH_INDEX_CASE(1);
            __GOLD_MACRO_WITH_INDEX_DEFAULT_CASE();
        );

        __GOLD_MACRO_MAKE_WITH_INDEX(18,
            __GOLD_MACRO_WITH_INDEX_CASE(17);
            __GOLD_MACRO_WITH_INDEX_CASE(16);
            __GOLD_MACRO_WITH_INDEX_CASE(15);
            __GOLD_MACRO_WITH_INDEX_CASE(14);
            __GOLD_MACRO_WITH_INDEX_CASE(13);
            __GOLD_MACRO_WITH_INDEX_CASE(12);
            __GOLD_MACRO_WITH_INDEX_CASE(11);
            __GOLD_MACRO_WITH_INDEX_CASE(10);
            __GOLD_MACRO_WITH_INDEX_CASE(9);
            __GOLD_MACRO_WITH_INDEX_CASE(8);
            __GOLD_MACRO_WITH_INDEX_CASE(7);
            __GOLD_MACRO_WITH_INDEX_CASE(6);
            __GOLD_MACRO_WITH_INDEX_CASE(5);
            __GOLD_MACRO_WITH_INDEX_CASE(4);
            __GOLD_MACRO_WITH_INDEX_CASE(3);
            __GOLD_MACRO_WITH_INDEX_CASE(2);
            __GOLD_MACRO_WITH_INDEX_CASE(1);
            __GOLD_MACRO_WITH_INDEX_DEFAULT_CASE();
        );

        __GOLD_MACRO_MAKE_WITH_INDEX(17,
            __GOLD_MACRO_WITH_INDEX_CASE(16);
            __GOLD_MACRO_WITH_INDEX_CASE(15);
            __GOLD_MACRO_WITH_INDEX_CASE(14);
            __GOLD_MACRO_WITH_INDEX_CASE(13);
            __GOLD_MACRO_WITH_INDEX_CASE(12);
            __GOLD_MACRO_WITH_INDEX_CASE(11);
            __GOLD_MACRO_WITH_INDEX_CASE(10);
            __GOLD_MACRO_WITH_INDEX_CASE(9);
            __GOLD_MACRO_WITH_INDEX_CASE(8);
            __GOLD_MACRO_WITH_INDEX_CASE(7);
            __GOLD_MACRO_WITH_INDEX_CASE(6);
            __GOLD_MACRO_WITH_INDEX_CASE(5);
            __GOLD_MACRO_WITH_INDEX_CASE(4);
            __GOLD_MACRO_WITH_INDEX_CASE(3);
            __GOLD_MACRO_WITH_INDEX_CASE(2);
            __GOLD_MACRO_WITH_INDEX_CASE(1);
            __GOLD_MACRO_WITH_INDEX_DEFAULT_CASE();
        );

        __GOLD_MACRO_MAKE_WITH_INDEX(16,
            __GOLD_MACRO_WITH_INDEX_CASE(15);
            __GOLD_MACRO_WITH_INDEX_CASE(14);
            __GOLD_MACRO_WITH_INDEX_CASE(13);
            __GOLD_MACRO_WITH_INDEX_CASE(12);
            __GOLD_MACRO_WITH_INDEX_CASE(11);
            __GOLD_MACRO_WITH_INDEX_CASE(10);
            __GOLD_MACRO_WITH_INDEX_CASE(9);
            __GOLD_MACRO_WITH_INDEX_CASE(8);
            __GOLD_MACRO_WITH_INDEX_CASE(7);
            __GOLD_MACRO_WITH_INDEX_CASE(6);
            __GOLD_MACRO_WITH_INDEX_CASE(5);
            __GOLD_MACRO_WITH_INDEX_CASE(4);
            __GOLD_MACRO_WITH_INDEX_CASE(3);
            __GOLD_MACRO_WITH_INDEX_CASE(2);
            __GOLD_MACRO_WITH_INDEX_CASE(1);
            __GOLD_MACRO_WITH_INDEX_DEFAULT_CASE();
        );

        __GOLD_MACRO_MAKE_WITH_INDEX(15,
            __GOLD_MACRO_WITH_INDEX_CASE(14);
            __GOLD_MACRO_WITH_INDEX_CASE(13);
            __GOLD_MACRO_WITH_INDEX_CASE(12);
            __GOLD_MACRO_WITH_INDEX_CASE(11);
            __GOLD_MACRO_WITH_INDEX_CASE(10);
            __GOLD_MACRO_WITH_INDEX_CASE(9);
            __GOLD_MACRO_WITH_INDEX_CASE(8);
            __GOLD_MACRO_WITH_INDEX_CASE(7);
            __GOLD_MACRO_WITH_INDEX_CASE(6);
            __GOLD_MACRO_WITH_INDEX_CASE(5);
            __GOLD_MACRO_WITH_INDEX_CASE(4);
            __GOLD_MACRO_WITH_INDEX_CASE(3);
            __GOLD_MACRO_WITH_INDEX_CASE(2);
            __GOLD_MACRO_WITH_INDEX_CASE(1);
            __GOLD_MACRO_WITH_INDEX_DEFAULT_CASE();
        );

        __GOLD_MACRO_MAKE_WITH_INDEX(14,
            __GOLD_MACRO_WITH_INDEX_CASE(13);
            __GOLD_MACRO_WITH_INDEX_CASE(12);
            __GOLD_MACRO_WITH_INDEX_CASE(11);
            __GOLD_MACRO_WITH_INDEX_CASE(10);
            __GOLD_MACRO_WITH_INDEX_CASE(9);
            __GOLD_MACRO_WITH_INDEX_CASE(8);
            __GOLD_MACRO_WITH_INDEX_CASE(7);
            __GOLD_MACRO_WITH_INDEX_CASE(6);
            __GOLD_MACRO_WITH_INDEX_CASE(5);
            __GOLD_MACRO_WITH_INDEX_CASE(4);
            __GOLD_MACRO_WITH_INDEX_CASE(3);
            __GOLD_MACRO_WITH_INDEX_CASE(2);
            __GOLD_MACRO_WITH_INDEX_CASE(1);
            __GOLD_MACRO_WITH_INDEX_DEFAULT_CASE();
        );

        __GOLD_MACRO_MAKE_WITH_INDEX(13,
            __GOLD_MACRO_WITH_INDEX_CASE(12);
            __GOLD_MACRO_WITH_INDEX_CASE(11);
            __GOLD_MACRO_WITH_INDEX_CASE(10);
            __GOLD_MACRO_WITH_INDEX_CASE(9);
            __GOLD_MACRO_WITH_INDEX_CASE(8);
            __GOLD_MACRO_WITH_INDEX_CASE(7);
            __GOLD_MACRO_WITH_INDEX_CASE(6);
            __GOLD_MACRO_WITH_INDEX_CASE(5);
            __GOLD_MACRO_WITH_INDEX_CASE(4);
            __GOLD_MACRO_WITH_INDEX_CASE(3);
            __GOLD_MACRO_WITH_INDEX_CASE(2);
            __GOLD_MACRO_WITH_INDEX_CASE(1);
            __GOLD_MACRO_WITH_INDEX_DEFAULT_CASE();
        );

        __GOLD_MACRO_MAKE_WITH_INDEX(12,
            __GOLD_MACRO_WITH_INDEX_CASE(11);
            __GOLD_MACRO_WITH_INDEX_CASE(10);
            __GOLD_MACRO_WITH_INDEX_CASE(9);
            __GOLD_MACRO_WITH_INDEX_CASE(8);
            __GOLD_MACRO_WITH_INDEX_CASE(7);
            __GOLD_MACRO_WITH_INDEX_CASE(6);
            __GOLD_MACRO_WITH_INDEX_CASE(5);
            __GOLD_MACRO_WITH_INDEX_CASE(4);
            __GOLD_MACRO_WITH_INDEX_CASE(3);
            __GOLD_MACRO_WITH_INDEX_CASE(2);
            __GOLD_MACRO_WITH_INDEX_CASE(1);
            __GOLD_MACRO_WITH_INDEX_DEFAULT_CASE();
        );

        __GOLD_MACRO_MAKE_WITH_INDEX(11,
            __GOLD_MACRO_WITH_INDEX_CASE(10);
            __GOLD_MACRO_WITH_INDEX_CASE(9);
            __GOLD_MACRO_WITH_INDEX_CASE(8);
            __GOLD_MACRO_WITH_INDEX_CASE(7);
            __GOLD_MACRO_WITH_INDEX_CASE(6);
            __GOLD_MACRO_WITH_INDEX_CASE(5);
            __GOLD_MACRO_WITH_INDEX_CASE(4);
            __GOLD_MACRO_WITH_INDEX_CASE(3);
            __GOLD_MACRO_WITH_INDEX_CASE(2);
            __GOLD_MACRO_WITH_INDEX_CASE(1);
            __GOLD_MACRO_WITH_INDEX_DEFAULT_CASE();
        );

        __GOLD_MACRO_MAKE_WITH_INDEX(10,
            __GOLD_MACRO_WITH_INDEX_CASE(9);
            __GOLD_MACRO_WITH_INDEX_CASE(8);
            __GOLD_MACRO_WITH_INDEX_CASE(7);
            __GOLD_MACRO_WITH_INDEX_CASE(6);
            __GOLD_MACRO_WITH_INDEX_CASE(5);
            __GOLD_MACRO_WITH_INDEX_CASE(4);
            __GOLD_MACRO_WITH_INDEX_CASE(3);
            __GOLD_MACRO_WITH_INDEX_CASE(2);
            __GOLD_MACRO_WITH_INDEX_CASE(1);
            __GOLD_MACRO_WITH_INDEX_DEFAULT_CASE();
        );

        __GOLD_MACRO_MAKE_WITH_INDEX(9,
            __GOLD_MACRO_WITH_INDEX_CASE(8);
            __GOLD_MACRO_WITH_INDEX_CASE(7);
            __GOLD_MACRO_WITH_INDEX_CASE(6);
            __GOLD_MACRO_WITH_INDEX_CASE(5);
            __GOLD_MACRO_WITH_INDEX_CASE(4);
            __GOLD_MACRO_WITH_INDEX_CASE(3);
            __GOLD_MACRO_WITH_INDEX_CASE(2);
            __GOLD_MACRO_WITH_INDEX_CASE(1);
            __GOLD_MACRO_WITH_INDEX_DEFAULT_CASE();
        );

        __GOLD_MACRO_MAKE_WITH_INDEX(8,
            __GOLD_MACRO_WITH_INDEX_CASE(7);
            __GOLD_MACRO_WITH_INDEX_CASE(6);
            __GOLD_MACRO_WITH_INDEX_CASE(5);
            __GOLD_MACRO_WITH_INDEX_CASE(4);
            __GOLD_MACRO_WITH_INDEX_CASE(3);
            __GOLD_MACRO_WITH_INDEX_CASE(2);
            __GOLD_MACRO_WITH_INDEX_CASE(1);
            __GOLD_MACRO_WITH_INDEX_DEFAULT_CASE();
        );

        __GOLD_MACRO_MAKE_WITH_INDEX(7,
            __GOLD_MACRO_WITH_INDEX_CASE(6);
            __GOLD_MACRO_WITH_INDEX_CASE(5);
            __GOLD_MACRO_WITH_INDEX_CASE(4);
            __GOLD_MACRO_WITH_INDEX_CASE(3);
            __GOLD_MACRO_WITH_INDEX_CASE(2);
            __GOLD_MACRO_WITH_INDEX_CASE(1);
            __GOLD_MACRO_WITH_INDEX_DEFAULT_CASE();
        );

        __GOLD_MACRO_MAKE_WITH_INDEX(6,
            __GOLD_MACRO_WITH_INDEX_CASE(5);
            __GOLD_MACRO_WITH_INDEX_CASE(4);
            __GOLD_MACRO_WITH_INDEX_CASE(3);
            __GOLD_MACRO_WITH_INDEX_CASE(2);
            __GOLD_MACRO_WITH_INDEX_CASE(1);
            __GOLD_MACRO_WITH_INDEX_DEFAULT_CASE();
        );

        __GOLD_MACRO_MAKE_WITH_INDEX(5,
            __GOLD_MACRO_WITH_INDEX_CASE(4);
            __GOLD_MACRO_WITH_INDEX_CASE(3);
            __GOLD_MACRO_WITH_INDEX_CASE(2);
            __GOLD_MACRO_WITH_INDEX_CASE(1);
            __GOLD_MACRO_WITH_INDEX_DEFAULT_CASE();
        );

        __GOLD_MACRO_MAKE_WITH_INDEX(4,
            __GOLD_MACRO_WITH_INDEX_CASE(3);
            __GOLD_MACRO_WITH_INDEX_CASE(2);
            __GOLD_MACRO_WITH_INDEX_CASE(1);
            __GOLD_MACRO_WITH_INDEX_DEFAULT_CASE();
        );

        __GOLD_MACRO_MAKE_WITH_INDEX(3,
            __GOLD_MACRO_WITH_INDEX_CASE(2);
            __GOLD_MACRO_WITH_INDEX_CASE(1);
            __GOLD_MACRO_WITH_INDEX_DEFAULT_CASE();
        );

        __GOLD_MACRO_MAKE_WITH_INDEX(2,
            __GOLD_MACRO_WITH_INDEX_CASE(1);
            __GOLD_MACRO_WITH_INDEX_DEFAULT_CASE();
        );

        __GOLD_MACRO_MAKE_WITH_INDEX(1,
            __GOLD_MACRO_WITH_INDEX_DEFAULT_CASE();
        );

        #undef __GOLD_MACRO_MAKE_WITH_INDEX
        #undef __GOLD_MACRO_WITH_INDEX_CASE

    } // namespace __with_index

    /// __algo::visit_with_index
    template <std::size_t I, typename F>
        requires (I > 0)
    constexpr auto visit_with_index(std::size_t i, F&& f) {
        if consteval { gold::assume(i < I); }
        return __with_index::with_index_impl<I>(i, std::forward<F>(f));
    }

} // namespace gold::__algo

#endif // __GOLD_BITS_ALGO_WITH_INDEX_HPP
