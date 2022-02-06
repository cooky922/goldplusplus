// <gold/bits/any/basic_any_handler.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_BASIC_ANY_HANDLER_HPP
#define __GOLD_BITS_BASIC_ANY_HANDLER_HPP

#include <gold/bits/memory/voidify.hpp>
#include <bits/allocator.h>
#include <bits/stl_construct.h>

namespace gold::__any {

    /// basic_any_handler
    template <bool Small, typename T, bool Copyable>
    struct basic_any_handler {

        /// ::any_kind
        using any_kind = std::conditional_t<Copyable, any, unique_any>;

        /// ::value_type
        using value_type = derived_any<T>;

        /// ::handle
        static constexpr auto handle(action_any act, const any_kind* this_ptr, any_kind* other_ptr) {
            using return_type = handler_func_ret_type;

            switch (act) {
                using enum action_any;

                case do_destroy:
                    s_destroy_(*const_cast<any_kind*>(this_ptr));
                    return return_type {};

                case do_copy:
                    if constexpr (Copyable)
                        s_copy_(*this_ptr, *other_ptr);
                    return return_type {};

                case do_move:
                    s_move_(*const_cast<any_kind*>(this_ptr), *other_ptr);
                    return return_type {};

                case do_swap:
                    s_swap_(*const_cast<any_kind*>(this_ptr), *other_ptr);
                    return return_type {};

                case do_compare:
                    if constexpr (std::equality_comparable<T>)
                        return return_type {
                            std::in_place_type<bool>, s_compare_(*this_ptr, *other_ptr)
                        };
                    else
                        return return_type {};

                case do_compare_3way:
                    if constexpr (std::three_way_comparable<T>)
                        return return_type {
                            std::in_place_type<std::partial_ordering>,
                            s_compare_3way_(*this_ptr, *other_ptr)
                        };
                    else
                        return return_type {};

                case do_get:
                    return return_type { std::in_place_type<base_any*>, s_get_(*const_cast<any_kind*>(this_ptr)) };

                case do_get_typeinfo:
                    return return_type { std::in_place_type<const std::type_info*>, s_type_info_() };

                case do_get_typename:
                    return return_type { std::in_place_type<std::string_view>, s_type_name_() };

                case do_get_sizeof:
                    return return_type { std::in_place_type<std::size_t>, sizeof(T) };

                case do_get_alignof:
                    return return_type { std::in_place_type<std::size_t>, alignof(T) };

            }

            gold::unreachable();
        }

        /// ::create
        template <typename... Args>
        static constexpr value_type& create(any_kind& dest, std::in_place_t, Args&&... args) {
            if (Small && !std::is_constant_evaluated()) {
                value_type* result_ = static_cast<value_type*>(gold::voidify(dest.m_data_.m_buf_));
                std::construct_at(result_, std::in_place, std::forward<Args>(args)...);
                dest.m_handle_ = &basic_any_handler::handle;
                return *result_;
            } else {
                std::allocator<value_type> alloc_;
                value_type* result_ = alloc_.allocate(1);
                std::construct_at(result_, std::in_place, std::forward<Args>(args)...);
                dest.m_data_.m_ptr_ = result_;
                dest.m_handle_ = &basic_any_handler::handle;
                return *result_;
            }

        }

        static constexpr value_type& create(any_kind& dest, const value_type& arg) {
            if (Small && !std::is_constant_evaluated()) {
                auto result_ = static_cast<value_type*>(gold::voidify(dest.m_data_.m_buf_));
                std::construct_at(result_, std::in_place, arg.value);
                dest.m_handle_ = &basic_any_handler::handle;
                return *result_;
            } else {
                std::allocator<value_type> alloc_;
                auto result_ = alloc_.allocate(1);
                std::construct_at(result_, arg);
                dest.m_data_.m_ptr_ = result_;
                dest.m_handle_ = &basic_any_handler::handle;
                return *result_;
            }
        }

