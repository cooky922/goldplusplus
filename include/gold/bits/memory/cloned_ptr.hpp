// <gold/bits/memory/cloned_ptr.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_MEMORY_CLONED_PTR_HPP
#define __GOLD_BITS_MEMORY_CLONED_PTR_HPP

#include <cstddef>
#include <bits/move.h>
#include <type_traits>
#include <concepts>
#include <initializer_list>
#include <gold/bits/algo/copy.hpp>
#include <gold/bits/memory/ops.hpp>
#include <gold/bits/memory/ops_algo.hpp>
#include <gold/bits/concepts/types.hpp>
#include <gold/bits/type_traits/conditional.hpp>
#include <bits/allocator.h>

namespace gold {

    namespace __memory {

        /// __memory::single_object_type
        template <typename T>
        concept single_object_type = std::is_object_v<T> && !std::is_unbounded_array_v<T>;

        /// __memory::array_type
        template <typename T>
        concept array_type = std::is_unbounded_array_v<T>;

        /// __memory::deleter_allocator
        template <typename Alloc>
        struct deleter_allocator {
            using allocator_type = Alloc;
            using traits_type    = std::allocator_traits<Alloc>;

            using value_type     = traits_type::value_type;
            using pointer        = traits_type::pointer;

            static constexpr bool stateless = traits_type::is_always_equal::value;

            using member_type = gold::conditional_t<
                stateless,
                Alloc,
                Alloc*
            >;

            [[no_unique_address]] member_type alloc = {};

            constexpr deleter_allocator() noexcept = default;
            constexpr deleter_allocator(const deleter_allocator&) noexcept = default;
            template <typename OtherAlloc>
            constexpr deleter_allocator(deleter_allocator<OtherAlloc>) noexcept
                requires stateless && deleter_allocator<OtherAlloc>::stateless
            {}

            constexpr deleter_allocator(Alloc&) noexcept requires stateless {}
            constexpr deleter_allocator(Alloc& a) noexcept requires (!stateless)
            : alloc(__builtin_addressof(a)) {}

            constexpr void operator()(pointer ptr) const {
                gold::destroy_at(ptr);
                if constexpr (stateless)
                    traits_type::deallocate(const_cast<Alloc&>(alloc), ptr, 1);
                else
                    traits_type::deallocate(*alloc, ptr, 1);
            }

            constexpr void operator()(pointer ptr, std::size_t n) const {
                gold::destroy_n(ptr, n);
                if constexpr (stateless)
                    traits_type::deallocate(const_cast<Alloc&>(alloc), ptr, n);
                else
                    traits_type::deallocate(*alloc, ptr, n);
            }
        };

        /// __memory::cloner_allocator
        template <typename Alloc>
        struct cloner_allocator {
            using allocator_type = Alloc;
            using traits_type    = std::allocator_traits<Alloc>;

            using value_type     = traits_type::value_type;
            using pointer        = traits_type::pointer;

            static constexpr bool stateless = traits_type::is_always_equal::value;

            using member_type = gold::conditional_t<
                stateless,
                Alloc,
                Alloc*
            >;

            [[no_unique_address]] member_type alloc = {};

            constexpr cloner_allocator() noexcept = default;
            constexpr cloner_allocator(const cloner_allocator&) noexcept = default;
            template <typename OtherAlloc>
            constexpr cloner_allocator(cloner_allocator<OtherAlloc>) noexcept
                requires stateless && cloner_allocator<OtherAlloc>::stateless
            {}

            constexpr cloner_allocator(Alloc&) noexcept requires stateless {}
            constexpr cloner_allocator(Alloc& a) noexcept requires (!stateless)
            : alloc(__builtin_addressof(a)) {}

            constexpr pointer operator()(pointer ptr) const {
                auto new_ptr = [&] {
                    if constexpr (stateless)
                        return traits_type::allocate(const_cast<Alloc&>(alloc), 1);
                    else
                        return traits_type::allocate(*alloc, 1);
                }();
                return gold::construct_at(new_ptr, *ptr);
            }

            constexpr pointer operator()(pointer ptr, std::size_t n) const {
                auto new_ptr = [&] {
                    if constexpr (stateless)
                        return traits_type::allocate(const_cast<Alloc&>(alloc), n);
                    else
                        return traits_type::allocate(*alloc, n);
                }();
                gold::uninit_copy_n(ptr, n, new_ptr, new_ptr + n);
                return new_ptr;
            }
        };

    } // namespace __memory

