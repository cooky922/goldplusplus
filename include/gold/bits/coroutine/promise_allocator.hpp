// <gold/bits/coroutine/promise_allocator.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_CORO_PROMISE_ALLOCATOR_HPP
#define __GOLD_BITS_CORO_PROMISE_ALLOCATOR_HPP

#include <cstdint>
#include <bits/alloc_traits.h>
#include <bits/uses_allocator.h>
#include <gold/bits/algo/min_max.hpp>
#include <gold/bits/concepts/allocator.hpp>

namespace gold {

    namespace __coro {

        /// __coro::aligned_block
        struct alignas(__STDCPP_DEFAULT_NEW_ALIGNMENT__) aligned_block {
            unsigned char pad[__STDCPP_DEFAULT_NEW_ALIGNMENT__];
        };

        /// __coro::rebind_alloc
        template <typename Alloc>
        using rebind_alloc = std::allocator_traits<Alloc>::template rebind_alloc<aligned_block>;

        /// [TODO] __coro::has_real_pointers
        // [NOTICE] [removed as of now]

        /// __coro::promise_allocator
        template <typename Alloc = void>
        class promise_allocator {
          private:
            using RebindedAlloc = rebind_alloc<Alloc>;

            inline static constexpr bool s_is_stateless_alloc_ = gold::stateless_allocator<RebindedAlloc>;

            inline static constexpr std::size_t s_align_ =
                __algo::max_element({alignof(RebindedAlloc), sizeof(aligned_block)});

            static void* s_allocate_(RebindedAlloc alloc, std::size_t n) {
                if constexpr (s_is_stateless_alloc_) {
                    const std::size_t count_ = (n + sizeof(aligned_block) - 1) / sizeof(aligned_block);
                    return alloc.allocate(count_);
                } else {
                    const std::size_t count_ = (n + sizeof(RebindedAlloc) + s_align_ - 1) / sizeof(aligned_block);
                    void* ptr_ = alloc.allocate(count_);
                    const auto alloc_address =
                        (reinterpret_cast<std::uintptr_t>(ptr_) + n + alignof(RebindedAlloc) - 1) & ~(alignof(RebindedAlloc) - 1);
                    ::new (reinterpret_cast<void*>(alloc_address)) RebindedAlloc(std::move(alloc));
                    return ptr_;
                }
            }

          public:
            static void* operator new(std::size_t n) requires std::default_initializable<RebindedAlloc> {
                return s_allocate_(RebindedAlloc{}, n);
            }

            template <std::convertible_to<Alloc> Alloc2, typename... Args>
            static void* operator new(std::size_t n, std::allocator_arg_t, Alloc2&& alloc, Args&...) {
                return s_allocate_(
                    static_cast<RebindedAlloc>(static_cast<Alloc>(std::forward<Alloc2>(alloc))), n
                );
            }

            template <typename Self, std::convertible_to<Alloc> Alloc2, typename... Args>
            static void* operator new(std::size_t n, Self&, std::allocator_arg_t, Alloc2& alloc, Args&...) {
                return s_allocate_(
                    static_cast<RebindedAlloc>(static_cast<Alloc>(std::forward<Alloc2>(alloc))), n
                );
            }

            static void operator delete(void* ptr, std::size_t n) noexcept {
                if constexpr (s_is_stateless_alloc_) {
                    RebindedAlloc alloc {};
                    const std::size_t count_ = (n + sizeof(aligned_block) - 1) / sizeof(aligned_block);
                    alloc.deallocate(static_cast<aligned_block*>(ptr), count_);
                } else {
                    const std::size_t count_ = (n + sizeof(RebindedAlloc) + s_align_ - 1) / sizeof(aligned_block);
                    const auto alloc_address =
                        (reinterpret_cast<std::uintptr_t>(ptr) + n + alignof(RebindedAlloc) - 1) & ~(alignof(RebindedAlloc) - 1);
                    auto& stored_alloc = *reinterpret_cast<RebindedAlloc*>(alloc_address);

                    RebindedAlloc alloc { std::move(stored_alloc) };
                    stored_alloc.~RebindedAlloc();
                    alloc.deallocate(static_cast<aligned_block*>(ptr), count_);
                }
            }

        };

