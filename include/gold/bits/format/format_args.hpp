// <gold/bits/format/format_args.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FORMAT_FORMAT_ARGS_HPP
#define __GOLD_BITS_FORMAT_FORMAT_ARGS_HPP

#include <gold/bits/__util/empty_t.hpp>
#include <gold/bits/format/fwd.hpp>
#include <gold/bits/format/error.hpp>
#include <gold/bits/format/format_arg_kind.hpp>
#include <gold/bits/memory/ops.hpp>

namespace gold {

    /// format_arg
    class format_arg {
       public:
        class handle {
          private:
            friend class format_arg;

            void* ptr;
            auto (* fn)(void*, format_parse_context&, format_context&) -> __format::result<void>;

          public:
            template <typename T>
            constexpr handle(T&) noexcept;

            constexpr handle(const handle&) = default;

            constexpr __format::result<void> try_format(format_parse_context& pctx, format_context& wctx) {
                return fn(ptr, pctx, wctx);
            }
        };

       private:
        using char_type = char;

        __format::arg_kind m_kind_;
        union {
            __util::empty_t            m_n_;
            bool                       m_b_;
            char                       m_c_;
            gold::int32_t              m_i32_;
            gold::uint32_t             m_u32_;
            gold::int64_t              m_i64_;
            gold::uint64_t             m_u64_;
            gold::int128_t             m_u128_;
            gold::uint128_t            m_i128_;
            gold::float16x_t           m_f16_;
            gold::bfloat16x_t          m_bf16_;
            gold::float32_t            m_f32_;
            gold::float64_t            m_f64_;
            gold::float80_t            m_f80_;
            gold::float128x_t          m_f128_;
            const char*                m_cstr_;
            std::string_view           m_sv_;
            std::strong_ordering       m_strong_ord_;
            std::weak_ordering         m_weak_ord_;
            std::partial_ordering      m_partial_ord_;
            const void*                m_ptr_;
            handle                     m_cust_;
        };

        template <typename T>
        using normalize_type = __format::normalize_type<T, handle>;

        template <typename T, typename Self>
        static constexpr auto& sf_get_(Self& u) noexcept {
            if constexpr (__is_same(__util::empty_t, T))
                return u.m_n_;
            else if constexpr (__is_same(bool, T))
                return u.m_b_;
            else if constexpr (__is_same(char, T))
                return u.m_c_;
            else if constexpr (__is_same(gold::int32_t, T))
                return u.m_i32_;
            else if constexpr (__is_same(gold::int64_t, T))
                return u.m_i64_;
            else if constexpr (__is_same(gold::int128_t, T))
                return u.m_i128_;
            else if constexpr (__is_same(gold::uint32_t, T))
                return u.m_u32_;
            else if constexpr (__is_same(gold::uint64_t, T))
                return u.m_u64_;
            else if constexpr (__is_same(gold::uint128_t, T))
                return u.m_u128_;
            else if constexpr (__is_same(gold::float16x_t, T))
                return u.m_f16_;
            else if constexpr (__is_same(gold::bfloat16x_t, T))
                return u.m_bf16_;
            else if constexpr (__is_same(gold::float32_t, T))
                return u.m_f32_;
            else if constexpr (__is_same(gold::float64_t, T))
                return u.m_f64_;
            else if constexpr (__is_same(gold::float80_t, T))
                return u.m_f80_;
            else if constexpr (__is_same(gold::float128x_t, T))
                return u.m_f128_;
            else if constexpr (__is_same(const char*, T))
                return u.m_cstr_;
            else if constexpr (__is_same(std::string_view, T))
                return u.m_sv_;
            else if constexpr (__is_same(const void*, T))
                return u.m_ptr_;
            else if constexpr (__is_same(std::strong_ordering, T))
                return u.m_strong_ord_;
            else if constexpr (__is_same(std::weak_ordering, T))
                return u.m_weak_ord_;
            else if constexpr (__is_same(std::partial_ordering, T))
                return u.m_partial_ord_;
            else if constexpr (__is_same(handle, T))
                return u.m_cust_;
        }

