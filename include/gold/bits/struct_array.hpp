// <gold/bits/struct_array.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_STRUCT_ARRAY_HPP
#define __GOLD_BITS_STRUCT_ARRAY_HPP

#include <string_view>
#include <array>
#include <bits/ranges_algo.h>

#include <gold/bits/concepts/types.hpp>
#include <gold/bits/value_sequence/factory.hpp>

namespace gold {

    /// struct_array
    template <structural T, std::size_t N>
    struct struct_array {
        /// type alias
        using element_type           = T;
        using value_type             = std::remove_cv_t<T>;
        using size_type              = std::size_t;
        using difference_type        = std::ptrdiff_t;
        using pointer                = value_type*;
        using const_pointer          = const value_type*;
        using reference              = value_type&;
        using const_reference        = const value_type&;
        using iterator               = value_type*;
        using const_iterator         = const value_type*;
        using reverse_iterator       = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        /// underlying data
        T m_data_[N];

        /// constructors
        constexpr struct_array() requires std::default_initializable<value_type> {
            /// 'std::ranges::fill_n' may not be used in 'constexpr'
            /// because '__builtin_memset' is not a constant expression
            for (std::size_t i = 0; i < N; ++i)
                m_data_[i] = T{};
        }

        constexpr struct_array(const value_type(&arg)[N]) noexcept {
            std::ranges::copy_n(arg, N, m_data_);
        }

        constexpr struct_array(std::initializer_list<value_type> args) noexcept {
            std::ranges::copy_n(args.begin(), N, m_data_);
        }

        constexpr struct_array(const std::array<value_type, N>& arr) noexcept {
            std::ranges::copy_n(arr.begin(), N, m_data_);
        }

        constexpr struct_array(const struct_array&) = default;
        constexpr struct_array(struct_array&&) = default;

        /// default destructor
        constexpr ~struct_array() = default;

        /// assignments
        constexpr struct_array& operator=(const struct_array& other) {
            std::ranges::copy_n(other.m_data_, N, m_data_);
            return *this;
        }

        constexpr struct_array& operator=(struct_array&& other) noexcept {
            std::ranges::move(other.m_data_, other.m_data_ + N, m_data_);
            return *this;
        }

        constexpr struct_array& operator=(const value_type(&arg)[N]) {
            std::ranges::copy_n(arg, N, m_data_);
            return *this;
        }

        constexpr struct_array& operator=(const std::initializer_list<value_type>& args) {
            std::ranges::copy_n(args.begin(), args.size(), m_data_);
            return *this;
        }

        /// swap
        constexpr void swap(struct_array& other) noexcept {
            std::ranges::swap_ranges(m_data_, m_data_ + N, other.m_data_, other.m_data_ + N);
        }

        /// iterator access
        constexpr auto begin() noexcept { return iterator{m_data_}; }
        constexpr auto begin() const noexcept { return const_iterator{m_data_}; }
        constexpr auto end() noexcept { return iterator{m_data_ + N}; }
        constexpr auto end() const noexcept { return const_iterator{m_data_ + N}; }
        constexpr auto cbegin() noexcept { return const_iterator{m_data_}; }
        constexpr auto cbegin() const noexcept { return const_iterator{m_data_}; }
        constexpr auto cend() noexcept { return const_iterator{m_data_ + N}; }
        constexpr auto cend() const noexcept { return const_iterator{m_data_ + N}; }
        constexpr auto rbegin() noexcept { return reverse_iterator{m_data_}; }
        constexpr auto rbegin() const noexcept { return const_reverse_iterator{m_data_}; }
        constexpr auto rend() noexcept { return reverse_iterator{m_data_ + N}; }
        constexpr auto rend() const noexcept { return const_reverse_iterator{m_data_ + N}; }
        constexpr auto crbegin() const noexcept { return const_reverse_iterator{m_data_}; }
        constexpr auto crend() const noexcept { return const_reverse_iterator{m_data_ + N}; }

        /// data
        constexpr pointer data() noexcept { return m_data_; }
        constexpr const_pointer data() const noexcept { return m_data_; }

        /// cdata
        constexpr const_pointer cdata() const noexcept { return m_data_; }

        /// size
        constexpr size_type size() const noexcept { return N; }

        /// size_bytes
        constexpr size_type size_bytes() const noexcept { return N * sizeof(value_type); }

        /// operator[]
        constexpr reference operator[](const size_type& pos) noexcept { return m_data_[pos]; }
        constexpr const_reference operator[](const size_type& pos) const noexcept { return m_data_[pos]; }

        /// at
        constexpr reference at(const size_type& pos) {
            if (pos < N)
                return m_data_[pos];
            else
                throw std::out_of_range("indexing request is out of range...");
        }

        constexpr const_reference at(const size_type& pos) const {
            if (pos < N)
                return m_data_[pos];
            else
                throw std::out_of_range("indexing request is out of range...");
        }

        /// front
        constexpr reference front() noexcept { return m_data_[0]; }
        constexpr const_reference front() const noexcept { return m_data_[0]; }

        /// back
        constexpr reference back() noexcept { return m_data_[N - 1]; }
        constexpr const_reference back() const noexcept { return m_data_[N - 1]; }

        /// empty
        [[nodiscard]] constexpr bool empty() const { return false; }

        /// operator==
        constexpr bool operator==(const struct_array&) const = default;

        template <std::size_t M> requires (M != N)
        constexpr bool operator==(const struct_array<T, M>&) const { return false; }

        constexpr bool operator==(const value_type(&arg)[N]) const {
            return *this == struct_array{arg};
        }

