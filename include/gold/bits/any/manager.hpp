// <gold/bits/any/manager.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ANY_MANAGER_HPP
#define __GOLD_BITS_ANY_MANAGER_HPP

#include <compare>
#include <bits/allocator.h>                 // std::allocator
#include <gold/ctype_info>
#include <gold/bits/any/fwd.hpp>
#include <gold/bits/memory/voidify.hpp>     // gold::voidify
#include <gold/bits/memory/ops.hpp>
#include <gold/utility>

namespace gold::__any {

    /// __any::manager_action
    enum class manager_action {
        do_copy,
        do_move,
        do_swap,
        do_destroy,
        do_eq,
        do_cmp,
        do_cget,
        do_rget,
        do_get_view,
        do_get_tinfo,
        do_get_tname,
        do_get_tsize,
        do_get_talign
    };

    /// __any::manager_result_kind
    enum class manager_result_kind {
        none, eq, cmp, cget, rget, view, tinfo, tname, tsize
    };

    /// __any::mini_view_any
    struct mini_view_any {
        union {
            __any::base_storage* m_cptr_;
            void*                m_rptr_;
        };
        auto (* m_manager_) (__any::manager_action, const view_any*, view_any*) -> __any::manager_result = nullptr;
    };

    /// __any::manager_result
    struct manager_result {
        using none_result  = std::nullptr_t;
        using eq_result    = bool;
        using cmp_result   = std::partial_ordering;
        using cget_result  = base_storage*;
        using rget_result  = void*;
        using view_result  = mini_view_any;
        using tinfo_result = gold::ctype_info;
        using tname_result = std::string_view;
        using tsize_result = std::size_t;

        union {
            none_result  none;
            eq_result    eq;
            cmp_result   cmp;
            cget_result  cget;
            rget_result  rget;
            view_result  view;
            tinfo_result tinfo;
            tname_result tname;
            tsize_result tsize;
        };

        manager_result_kind kind;

        static consteval manager_result get_none() {
            return { .none { nullptr }, .kind = manager_result_kind::none };
        }

        static constexpr manager_result get_eq(eq_result b) {
            manager_result result { {}, manager_result_kind::eq };
            result.eq = b;
            return result;
        }

        static constexpr manager_result get_cmp(cmp_result c) {
            manager_result result { {}, manager_result_kind::cmp };
            result.cmp = c;
            return result;
        }

        static constexpr manager_result get_cget(cget_result g) {
            manager_result result { {}, manager_result_kind::cget };
            result.cget = g;
            return result;
        }

        static constexpr manager_result get_rget(rget_result g) {
            manager_result result { {}, manager_result_kind::rget };
            result.rget = g;
            return result;
        }

        static constexpr manager_result get_view(view_result v) {
            manager_result result { {}, manager_result_kind::view };
            result.view = v;
            return result;
        }

        static constexpr manager_result get_tinfo(tinfo_result t) {
            manager_result result { {}, manager_result_kind::tinfo };
            result.tinfo = t;
            return result;
        }

        static constexpr manager_result get_tname(tname_result t) {
            manager_result result { {}, manager_result_kind::tname };
            result.tname = t;
            return result;
        }

        static constexpr manager_result get_tsize(tsize_result t) {
            manager_result result { {}, manager_result_kind::tsize };
            result.tsize = t;
            return result;
        }
    };

    /// __any::as_lvalue [ TODO: delete ]
    template <typename T>
    constexpr T& as_lvalue(T&& op) noexcept {
        return static_cast<T&>(op);
    }

    /// __any::manager
    template <typename T, typename Any = void>
    struct manager {
        using element_type = T;

        static constexpr gold::ctype_info s_tinfo_() noexcept {
            return gold::ctype_id<T>();
        }

        static constexpr std::string_view s_tname_() noexcept {
            return s_tinfo_().name();
        }

        static constexpr std::size_t s_tsize_() noexcept {
            return sizeof(T);
        }

        static constexpr std::size_t s_talign_() noexcept {
            return alignof(T);
        }
    };

    /// __any::manager<T, view_any>
    template <typename T, typename Any>
        requires std::same_as<Any, view_any>
    struct manager<T, Any> : manager<T> {
        using kind_any             = Any;
        using base                 = manager<T>;
        using typename base::element_type;
        using wrapped_element_type = derived_view_storage<element_type>;

        /// s_tinfo_
        using base::s_tinfo_;

