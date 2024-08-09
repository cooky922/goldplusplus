// <gold/bits/type_traits/conditional.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TYPE_TRAITS_CONDITIONAL_HPP
#define __GOLD_BITS_TYPE_TRAITS_CONDITIONAL_HPP

namespace gold {

    namespace __type_trait {

        /// __type_trait::conditional
        template <bool>
        struct conditional {
            template <typename T, typename>
            using type = T;
        };

        template <>
        struct conditional<false> {
            template <typename, typename F>
            using type = F;
        };

    } // namespace __type_trait

    /// conditional_t
    template <bool Cond, typename TrueType, typename FalseType>
    using conditional_t = __type_trait::conditional<Cond>::template type<TrueType, FalseType>;

    /// conditional
    template <bool Cond, typename TrueType, typename FalseType>
    struct conditional {
        using type = conditional_t<Cond, TrueType, FalseType>;
    };

} // namespace gold

#endif // __GOLD_BITS_TYPE_TRAITS_CONDITIONAL_HPP
