// <gold/bits/any/store_any.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_STORE_ANY_HPP
#define __GOLD_BITS_STORE_ANY_HPP

#include <compare>
#include <typeinfo>
#include <variant>
#include <gold/utility>
#include <gold/type_name>

namespace gold {

    /// any [fwd dcl]
    class any;

    /// unique_any [fwd decl]
    class unique_any;

    namespace __any {

        /// base_any
        struct base_any {};

        /// derived_any
        template <typename T>
        struct derived_any final : base_any {
            [[no_unique_address]] T value;

            template <typename... Args>
                requires std::constructible_from<T, Args...>
            constexpr explicit derived_any(std::in_place_t, Args&&... args)
            : value(std::forward<Args>(args)...) {}

            template <typename... Args>
                requires gold::brace_constructible_from<T, Args...>
            constexpr explicit derived_any(gold::in_place_braced_t, Args&&... args)
            : value{std::forward<Args>(args)...} {}

            constexpr derived_any() requires std::default_initializable<T> = default;
            constexpr derived_any(const derived_any&) requires std::copy_constructible<T> = default;
            constexpr derived_any(derived_any&&) requires std::move_constructible<T> = default;
            constexpr ~derived_any() requires std::destructible<T> = default;
            constexpr derived_any& operator=(const derived_any&) requires std::copyable<T> = default;
            constexpr derived_any& operator=(derived_any&&) requires std::movable<T> = default;
        };

        /// buffer_any
        using buffer_any = std::aligned_storage_t<3 * sizeof(void*), alignof(void*)>;

        /// small_object
        template <typename T>
        concept small_object = (
            (sizeof(T) <= sizeof(buffer_any)) &&
            (alignof(buffer_any) % alignof(T) == 0) &&
            (std::is_nothrow_move_constructible_v<T>)
        );

        /// action_any
        enum class action_any : unsigned char {
            do_destroy, do_copy, do_move, do_swap,
            do_compare, do_compare_3way, do_get,
            do_get_typeinfo, do_get_typename,
            do_get_sizeof, do_get_alignof
        };

        /// handler_func_ret_type
        using handler_func_ret_type = std::variant<
            std::monostate,        // do_destroy, do_copy, do_move, do_swap
            bool,                  // do_compare
            std::partial_ordering, // do_compare_3way
            base_any*,             // do_get
            const std::type_info*, // do_get_typeinfo
            std::string_view,      // do_get_typename
            std::size_t            // do_get_sizeof, do_get_alignof
        >;

        /// basic_any_handler [forward decl.]
        template <bool, typename, bool>
        struct basic_any_handler;

        /// any_handler
        template <typename T, bool Copyable>
        using any_handler = basic_any_handler<
            small_object<T>, T, Copyable
        >;

        /// compare_typeid
        template <typename T>
        constexpr bool compare_typeid(const std::type_info& info, std::string_view name) {
            if consteval {
                return gold::type_name<T>() == name;
            } else {
                return info == typeid(T);
            }
        }

        /// any_data_storage
        union any_data_storage {

            /// storage::storage_t() [ default ctor ]
            constexpr any_data_storage()
            : m_ptr_(nullptr) {}

            /// alternative members
            // heap allocation (for both compile-time and runtime and suitable for large-size type)
            __any::base_any*  m_ptr_;
            // preallocated (only for runtime and small-sized type)
            __any::buffer_any m_buf_;

        };

    } // namespace __any

} // namespace gold

#endif // __GOLD_BITS_STORE_ANY_HPP