    /// derived_t
    template <typename T>
    struct derived_t {
        explicit derived_t() noexcept = default;
    };

    /// derived
    template <typename T>
    inline constexpr derived_t<T> derived {};

    /// default_deleter
    template <typename T>
    struct default_deleter {

        template <typename U>
        using rebind = default_deleter<U>;

        using element_type = std::remove_extent_t<T>;
        using pointer      = element_type*;

        //// ctors
        constexpr default_deleter() noexcept = default;

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        constexpr default_deleter(default_deleter<U>) noexcept {}

        //// operator()
        static constexpr void operator()(element_type* ptr) requires __memory::single_object_type<T> {
            delete ptr;
        }

        static constexpr void operator()(element_type* ptr, std::size_t = 0) requires __memory::array_type<T> {
            delete[] ptr;
        }

        template <typename U>
        static constexpr void operator()(gold::derived_t<U>, element_type* ptr) requires __memory::single_object_type<T> {
            if constexpr (std::has_virtual_destructor_v<element_type>)
                delete ptr;
            else
                delete static_cast<U*>(ptr);
        }

        template <typename U>
        static constexpr void operator()(gold::derived_t<U>, element_type* ptr, std::size_t = 0) requires __memory::array_type<T> {
            if constexpr (std::has_virtual_destructor_v<element_type>)
                delete[] ptr;
            else
                delete[] static_cast<U*>(ptr);
        }
    };

    /// default_cloner
    template <typename T>
    struct default_cloner {

        template <typename U>
        using rebind = default_deleter<U>;

        using element_type = std::remove_extent_t<T>;
        using pointer      = element_type*;

        //// ctors
        constexpr default_cloner() noexcept = default;

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        constexpr default_cloner(default_cloner<U>) noexcept {}

        static constexpr pointer operator()(pointer ptr) requires __memory::single_object_type<T> {
            return new element_type(*ptr);
        }

        template <typename U>
        static constexpr pointer operator()(derived_t<U>, pointer ptr) requires __memory::single_object_type<T> {
            return new U(*static_cast<U*>(ptr));
        }

        static constexpr pointer operator()(pointer ptr, std::size_t n) requires __memory::array_type<T> {
            pointer new_pointer = new element_type[n];
            __algo::copy(ptr, ptr + n, new_pointer);
            return new_pointer;
        }

        template <typename U>
        static constexpr pointer operator()(derived_t<U>, pointer ptr, std::size_t n) requires __memory::array_type<T> {
            pointer new_pointer = new U[n];
            U* p = static_cast<U*>(new_pointer);
            __algo::copy(ptr, ptr + n, p);
            return new_pointer;
        }

    };

    template <typename T, typename Deleter = default_deleter<T>,
                          typename Cloner  = default_cloner<T>>
    class cloned_ptr;

    template <typename T, typename Deleter, typename Cloner>
        requires __memory::single_object_type<T>
    class cloned_ptr<T, Deleter, Cloner> {
      public:
        using element_type  = std::remove_cv_t<T>;
        using pointer       = element_type*;
        using const_pointer = const element_type*;
        using deleter_type  = Deleter;
        using cloner_type   = Cloner;

      private:
        using manage_type   = void (*) (bool, const cloned_ptr*, T**, T*);

        [[no_unique_address]] deleter_type m_del_   = {};
        [[no_unique_address]] cloner_type  m_clone_ = {};
        pointer     m_data_   = nullptr;
        manage_type m_manage_ = nullptr;

        template <typename U = T>
        static constexpr void s_manage_(bool is_clone, const cloned_ptr* parent, T** this_ptr, T* that_ptr) {
            if (is_clone) {
                if constexpr (std::is_same_v<T, U>)
                    *this_ptr = parent->m_clone_(that_ptr);
                else
                    *this_ptr = parent->m_clone_(derived<U>, that_ptr);
            } else {
                if constexpr (std::has_virtual_destructor_v<T> || std::is_same_v<T, U>)
                    parent->m_del_(*this_ptr);
                else
                    parent->m_del_(derived<U>, *this_ptr);
            }
        }

        constexpr T* mf_clone_() const {
            if (m_data_ == nullptr)
                return nullptr;
            T* result;
            m_manage_(true, this, &const_cast<pointer&>(result), const_cast<pointer&>(m_data_));
            return result;
        }

        constexpr void mf_delete_() const {
            if (m_data_ != nullptr)
                m_manage_(false, this, &const_cast<pointer&>(m_data_), nullptr);
        }

