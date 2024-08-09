// <gold/bits/ranges/maybe_view.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RANGES_MAYBE_VIEW_HPP
#define __GOLD_BITS_RANGES_MAYBE_VIEW_HPP

#include <gold/bits/iterator/iter_primitives.hpp>
#include <gold/bits/ranges/view_util.hpp>
#include <gold/bits/type_traits/specialization_of.hpp>

namespace gold::ranges {

    namespace __ranges {

        /// ranges::__ranges::readable_reference
        template <typename Ref, typename CRef>
        concept readable_references =
            std::is_lvalue_reference_v<Ref> &&
            std::is_object_v<std::remove_reference_t<Ref>> &&
            std::is_lvalue_reference_v<CRef> &&
            std::is_object_v<std::remove_reference_t<CRef>> &&
            std::convertible_to<std::add_pointer_t<CRef>,
                                const std::remove_reference_t<Ref>*>;

        /// ranges::__ranges::nullable
        template <typename T>
        concept nullable =
            std::is_object_v<T> &&
            requires (T& t, const T& ct) {
                { static_cast<bool>(ct) };
                { *t  };
                { *ct };
            };

        /// ranges::__ranges::nullable_val
        template <typename T>
        concept nullable_val =
            nullable<T> &&
            readable_references<std::iter_reference_t<T>, std::iter_reference_t<const T>>;

        /// ranges::__ranges::nullable_ref
        template <typename T>
        concept nullable_ref =
            gold::is_specialization_of_v<T, std::reference_wrapper>() &&
            nullable_val<typename T::type>;

        /// ranges::__ranges::movable_object
        template <typename T>
        concept movable_object =
            std::move_constructible<T> &&
            std::is_object_v<T>;

    } // namespace __ranges

    /// ranges::maybe_view
    template <__ranges::movable_object Value>
    class maybe_view : public std::ranges::view_interface<maybe_view<Value>> {
      private:
        std::optional<Value> m_value_;

      public:
        constexpr maybe_view() noexcept = default;

        constexpr explicit maybe_view(const Value& value)
            requires std::copy_constructible<Value>
        : m_value_(value) {}

        constexpr explicit maybe_view(Value&& value)
        : m_value_(std::move(value)) {}

        template <typename... Args>
            requires std::constructible_from<Value, Args...>
        constexpr explicit maybe_view(std::in_place_t, Args&&... args)
        : m_value_(std::in_place, std::forward<Args>(args)...) {}

        constexpr Value* begin() noexcept { return data(); }
        constexpr const Value* begin() const noexcept { return data(); }
        constexpr Value* end() noexcept { return data() + size(); }
        constexpr const Value* end() const noexcept { return data() + size(); }

        constexpr std::size_t size() const noexcept {
            return static_cast<bool>(m_value_);
        }

        constexpr Value* data() noexcept { return std::addressof(*m_value_); }
        constexpr const Value* data() const noexcept { return std::addressof(*m_value_); }

    };

    template <__ranges::movable_object Maybe>
        requires (__ranges::nullable_val<Maybe> || __ranges::nullable_ref<Maybe>)
    class maybe_view<Maybe> : public std::ranges::view_interface<maybe_view<Maybe>> {
      private:
        using T = std::remove_reference_t<std::iter_reference_t<std::unwrap_reference_t<Maybe>>>;

        __ranges::box<Maybe> m_value_;

      public:
        constexpr maybe_view() noexcept = default;

        constexpr explicit maybe_view(const Maybe& maybe)
            requires std::copy_constructible<Maybe>
        : m_value_(maybe) {}

        constexpr explicit maybe_view(Maybe&& maybe)
        : m_value_(std::move(maybe)) {}

        template <typename... Args>
            requires std::constructible_from<Maybe, Args...>
        constexpr explicit maybe_view(std::in_place_t, Args&&... args)
        : m_value_(std::in_place, std::forward<Args>(args)...) {}

        constexpr T* begin() noexcept { return data(); }
        constexpr const T* begin() const noexcept { return data(); }
        constexpr T* end() noexcept { return data() + size(); }
        constexpr const T* end() const noexcept { return data() + size(); }

        constexpr std::size_t size() const noexcept {
            if constexpr (gold::is_specialization_of_v<Maybe, std::reference_wrapper>())
                return static_cast<bool>(m_value_->get());
            else
                return static_cast<bool>(*m_value_);
        }

        constexpr T* data() noexcept {
            Maybe& m = *m_value_;
            if constexpr (gold::is_specialization_of_v<Maybe, std::reference_wrapper>())
                return m.get() ? std::addressof(*m.get()) : nullptr;
            else
                return m ? std::addressof(*m) : nullptr;
        }

        constexpr const T* data() const noexcept {
            const Maybe& m = *m_value_;
            if constexpr (gold::is_specialization_of_v<Maybe, std::reference_wrapper>())
                return m.get() ? std::addressof(*m.get()) : nullptr;
            else
                return m ? std::addressof(*m) : nullptr;
        }

    };

    template <typename T>
    maybe_view(T) -> maybe_view<T>;

    namespace views {

        namespace __views {

            /// ranges::views::__views::can_maybe_view
            template <typename T>
            concept can_maybe_view = requires { maybe_view { std::declval<T>() }; };

            /// ranges::views::__views::maybe_fn
            struct maybe_fn {

                template <typename T>
                    requires can_maybe_view<T>
                [[nodiscard]] constexpr auto operator()(T&& t) const noexcept {
                    return maybe_view{t};
                }

            };

        } // namespace __views

        /// ranges::views::maybe
        inline constexpr __views::maybe_fn maybe {};

    } // namespace views

} // namepace gold::ranges

namespace std::ranges {

    template <typename T>
    inline constexpr bool enable_borrowed_range<gold::ranges::maybe_view<T*>> = true;

    template <typename T>
        requires gold::ranges::__ranges::nullable_val<T>
    inline constexpr bool enable_borrowed_range<gold::ranges::maybe_view<std::reference_wrapper<T>>> = true;

} // namespace std::ranges

#endif // __GOLD_BITS_RANGES_MAYBE_VIEW_HPP