        /// s_tinfo_from_
        static constexpr gold::ctype_info s_tinfo_from_(const kind_any& op) noexcept {
            return op.type_info();
        }

        /// s_tname_
        using base::s_tname_;

        /// s_tname_from_
        static constexpr std::string_view s_tname_from_(const kind_any& op) noexcept {
            return op.type_name();
        }

        /// s_tsize_
        using base::s_tsize_;

        /// s_talign_
        using base::s_talign_;

        /// s_eq_type_
        static constexpr bool s_eq_type_(const kind_any& lhs, const kind_any& rhs) noexcept {
            return lhs.type_info() == rhs.type_info();
        }

        /// s_cget_
        static constexpr base_storage* s_cget_(kind_any& op) {
            if (s_tinfo_from_(op) == s_tinfo_())
                return op.m_cptr_;
            else
                return nullptr;
        }

        /// s_rget_
        static constexpr void* s_rget_(kind_any& op) {
            if (s_tinfo_from_(op) == s_tinfo_())
                return op.m_rptr_;
            else
                return nullptr;
        }

        /// s_unsafe_get_
        static constexpr element_type& s_unsafe_get_(kind_any& op) {
            if consteval {
                return *static_cast<derived_view_storage<element_type>*>(s_cget_(op))->ptr;
            } else {
                return *static_cast<element_type*>(s_rget_(op));
            }
        }

        /// s_create_
        static constexpr void s_create_(kind_any& dest_ref, element_type& op) noexcept {
            if consteval {
                dest_ref.m_cptr_ = new derived_view_storage<element_type>(op);
            } else {
                dest_ref.m_rptr_ = std::addressof(op);
            }
            dest_ref.m_manager_ = &manager::fn;
        }

        /// s_create_mini_
        static constexpr mini_view_any s_create_mini_(element_type& op) noexcept {
            mini_view_any result;
            if consteval {
                result.m_cptr_ = new derived_view_storage<element_type>(op);
            } else {
                result.m_rptr_ = std::addressof(op);
            }
            result.m_manager_ = &manager::fn;
            return result;
        }

        /// s_destroy_and_sustain_manager_
        static constexpr void s_destroy_and_sustain_manager_(kind_any& this_ref) noexcept {
            if consteval {
                delete static_cast<derived_view_storage<element_type>*>(this_ref.m_cptr_);
                this_ref.m_cptr_ = nullptr;
            } else {
                this_ref.m_rptr_ = nullptr;
            }
        }

        /// s_destroy_
        static constexpr void s_destroy_(kind_any& this_ref) noexcept {
            s_destroy_and_sustain_manager_(this_ref);
            this_ref.m_manager_ = nullptr;
        }

        /// s_copy_
        static constexpr void s_copy_(const kind_any& this_ref, kind_any& dest_ref) noexcept {
            if consteval {
                dest_ref.m_cptr_ = new derived_view_storage<element_type>(s_unsafe_get_(as_mutable(this_ref)));
            } else {
                dest_ref.m_rptr_ = this_ref.m_rptr_;
            }
            dest_ref.m_manager_ = this_ref.m_manager_;
        }

        /// s_move_and_sustain_manager_
        static constexpr void s_move_and_sustain_manager_(kind_any& this_ref, kind_any& dest_ref) noexcept {
            if consteval {
                dest_ref.m_cptr_ = std::exchange(this_ref.m_cptr_, nullptr);
            } else {
                dest_ref.m_rptr_ = std::exchange(this_ref.m_rptr_, nullptr);
            }
        }

        /// s_move_
        static constexpr void s_move_(kind_any& this_ref, kind_any& dest_ref) noexcept {
            s_move_and_sustain_manager_(this_ref, dest_ref);
            dest_ref.m_manager_  = std::exchange(this_ref.m_manager_, nullptr);
        }

        /// s_swap_
        static constexpr void s_swap_(kind_any& this_ref, kind_any& dest_ref) noexcept {
            using std::swap;
            if consteval {
                swap(this_ref.m_cptr_, dest_ref.m_cptr_);
            } else {
                swap(this_ref.m_rptr_, dest_ref.m_rptr_);
            }
            swap(this_ref.m_manager_, dest_ref.m_manager_);
        }

        /// s_eq_
        static constexpr bool s_eq_(const kind_any& lhs, const kind_any& rhs) {
            // precondition:
            // lhs and rhs must both contain the same type or false
            // lhs's contained type must be comparable or throw an exception
            return s_eq_type_(lhs, rhs) && (s_unsafe_get_(as_mutable(lhs)) == s_unsafe_get_(as_mutable(rhs)));
        }