        template <typename U, typename OtherDeleter, typename OtherCloner>
        friend class cloned_ptr;

      public:
        constexpr cloned_ptr() noexcept = default;
        constexpr cloned_ptr(std::nullptr_t) noexcept : cloned_ptr() {}

        [[gnu::nonnull]] constexpr cloned_ptr(pointer ptr) noexcept
            requires std::default_initializable<deleter_type> &&
                     std::default_initializable<cloner_type>
        : m_data_(ptr), m_manage_(&s_manage_<>) {}

        [[gnu::nonnull]] explicit constexpr cloned_ptr(pointer ptr,
                                                       const deleter_type& d,
                                                       const cloner_type&  c) noexcept
            requires (!std::is_reference_v<deleter_type> &&
                      !std::is_reference_v<cloner_type>) &&
                       std::copy_constructible<deleter_type> &&
                       std::copy_constructible<cloner_type>
        : m_del_(d), m_clone_(c), m_data_(ptr), m_manage_(&s_manage_<>) {}

        [[gnu::nonnull]] explicit constexpr cloned_ptr(pointer ptr,
                                                       deleter_type&& d,
                                                       cloner_type&&  c) noexcept
            requires (!std::is_reference_v<deleter_type> &&
                      !std::is_reference_v<cloner_type>) &&
                       std::move_constructible<deleter_type> &&
                       std::move_constructible<cloner_type>
        : m_del_(std::move(d)), m_clone_(std::move(c)), m_data_(ptr), m_manage_(&s_manage_<>) {}

        [[gnu::nonnull]] explicit constexpr cloned_ptr(pointer ptr,
                                                       deleter_type& d,
                                                       cloner_type&  c) noexcept
            requires (std::is_lvalue_reference_v<deleter_type> &&
                      std::is_lvalue_reference_v<cloner_type>)
        : m_del_(d), m_clone_(c), m_data_(ptr), m_manage_(&s_manage_<>) {}

        [[gnu::nonnull]] explicit constexpr cloned_ptr(pointer,
                                                       std::remove_reference_t<deleter_type>&&,
                                                       std::remove_reference_t<cloner_type>&&) noexcept
            requires (std::is_lvalue_reference_v<deleter_type> &&
                      std::is_lvalue_reference_v<cloner_type>)
        = delete;

        template <std::derived_from<element_type> U>
            requires std::default_initializable<deleter_type> &&
                     std::default_initializable<cloner_type>
        [[gnu::nonnull]] constexpr cloned_ptr(U* ptr) noexcept
        : m_data_(ptr), m_manage_(&s_manage_<U>) {}

        template <std::derived_from<element_type> U>
            requires (!std::is_reference_v<deleter_type> &&
                      !std::is_reference_v<cloner_type>) &&
                       std::copy_constructible<deleter_type> &&
                       std::copy_constructible<cloner_type>
        [[gnu::nonnull]] explicit constexpr cloned_ptr(U* ptr,
                                                       const deleter_type& d,
                                                       const cloner_type&  c) noexcept
        : m_del_(d), m_clone_(c), m_data_(ptr), m_manage_(&s_manage_<U>) {}

        template <std::derived_from<element_type> U>
            requires (!std::is_reference_v<deleter_type> &&
                      !std::is_reference_v<cloner_type>) &&
                       std::move_constructible<deleter_type> &&
                       std::move_constructible<cloner_type>
        [[gnu::nonnull]] explicit constexpr cloned_ptr(U* ptr,
                                                       deleter_type&& d,
                                                       cloner_type&&  c) noexcept
        : m_del_(std::move(d)), m_clone_(std::move(c)), m_data_(ptr), m_manage_(&s_manage_<U>) {}

        template <std::derived_from<element_type> U>
            requires (std::is_lvalue_reference_v<deleter_type> &&
                      std::is_lvalue_reference_v<cloner_type>)
        [[gnu::nonnull]] explicit constexpr cloned_ptr(U* ptr,
                                                       deleter_type& d,
                                                       cloner_type&  c) noexcept
        : m_del_(d), m_clone_(c), m_data_(ptr), m_manage_(&s_manage_<U>) {}

        template <std::derived_from<element_type> U>
            requires (std::is_lvalue_reference_v<deleter_type> &&
                      std::is_lvalue_reference_v<cloner_type>)
        [[gnu::nonnull]] explicit constexpr cloned_ptr(U*,
                                                       std::remove_reference_t<deleter_type>&&,
                                                       std::remove_reference_t<cloner_type>&&) noexcept
        = delete;

