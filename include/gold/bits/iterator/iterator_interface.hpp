// <gold/bits/iterator/iterator_interface.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ITERATOR_ITERATOR_INTERFACE_HPP
#define __GOLD_BITS_ITERATOR_ITERATOR_INTERFACE_HPP

#include <compare>
#include <bits/iterator_concepts.h>
#include <gold/bits/__util/uneval.hpp>
#include <gold/bits/type_traits/conditional.hpp>

namespace gold {

    /// iterator_interface_access
    struct iterator_interface_access {
        template <typename Iter>
        static constexpr auto base(Iter& iter) noexcept -> decltype(iter.base_reference()) {
            return iter.base_reference();
        }
        template <typename Iter>
        static constexpr auto base(const Iter& iter) noexcept -> decltype(iter.base_reference()) {
            return iter.base_reference();
        }
    };

    /// proxy_arrow_result
    template <typename T>
        requires std::is_object_v<T>
    class proxy_arrow_result {
      private:
        T m_value_;

      public:
        constexpr proxy_arrow_result(const T& value) noexcept(noexcept(T(value)))
        : m_value_(value) {}

        constexpr proxy_arrow_result(T&& value) noexcept(noexcept(T(std::move(value))))
        : m_value_(std::move(value)) {}

        constexpr const T* operator->() const noexcept { return std::addressof(m_value_); }
        constexpr T* operator->() noexcept { return std::addressof(m_value_); }
    };

    /// iterator_interface [fwd]
    template <typename IterConcept,
              typename ValueType,
              typename Reference = ValueType&,
              typename Pointer   = ValueType*,
              typename DiffType  = std::ptrdiff_t>
    class iterator_interface;

    namespace __iters {

        /// __iters::iter_concept
        template <typename Iter>
        using iter_concept = std::__detail::__iter_concept<Iter>;

        /// __iters::is_derived_from_iter_interface_fn
        template <typename T, typename C, typename V, typename R, typename P, typename D>
            requires (!std::same_as<T, iterator_interface<C, V, R, P, D>>)
        void is_derived_from_iter_interface_fn(const T&, const iterator_interface<C, V, R, P, D>&);

        /// __iters::is_derived_from_iter_interface
        template <typename T>
        concept is_derived_from_iter_interface = requires (T op) {
            __iters::is_derived_from_iter_interface_fn(op, op);
        };

        /// __iters::base_iter_comparable
        template <typename Iter1, typename Iter2>
        concept base_iter_comparable = requires (Iter1 iter1, Iter2 iter2) {
            iterator_interface_access::base(iter1) == iterator_interface_access::base(iter2);
        };

        /// __iters::base_3way
        template <typename Iter1, typename Iter2 = Iter1>
        concept base_3way = requires (Iter1 iter1, Iter2 iter2) {
            iterator_interface_access::base(iter1) <=> iterator_interface_access::base(iter2);
        };

        /// __iters::iter_sub
        template <typename Iter1, typename Iter2 = Iter1>
        concept iter_sub = requires (Iter1 iter1, Iter2 iter2) {
            typename Iter1::difference_type;
            { iter1 - iter2 } -> std::convertible_to<typename Iter1::difference_type>;
        };

        /// __iters::make_iter_ptr
        template <typename Ptr, typename T>
            requires std::is_pointer_v<Ptr>
        constexpr decltype(auto) make_iter_ptr(T&& value) {
            return std::addressof(value);
        }

        template <typename Ptr, typename T>
        constexpr auto make_iter_ptr(T&& value) {
            return Ptr(std::forward<T>(value));
        }

    } // namespace __iters

    /// iterator_interface
    template <typename IterConcept,
              typename ValueType,
              typename Reference,
              typename Pointer,
              typename DiffType>
    class iterator_interface {
      private:
        static consteval auto sf_iter_cat_() {
            if constexpr (!std::is_reference_v<Reference>)
                return std::input_iterator_tag {};
            else if constexpr (std::derived_from<IterConcept, std::random_access_iterator_tag>)
                return std::random_access_iterator_tag {};
            else if constexpr (std::derived_from<IterConcept, std::bidirectional_iterator_tag>)
                return std::bidirectional_iterator_tag {};
            else
                return std::forward_iterator_tag {};
        }

      public:
        using iterator_concept  = IterConcept;
        using iterator_category = decltype(sf_iter_cat_());
        using value_type        = std::remove_const_t<ValueType>;
        using reference         = Reference;
        using pointer           = gold::conditional_t<
            std::is_same_v<iterator_concept, std::output_iterator_tag>,
            void, Pointer
        >;
        using difference_type   = DiffType;

        /// operator*
        constexpr decltype(auto) operator*(this auto&& self)
            requires requires { *iterator_interface_access::base(self); }
        { return *iterator_interface_access::base(self); }

