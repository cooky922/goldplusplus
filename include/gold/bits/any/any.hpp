// <gold/bits/any/any.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ANY_ANY_HPP
#define __GOLD_BITS_ANY_ANY_HPP

#include <gold/bits/any/fwd.hpp>
#include <gold/bits/any/bad_any_access.hpp>
#include <gold/bits/any/storage.hpp>
#include <gold/bits/any/manager.hpp>
#include <gold/bits/any/view_any.hpp>

namespace gold {

    //// [gold.any.any]
    class any {
      private:
        union {
            // heap allocated (for both compile-time and runtime with suitable large-size types)
            __any::base_storage* m_ptr_;
            // preallocated (only for runtime with small-sized types)
            __any::sbo_buffer    m_buf_;
        };

        auto (* m_manager_) (__any::manager_action, const any*, any*) -> __any::manager_result = nullptr;

        template <typename, typename>
        friend struct __any::manager;

        template <typename T>
        using manager_type = __any::manager<T, any>;

        constexpr __any::manager_result mf_manage_(__any::manager_action act, any* other = nullptr) {
            return m_manager_(act, this, other);
        }

        constexpr __any::manager_result mf_manage_(__any::manager_action act, any* other = nullptr) const {
            return m_manager_(act, this, other);
        }

        template <typename T>
        static constexpr T* s_cast_to_ptr_(any* op) noexcept {
            const bool not_null = op && op->has_value() && holds_current_type<T>(*op) && !std::is_function_v<T>;
            if (not_null) {
                return std::addressof(manager_type<T>::s_unsafe_get_(*op));
            }
            return nullptr;
        }

        template <typename T>
        static constexpr const T* s_cast_to_ptr_(const any* op) noexcept {
            return s_cast_to_ptr_<T>(const_cast<any*>(op));
        }

      public:
        /// default ctor
        constexpr any() noexcept
        : m_manager_(nullptr) {
            if consteval { m_ptr_ = nullptr; }
        }

        /// copy ctor
        constexpr any(const any& other) {
            if (other.has_value())
                other.mf_manage_(__any::manager_action::do_copy, this);
        }

        /// move ctor
        constexpr any(any&& other) {
            if (other.has_value())
                other.mf_manage_(__any::manager_action::do_move, this);
        }

        /// in-placed ctor
        template <typename T, typename... Args>
            requires requires (T op) {
                { gold::decay_copy(op) } -> std::constructible_from<Args...>;
                { gold::decay_copy(op) } -> std::copy_constructible;
            }
        constexpr any(std::in_place_type_t<T>, Args&&... args) {
            manager_type<std::decay_t<T>>::s_create_(*this, std::in_place, std::forward<Args>(args)...);
        }

        template <typename T, typename U, typename... Args>
            requires requires (T op) {
                { gold::decay_copy(op) } -> std::constructible_from<std::initializer_list<U>, Args...>;
                { gold::decay_copy(op) } -> std::copy_constructible;
            }
        constexpr any(std::in_place_type_t<T>, std::initializer_list<U> il, Args&&... args) {
            manager_type<std::decay_t<T>>::s_create_(*this, std::in_place, il, std::forward<Args>(args)...);
        }

        /// direct ctor
        template <typename T>
            requires (!std::same_as<std::decay_t<T>, any> && requires (T&& op) {
                { gold::decay_copy(std::forward<T>(op)) } -> std::copy_constructible;
            })
        constexpr any(T&& op) {
            manager_type<std::decay_t<T>>::s_create_(*this, std::in_place, std::forward<T>(op));
        }

        /// dtor
        constexpr ~any() { this->reset(); }

        /// copy assign
        constexpr any& operator=(const any& other) {
            if (this != &other)
                auto(other).swap(*this);
            return *this;
        }

        /// move assign
        constexpr any& operator=(any&& other) {
            if (this != &other)
                auto(std::move(other)).swap(*this);
            return *this;
        }

        /// one-arg assign
        template <typename T>
            requires (!std::same_as<std::decay_t<T>, any> && requires (T&& op) {
                { gold::decay_copy(std::forward<T>(op)) } -> std::copy_constructible;
            })
        constexpr any& operator=(T&& op) {
            if constexpr (std::copyable<std::decay_t<T>>) {
                if (holds_current_type<std::decay_t<T>>(*this) && this->has_value()) {
                    *s_cast_to_ptr_<std::decay_t<T>>(this) = std::forward<T>(op);
                    return *this;
                }
            }
            any(std::forward<T>(op)).swap(*this);
            return *this;
        }

        /// swap
        constexpr void swap(any& other) noexcept {
            if (this == &other)
                return;
            if (this->has_value() && other.has_value())
                this->mf_manage_(__any::manager_action::do_swap, &other);
            else if (this->has_value())
                this->mf_manage_(__any::manager_action::do_move, &other);
            else if (other.has_value())
                other.mf_manage_(__any::manager_action::do_move, this);
        }