        constexpr cloned_ptr(const cloned_ptr& other)
            requires std::copy_constructible<deleter_type> &&
                     std::copy_constructible<cloner_type>
        : m_del_(other.m_del_), m_clone_(other.m_clone_),
          m_data_(other.mf_clone_()), m_manage_(other.m_manage_) {}

        template <std::derived_from<element_type> U, typename OtherDeleter, typename OtherCloner>
            requires std::constructible_from<deleter_type, const OtherDeleter&> &&
                     std::constructible_from<cloner_type, const OtherCloner&>
        constexpr cloned_ptr(const cloned_ptr<U, OtherDeleter, OtherCloner>& other)
        : m_del_(other.m_del_), m_clone_(other.m_clone_),
          m_data_(other.mf_clone_()), m_manage_(&s_manage_<U>) {}

        constexpr cloned_ptr(cloned_ptr&& other)
            requires std::move_constructible<deleter_type> &&
                     std::move_constructible<cloner_type>
        : m_del_(std::move(other.m_del_)), m_clone_(std::move(other.m_clone_)),
          m_data_(std::exchange(other.m_data_, nullptr)),
          m_manage_(std::exchange(other.m_manage_, nullptr)) {}

        template <std::derived_from<element_type> U, typename OtherDeleter, typename OtherCloner>
            requires std::constructible_from<deleter_type, OtherDeleter> &&
                     std::constructible_from<cloner_type, OtherCloner>
        constexpr cloned_ptr(cloned_ptr<U, OtherDeleter, OtherCloner>&& other)
        : m_del_(std::move(other.m_del_)), m_clone_(std::move(other.m_clone_)),
          m_data_(std::exchange(other.m_data_, nullptr)), m_manage_(&s_manage_<U>) {
            other.m_manage_ = nullptr;
        }

        constexpr ~cloned_ptr() { mf_delete_(); }

        constexpr cloned_ptr& operator=(std::nullptr_t) noexcept {
            reset();
            return *this;
        }

        constexpr cloned_ptr& operator=(pointer item) noexcept {
            reset(item);
            return *this;
        }

        template <std::derived_from<element_type> U>
        constexpr cloned_ptr& operator=(U* item) noexcept {
            reset(item);
            return *this;
        }

        constexpr cloned_ptr& operator=(const cloned_ptr& other) {
            if (this != &other)
                auto(other).swap(*this);
            return *this;
        }

        constexpr cloned_ptr& operator=(cloned_ptr&& other) noexcept {
            if (this != &other)
                auto(std::move(other)).swap(*this);
            return *this;
        }

        constexpr void swap(cloned_ptr& other) noexcept {
            std::ranges::swap(m_del_, other.m_del_);
            std::ranges::swap(m_clone_, other.m_clone_);
            std::ranges::swap(m_data_, other.m_data_);
            std::ranges::swap(m_manage_, other.m_manage_);
        }

        friend constexpr void swap(cloned_ptr& lhs, cloned_ptr& rhs) noexcept {
            lhs.swap(rhs);
        }

        constexpr explicit operator bool() const noexcept { return m_data_; }

        constexpr void reset() noexcept {
            if (*this) {
                mf_delete_();
                m_data_   = nullptr;
                m_manage_ = nullptr;
            }
        }

        constexpr void reset(pointer item) noexcept {
            reset();
            m_data_   = item;
            m_manage_ = &s_manage_<>;
        }

        template <std::derived_from<element_type> U>
        constexpr void reset(U* item) noexcept {
            reset();
            m_data_   = item;
            m_manage_ = &s_manage_<U>;
        }

        constexpr pointer release() noexcept {
            m_manage_ = nullptr;
            return std::exchange(m_data_, nullptr);
        }

        constexpr pointer get() const noexcept { return m_data_; }
        constexpr deleter_type& get_deleter() noexcept { return m_del_; }
        constexpr const deleter_type& get_deleter() const noexcept { return m_del_; }
        constexpr cloner_type& get_cloner() noexcept { return m_clone_; }
        constexpr const cloner_type& get_cloner() const noexcept { return m_clone_; }

        constexpr element_type& operator*() const noexcept { return *m_data_; }
        constexpr element_type* operator->() const noexcept { return m_data_; }

        template <typename U, typename OtherDeleter, typename OtherCloner>
        friend constexpr bool operator==(const cloned_ptr& lhs, const cloned_ptr<U, OtherDeleter, OtherCloner>& rhs) noexcept {
            return lhs.get() == rhs.get();
        }

