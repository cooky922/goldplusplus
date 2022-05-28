// <gold/bits/any/view_any.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ANY_VIEW_ANY_HPP
#define __GOLD_BITS_ANY_VIEW_ANY_HPP

#include <gold/bits/any/fwd.hpp>
#include <gold/bits/any/bad_any_access.hpp>
#include <gold/bits/any/storage.hpp>
#include <gold/bits/any/manager.hpp>

namespace gold {

    //// [gold.any.view_any]
    /// view_any
    class view_any {
      private:
        union {
            __any::base_storage* m_cptr_;
            void*                m_rptr_;
        };
        auto (* m_manager_) (__any::manager_action, const view_any*, view_any*) -> __any::manager_result = nullptr;

        template <typename, typename>
        friend struct __any::manager;

        template <typename T>
        using manager_type = __any::manager<T, view_any>;

        constexpr __any::manager_result mf_manage_(__any::manager_action act, view_any* other = nullptr) {
            return m_manager_(act, this, other);
        }

        constexpr __any::manager_result mf_manage_(__any::manager_action act, view_any* other = nullptr) const {
            return m_manager_(act, this, other);
        }

        template <typename T>
        static constexpr T* s_cast_to_ptr_(view_any* op) noexcept {
            const bool not_null = op && op->has_value() && holds_current_type<T>(*op) && !std::is_function_v<T>;
            if (not_null) {
                if consteval {
                    auto result = op->mf_manage_(__any::manager_action::do_cget).cget;
                    return static_cast<__any::derived_view_storage<T>*>(result)->ptr;
                } else {
                    auto result = op->mf_manage_(__any::manager_action::do_rget).rget;
                    return static_cast<T*>(result);
                }
            }
            return nullptr;
        }

        template <typename T>
        static constexpr const T* s_cast_to_ptr_(const view_any* op) noexcept {
            return s_cast_to_ptr_<T>(const_cast<view_any*>(op));
        }

        /// private ctor
        constexpr view_any(__any::mini_view_any other)
        : m_manager_(other.m_manager_) {
            if consteval {
                m_cptr_ = other.m_cptr_;
            } else {
                m_rptr_ = other.m_rptr_;
            }
        }

        friend any;
        friend unique_any;

      public:

        /// default ctor
        constexpr view_any() noexcept
        : m_manager_(nullptr) {
            if consteval { m_cptr_ = nullptr; }
            else { m_rptr_ = nullptr; }
        }

        /// copy ctor
        constexpr view_any(const view_any& other) {
            if (other.has_value())
                other.mf_manage_(__any::manager_action::do_copy, this);
        }

        /// move ctor
        constexpr view_any(view_any&& other) {
            if (other.has_value())
                other.mf_manage_(__any::manager_action::do_move, this);
        }

        /// custom ctor
        template <typename T>
            requires (!std::is_same_v<std::remove_cvref_t<T>, view_any> && !std::is_function_v<std::remove_cvref_t<T>>)
        constexpr view_any(T& op) {
                manager_type<std::remove_cvref_t<T>>::s_create_(*this, const_cast<std::remove_cvref_t<T>&>(op));
        }

        template <typename T>
            requires (!std::is_same_v<T, view_any>)
        constexpr view_any(const T&&) = delete;

        /// dtor
        constexpr ~view_any() { this->reset(); }

        /// copy assign
        constexpr view_any& operator=(const view_any& other) {
            if (this != &other)
                auto(other).swap(*this);
            return *this;
        }

        /// move assign
        constexpr view_any& operator=(view_any&& other) {
            if (this != &other)
                auto(std::move(other)).swap(*this);
            return *this;
        }

        /// one-arg assign
        template <typename T>
            requires (!std::is_same_v<std::remove_cvref_t<T>, view_any> && !std::is_function_v<std::remove_cvref_t<T>>)
        constexpr view_any& operator=(T& op) {
            view_any(op).swap(*this);
            return *this;
        }

        /// swap
        constexpr void swap(view_any& other) noexcept {
            if (this == &other)
                return;
            if (this->has_value() && other.has_value())
                this->mf_manage_(__any::manager_action::do_swap, &other);
            else if (this->has_value())
                this->mf_manage_(__any::manager_action::do_move, &other);
            else
                other.mf_manage_(__any::manager_action::do_move, this);
        }

        friend constexpr void swap(view_any& lhs, view_any& rhs) noexcept {
            lhs.swap(rhs);
        }

        /// reset
        constexpr void reset() noexcept {
            if (this->has_value())
                this->mf_manage_(__any::manager_action::do_destroy);
        }

        /// emplace
        template <typename T>
        constexpr T& emplace(T& op) noexcept {
            view_any(op).swap(*this);
            return op;
        }

        /// has_value
        constexpr bool has_value() const noexcept { return m_manager_ != nullptr; }

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
        constexpr bool operator==(const view_any& other) const {
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

        template <typename T>
        constexpr bool operator==(const T& op) const {
            gold::view_any other = op;
            return this->operator==(other);
        }

        /// operator<=>
        constexpr auto operator<=>(const view_any& other) const {
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

        template <typename T>
        constexpr auto operator<=>(const T& op) const {
            gold::view_any other = op;
            return this->operator<=>(op);
        }

        /// friend holds_current_type
        template <typename T>
        friend constexpr bool holds_current_type(const view_any& op) noexcept {
            return op.type_info() == gold::ctype_id<T>();
        }

        /// friend any_cast
        template <typename T>
            requires (!std::is_reference_v<T>)
        friend constexpr const T* any_cast(const view_any* op) noexcept {
            return s_cast_to_ptr_<T>(op);
        }

        template <typename T>
            requires (!std::is_reference_v<T>)
        friend constexpr T* any_cast(view_any* op) noexcept {
            return s_cast_to_ptr_<T>(op);
        }

        template <typename T>
            requires (!std::is_reference_v<T>)
        friend constexpr decltype(auto) any_cast(view_any op) { // pass by value, since it's "cheap"
            auto* result = s_cast_to_ptr_<T>(&op);
            if (result == nullptr) {
                if consteval { __bad_any_access::bad_cast(); }
                else { throw bad_any_access("bad any cast"); }
            }
            return *result;
        }
    };

} // namespace gold

#endif // __GOLD_BITS_ANY_VIEW_ANY_HPP