        /// s_cmp_
        static constexpr std::partial_ordering s_cmp_(const kind_any& lhs, const kind_any& rhs) {
            // precondition:
            // lhs and rhs must both contain the same type or false
            // lhs's contained type must be 3-way comparable or throw an exception
            if (!s_eq_type_(lhs, rhs))
                return std::partial_ordering::unordered;

            return s_unsafe_get_(as_mutable(lhs)) <=> s_unsafe_get_(as_mutable(rhs));
        }

        /// fn
        static constexpr manager_result fn(manager_action act,
                                     const kind_any* this_ptr,
                                           kind_any* other_ptr = nullptr) noexcept {
            switch (act) {
                using enum manager_action;
                case do_copy:
                    s_copy_(*this_ptr, *other_ptr);
                    return manager_result::get_none();
                case do_move:
                    s_move_(as_mutable(*this_ptr), *other_ptr);
                    return manager_result::get_none();
                case do_swap:
                    s_swap_(as_mutable(*this_ptr), *other_ptr);
                    return manager_result::get_none();
                case do_destroy:
                    s_destroy_(as_mutable(*this_ptr));
                    return manager_result::get_none();
                case do_eq:
                    if constexpr (std::equality_comparable<element_type>)
                        return manager_result::get_eq(s_eq_(*this_ptr, *other_ptr));
                    else
                        return manager_result::get_none();
                case do_cmp:
                    if constexpr (std::three_way_comparable<element_type>)
                        return manager_result::get_cmp(s_cmp_(*this_ptr, *other_ptr));
                    else
                        return manager_result::get_none();
                case do_cget:
                    return manager_result::get_cget(s_cget_(as_mutable(*this_ptr)));
                case do_rget:
                    return manager_result::get_rget(s_rget_(as_mutable(*this_ptr)));
                case do_get_tinfo:
                    return manager_result::get_tinfo(s_tinfo_());
                case do_get_tname:
                    return manager_result::get_tname(s_tname_());
                case do_get_tsize:
                    return manager_result::get_tsize(s_tsize_());
                case do_get_talign:
                    return manager_result::get_tsize(s_talign_());
            }
            gold::unreachable();
        }

    };

    /// __any::manager<T, any> || __any::view<T, unique_any>
    template <typename T, typename Any>
        requires std::same_as<Any, any> || std::same_as<Any, unique_any>
    struct manager<T, Any> : manager<T> {

        /// kind_any
        using kind_any = Any;
        /// base
        using base     = manager<T>;
        /// element_type
        using typename base::element_type;
        /// wrapped_element_type
        using wrapped_element_type = derived_storage<element_type>;
        /// allocator_type
        using allocator_type = std::allocator<wrapped_element_type>;
        /// is_sbo_compatible
        inline static constexpr bool is_sbo_compatible = sbo_compatible<T>;
        /// is_copyable
        inline static constexpr bool is_copyable       = std::same_as<Any, any>;

        /// s_tinfo_
        using base::s_tinfo_;

        /// s_tinfo_from_
        static constexpr gold::ctype_info s_tinfo_from_(const kind_any& op) noexcept {
            return op.type_info();
        }

        /// s_tname_
        using base::s_tname_;

        /// s_tname_from_
        static constexpr std::string_view s_tname_from_(const kind_any& op) noexcept {
            return op.type_name();
        }

        /// s_tsize_
        using base::s_tsize_;

        /// s_talign_
        using base::s_talign_;

        /// s_eq_type_
        static constexpr bool s_eq_type_(const kind_any& lhs, const kind_any& rhs) noexcept {
            return lhs.type_info() == rhs.type_info();
        }

        /// s_get_
        static constexpr base_storage* s_get_(kind_any& this_ref) {
            if (s_tinfo_from_(this_ref) == s_tinfo_()) {
                if (is_sbo_compatible && !std::is_constant_evaluated())
                    return static_cast<base_storage*>(gold::voidify(this_ref.m_buf_));
                else
                    return this_ref.m_ptr_;
            } else {
                return nullptr;
            }
        }

        /// s_unsafe_get_
        static constexpr element_type& s_unsafe_get_(kind_any& this_ref) {
            return static_cast<wrapped_element_type*>(s_get_(this_ref))->value;
        }