        constexpr bool operator==(std::nullptr_t) const noexcept {
            return m_data_ == nullptr;
        }

    };

    template <typename T, typename Deleter, typename Cloner>
        requires __memory::array_type<T>
    class cloned_ptr<T, Deleter, Cloner> {
      public:
        using element_type  = std::remove_cv_t<std::remove_extent_t<T>>;
        using pointer       = element_type*;
        using const_pointer = const element_type*;
        using deleter_type  = Deleter;
        using cloner_type   = Cloner;

      private:
        using size_type     = std::size_t;
        using E             = std::remove_extent_t<T>;
        using manage_type   = void (*) (bool, const cloned_ptr*, E**, E*, size_type);

        [[no_unique_address]] deleter_type m_del_   = {};
        [[no_unique_address]] cloner_type  m_clone_ = {};
        pointer     m_data_   = nullptr;
        size_type   m_size_   = 0;
        manage_type m_manage_ = nullptr;

        template <typename U = E>
        static constexpr void s_manage_(bool is_clone, const cloned_ptr* parent, E** this_ptr, E* that_ptr, size_type n) {
            if (is_clone) {
                if constexpr (std::is_same_v<E, U>)
                    *this_ptr = parent->m_clone_(that_ptr, n);
                else
                    *this_ptr = parent->m_clone_(derived<U>, that_ptr, n);
            } else {
                if constexpr (std::has_virtual_destructor_v<E> || std::is_same_v<E, U>)
                    parent->m_del_(*this_ptr, n);
                else
                    parent->m_del_(derived<U>, *this_ptr, n);
            }
        }

        constexpr E* mf_clone_() const {
            if (m_data_ == nullptr)
                return nullptr;
            E* result;
            m_manage_(true, this, &const_cast<pointer&>(result), const_cast<pointer&>(m_data_), m_size_);
            return result;
        }

        constexpr void mf_delete_() const {
            if (m_data_ != nullptr)
                m_manage_(false, this, &const_cast<pointer&>(m_data_), nullptr, m_size_);
        }

        template <typename U, typename OtherDeleter, typename OtherCloner>
        friend class cloned_ptr;

      public:
        constexpr cloned_ptr() noexcept = default;
        constexpr cloned_ptr(std::nullptr_t) noexcept : cloned_ptr() {}

        [[gnu::nonnull]] explicit constexpr cloned_ptr(pointer ptr, size_type n) noexcept
            requires std::default_initializable<deleter_type> &&
                     std::default_initializable<cloner_type>
        : m_data_(ptr), m_size_(n), m_manage_(&s_manage_<>) {}

        [[gnu::nonnull]] explicit constexpr cloned_ptr(pointer ptr,
                                                       size_type n,
                                                       const deleter_type& d,
                                                       const cloner_type&  c) noexcept
            requires (!std::is_reference_v<deleter_type> &&
                      !std::is_reference_v<cloner_type>) &&
                       std::copy_constructible<deleter_type> &&
                       std::copy_constructible<cloner_type>
        : m_del_(d), m_clone_(c), m_data_(ptr), m_size_(n), m_manage_(&s_manage_<>) {}

        [[gnu::nonnull]] explicit constexpr cloned_ptr(pointer ptr,
                                                       size_type n,
                                                       deleter_type&& d,
                                                       cloner_type&&  c) noexcept
            requires (!std::is_reference_v<deleter_type> &&
                      !std::is_reference_v<cloner_type>) &&
                       std::move_constructible<deleter_type> &&
                       std::move_constructible<cloner_type>
        : m_del_(std::move(d)), m_clone_(std::move(c)), m_data_(ptr), m_size_(n), m_manage_(&s_manage_<>) {}

        [[gnu::nonnull]] explicit constexpr cloned_ptr(pointer ptr,
                                                       size_type n,
                                                       deleter_type& d,
                                                       cloner_type&  c) noexcept
            requires (std::is_lvalue_reference_v<deleter_type> &&
                      std::is_lvalue_reference_v<cloner_type>)
        : m_del_(d), m_clone_(c), m_data_(ptr), m_size_(n), m_manage_(&s_manage_<>) {}

        [[gnu::nonnull]] explicit constexpr cloned_ptr(pointer,
                                                       size_type,
                                                       std::remove_reference_t<deleter_type>&&,
                                                       std::remove_reference_t<cloner_type>&&) noexcept
            requires (std::is_lvalue_reference_v<deleter_type> &&
                      std::is_lvalue_reference_v<cloner_type>)
        = delete;

