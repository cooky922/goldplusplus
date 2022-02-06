// <gold/bits/any/any_impl.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ANY_IMPL_HPP
#define __GOLD_BITS_ANY_IMPL_HPP

#include <gold/bits/any/bad_any_access.hpp>
#include <gold/bits/any/store_any.hpp>

namespace gold {

    /// holds_current_type [fwd decl]
    template <typename T>
    constexpr bool holds_current_type(const any&) noexcept;

    /// any
    class any {
      private:
        /// ::action_any
        using action_any = __any::action_any;

        /// ::handle_func_ptr
        using handle_func_ptr = auto (*) (action_any, const any*, any*) -> gold::__any::handler_func_ret_type;

        /// ::data_storage
        using data_storage = __any::any_data_storage;

        /// ::any_handler
        template <typename T>
        using handler = __any::any_handler<T, true>;

        /// friend declaration of __any::basic_any_handler
        template <bool, typename, bool>
        friend struct __any::basic_any_handler;

        /// private data members
        // handling 'm_data_'
        handle_func_ptr         m_handle_ { nullptr };

        // storage for memory
        data_storage            m_data_;

        /// s_cast_to_ptr_
        template <typename T>
            requires (!std::is_reference_v<T>)
        static constexpr T* s_cast_to_ptr_(any* op) noexcept {
            const bool not_null_ = op && op->has_value() && holds_current_type<T>(*op) && !std::is_function_v<T>;
            if (not_null_) {
                auto result = std::get<__any::base_any*>(op->mf_call_(__any::action_any::do_get));
                return std::addressof(static_cast<__any::derived_any<T>*>(result)->value);
            }
            return nullptr;
        }

        template <typename T>
            requires (!std::is_reference_v<T>)
        static constexpr const T* s_cast_to_ptr_(const any* op) noexcept {
            return s_cast_to_ptr_<T>(const_cast<any*>(op));
        }

        /// mf_call_
        constexpr __any::handler_func_ret_type mf_call_(action_any act, any* other = nullptr) const {
            return m_handle_(act, this, other);
        }

        constexpr __any::handler_func_ret_type mf_call_(action_any act, any* other = nullptr) {
            return m_handle_(act, this, other);
        }

      public:
        /// default ctor
        constexpr any() noexcept = default;

        /// copy ctor
        constexpr any(const any& other) {
            if (other.has_value())
                other.mf_call_(action_any::do_copy, this);
        }

        /// move ctor
        constexpr any(any&& other) {
            if (other.has_value())
                other.mf_call_(action_any::do_move, this);
        }

        /// in-placed ctor
        template <typename T, typename... Args>
            requires requires (T op) {
                { gold::decay_copy(op) } -> std::constructible_from<Args...>;
                { gold::decay_copy(op) } -> std::copy_constructible;
            }
        constexpr any(std::in_place_type_t<T>, Args&&... args) {
            handler<std::decay_t<T>>::create(*this, std::in_place, std::forward<Args>(args)...);
        }

        template <typename T, typename U, typename... Args>
            requires requires (T op) {
                { gold::decay_copy(op) } -> std::constructible_from<std::initializer_list<U>, Args...>;
                { gold::decay_copy(op) } -> std::copy_constructible;
            }
        constexpr any(std::in_place_type_t<T>, std::initializer_list<U> il, Args&&... args) {
            handler<std::decay_t<T>>::create(*this, std::in_place, il, std::forward<Args>(args)...);
        }

        /// direct ctor
        template <typename T>
            requires (!std::same_as<std::decay_t<T>, any> && requires (T&& op) {
                { gold::decay_copy(std::forward<T>(op)) } -> std::copy_constructible;
            })
        constexpr any(T&& op) {
            handler<std::decay_t<T>>::create(*this, std::in_place, std::forward<T>(op));
        }

        /// dtor
        constexpr ~any() { this->reset(); }

        /// copy assign
        constexpr any& operator=(const any& other) {
            gold::decay_copy(other).swap(*this);
            return *this;
        }

