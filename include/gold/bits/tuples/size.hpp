// <gold/bits/tuples/size.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TUPLES_SIZE_HPP
#define __GOLD_BITS_TUPLES_SIZE_HPP

#include <gold/bits/tuples/tuple_size.hpp>

namespace gold::tuples {

    namespace __cpo_access {

        /// tuples::__cpo_access::size_fn
        struct size_fn {

            template <typename Tuple>
                requires __tuples::viable_tuple_size<std::remove_cvref_t<Tuple>>
            static constexpr std::size_t operator()(Tuple&&) noexcept {
                return __tuples::decayed_tuple_size<Tuple>;
            }

        };

    } // namespace __cpo_access

    inline namespace __cpo {

        /// tuples::size
        // for convenience use
        // instead of 'gold::tuples::tuple_size_v<std::remove_cvref_t<decltype(tup)>>;
        // you can do 'gold::tuples::size(tup)'
        inline constexpr __cpo_access::size_fn size {};

    } // inline namespace __cpo

} // namespace gold::tuples

#endif // __GOLD_BITS_TUPLES_SIZE_HPP