        template <std::derived_from<element_type> U>
            requires std::default_initializable<deleter_type> &&
                     std::default_initializable<cloner_type>
        [[gnu::nonnull]] explicit constexpr cloned_ptr(U* ptr, size_type n) noexcept
        : m_data_(ptr), m_size_(n), m_manage_(&s_manage_<U>) {}

        template <std::derived_from<element_type> U>
            requires (!std::is_reference_v<deleter_type> &&
                      !std::is_reference_v<cloner_type>) &&
                       std::copy_constructible<deleter_type> &&
                       std::copy_constructible<cloner_type>
        [[gnu::nonnull]] explicit constexpr cloned_ptr(U* ptr,
                                                       size_type n,
                                                       const deleter_type& d,
                                                       const cloner_type& c) noexcept
        : m_del_(d), m_clone_(c), m_data_(ptr), m_size_(n), m_manage_(&s_manage_<U>) {}

        template <std::derived_from<element_type> U>
            requires (!std::is_reference_v<deleter_type> &&
                      !std::is_reference_v<cloner_type>) &&
                       std::move_constructible<deleter_type> &&
                       std::move_constructible<cloner_type>
        [[gnu::nonnull]] explicit constexpr cloned_ptr(U* ptr,
                                                       size_type n,
                                                       deleter_type&& d,
                                                       cloner_type&&  c) noexcept
        : m_del_(std::move(d)), m_clone_(std::move(c)), m_data_(ptr), m_size_(n), m_manage_(&s_manage_<U>) {}

        template <std::derived_from<element_type> U>
            requires (std::is_lvalue_reference_v<deleter_type> &&
                      std::is_lvalue_reference_v<cloner_type>)
        [[gnu::nonnull]] explicit constexpr cloned_ptr(U* ptr,
                                                       size_type n,
                                                       deleter_type& d,
                                                       cloner_type&  c) noexcept
        : m_del_(d), m_clone_(c), m_data_(ptr), m_size_(n), m_manage_(&s_manage_<U>) {}

        template <std::derived_from<element_type> U>
            requires (std::is_lvalue_reference_v<deleter_type> &&
                      std::is_lvalue_reference_v<cloner_type>)
        [[gnu::nonnull]] explicit constexpr cloned_ptr(U*,
                                                       size_type,
                                                       std::remove_reference_t<deleter_type>&&,
                                                       std::remove_reference_t<cloner_type>&&) noexcept
        = delete;

        // ...

        constexpr cloned_ptr(const cloned_ptr& other)
            requires std::copy_constructible<deleter_type> &&
                     std::copy_constructible<cloner_type>
        : m_del_(other.m_del_), m_clone_(other.m_clone_),
          m_data_(other.mf_clone_()), m_size_(other.m_size_), m_manage_(other.m_manage_) {}

        template <std::derived_from<element_type> U, typename OtherDeleter, typename OtherCloner>
            requires std::constructible_from<deleter_type, const OtherDeleter&> &&
                     std::constructible_from<cloner_type, const OtherCloner&>
        constexpr cloned_ptr(const cloned_ptr<U, OtherDeleter, OtherCloner>& other)
        : m_del_(other.m_del_), m_clone_(other.m_clone_),
          m_data_(other.mf_clone_()), m_size_(other.m_size_), m_manage_(&s_manage_<U>) {}

        constexpr cloned_ptr(cloned_ptr&& other)
            requires std::move_constructible<deleter_type> &&
                     std::move_constructible<cloner_type>
        : m_del_(std::move(other.m_del_)), m_clone_(std::move(other.m_clone_)),
          m_data_(std::exchange(other.m_data_, nullptr)),
          m_size_(std::exchange(other.m_size_, 0)),
          m_manage_(std::exchange(other.m_manage_, nullptr)) {}

        template <std::derived_from<element_type> U, typename OtherDeleter, typename OtherCloner>
            requires std::constructible_from<deleter_type, OtherDeleter> &&
                     std::constructible_from<cloner_type, OtherCloner>
        constexpr cloned_ptr(cloned_ptr<U, OtherDeleter, OtherCloner>&& other)
        : m_del_(std::move(other.m_del_)), m_clone_(std::move(other.m_clone_)),
          m_data_(std::exchange(other.m_data_, nullptr)), m_size_(std::exchange(other.m_size_, 0)), m_manage_(&s_manage_<U>) {
            other.m_manage_ = nullptr;
        }