        /// operator->
        constexpr auto operator->(this auto&& self)
            requires (!std::same_as<pointer, void>) &&
                       std::is_reference_v<reference> &&
                       requires { *self; }
        { return __iters::make_iter_ptr<pointer, reference>(*self); }

        /// operator[]
        constexpr decltype(auto) operator[](this const auto& self, difference_type n)
            requires requires { self + n; }
        {
            auto ret = self;
            ret = ret + n;
            return *ret;
        }

        /// operator++
        constexpr decltype(auto) operator++(this auto& self)
            requires requires { ++iterator_interface_access::base(self); } &&
                     (!requires { self += difference_type(1); })
        {
            ++iterator_interface_access::base(self);
            return self;
        }

        constexpr decltype(auto) operator++(this auto& self)
            requires requires { self += difference_type(1); }
        { return self += difference_type(1); }

        /// operator++(int)
        constexpr auto operator++(this auto& self, int) requires requires { ++self; } {
            if constexpr (std::is_same_v<iterator_concept, std::input_iterator_tag>) {
                ++self;
            } else {
                auto ret = self;
                ++self;
                return ret;
            }
        }

        /// operator+=
        constexpr decltype(auto) operator+=(this auto& self, difference_type n)
            requires requires { iterator_interface_access::base(self) += n; }
        {
            iterator_interface_access::base(self) += n;
            return self;
        }

        /// operator--
        constexpr decltype(auto) operator--(this auto& self)
            requires requires { --iterator_interface_access::base(self); } &&
                     (!requires { self += -difference_type(1); })
        {
            --iterator_interface_access::base(self);
            return self;
        }

        constexpr decltype(auto) operator--(this auto& self)
            requires requires { self += -difference_type(1); }
        {
            return self += -difference_type(1);
        }

        /// operator--(int)
        constexpr auto operator--(this auto& self, int) requires requires { --self; } {
            auto ret = self;
            --self;
            return ret;
        }

        /// operator-=
        constexpr decltype(auto) operator-=(this auto& self, difference_type n)
            requires requires { self += -n; }
        {
            return self += -n;
        }
    };

    /// operator+
    template <__iters::is_derived_from_iter_interface I>
    constexpr auto operator+(I iter, typename I::difference_type n) requires requires { iter += n; } {
        return iter += n;
    }

    template <__iters::is_derived_from_iter_interface I>
    constexpr auto operator+(typename I::difference_type n, I iter) requires requires { iter += n; } {
        return iter += n;
    }

    /// operator-
    template <__iters::is_derived_from_iter_interface I1,
              __iters::is_derived_from_iter_interface I2>
    constexpr auto operator-(I1 lhs, I2 rhs)
        requires requires { iterator_interface_access::base(lhs) -
                            iterator_interface_access::base(rhs); }
    {
        return iterator_interface_access::base(lhs) -
               iterator_interface_access::base(rhs);
    }

    template <__iters::is_derived_from_iter_interface I>
    constexpr auto operator-(I iter, typename I::difference_type n)
        requires requires { iter += -n; }
    {
        return iter += -n;
    }

    /// operator==
    template <__iters::is_derived_from_iter_interface I1,
              __iters::is_derived_from_iter_interface I2>
    constexpr bool operator==(I1 lhs, I2 rhs)
        requires (std::is_convertible_v<I2, I1> || std::is_convertible_v<I1, I2>) &&
                 (__iters::base_iter_comparable<I1, I2> || __iters::iter_sub<I1>)
    {
        if constexpr (__iters::base_iter_comparable<I1, I2>) {
            return iterator_interface_access::base(lhs) ==
                   iterator_interface_access::base(rhs);
        } else {
            return (lhs - rhs) == typename I1::difference_type(0);
        }
    }

    /// operator<=>
    template <__iters::is_derived_from_iter_interface I1,
              __iters::is_derived_from_iter_interface I2>
    constexpr auto operator<=>(I1 lhs, I2 rhs)
        requires (__iters::base_3way<I1, I2> || __iters::iter_sub<I1>)
    {
        if constexpr (__iters::base_3way<I1, I2>) {
            return iterator_interface_access::base(lhs) <=>
                   iterator_interface_access::base(rhs);
        } else {
            using iter_diff_t = typename I1::difference_type;
            const iter_diff_t diff = lhs - rhs;
            return diff < iter_diff_t(0) ? std::strong_ordering::less :
                   iter_diff_t(0) < diff ? std::strong_ordering::greater :
                                           std::strong_ordering::equal;
        }
    }

    /// operator<
    template <__iters::is_derived_from_iter_interface I1,
              __iters::is_derived_from_iter_interface I2>
    constexpr auto operator<(I1 lhs, I2 rhs) requires __iters::iter_sub<I1, I2> {
        return (lhs - rhs) < typename I1::difference_type(0);
    }

