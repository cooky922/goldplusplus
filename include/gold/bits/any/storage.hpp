// <gold/bits/any/storage.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ANY_STORAGE_HPP
#define __GOLD_BITS_ANY_STORAGE_HPP

#include <gold/bits/any/fwd.hpp>
#include <gold/bits/concepts/types.hpp> // gold::brace_constructible_from
#include <gold/bits/in_place.hpp>       // std::in_place[_t], gold::in_place_braced[_t]
#include <concepts>

namespace gold::__any {

    //// [gold.any.storage]
    /// __any::base_storage
    struct base_storage {};

    /// __any::derived_storage
    template <typename T>
    struct derived_storage : base_storage {
        [[no_unique_address]] T value;

        template <typename... Args>
            requires std::constructible_from<T, Args...>
        constexpr explicit derived_storage(std::in_place_t, Args&&... args)
        : value(std::forward<Args>(args)...) {}

        template <typename... Args>
            requires gold::brace_constructible_from<T, Args...>
        constexpr explicit derived_storage(gold::in_place_braced_t, Args&&... args)
        : value{std::forward<Args>(args)...} {}

        constexpr derived_storage() noexcept requires std::default_initializable<T> = default;
        constexpr derived_storage(const derived_storage&) requires std::copy_constructible<T> = default;
        constexpr derived_storage(derived_storage&&) noexcept requires std::move_constructible<T> = default;
        constexpr ~derived_storage() noexcept requires std::destructible<T> = default;
        constexpr derived_storage& operator=(const derived_storage&) requires std::copyable<T> = default;
        constexpr derived_storage& operator=(derived_storage&&) noexcept requires std::movable<T> = default;
    };

    /// __any::sbo_buffer
    struct alignas(void*) sbo_buffer {
        /* alignas(void*) */ std::byte data[3 * sizeof(void*)];
    } __attribute__((packed));

    /// __any::sbo_compatible
    template <typename T>
    concept sbo_compatible = sizeof(T) <= sizeof(sbo_buffer)
        && alignof(sbo_buffer) % alignof(T) == 0
        && std::is_nothrow_move_constructible_v<T>;

} // namespace gold::__any

#endif // __GOLD_BITS_ANY_STORAGE_HPP