        template <std::size_t M> requires (M != N)
        constexpr bool operator==(const value_type(&)[M]) const { return false; }

        /// operator<=>
        constexpr auto operator<=>(const struct_array&) const = default;

        /// fill
        constexpr void fill(const value_type& value) {
            std::ranges::fill_n(m_data_, N, value);
        }

        /// first
        template <std::size_t I>
            requires (I <= N)
        constexpr auto first() {
            if constexpr (I != 0) {
                return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                    return struct_array<value_type, I> { m_data_[Is]... };
                }(gold::make_consecutive_index_sequence<0, I>{});
            } else {
                return struct_array<value_type, 0>{};
            }
        }

        /// last
        template <std::size_t I>
            requires (I <= N)
        constexpr auto last() {
            if constexpr (I != 0) {
                return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                    return struct_array<value_type, I> { m_data_[Is]... };
                }(gold::make_consecutive_index_sequence<N - I, I>{});
            } else {
                return struct_array<value_type, 0>{};
            }
        }

        /// subarray
        template <std::size_t I, std::size_t J>
            requires (I < N && J <= N && I <= J)
        constexpr auto subarray() {
            if constexpr (I == J)
                return struct_array<value_type, 0>{};
            else
                return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                    return struct_array<value_type, J - I> { m_data_[Is]... };
                }(gold::make_consecutive_index_sequence<I, J - I>{});
        }

        /// as_string_view - only if the last element is null
        constexpr auto as_string_view() const noexcept
            requires gold::char_type<value_type>
        {
            if (back() == '\0')
                return std::basic_string_view { data(), size() - 1 };
            else {
                return std::basic_string_view { data(), size() };
            }
        }

        /// get
        template <std::size_t I>
        constexpr value_type& get() & noexcept {
            return m_data_[I];
        }

        template <std::size_t I>
        constexpr value_type&& get() && noexcept {
            return std::move(m_data_[I]);
        }

        template <std::size_t I>
        constexpr const value_type& get() const& noexcept {
            return m_data_[I];
        }

        template <std::size_t I>
        constexpr const value_type&& get() const&& noexcept {
            return std::move(m_data_[I]);
        }

    }; // class template struct_array

    /// specialization for array-size 0
    template <structural T>
    struct struct_array<T, 0> {
        /// type alias
        using element_type           = T;
        using value_type             = std::remove_cv_t<T>;
        using size_type              = std::size_t;
        using difference_type        = std::ptrdiff_t;
        using pointer                = value_type*;
        using const_pointer          = const value_type*;
        using reference              = value_type&;
        using const_reference        = const value_type&;
//        using iterator               = value_type*;
//        using const_iterator         = const value_type*;
//        using reverse_iterator       = std::reverse_iterator<iterator>;
//        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        /// constructors
        constexpr struct_array() requires std::default_initializable<value_type> {}

        constexpr struct_array(const struct_array&) {}
        constexpr struct_array(struct_array&&) {}

        /// assignments
        constexpr struct_array& operator=(const struct_array&) noexcept { return *this; }
        constexpr struct_array& operator=(struct_array&&) noexcept { return *this; }

        /// swap
        constexpr void swap(struct_array&) noexcept {}

        /// data
        constexpr pointer data() const { return nullptr; }

        /// cdata
        constexpr const_pointer cdata() const noexcept { return nullptr; }

        /// size
        constexpr size_type size() const noexcept { return 0; }

        /// size_bytes
        constexpr size_type size_bytes() const noexcept { return 0; }

        /// empty
        constexpr bool empty() const noexcept { return true; }

        /// operator==
        template <typename U, std::size_t M>
        constexpr bool operator==(const struct_array<U, M>&) const noexcept { return false; }

        constexpr bool operator==(const struct_array&) const noexcept { return true; }

        /// operator<=>
        constexpr auto operator<=>(const struct_array&) const = default;

        /// as_string_view
        constexpr auto as_string_view() const noexcept requires gold::char_type<value_type> {
            return "";
        }
    };

    /// deduction guides for struct_array
    template <typename T, std::size_t N>
    struct_array(const T(&)[N]) -> struct_array<T, N>;

    template <typename T, std::same_as<T>... Ts>
    struct_array(T, Ts...) -> struct_array<T, sizeof...(Ts) + 1>;

    template <typename T, std::size_t N>
    struct_array(std::array<T, N>) -> struct_array<T, N>;

    namespace __detail {

        template <typename>
        struct is_struct_array_ : std::false_type {};

        template <typename T, std::size_t N>
        struct is_struct_array_<struct_array<T, N>> : std::true_type {};

    }

    /// concept be_struct_string
    template <typename T>
    concept be_struct_string = __detail::is_struct_array_<T>::value && gold::char_type<typename T::value_type>;

} // namespace gold

/// specialization for tuple-like support
namespace std {

    template <typename T, std::size_t N>
    struct tuple_size<gold::struct_array<T, N>> : integral_constant<std::size_t, N> {};

    template <std::size_t I, typename T, std::size_t N>
    struct tuple_element<I, gold::struct_array<T, N>> : type_identity<T> {};

    namespace ranges {
        template <typename T, std::size_t N>
        inline constexpr bool enable_view<gold::struct_array<T, N>> = true;

        template <typename T, std::size_t N>
        inline constexpr bool enable_borrowed_range<gold::struct_array<T, N>> = true;

    } // namespace ranges

} // namespace std

#endif // __GOLD_BITS_STRUCT_ARRAY_HPP