        friend constexpr void swap(any& lhs, any& rhs) noexcept {
            lhs.swap(rhs);
        }

        /// reset
        constexpr void reset() noexcept {
            if (this->has_value())
                this->mf_manage_(__any::manager_action::do_destroy);
        }

        /// emplace
        template <typename T, typename... Args>
            requires requires (T op) {
                { gold::decay_copy(op) } -> std::constructible_from<Args...>;
                { gold::decay_copy(op) } -> std::copy_constructible;
            }
        constexpr T& emplace(Args&&... args) {
            this->reset();
            return manager_type<std::decay_t<T>>::s_create_(*this, std::in_place, std::forward<Args>(args)...);
        }

        template <typename T, typename U, typename... Args>
            requires requires (T op) {
                { gold::decay_copy(op) } -> std::constructible_from<std::initializer_list<U>, Args...>;
                { gold::decay_copy(op) } -> std::copy_constructible;
            }
        constexpr T& emplace(std::initializer_list<U> il, Args&&... args) {
            this->reset();
            return manager_type<std::decay_t<T>>::s_create_(*this, std::in_place, il, std::forward<Args>(args)...);
        }

        /// has_value
        constexpr bool has_value() const noexcept { return m_manager_ != nullptr; }

        /// view
        constexpr view_any view() const& noexcept {
            if (this->has_value())
                return view_any(this->mf_manage_(__any::manager_action::do_get_view).view);
            else
                return view_any();
        }

        constexpr void view() && = delete;

        /// type_info
        constexpr gold::ctype_info type_info() const noexcept {
            if (this->has_value())
                return this->mf_manage_(__any::manager_action::do_get_tinfo).tinfo;
            else
                return gold::ctype_id<void>();
        }

        /// type_name
        constexpr std::string_view type_name() const noexcept {
            if (this->has_value())
                return this->mf_manage_(__any::manager_action::do_get_tname).tname;
            else
                return "void";
        }

        /// type_size
        constexpr std::size_t type_size() const noexcept {
            return this->has_value() ? this->mf_manage_(__any::manager_action::do_get_tsize).tsize : 0;
        }

        /// type_align
        constexpr std::size_t type_align() const noexcept {
            return this->has_value() ? this->mf_manage_(__any::manager_action::do_get_talign).tsize : 0;
        }

        /// operator==
        constexpr bool operator==(const any& other) const {
            if (!this->has_value() && !other.has_value())
                return true;
            else if (!this->has_value() || !other.has_value())
                return false;
            else {
                auto result = this->mf_manage_(__any::manager_action::do_eq, &as_mutable(other));
                if (result.kind == __any::manager_result_kind::none) {
                    if consteval { __bad_any_access::unimplemented_equal_operator(); }
                    else { throw bad_any_access("no 'operator==' found in current type"); }
                }
                return result.eq;
            }
        }

        /// operator<=>
        constexpr auto operator<=>(const any& other) const {
            using return_type = std::partial_ordering;
            if (!this->has_value() && !other.has_value())
                return return_type::equivalent;
            else if (!this->has_value() || !other.has_value())
                return return_type::unordered;
            else {
                auto result = this->mf_manage_(__any::manager_action::do_cmp, &as_mutable(other));
                if (result.kind == __any::manager_result_kind::none) {
                    if consteval { __bad_any_access::unimplemented_spaceship_operator(); }
                    else { throw bad_any_access("no 'operator<=>' found in current type"); }
                }
                return result.cmp;
            }
        }

        /// friend holds_current_type
        template <typename T>
        friend constexpr bool holds_current_type(const any& op) noexcept {
            return op.type_info() == gold::ctype_id<T>();
        }

        /// friend any_cast
        template <typename T>
            requires (!std::is_reference_v<T>)
        friend constexpr const T* any_cast(const any* op) noexcept {
            return s_cast_to_ptr_<T>(op);
        }

        template <typename T>
            requires (!std::is_reference_v<T>)
        friend constexpr T* any_cast(any* op) noexcept {
            return s_cast_to_ptr_<T>(op);
        }

    };

    /// make_any
    template <typename T, typename... Args>
        requires requires (T op) {
            { gold::decay_copy(op) } -> std::constructible_from<Args...>;
            { gold::decay_copy(op) } -> std::copy_constructible;
        }
    constexpr any make_any(Args&&... args) {
        return any(std::in_place_type<T>, std::forward<Args>(args)...);
    }

    template <typename T, typename U, typename... Args>
        requires requires (T op) {
            { gold::decay_copy(op) } -> std::constructible_from<std::initializer_list<U>, Args...>;
            { gold::decay_copy(op) } -> std::copy_constructible;
        }
    constexpr any make_any(std::initializer_list<U> il, Args&&... args) {
        return any(std::in_place_type<T>, il, std::forward<Args>(args)...);
    }

} // namespace gold

#endif // __GOLD_BITS_ANY_ANY_HPP