        /// move assign
        constexpr any& operator=(any&& other) {
            gold::decay_move(other).swap(*this);
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
            if (this == std::addressof(other))
                return;
            if (has_value() && other.has_value())
                this->mf_call_(action_any::do_swap, &other);
            else if (has_value())
                this->mf_call_(action_any::do_move, &other);
            else if (other.has_value())
                other.mf_call_(action_any::do_move, this);
        }

        /// reset
        constexpr void reset() noexcept { if (has_value()) this->mf_call_(action_any::do_destroy); }

        /// emplace
        template <typename T, typename... Args>
            requires requires (T op) {
                { gold::decay_copy(op) } -> std::constructible_from<Args...>;
                { gold::decay_copy(op) } -> std::copy_constructible;
            }
        constexpr T& emplace(Args&&... args) {
            this->reset();
            auto& result = handler<std::decay_t<T>>::create(*this, std::in_place, std::forward<Args>(args)...);
            return result.value;
        }

        template <typename T, typename U, typename... Args>
            requires requires (T op) {
                { gold::decay_copy(op) } -> std::constructible_from<std::initializer_list<U>, Args...>;
                { gold::decay_copy(op) } -> std::copy_constructible;
            }
        constexpr T& emplace(std::initializer_list<U> il, Args&&... args) {
            this->reset();
            auto& result = handler<std::decay_t<T>>::create(*this, std::in_place, il, std::forward<Args>(args)...);
            return result.value;
        }

        /// has_value
        constexpr bool has_value() const noexcept { return m_handle_ != nullptr; }

        /// type_info
        constexpr const std::type_info& type_info() const noexcept {
            if (has_value())
                return *std::get<const std::type_info*>(this->mf_call_(action_any::do_get_typeinfo));
            else
                return typeid(void);
        }

        /// type_name
        constexpr std::string_view type_name() const noexcept {
            if (has_value())
                return std::get<std::string_view>(this->mf_call_(action_any::do_get_typename));
            else
                return "void";
        }

        /// type_size
        constexpr std::size_t type_size() const noexcept {
            return has_value() ? std::get<std::size_t>(this->mf_call_(action_any::do_get_sizeof)) : 0;
        }

        /// type_alignment
        constexpr std::size_t type_alignment() const noexcept {
            return has_value() ? std::get<std::size_t>(this->mf_call_(action_any::do_get_alignof)) : 0;
        }

        /// operator==
        constexpr bool operator==(const any& other) const {
            if (!this->has_value() && !other.has_value()) {
                return true;
            } else if (!this->has_value() || !other.has_value()) {
                return false;
            } else {
                auto result = this->mf_call_(action_any::do_compare, const_cast<any*>(&other));
                if (result.index() == 0) // monostate, if not equality comparable
                    throw bad_any_access(bad_any_access_kind::no_equality_operator);
                return std::get<bool>(result);
            }
        }

        /// operator<=>
        constexpr auto operator<=>(const any& other) const {
            if (!this->has_value() && !other.has_value()) {
                return std::partial_ordering::equivalent;
            } else if (!this->has_value() || !other.has_value()) {
                return std::partial_ordering::unordered;
            } else {
                auto result = this->mf_call_(action_any::do_compare_3way, const_cast<any*>(&other));
                if (result.index() == 0) // monostate, if not 3-way comparable
                    throw bad_any_access(bad_any_access_kind::no_spaceship_operator);
                return std::get<std::partial_ordering>(result);
            }
        }

        /// friend declaration for any_cast
        template <typename T>
            requires (!std::is_reference_v<T>)
        friend constexpr const T* any_cast(const any*) noexcept;

        template <typename T>
            requires (!std::is_reference_v<T>)
        friend constexpr T* any_cast(any*) noexcept;

    };

    /// swap
    constexpr void swap(any& lhs, any& rhs) noexcept { lhs.swap(rhs); }

    /// holds_current_type
    template <typename T>
    constexpr bool holds_current_type(const any& op) noexcept {
        return __any::compare_typeid<T>(op.type_info(), op.type_name());
    }

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

#endif // __GOLD_BITS_ANY_IMPL_HPP