        template <typename T>
        constexpr void mf_set_(T& op) noexcept {
            using D = normalize_type<T>;
            m_kind_ = __format::type_to_enum<D, handle>();
            if constexpr (__is_same(std::string, __remove_cv(T))) {
                if consteval { gold::construct_at(&sf_get_<D>(*this)); }
                sf_get_<D>(*this) = std::string_view { op.data(), op.size() };
            } else if constexpr (__format::is_char_array<__remove_cv(T)>) {
                // undefined behavior if the null character is also in the middle
                // of the array
                const bool has_null_end = op[std::extent_v<__remove_cv(T)> - 1] == '\0';
                const std::size_t n = std::extent_v<__remove_cv(T)> - (has_null_end ? 1 : 0);
                if consteval { gold::construct_at(&sf_get_<D>(*this)); }
                sf_get_<D>(*this) = std::string_view { op, n };
           } else if constexpr (__is_same(handle, D)) {
                gold::construct_at(&sf_get_<D>(*this), op);
           } else {
                auto* ptr = &sf_get_<D>(*this);
                if consteval {
                    bool active = __builtin_constant_p(*ptr);
                    if (!active)
                        gold::construct_at(ptr, static_cast<D>(op));
                } else {
                    *ptr = static_cast<D>(op);
                }
           }
        }

        template <typename T>
        static constexpr void sf_maybe_construct_(T* ptr) {
            if consteval { gold::construct_at(ptr); }
            else { *ptr = T{}; }
        }

        template <typename T, typename Arg>
        static constexpr void sf_maybe_construct_(T* ptr, Arg&& arg) {
            if consteval { gold::construct_at(ptr, std::forward<Arg>(arg)); }
            else { *ptr = std::forward<Arg>(arg); }
        }

        template <typename T>
        constexpr explicit format_arg(T& op) noexcept {
            mf_set_<T>(op);
        }

        template <typename... Args>
        friend constexpr auto make_format_args(Args&&...) noexcept;

        template <typename F>
        friend constexpr decltype(auto) visit_format_arg(F&&, format_arg);

      public:
        constexpr format_arg() noexcept
        : m_kind_(__format::arg_kind::none_type), m_n_() {}

        constexpr format_arg(const format_arg& other)
        : m_kind_(other.m_kind_) {
            switch (m_kind_) {
                case __format::arg_kind::none_type:
                    m_n_ = {};
                    break;
                case __format::arg_kind::bool_type:
                    m_b_ = other.m_b_;
                    break;
                case __format::arg_kind::char_type:
                    m_c_ = other.m_c_;
                    break;
                case __format::arg_kind::i32_type:
                    m_i32_ = other.m_i32_;
                    break;
                case __format::arg_kind::i64_type:
                    m_i64_ = other.m_i64_;
                    break;
                case __format::arg_kind::i128_type:
                    m_i128_ = other.m_i128_;
                    break;
                case __format::arg_kind::u32_type:
                    m_u32_ = other.m_u32_;
                    break;
                case __format::arg_kind::u64_type:
                    m_u64_ = other.m_u64_;
                    break;
                case __format::arg_kind::u128_type:
                    m_u128_ = other.m_u128_;
                    break;
                case __format::arg_kind::f16_type:
                    m_f16_ = other.m_f16_;
                    break;
                case __format::arg_kind::bf16_type:
                    m_bf16_ = other.m_bf16_;
                    break;
                case __format::arg_kind::f32_type:
                    m_f32_ = other.m_f32_;
                    break;
                case __format::arg_kind::f64_type:
                    m_f64_ = other.m_f64_;
                    break;
                case __format::arg_kind::f80_type:
                    m_f80_ = other.m_f80_;
                    break;
                case __format::arg_kind::f128_type:
                    m_f128_ = other.m_f128_;
                    break;
                case __format::arg_kind::cstring_view_type:
                    m_cstr_ = other.m_cstr_;
                    break;
                case __format::arg_kind::string_view_type:
                    sf_maybe_construct_(&m_sv_, other.m_sv_);
                    break;
                case __format::arg_kind::ptr_type:
                    m_ptr_ = other.m_ptr_;
                    break;
                case __format::arg_kind::strong_ord_type:
                    sf_maybe_construct_(&m_strong_ord_, other.m_strong_ord_);
                    break;
                case __format::arg_kind::weak_ord_type:
                    sf_maybe_construct_(&m_weak_ord_, other.m_weak_ord_);
                    break;
                case __format::arg_kind::partial_ord_type:
                    sf_maybe_construct_(&m_partial_ord_, other.m_partial_ord_);
                    break;
                case __format::arg_kind::custom_type:
                    sf_maybe_construct_(&m_cust_, other.m_cust_);
                default:;
            }
        }

