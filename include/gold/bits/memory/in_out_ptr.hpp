// <gold/bits/memory/in_out_ptr.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_MEMORY_IN_OUT_PTR_HPP
#define __GOLD_BITS_MEMORY_IN_OUT_PTR_HPP

#include <tuple>
#include <bits/ptr_traits.h>
#include <gold/bits/type_traits/conditional.hpp>

namespace gold {

    namespace __memory {

        /// __memory::pointer_of
        template <typename>
        struct pointer_of {};

        template <typename T>
            requires requires { typename T::pointer; }
        struct pointer_of<T> {
            using type = T::pointer;
        };

        template <typename T>
            requires (!requires { typename T::pointer; })
                    && requires { typename T::element_type; }
        struct pointer_of<T> {
            using type = T::element_type*;
        };

        template <typename T>
            requires (!(requires { typename T::pointer; }
                     && requires { typename T::element_type; }))
                     && requires { typename std::pointer_traits<T>::element_type; }
        struct pointer_of<T> {
            using type = std::pointer_traits<T>::element_type*;
        };

        /// __memory::pointer_of_t
        template <typename T>
            requires requires { typename pointer_of<T>::type; }
        using pointer_of_t = pointer_of<T>::type;

        /// __memory::pointer_of_or_t
        template <typename T, typename U>
        using pointer_of_or_t = gold::conditional_t<
            requires { typename pointer_of_t<T>; },
            pointer_of_t<T>, U
        >;

        /// __memory::resettable_pointer
        template <typename Smart, typename Sp, typename Pointer, typename... Args>
        concept resettable_pointer = requires (Smart& smart_ptr, Pointer ptr, Args&&... args) {
            smart_ptr.reset(static_cast<Sp>(ptr), std::forward<Args>(args)...);
        };


    } // namespace __memory

    /// out_ptr_t
    template <typename Smart, typename Pointer, typename... Args>
    class out_ptr_t {
      private:
        Smart&  m_smart_ptr_;
        Pointer m_ptr_;
        [[no_unique_address]] std::tuple<Args...> m_args_;

        constexpr Pointer& mf_ptr_() const noexcept {
            return const_cast<Pointer&>(m_ptr_);
        }

        // assert: sizeof...(Args) != 0

      public:
        constexpr explicit out_ptr_t(Smart& smart_ptr, Args... args)
            noexcept(std::is_nothrow_constructible_v<std::tuple<Args...>, Args...>)
        : m_smart_ptr_(smart_ptr), m_ptr_(), m_args_(std::forward<Args>(args)...) {}

        constexpr out_ptr_t(const out_ptr_t&) = delete;
        constexpr out_ptr_t& operator=(const out_ptr_t&) = delete;

        constexpr ~out_ptr_t() {
            if (!m_ptr_)
                return;
            std::apply([this](auto&&... args) {
                using Sp = __memory::pointer_of_or_t<Smart, Pointer>;
                if constexpr (__memory::resettable_pointer<Smart, Sp, Pointer, Args...>)
                    m_smart_ptr_.reset(static_cast<Sp>(mf_ptr_()), std::forward<Args>(args)...);
                else {
                    static_assert(std::is_constructible_v<Smart, Sp, Args...>);
                    m_smart_ptr_ = Smart(static_cast<Sp>(mf_ptr_()), std::forward<Args>(args)...);
                }
            }, std::move(m_args_));
        }

        constexpr operator Pointer*() const noexcept {
            return __builtin_addressof(mf_ptr_());
        }

        operator void**() const noexcept requires (std::is_pointer_v<Pointer> && !std::is_same_v<Pointer, void*>) {
            return reinterpret_cast<void**>(__builtin_addressof(mf_ptr_()));
        }
    };

    /// out_ptr
    template <typename Pointer = void, typename Smart, typename... Args>
    [[nodiscard]] constexpr auto out_ptr(Smart& smart_ptr, Args&&... args) {
        using return_type = gold::conditional_t<
            std::is_void_v<Pointer>,
            out_ptr_t<Smart, __memory::pointer_of_t<Smart>, Args&&...>,
            out_ptr_t<Smart, Pointer, Args&&...>
        >;
        return return_type(smart_ptr, std::forward<Args>(args)...);
    }

    /// inout_ptr_t
    template <typename Smart, typename Pointer, typename... Args>
    class inout_ptr_t {
      private:
        Smart&  m_smart_ptr_;
        Pointer m_ptr_;
        [[no_unique_address]] std::tuple<Args...> m_args_;

        [[nodiscard]] static constexpr auto s_get_ptr_from_smart_(Smart& smart_ptr)
            noexcept requires std::is_pointer_v<Smart>
        {
            return smart_ptr;
        }

        [[nodiscard]] static constexpr auto s_get_ptr_from_smart_(Smart& smart_ptr)
            noexcept(noexcept(smart_ptr.get()))
        {
            return smart_ptr.get();
        }

        constexpr Pointer& mf_ptr_() const noexcept {
            return const_cast<Pointer&>(m_ptr_);
        }

      public:
        constexpr explicit inout_ptr_t(Smart& smart_ptr, Args... args)
            noexcept(std::is_nothrow_constructible_v<std::tuple<Args...>, Args...>
                  && noexcept(s_get_ptr_from_smart_(smart_ptr)))
        : m_smart_ptr_(smart_ptr), m_ptr_(), m_args_(std::forward<Args>(args)...) {}

        constexpr inout_ptr_t(const inout_ptr_t&) = delete;
        constexpr inout_ptr_t& operator=(const inout_ptr_t&) = delete;

        constexpr ~inout_ptr_t() {
            if (!m_ptr_)
                return;
            std::apply([this](auto&&... args) {
                using Sp = __memory::pointer_of_or_t<Smart, Pointer>;
                if constexpr (std::is_pointer_v<Smart>) {
                    m_smart_ptr_ = Smart(static_cast<Sp>(mf_ptr_()), std::forward<Args>(args)...);
                } else if constexpr (__memory::resettable_pointer<Smart, Sp, Pointer, Args...>) {
                    m_smart_ptr_.release();
                    m_smart_ptr_.reset(static_cast<Sp>(mf_ptr_()), std::forward<Args>(args)...);
                } else {
                    static_assert(std::is_constructible_v<Smart, Sp, Args...>);
                    m_smart_ptr_.release();
                    m_smart_ptr_ = Smart(static_cast<Sp>(mf_ptr_()), std::forward<Args>(args)...);
                }
            }, std::move(m_args_));
        }

        constexpr operator Pointer*() const noexcept {
            return __builtin_addressof(mf_ptr_());
        }

        operator void**() const noexcept requires (std::is_pointer_v<Pointer> && !std::is_same_v<Pointer, void*>) {
            return reinterpret_cast<void**>(__builtin_addressof(mf_ptr_()));
        }
    };

    /// inout_ptr
    template <typename Pointer = void, typename Smart, typename... Args>
    [[nodiscard]] constexpr auto inout_ptr(Smart& smart_ptr, Args&&... args) {
        using return_type = gold::conditional_t<
            std::is_void_v<Pointer>,
            inout_ptr_t<Smart, __memory::pointer_of_t<Smart>, Args&&...>,
            inout_ptr_t<Smart, Pointer, Args&&...>
        >;
        return return_type(smart_ptr, std::forward<Args>(args)...);
    }

} // namespace gold

#endif // __GOLD_BITS_MEMORY_IN_OUT_PTR_HPP
