// <gold/bits/type_traits/base_traits.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TYPE_TRAITS_BASE_TRAITS_HPP
#define __GOLD_BITS_TYPE_TRAITS_BASE_TRAITS_HPP

#include <gold/bits/sequence/type_seq.hpp>

namespace gold {

    /// bases
    template <typename T>
    struct bases {
        using type = type_seq_t<__bases(T)...>;
    };

    /// bases_t
    template <typename T>
    using bases_t = bases<T>::type;

    /// direct_bases
    template <typename T>
    struct direct_bases {
        using type = type_seq_t<__direct_bases(T)...>;
    };

    /// direct_bases_t
    template <typename T>
    using direct_bases_t = direct_bases<T>::type;

    /// has_bases_v
    template <typename T>
    inline constexpr bool has_bases_v = !bases_t<T>::template empty<>;

    /// has_bases
    template <typename T>
    struct has_bases : std::bool_constant<has_bases_v<T>> {};

    /// is_direct_base_of_v
    template <typename Base, typename Derived>
    inline constexpr bool is_direct_base_of_v =
        std::is_base_of_v<Base, Derived> &&
        direct_bases_t<Derived>::template contains_single<Base>;

    /// is_direct_base_of
    template <typename Base, typename Derived>
    struct is_direct_base_of : std::bool_constant<is_direct_base_of_v<Base, Derived>> {};

    /// is_indirect_base_of_v
    template <typename Base, typename Derived>
    inline constexpr bool is_indirect_base_of_v =
        std::is_base_of_v<Base, Derived> &&
        !is_direct_base_of_v<Base, Derived>;

    /// is_indirect_base_of
    template <typename Base, typename Derived>
    struct is_indirect_base_of : std::bool_constant<is_indirect_base_of_v<Base, Derived>> {};

    /// base_at
    template <typename T, std::size_t I>
    struct base_at {
        using type = __type_pack_element<I, __bases(T)...>;
    };

    /// base_at_t
    template <typename T, std::size_t I>
    using base_at_t = base_at<T, I>::type;

    /// base_count
    template <typename T>
    struct base_count {
        static constexpr std::size_t value = gold::pack_size<__bases(T)...>;
    };

    /// base_count_v
    template <typename T>
    inline constexpr std::size_t base_count_v = base_count<T>::value;

    /// direct_base_at
    template <typename T, std::size_t I>
    struct direct_base_at {
        using type = __type_pack_element<I, __direct_bases(T)...>;
    };

    /// direct_base_at_t
    template <typename T, std::size_t I>
    using direct_base_at_t = direct_base_at<T, I>::type;

    /// direct_base_count
    template <typename T>
    struct direct_base_count {
        static constexpr std::size_t value = gold::pack_size<__direct_bases(T)...>;
    };

    /// direct_base_count_v
    template <typename T>
    inline constexpr std::size_t direct_base_count_v = direct_base_count<T>::value;

} // namespace gold

#endif // __GOLD_BITS_TYPE_TRAITS_BASE_TRAITS_HPP