        static constexpr value_type& create(any_kind& dest, value_type&& arg) {
            if (Small && !std::is_constant_evaluated()) {
                auto result_ = static_cast<value_type*>(gold::voidify(dest.m_data_.m_buf_));
                std::construct_at(result_, std::in_place, std::move(arg.value));
                dest.m_handle_ = &basic_any_handler::handle;
                return *result_;
            } else {
                std::allocator<value_type> alloc_;
                auto result_ = alloc_.allocate(1);
                std::construct_at(result_, std::move(arg));
                dest.m_data_.m_ptr_ = result_;
                dest.m_handle_ = &basic_any_handler::handle;
                return *result_;
            }
        }

        /// ::s_destroy_
        static constexpr void s_destroy_(any_kind& this_ref) {
            if (Small && !std::is_constant_evaluated()) {
                auto ptr_ = static_cast<value_type*>(gold::voidify(this_ref.m_data_.m_buf_));
                std::destroy_at(ptr_);
                this_ref.m_handle_ = nullptr;
            } else {
                std::allocator<value_type> alloc_;
                auto ptr_ = static_cast<value_type*>(this_ref.m_data_.m_ptr_);
                std::destroy_at(ptr_);
                alloc_.deallocate(ptr_, 1);
                this_ref.m_handle_ = nullptr;
            }
        }

        /// ::s_destroy_and_sustain_handle_ [ not visible to any choice ]
        static constexpr void s_destroy_and_sustain_handle_(any_kind& this_ref) {
            if (Small && !std::is_constant_evaluated()) {
                auto ptr_ = static_cast<value_type*>(gold::voidify(this_ref.m_data_.m_buf_));
                std::destroy_at(ptr_);
            } else {
                std::allocator<value_type> alloc_;
                auto ptr_ = static_cast<value_type*>(this_ref.m_data_.m_ptr_);
                std::destroy_at(ptr_);
                alloc_.deallocate(ptr_, 1);
            }
        }

        /// ::s_copy_
        static constexpr void s_copy_(const any_kind& this_ref, any_kind& dest) requires Copyable {
            if (Small && !std::is_constant_evaluated()) {
                auto this_data = static_cast<const value_type*>(gold::voidify(this_ref.m_data_.m_buf_));
                auto dest_data = static_cast<value_type*>(gold::voidify(dest.m_data_.m_buf_));
                std::construct_at(dest_data, *this_data);
                dest.m_handle_ = this_ref.m_handle_;
            } else {
                std::allocator<value_type> alloc_;
                auto this_data = static_cast<const value_type*>(this_ref.m_data_.m_ptr_);
                auto dest_data = alloc_.allocate(1);
                std::construct_at(dest_data, *this_data);
                dest.m_data_.m_ptr_ = dest_data;
                dest.m_handle_ = this_ref.m_handle_;
            }
        }

        /// ::s_move_
        static constexpr void s_move_(any_kind& this_ref, any_kind& dest) {
            if (Small && !std::is_constant_evaluated()) {
                auto this_data = static_cast<const value_type*>(gold::voidify(this_ref.m_data_.m_buf_));
                auto dest_data = static_cast<value_type*>(gold::voidify(dest.m_data_.m_buf_));
                if constexpr (std::movable<T>) {
                    dest.m_handle_ = std::exchange(this_ref.m_handle_, nullptr);
                    *dest_data     = std::move(*this_data);
                } else {
                    std::construct_at(dest_data, std::move(*this_data));
                    dest.m_handle_      = std::exchange(this_ref.m_handle_, nullptr);
                    s_destroy_(this_ref);
                }
            } else {
                dest.m_handle_      = std::exchange(this_ref.m_handle_, nullptr);
                dest.m_data_.m_ptr_ = std::exchange(this_ref.m_data_.m_ptr_, nullptr);
            }
        }

        /// ::s_move_and_sustain_handle_ [ not visible to any choice ]
        static constexpr void s_move_and_sustain_handle_(any_kind& this_ref, any_kind& dest) {
            if (Small && !std::is_constant_evaluated()) {
                auto this_data = static_cast<const value_type*>(gold::voidify(this_ref.m_data_.m_buf_));
                auto dest_data = static_cast<value_type*>(gold::voidify(dest.m_data_.m_buf_));
                if constexpr (std::movable<T>) {
                    *dest_data = std::move(*this_data);
                } else {
                    std::construct_at(dest_data, std::move(*this_data));
                    s_destroy_and_sustain_handle_(this_ref);
                }
            } else {
                dest.m_data_.m_ptr_ = std::exchange(this_ref.m_data_.m_ptr_, nullptr);
            }
        }

