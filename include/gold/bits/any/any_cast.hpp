// <gold/bits/any/any_cast.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ANY_CAST_HPP
#define __GOLD_BITS_ANY_CAST_HPP

#include <optional>
#include <gold/bits/any/any_impl.hpp>
#include <gold/bits/any/unique_any_impl.hpp>
#include <gold/bits/any/bad_any_access.hpp>

namespace gold {

    /// any_cast (pointer)
    template <typename T>
        requires (!std::is_reference_v<T>)
    constexpr T* any_cast(any* op) noexcept {
        return any::s_cast_to_ptr_<T>(op);
    }

    template <typename T>
        requires (!std::is_reference_v<T>)
    constexpr const T* any_cast(const any* op) noexcept {
        return any::s_cast_to_ptr_<T>(op);
    }

    /// any_cast (value-cat)
    template <typename T>
        requires std::constructible_from<T, std::remove_cvref_t<T>&>
    constexpr T any_cast(any& op) {
        using raw_value_type = std::remove_cvref_t<T>;
        auto temp_ = gold::any_cast<raw_value_type>(std::addressof(op));
        if (temp_ == nullptr)
            throw bad_any_access(bad_any_access_kind::bad_cast);
        return static_cast<T>(*temp_);
    }

    template <typename T>
        requires std::constructible_from<T, const std::remove_cvref_t<T>&>
    constexpr T any_cast(const any& op) {
        using raw_value_type = std::remove_cvref_t<T>;
        auto temp_ = gold::any_cast<const raw_value_type>(std::addressof(op));
        if (temp_ == nullptr)
            throw bad_any_access(bad_any_access_kind::bad_cast);
        return static_cast<T>(*temp_);
    }

    template <typename T>
        requires std::constructible_from<T, std::remove_cvref_t<T>>
    constexpr T any_cast(any&& op) {
        using raw_value_type = std::remove_cvref_t<T>;
        auto temp_ = gold::any_cast<raw_value_type>(std::addressof(op));
        if (temp_ == nullptr)
            throw bad_any_access(bad_any_access_kind::bad_cast);
        return static_cast<T>(std::move(*temp_));
    }

    /// unique_any_cast
    template <typename T>
        requires (!std::is_reference_v<T>)
    constexpr T* unique_any_cast(unique_any* op) noexcept {
        return unique_any::s_cast_to_ptr_<T>(op);
    }

    template <typename T>
        requires (!std::is_reference_v<T>)
    constexpr const T* unique_any_cast(const unique_any* op) noexcept {
        return unique_any::s_cast_to_ptr_<T>(op);
    }

    template <typename T>
        requires std::constructible_from<T, std::remove_cvref_t<T>&>
    constexpr T unique_any_cast(unique_any& op) {
        using raw_value_type = std::remove_cvref_t<T>;
        auto temp_ = gold::unique_any_cast<raw_value_type>(std::addressof(op));
        if (temp_ == nullptr)
            throw bad_any_access(bad_any_access_kind::bad_cast);
        return static_cast<T>(*temp_);
    }

    template <typename T>
    constexpr void unique_any_cast(const unique_any&) = delete;

    template <typename T>
        requires std::constructible_from<T, std::remove_cvref_t<T>>
    constexpr T unique_any_cast(unique_any&& op) {
        using raw_value_type = std::remove_cvref_t<T>;
        auto temp_ = gold::unique_any_cast<raw_value_type>(std::addressof(op));
        if (temp_ == nullptr)
            throw bad_any_access(bad_any_access_kind::bad_cast);
        return static_cast<T>(std::move(*temp_));
    }

    /// maybe_any_cast
    template <typename T>
        requires std::constructible_from<T, std::remove_cvref_t<T>&>
    constexpr std::optional<T> maybe_any_cast(any& op) {
        using raw_value_type = std::remove_cvref_t<T>;
        auto temp_ = gold::any_cast<raw_value_type>(std::addressof(op));
        if (temp_ == nullptr)
            return std::nullopt;
        return static_cast<T>(*temp_);
    }

    template <typename T>
        requires std::constructible_from<T, const std::remove_cvref_t<T>&>
    constexpr std::optional<const T> maybe_any_cast(const any& op) {
        using raw_value_type = std::remove_cvref_t<T>;
        auto temp_ = gold::any_cast<const raw_value_type>(std::addressof(op));
        if (temp_ == nullptr)
            return std::nullopt;
        return static_cast<T>(*temp_);
    }

    template <typename T>
        requires std::constructible_from<T, std::remove_cvref_t<T>>
    constexpr std::optional<T> maybe_any_cast(any&& op) {
        using raw_value_type = std::remove_cvref_t<T>;
        auto temp_ = gold::any_cast<raw_value_type>(std::addressof(op));
        if (temp_ == nullptr)
            return std::nullopt;
        return static_cast<T>(std::move(*temp_));
    }

    /// maybe_unique_any_cast
    template <typename T>
        requires std::constructible_from<T, std::remove_cvref_t<T>&>
    constexpr std::optional<T> maybe_unique_any_cast(unique_any& op) {
        using raw_value_type = std::remove_cvref_t<T>;
        auto temp_ = gold::unique_any_cast<raw_value_type>(std::addressof(op));
        if (temp_ == nullptr)
            return std::nullopt;
        return static_cast<T>(*temp_);
    }

    template <typename T>
    constexpr void maybe_unique_any_cast(const unique_any&) = delete;

    template <typename T>
        requires std::constructible_from<T, std::remove_cvref_t<T>>
    constexpr std::optional<T> maybe_unique_any_cast(unique_any&& op) {
        using raw_value_type = std::remove_cvref_t<T>;
        auto temp_ = gold::unique_any_cast<raw_value_type>(std::addressof(op));
        if (temp_ == nullptr)
            return std::nullopt;
        return static_cast<T>(std::move(*temp_));
    }


} // namespace gold

#endif // __GOLD_BITS_ANY_CAST_HPP