        constexpr ~cloned_ptr() { mf_delete_(); }

        constexpr cloned_ptr& operator=(std::nullptr_t) noexcept {
            reset();
            return *this;
        }

        constexpr cloned_ptr& operator=(pointer) = delete;

        template <std::derived_from<element_type> U>
        constexpr cloned_ptr& operator=(U*) = delete;

        constexpr cloned_ptr& operator=(const cloned_ptr& other) {
            if (this != &other)
                auto(other).swap(*this);
            return *this;
        }

        constexpr cloned_ptr& operator=(cloned_ptr&& other) noexcept {
            if (this != &other)
                auto(std::move(other)).swap(*this);
            return *this;
        }

        constexpr void swap(cloned_ptr& other) noexcept {
            std::ranges::swap(m_del_, other.m_del_);
            std::ranges::swap(m_clone_, other.m_clone_);
            std::ranges::swap(m_data_, other.m_data_);
            std::ranges::swap(m_size_, other.m_size_);
            std::ranges::swap(m_manage_, other.m_manage_);
        }

        friend constexpr void swap(cloned_ptr& lhs, cloned_ptr& rhs) noexcept {
            lhs.swap(rhs);
        }

        constexpr explicit operator bool() const noexcept { return m_data_; }

        constexpr void reset() noexcept {
            if (*this) {
                mf_delete_();
                m_data_   = nullptr;
                m_size_   = 0;
                m_manage_ = nullptr;
            }
        }

        constexpr void reset(pointer item, size_type n) noexcept {
            reset();
            m_data_   = item;
            m_size_   = n;
            m_manage_ = &s_manage_<>;
        }

        template <std::derived_from<element_type> U>
        constexpr void reset(U* item, size_type n) noexcept {
            reset();
            m_data_   = item;
            m_size_   = n;
            m_manage_ = &s_manage_<U>;
        }

        constexpr pointer release() noexcept {
            m_size_   = 0;
            m_manage_ = nullptr;
            return std::exchange(m_data_, nullptr);
        }

        constexpr pointer get() const noexcept { return m_data_; }
        constexpr size_type size() const noexcept { return m_size_; }
        constexpr deleter_type& get_deleter() noexcept { return m_del_; }
        constexpr const deleter_type& get_deleter() const noexcept { return m_del_; }
        constexpr cloner_type& get_cloner() noexcept { return m_clone_; }
        constexpr const cloner_type& get_cloner() const noexcept { return m_clone_; }

        constexpr element_type& operator[](size_type n) const noexcept {
            // pre: n < m_size_
            return m_data_[n];
        }

        template <typename U, typename OtherDeleter, typename OtherCloner>
        friend constexpr bool operator==(const cloned_ptr& lhs, const cloned_ptr<U, OtherDeleter, OtherCloner>& rhs) noexcept {
            return lhs.get() == rhs.get();
        }

        constexpr bool operator==(std::nullptr_t) const noexcept {
            return m_data_ == nullptr;
        }

    };

    /// make_cloned
    template <typename T, typename... Args>
    constexpr cloned_ptr<T> make_cloned(Args&&... args) {
        return cloned_ptr<T>(new T(std::forward<Args>(args)...));
    }

    template <typename T>
        requires std::is_unbounded_array_v<T>
    constexpr cloned_ptr<T> make_cloned(std::size_t n) {
        return cloned_ptr<T>(new std::remove_extent_t<T>[n](), n);
    }

    template <typename T, typename E = std::remove_extent_t<T>>
        requires std::is_unbounded_array_v<T>
    constexpr cloned_ptr<T> make_cloned(std::size_t n, const E& val) {
        if (n == 0)
            return cloned_ptr<T>();
        auto p = new E[n];
        auto out = p;
        for (std::size_t i = 0; i != n; ++i)
            *out++ = val;
        return cloned_ptr<T>(p, n);
    }

    template <typename T, typename E = std::remove_extent_t<T>>
        requires std::is_bounded_array_v<T>
    constexpr cloned_ptr<E[]> make_cloned() {
        constexpr std::size_t n = std::extent_v<T>;
        return cloned_ptr<E[]>(new E[n](), n);
    }

    template <typename T, typename E = std::remove_extent_t<T>>
        requires std::is_bounded_array_v<T>
    constexpr cloned_ptr<E[]> make_cloned(const E& val) {
        constexpr std::size_t n = std::extent_v<T>;
        return make_cloned<E[]>(n, val);
    }