        /// s_get_view_
        static constexpr mini_view_any s_get_view_(kind_any& this_ref) {
            return manager<element_type, view_any>::s_create_mini_(s_unsafe_get_(this_ref));
        }

        /// s_create_
        template <typename... Args>
        static constexpr element_type& s_create_(kind_any& dest_ref, std::in_place_t, Args&&... args) {
            if (is_sbo_compatible && !std::is_constant_evaluated()) {
                auto* result = static_cast<wrapped_element_type*>(gold::voidify(dest_ref.m_buf_));
                gold::construct_at(result, std::in_place, std::forward<Args>(args)...);
                dest_ref.m_manager_ = &manager::fn;
                return result->value;
            } else {
                allocator_type alloc;
                auto* result = alloc.allocate(1);
                gold::construct_at(result, std::in_place, std::forward<Args>(args)...);
                dest_ref.m_ptr_ = result;
                dest_ref.m_manager_ = &manager::fn;
                return result->value;
            }
        }

        static constexpr element_type& s_create_(kind_any& dest_ref, const element_type& arg) {
            if (is_sbo_compatible && !std::is_constant_evaluated()) {
                auto* result = static_cast<wrapped_element_type*>(gold::voidify(dest_ref.m_buf_));
                gold::construct_at(result, std::in_place, arg);
                dest_ref.m_manager_ = &manager::fn;
                return result->value;
            } else {
                allocator_type alloc;
                auto* result = alloc.allocate(1);
                gold::construct_at(result, std::in_place, arg);
                dest_ref.m_ptr_ = result;
                dest_ref.m_manager_ = &manager::fn;
                return result->value;
            }
        }

        static constexpr element_type& s_create_(kind_any& dest_ref, element_type&& arg) {
            if (is_sbo_compatible && !std::is_constant_evaluated()) {
                auto* result = static_cast<wrapped_element_type*>(gold::voidify(dest_ref.m_buf_));
                gold::construct_at(result, std::in_place, std::move(arg));
                dest_ref.m_manager_ = &manager::fn;
                return result->value;
            } else {
                allocator_type alloc;
                auto* result = alloc.allocate(1);
                gold::construct_at(result, std::in_place, std::move(arg));
                dest_ref.m_ptr_ = result;
                dest_ref.m_manager_ = &manager::fn;
                return result->value;
            }
        }

        /// s_destroy_and_sustain_manager_
        static constexpr void s_destroy_and_sustain_manager_(kind_any& this_ref) noexcept {
            if (is_sbo_compatible && !std::is_constant_evaluated()) {
                auto* ptr = static_cast<wrapped_element_type*>(gold::voidify(this_ref.m_buf_));
                gold::destroy_at(ptr);
            } else {
                allocator_type alloc;
                auto* ptr = static_cast<wrapped_element_type*>(this_ref.m_ptr_);
                gold::destroy_at(ptr);
                alloc.deallocate(ptr, 1);
            }
        }

        /// s_destroy_
        static constexpr void s_destroy_(kind_any& this_ref) noexcept {
            s_destroy_and_sustain_manager_(this_ref);
            this_ref.m_manager_ = nullptr;
        }

        /// s_copy_
        static constexpr void s_copy_(const kind_any& this_ref, kind_any& dest_ref) requires is_copyable {
            if (is_sbo_compatible && !std::is_constant_evaluated()) {
                auto* this_data = static_cast<const wrapped_element_type*>(gold::voidify(this_ref.m_buf_));
                auto* dest_data = static_cast<wrapped_element_type*>(gold::voidify(dest_ref.m_buf_));
                if constexpr (std::copyable<element_type>)
                    *dest_data = *this_data;
                else
                    gold::construct_at(dest_data, *this_data);
            } else {
                allocator_type alloc;
                auto* this_data = static_cast<const wrapped_element_type*>(this_ref.m_ptr_);
                auto* dest_data = alloc.allocate(1);
                gold::construct_at(dest_data, *this_data);
                dest_ref.m_ptr_ = dest_data;
            }
            dest_ref.m_manager_ = this_ref.m_manager_;
        }

        /// s_move_and_sustain_manager_
        static constexpr void s_move_and_sustain_manager_(kind_any& this_ref, kind_any& dest_ref) {
            if (is_sbo_compatible && !std::is_constant_evaluated()) {
                auto* this_data = static_cast<const wrapped_element_type*>(gold::voidify(this_ref.m_buf_));
                auto* dest_data = static_cast<wrapped_element_type*>(gold::voidify(dest_ref.m_buf_));
                if constexpr (std::movable<element_type>)
                    *dest_data = std::move(*this_data);
                else {
                    gold::construct_at(dest_data, std::move(*this_data));
                    s_destroy_and_sustain_manager_(this_ref);
                }
            } else {
                dest_ref.m_ptr_ = std::exchange(this_ref.m_ptr_, nullptr);
            }
        }