        constexpr ~format_arg() noexcept {
            if (m_kind_ == __format::arg_kind::custom_type)
                gold::destroy_at(&m_cust_);
        }

        constexpr explicit operator bool() const noexcept {
            return m_kind_ != __format::arg_kind::none_type;
        }

    };

   namespace __format {

        /// __format::format_arg_store
        template <typename... Args>
        struct format_arg_store {
            format_arg m_args_[sizeof...(Args)];
        };

        template <>
        struct format_arg_store <> {};

    } // namespace __format

    /// format_args
    class format_args {
      private:
        const format_arg* m_data_;
        std::size_t       m_size_;

      public:
        template <typename... Args>
        constexpr format_args(const __format::format_arg_store<Args...>& op) noexcept
        : m_data_(op.m_args_), m_size_(sizeof...(Args)) {}

        constexpr format_args(__format::format_arg_store<>) noexcept
        : m_data_(nullptr), m_size_(0) {}

        constexpr format_arg at(std::size_t id) const noexcept {
            return id < m_size_ ? m_data_[id] : format_arg();
        }

        constexpr std::size_t size() const noexcept { return m_size_; }
        constexpr auto begin() const noexcept { return m_data_; }
        constexpr auto end() const noexcept { return m_data_ + m_size_; }
    };

    /// make_format_args
    template <typename... Args>
    constexpr auto make_format_args(Args&&... args) noexcept {
        using Store = __format::format_arg_store<
            __format::normalize_type<__remove_reference(Args), gold::format_arg::handle> ...
        >;
        return Store { gold::format_arg(args) ... };
    }

    /// visit_format_arg
    template <typename F>
    constexpr decltype(auto) visit_format_arg(F&& f, format_arg arg) {
        switch (arg.m_kind_) {
            using enum __format::arg_kind;
            case none_type:
                return std::forward<F>(f)(arg.m_n_);
            case bool_type:
                return std::forward<F>(f)(arg.m_b_);
            case char_type:
                return std::forward<F>(f)(arg.m_c_);
            case i32_type:
                return std::forward<F>(f)(arg.m_i32_);
            case i64_type:
                return std::forward<F>(f)(arg.m_i64_);
            case i128_type:
                return std::forward<F>(f)(arg.m_i128_);
            case u32_type:
                return std::forward<F>(f)(arg.m_u32_);
            case u64_type:
                return std::forward<F>(f)(arg.m_u64_);
            case u128_type:
                return std::forward<F>(f)(arg.m_u128_);
            case f16_type:
                return std::forward<F>(f)(arg.m_f16_);
            case bf16_type:
                return std::forward<F>(f)(arg.m_bf16_);
            case f32_type:
                return std::forward<F>(f)(arg.m_f32_);
            case f64_type:
                return std::forward<F>(f)(arg.m_f64_);
            case f80_type:
                return std::forward<F>(f)(arg.m_f80_);
            case f128_type:
                return std::forward<F>(f)(arg.m_f128_);
            case cstring_view_type:
                return std::forward<F>(f)(arg.m_cstr_);
            case string_view_type:
                return std::forward<F>(f)(arg.m_sv_);
            case ptr_type:
                return std::forward<F>(f)(arg.m_ptr_);
            case strong_ord_type:
                return std::forward<F>(f)(arg.m_strong_ord_);
            case weak_ord_type:
                return std::forward<F>(f)(arg.m_weak_ord_);
            case partial_ord_type:
                return std::forward<F>(f)(arg.m_partial_ord_);
            case custom_type:
                return std::forward<F>(f)(arg.m_cust_);
            default:
                __builtin_unreachable();
        }
    }

} // namespace gold

#endif // __GOLD_BITS_FORMAT_FORMAT_ARGS_HPP