    template <typename T, typename E = std::remove_extent_t<T>>
        requires std::is_unbounded_array_v<T>
    constexpr cloned_ptr<T> make_cloned(std::initializer_list<T> il) {
        if (!il.size())
            return cloned_ptr<T>();
        auto p = new E[il.size()];
        auto out = p;
        for (auto iter = il.begin(); iter != il.end(); )
            *out++ = std::move(*iter++);
        return cloned_ptr<T>(p, il.size());
    }

    /// make_cloned_for_overwrite
    template <typename T>
        requires (!std::is_unbounded_array_v<T>)
    constexpr cloned_ptr<T> make_cloned_for_overwrite() {
        return cloned_ptr<T>(new T);
    }

    template <typename T>
        requires std::is_unbounded_array_v<T>
    constexpr cloned_ptr<T> make_cloned_for_overwrite(std::size_t n) {
        return cloned_ptr<T>(new std::remove_extent_t<T>[n], n);
    }

    template <typename T, typename E = std::remove_extent_t<T>>
        requires std::is_bounded_array_v<T>
    constexpr cloned_ptr<E[]> make_cloned_for_overwrite() {
        constexpr std::size_t n = std::extent_v<T>;
        return cloned_ptr<E[]>(new E[n], n);
    }

    /// allocate_cloned_ptr
    template <typename T, typename Alloc, typename... Args>
        requires (!std::is_unbounded_array_v<T>) &&
                   std::is_same_v<T, typename std::allocator_traits<Alloc>::value_type>
    constexpr auto allocate_cloned_ptr(Alloc& alloc, Args&&... args) {
        using Deleter = __memory::deleter_allocator<Alloc>;
        using Cloner  = __memory::cloner_allocator<Alloc>;

        auto ptr = std::allocator_traits<Alloc>::allocate(alloc, 1);

        return cloned_ptr<T, Deleter, Cloner>(
            gold::construct_at(ptr, std::forward<Args>(args)...),
            Deleter (alloc), Cloner (alloc)
        );
    }

    template <typename T, typename Alloc, typename E = std::remove_extent_t<T>>
        requires std::is_unbounded_array_v<T> &&
                 std::is_same_v<E, typename std::allocator_traits<Alloc>::value_type>
    constexpr auto allocate_cloned_ptr(Alloc& alloc, std::size_t n) {
        using Deleter = __memory::deleter_allocator<Alloc>;
        using Cloner  = __memory::cloner_allocator<Alloc>;

        auto ptr = std::allocator_traits<Alloc>::allocate(alloc, n);
        gold::uninit_default_construct_n(ptr, n);

        return cloned_ptr<T, Deleter, Cloner>(
            ptr, n, Deleter (alloc), Cloner (alloc)
        );
    }

    template <typename T, typename Alloc, typename E = std::remove_extent_t<T>>
        requires std::is_unbounded_array_v<T> &&
                 std::is_same_v<E, typename std::allocator_traits<Alloc>::value_type>
    constexpr auto allocate_cloned_ptr(Alloc& alloc, std::size_t n, const E& val) {
        using Deleter = __memory::deleter_allocator<Alloc>;
        using Cloner  = __memory::cloner_allocator<Alloc>;

        auto ptr = std::allocator_traits<Alloc>::allocate(alloc, n);
        gold::uninit_fill_n(ptr, n, val);

        return cloned_ptr<T, Deleter, Cloner>(
            ptr, n, Deleter (alloc), Cloner (alloc)
        );
    }

    template <typename T, typename Alloc, typename E = std::remove_extent_t<T>>
        requires std::is_bounded_array_v<T> &&
                 std::is_same_v<E, typename std::allocator_traits<Alloc>::value_type>
    constexpr auto allocate_cloned_ptr(Alloc& alloc) {
        return allocate_cloned_ptr<E[]>(alloc, std::extent_v<T>);
    }

    template <typename T, typename Alloc, typename E = std::remove_extent_t<T>>
        requires std::is_bounded_array_v<T> &&
                 std::is_same_v<E, typename std::allocator_traits<Alloc>::value_type>
    constexpr auto allocate_cloned_ptr(Alloc& alloc, const E& val) {
        return allocate_cloned_ptr<E[]>(alloc, std::extent_v<T>, val);
    }

} // namespace gold

#endif // __GOLD_BITS_MEMORY_CLONED_PTR_HPP

