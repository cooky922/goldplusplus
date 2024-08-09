// <gold/bits/concepts/allocator.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_CONCEPTS_ALLOCATOR_HPP
#define __GOLD_BITS_CONCEPTS_ALLOCATOR_HPP

#include <concepts>
#include <bits/alloc_traits.h>

namespace gold {

    /// allocator_of
    template <typename Alloc, typename T>
    concept allocator_of =
        std::copy_constructible<Alloc>  &&
        std::equality_comparable<Alloc> &&
        requires (Alloc& alloc, T* ptr, std::size_t n) {
            requires std::same_as<typename Alloc::value_type, T>;
            typename std::allocator_traits<Alloc>;
            typename std::allocator_traits<Alloc>::pointer;

            { alloc.allocate(n) } -> std::same_as<typename std::allocator_traits<Alloc>::pointer>;
            { alloc.deallocate(ptr, n) };
        };

    /// stateless_allocator
    template <typename Alloc>
    concept stateless_allocator =
        std::default_initializable<Alloc> &&
        std::allocator_traits<Alloc>::is_always_equal::value;

} // namespace gold

#endif // __GOLD_BITS_CONCEPTS_ALLOCATOR_HPP