        /// __coro::promise_allocator<void>
        // type-erased promise allocator
        template <>
        class promise_allocator<void> {
          private:
            using dealloc_fn = void (*) (void*, std::size_t);

            template <typename Alloc>
            static void* s_allocate_(Alloc alloc, std::size_t n) {
                using RebindedAlloc = rebind_alloc<Alloc>;
                auto rebinded_alloc = static_cast<RebindedAlloc>(alloc);

                if constexpr (stateless_allocator<RebindedAlloc>) {
                    const dealloc_fn dealloc = [](void* ptr, std::size_t n) {
                        RebindedAlloc alloc;
                        const std::size_t count_ = (n + sizeof(dealloc_fn) + sizeof(aligned_block) - 1) / sizeof(aligned_block);
                        alloc.deallocate(static_cast<aligned_block*>(ptr), count_);
                    };
                    const std::size_t count_ = (n + sizeof(dealloc_fn) + sizeof(aligned_block) - 1) / sizeof(aligned_block);
                    void* ptr = rebinded_alloc.allocate(count_);
                    __builtin_memcpy(static_cast<char*>(ptr) + n, &dealloc, sizeof(dealloc));
                    return ptr;
                } else {
                    static constexpr std::size_t align_ = __algo::max_element({alignof(RebindedAlloc), sizeof(aligned_block)});
                    const dealloc_fn dealloc = [](void* ptr, std::size_t n) {
                        n += sizeof(dealloc_fn);
                        const std::size_t count_ = (n + sizeof(RebindedAlloc) + align_ - 1) / sizeof(aligned_block);
                        const auto alloc_address =
                            (reinterpret_cast<std::uintptr_t>(ptr) + n + alignof(RebindedAlloc) - 1) & ~(alignof(RebindedAlloc) - 1);
                        auto& stored_alloc = *reinterpret_cast<const RebindedAlloc*>(alloc_address);
                        RebindedAlloc alloc { std::move(stored_alloc) };
                        stored_alloc.~RebindedAlloc();
                        alloc.deallocate(static_cast<aligned_block*>(ptr), count_);
                    };
                    const std::size_t count_ =
                        (n + sizeof(dealloc_fn) + sizeof(RebindedAlloc) + align_ - 1) / sizeof(aligned_block);
                    void* ptr = rebinded_alloc.allocate(count_);
                    __builtin_memcpy(static_cast<char*>(ptr) + n, &dealloc, sizeof(dealloc));
                    n += sizeof(dealloc_fn);
                    const auto alloc_address =
                        (reinterpret_cast<std::uintptr_t>(ptr) + n + alignof(RebindedAlloc) - 1) & ~(alignof(RebindedAlloc) - 1);
                    ::new (reinterpret_cast<void*>(alloc_address)) RebindedAlloc { std::move(rebinded_alloc) };
                    return ptr;
                }
            }

          public:
            static void* operator new(std::size_t n) {
                void* ptr = ::operator new[](n + sizeof(dealloc_fn));
                const dealloc_fn dealloc = [](void* ptr, std::size_t n) {
                    ::operator delete[](ptr, n + sizeof(dealloc_fn));
                };
                __builtin_memcpy(static_cast<char*>(ptr) + n, &dealloc, sizeof(dealloc));
                return ptr;
            }

            template <typename Alloc, typename... Args>
            static void* operator new(std::size_t n, std::allocator_arg_t, const Alloc& alloc, Args&...) {
                return s_allocate_(alloc, n);
            }

            template <typename Self, typename Alloc, typename... Args>
            static void* operator new(std::size_t n, Self&, std::allocator_arg_t, const Alloc& alloc, Args&...) {
                return s_allocate_(alloc, n);
            }

            static void operator delete(void* ptr, std::size_t n) noexcept {
                dealloc_fn dealloc;
                __builtin_memcpy(&dealloc, static_cast<const char*>(ptr) + n, sizeof(dealloc));
                dealloc(ptr, n);
            }
        };

    } // namespace __coro

} // namespace gold

#endif // __GOLD_BITS_CORO_PROMISE_ALLOCATOR_HPP