    /// operator<=
    template <__iters::is_derived_from_iter_interface I1,
              __iters::is_derived_from_iter_interface I2>
    constexpr auto operator<=(I1 lhs, I2 rhs) requires __iters::iter_sub<I1, I2> {
        return (lhs - rhs) <= typename I1::difference_type(0);
    }

    /// operator>
    template <__iters::is_derived_from_iter_interface I1,
              __iters::is_derived_from_iter_interface I2>
    constexpr auto operator>(I1 lhs, I2 rhs) requires __iters::iter_sub<I1, I2> {
        return (lhs - rhs) > typename I1::difference_type(0);
    }

    /// operator>=
    template <__iters::is_derived_from_iter_interface I1,
              __iters::is_derived_from_iter_interface I2>
    constexpr auto operator>=(I1 lhs, I2 rhs) requires __iters::iter_sub<I1, I2> {
        return (lhs - rhs) >= typename I1::difference_type(0);
    }

    /// proxy_iterator_interface
    template <typename IterConcept,
              typename ValueType,
              typename Reference = ValueType&,
              typename DifferenceType = std::ptrdiff_t>
    using proxy_iterator_interface = iterator_interface<
        IterConcept, ValueType, Reference,
        proxy_arrow_result<Reference>,
        DifferenceType
    >;

    namespace __iters {

        /// __iters::iterator_interface_with_traits_impl
        template <typename Traits>
        struct iterator_interface_with_traits_impl {};

        template <typename Traits>
            requires (!requires { typename Traits::value_type; } &&
                      !requires { typename Traits::reference; })
        struct iterator_interface_with_traits_impl<Traits> {};

        /// __iters::is_iterator_tag
        template <typename Tag>
        inline constexpr bool is_iterator_tag =
            std::derived_from<Tag, std::input_iterator_tag> ||
            std::derived_from<Tag, std::output_iterator_tag>;

        /// __iters::traits_require
        template <typename Traits>
        concept traits_require =
            requires {
                typename Traits::iterator_concept;
                requires __iters::is_iterator_tag<typename Traits::iterator_concept>;
            } ||
            requires {
                typename Traits::underlying_iterator;
            };

        template <__iters::traits_require Traits>
        struct iterator_interface_with_traits_impl<Traits> {
            // iterator_concept
            using iterator_concept = decltype([]{
                if constexpr (requires { requires __iters::is_iterator_tag<typename Traits::iterator_concept>; })
                    return __util::uneval<typename Traits::iterator_concept>;
                else if constexpr (requires { typename __iters::iter_concept<typename Traits::underlying_iterator>; })
                    return __util::uneval<__iters::iter_concept<typename Traits::underlying_iterator>>;
                else
                    return __util::uneval<void>;
            }()());
            // value_type
            using value_type = decltype([]{
                if constexpr (requires { typename Traits::value_type; })
                    return __util::uneval<typename Traits::value_type>;
                else if constexpr (requires { typename Traits::reference; })
                    return __util::uneval<std::remove_cvref_t<typename Traits::reference>>;
                else if constexpr (requires { typename std::iter_value_t<typename Traits::underlying_iterator>; })
                    return __util::uneval<std::iter_value_t<typename Traits::underlying_iterator>>;
                else
                    return __util::uneval<void>;
            }()());
            // reference
            using reference = decltype([]{
                if constexpr (requires { typename Traits::reference; })
                    return __util::uneval<typename Traits::reference>;
                else if constexpr (requires { typename Traits::value_type; })
                    return __util::uneval<typename Traits::value_type&>;
                else if constexpr (requires { typename std::iter_reference_t<typename Traits::underlying_iterator>; })
                    return __util::uneval<std::iter_reference_t<typename Traits::underlying_iterator>>;
                else
                    return __util::uneval<void>;
            }()());
            // pointer
            using pointer = decltype([]{
                if constexpr (requires { typename Traits::pointer; })
                    return __util::uneval<typename Traits::pointer>;
                else
                    return __util::uneval<void>;
            }()());
            // difference_type
            using difference_type = decltype([]{
                if constexpr (requires { typename Traits::difference_type; })
                    return __util::uneval<typename Traits::difference_type>;
                else if constexpr (requires { typename std::iter_difference_t<typename Traits::underlying_iterator>; })
                    return __util::uneval<std::iter_difference_t<typename Traits::underlying_iterator>>;
                else
                    return __util::uneval<std::ptrdiff_t>;
            }()());
            using type = iterator_interface<
                iterator_concept,
                value_type,
                reference,
                pointer,
                difference_type
            >;
        };

    } // namespace __iters

    /// iterator_interface_with_traits
    // traits only define necessary member types
    template <typename Traits>
    using iterator_interface_with_traits =
        __iters::iterator_interface_with_traits_impl<Traits>::type;

} // namespace gold

#endif // __GOLD_BITS_ITERATOR_ITERATOR_INTERFACE_HPP