        /// ::s_swap_
        static constexpr void s_swap_(any_kind& lhs, any_kind& rhs) {
            // precondition: both 'lhs' and 'rhs' must contain a value
            using std::swap;
            swap(lhs.m_handle_, rhs.m_handle_);

            if constexpr (std::swappable<T>) {
                if not consteval {
                    auto lhs_data = Small ? static_cast<value_type*>(gold::voidify(lhs.m_data_.m_buf_))
                                          : static_cast<value_type*>(lhs.m_data_.m_ptr_);
                    auto rhs_data = Small ? static_cast<value_type*>(gold::voidify(rhs.m_data_.m_buf_))
                                          : static_cast<value_type*>(rhs.m_data_.m_ptr_);
                    swap(lhs_data->value, rhs_data->value);
                    return;
                }
            }
            any_kind temp_;
            s_move_and_sustain_handle_(lhs, temp_);
            s_move_and_sustain_handle_(rhs, lhs);
            s_move_and_sustain_handle_(temp_, rhs);
        }

        /// ::s_compare_
        static constexpr bool s_compare_(const any_kind& lhs, const any_kind& rhs) {
            // precondition:
            // lhs and rhs must both contain the same type or false
            // lhs's contained type must be comparable or throw an exception
            return s_compare_type_(lhs, rhs) && static_cast<
                const value_type*
            >(s_unsafe_get_(const_cast<any_kind&>(lhs)))->value == static_cast<
                const value_type*
            >(s_unsafe_get_(const_cast<any_kind&>(rhs)))->value;
        }

        /// ::s_compare_3way_
        static constexpr std::partial_ordering s_compare_3way_(const any_kind& lhs, const any_kind& rhs) {
            // precondition:
            // lhs and rhs must both contain the same type or false
            // lhs's contained type must be 3-way comparable or throw an exception
            if (!s_compare_type_(lhs, rhs))
                return std::partial_ordering::unordered;

            return static_cast<
                const value_type*
            >(s_unsafe_get_(const_cast<any_kind&>(lhs)))->value <=> static_cast<
                const value_type*
            >(s_unsafe_get_(const_cast<any_kind&>(rhs)))->value;
        }

        /// ::s_unsafe_get_ [ not visible to any choice ]
        static constexpr base_any* s_unsafe_get_(any_kind& this_ref) {
            if (Small && !std::is_constant_evaluated())
                return static_cast<base_any*>(gold::voidify(this_ref.m_data_.m_buf_));
            else
                return this_ref.m_data_.m_ptr_;
        }

        /// ::s_get_
        static constexpr base_any* s_get_(any_kind& this_ref) {
            if (compare_typeid<T>(*s_type_info_(), s_type_name_())) {
                if (Small && !std::is_constant_evaluated())
                    return static_cast<base_any*>(gold::voidify(this_ref.m_data_.m_buf_));
                else
                    return this_ref.m_data_.m_ptr_;
            } else {
                return nullptr;
            }
        }

        /// ::s_compare_type_
        static constexpr bool s_compare_type_(const any_kind& lhs, const any_kind& rhs) {
            if consteval {
                // use 's_type_name_from_'
                if (s_type_name_from_(lhs) != s_type_name_from_(rhs))
                    return false;
            } else {
                // use 's_type_info_from_'
                if (*s_type_info_from_(lhs) != *s_type_info_from_(rhs))
                    return false;
            }
            return true;
        }

        /// ::s_type_info_
        static constexpr const std::type_info* s_type_info_() {
            return &typeid(T);
        }

        /// ::s_type_info_from_
        static constexpr const std::type_info* s_type_info_from_(const any_kind& op) {
            return &op.type_info();
        }

        /// ::s_type_name_
        static constexpr std::string_view s_type_name_() {
            return gold::type_name<T>();
        }

        /// ::s_type_name_from_
        static constexpr std::string_view s_type_name_from_(const any_kind& op) {
            return op.type_name();
        }

    };

} // namespace gold::__any

#endif // __GOLD_BITS_BASIC_ANY_HANDLER_HPP