        /// s_move_
        static constexpr void s_move_(kind_any& this_ref, kind_any& dest_ref) {
            s_move_and_sustain_manager_(this_ref, dest_ref);
            dest_ref.m_manager_ = std::exchange(this_ref.m_manager_, nullptr);
        }

        /// s_swap_
        static constexpr void s_swap_(kind_any& lhs, kind_any& rhs) {

            // precondition: both 'lhs' and 'rhs' must contain a value
            using std::swap;
            swap(lhs.m_manager_, rhs.m_manager_);

            if constexpr (std::swappable<element_type>) {
                if not consteval {
                    auto* lhs_data = is_sbo_compatible
                                   ? static_cast<wrapped_element_type*>(gold::voidify(lhs.m_buf_))
                                   : static_cast<wrapped_element_type*>(lhs.m_ptr_);
                    auto* rhs_data = is_sbo_compatible
                                   ? static_cast<wrapped_element_type*>(gold::voidify(rhs.m_buf_))
                                   : static_cast<wrapped_element_type*>(rhs.m_ptr_);
                    swap(lhs_data->value, rhs_data->value);
                    return;
                }
            }

            kind_any temp;
            s_move_and_sustain_manager_(lhs, temp);
            s_move_and_sustain_manager_(rhs, lhs);
            s_move_and_sustain_manager_(temp, rhs);
        }

        /// s_eq_
        static constexpr bool s_eq_(const kind_any& lhs, const kind_any& rhs) {
            // precondition:
            // lhs and rhs must both contain the same type or false
            // lhs's contained type must be comparable or throw an exception
            return s_eq_type_(lhs, rhs) && (s_unsafe_get_(as_mutable(lhs)) == s_unsafe_get_(as_mutable(rhs)));
        }

        /// s_cmp_
        static constexpr std::partial_ordering s_cmp_(const kind_any& lhs, const kind_any& rhs) {
            // precondition:
            // lhs and rhs must both contain the same type or false
            // lhs's contained type must be 3-way comparable or throw an exception
            if (!s_eq_type_(lhs, rhs))
                return std::partial_ordering::unordered;

            return s_unsafe_get_(as_mutable(lhs)) <=> s_unsafe_get_(as_mutable(rhs));
        }

        /// fn
        static constexpr manager_result fn(manager_action act,
                                     const kind_any* this_ptr,
                                           kind_any* other_ptr = nullptr) noexcept {
            switch (act) {
                using enum manager_action;
                case do_copy:
                    if constexpr (is_copyable)
                        s_copy_(*this_ptr, *other_ptr);
                    return manager_result::get_none();
                case do_move:
                    s_move_(as_mutable(*this_ptr), *other_ptr);
                    return manager_result::get_none();
                case do_swap:
                    s_swap_(as_mutable(*this_ptr), *other_ptr);
                    return manager_result::get_none();
                case do_destroy:
                    s_destroy_(as_mutable(*this_ptr));
                    return manager_result::get_none();
                case do_eq:
                    if constexpr (std::equality_comparable<element_type>)
                        return manager_result::get_eq(s_eq_(*this_ptr, *other_ptr));
                    else
                        return manager_result::get_none();
                case do_cmp:
                    if constexpr (std::three_way_comparable<element_type>)
                        return manager_result::get_cmp(s_cmp_(*this_ptr, *other_ptr));
                    else
                        return manager_result::get_none();
                case do_cget:
                    return manager_result::get_cget(s_get_(as_mutable(*this_ptr)));
                case do_get_view:
                    return manager_result::get_view(s_get_view_(as_mutable(*this_ptr)));
                case do_get_tinfo:
                    return manager_result::get_tinfo(s_tinfo_());
                case do_get_tname:
                    return manager_result::get_tname(s_tname_());
                case do_get_tsize:
                    return manager_result::get_tsize(s_tsize_());
                case do_get_talign:
                    return manager_result::get_tsize(s_talign_());
                default:;
            }
            gold::unreachable();
        }
    };

} // namespace gold::__any

#endif // __GOLD_BITS_ANY_MANAGER_HPP
