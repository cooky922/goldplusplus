// <gold/bits/ranges/range_adaptor.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RANGES_RANGE_ADAPTOR_HPP
#define __GOLD_BITS_RANGES_RANGE_ADAPTOR_HPP 1

#include <bits/utility.h>
#include <bits/ranges_base.h>
#if __has_include(<bits/ranges_adaptor.h>)
#include <bits/ranges_adaptor.h>
#else
#include <ranges>
#endif
#include <bits/refwrap.h>
#include <gold/bits/__util/like_t.hpp>

namespace gold::ranges {

    namespace __ranges {

        /// ranges::__ranges::uneval_range
        struct uneval_range {
            const int* begin();
            const int* end();
        };

    } // namespace __ranges

    /// ranges::range_adaptor_closure_object
    // every range adaptor closure object inherits primary base class 'std::views::__adaptor::_RangeAdaptorClosure'
    // and it has following properties
    // - it must not be a range
    // - it must be a unary invocable taking a range parameter
    // then it benefits the following operations
    //  { R | closure } which is same as { closure(R) }
    //  { closure | another_closure } which returns another range adaptor closure object to compose
    template <typename T>
    concept range_adaptor_closure_object =
        std::views::__adaptor::__is_range_adaptor_closure<T> &&
        !std::ranges::range<T>;

    /// ranges::range_adaptor_closure [fwd]
    template <typename>
    struct range_adaptor_closure;

    /// ranges::range_adaptor [fwd]
    template <typename>
    struct range_adaptor;

    namespace __ranges {

        /// ranges::__ranges::closure_has_simple_call_op
        // True if the range adaptor closure Adaptor has a simple operator(), i.e.
        // one that's not overloaded according to constness or value category of the
        // Adaptor object.
        template <typename Closure>
        concept closure_has_simple_call_op = Closure::has_simple_call_op;

        /// ranges::__ranges::adaptor_has_simple_extra_args
        // True if the behavior of the range adaptor non-closure Adaptor is
        // independent of the value category of its extra arguments Args.
        template <typename Adaptor, typename... Args>
        concept adaptor_has_simple_extra_args = Adaptor::has_simple_extra_args
            || Adaptor::template has_simple_extra_args<Args...>;

        /// ranges::__ranges::adaptor_invocable
        // True if the range adaptor Adaptor can be applied with Args.
        template <typename Adaptor, typename... Args>
        concept adaptor_invocable = requires { std::declval<Adaptor>()(std::declval<Args>()...); };

        /// ranges::__ranges::defined_has_simple_extra_args_base
        template <typename Adaptor>
        struct defined_has_simple_extra_args_base {

            template <typename... Args>
            inline static constexpr bool _S_has_simple_extra_args = [] {
                if constexpr (requires { Adaptor::template has_simple_extra_args<Args...>; })
                    return Adaptor::template has_simple_extra_args<Args...>;
                else if constexpr (requires { Adaptor::template _S_has_simple_extra_args<Args...>; })
                    return Adaptor::template _S_has_simple_extra_args<Args...>;
                else
                    return (std::is_trivially_copyable_v<Args> && ...);
            };

            template <typename... Args>
            inline static constexpr bool has_simple_extra_args = _S_has_simple_extra_args<Args...>;

        };

        template <typename Adaptor>
            requires requires { Adaptor::has_simple_extra_args; }
                  || requires { Adaptor::_S_has_simple_extra_args; }
        struct defined_has_simple_extra_args_base<Adaptor> {

            inline static constexpr bool _S_has_simple_extra_args = []{
                if constexpr (requires { Adaptor::has_simple_extra_args; })
                    return Adaptor::has_simple_extra_args;
                else
                    return Adaptor::_S_has_simple_extra_args;
            }();

            inline static constexpr bool has_simple_extra_args = _S_has_simple_extra_args;

        };

        /// ranges::__ranges::defined_arity_base
        template <typename Adaptor>
        struct defined_arity_base {};

        template <typename Adaptor>
            requires requires { { Adaptor::arity } -> std::convertible_to<int>; }
                  || requires { { Adaptor::_S_arity } -> std::convertible_to<int>; }
        struct defined_arity_base<Adaptor> {
            inline static constexpr int _S_arity = []{
                if constexpr (requires { { Adaptor::arity } -> std::convertible_to<int>; })
                    return Adaptor::arity;
                else
                    return Adaptor::_S_arity;
            }();
            inline static constexpr int arity = _S_arity;
        };

        /// ranges::__ranges::partial
        // A range adaptor closure that represents partial application of
        // the range adaptor Adaptor with arguments BoundedArgs
        template <typename Adaptor, typename... BoundedArgs>
        struct partial : std::views::__adaptor::_RangeAdaptorClosure<partial<Adaptor, BoundedArgs...>> {
            Adaptor m_adaptor_ = Adaptor();
            [[no_unique_address]] std::tuple<BoundedArgs...> m_args_ = std::tuple<BoundedArgs...>();

            inline static constexpr bool _S_has_simple_call_op = false;

            template <typename Self, typename R>
            static constexpr bool adaptor_invocable_from_this =
                std::is_same_v<std::remove_cvref_t<Self>, partial> &&
                __ranges::adaptor_invocable<__util::merge_cvref_t<Adaptor, Self>, R,
                                            __util::merge_cvref_t<BoundedArgs, Self>...>;

            constexpr partial() noexcept
                requires std::default_initializable<Adaptor>
                     && (std::default_initializable<BoundedArgs> && ...)
            = default;

            constexpr partial(Adaptor adaptor, BoundedArgs... args)
            : m_adaptor_(std::move(adaptor)), m_args_(std::move(args)...) {}

            // Invoke Adaptor with arguments r, m_args_...
            // according to the value category of this partial object
            template <typename Self, typename R>
                requires adaptor_invocable_from_this<Self, R>
            constexpr auto operator()(this Self&& self, R&& r) {
                auto forwarder = [&self, &r](auto&... args) {
                    return std::forward_like<Self>(self.m_adaptor_)(std::forward<R>(r),
                                                                    std::forward_like<Self>(args)...);
                };
                return std::apply(forwarder, self.m_args_);
            }
        };

        template <typename Adaptor, typename... BoundedArgs>
            requires std::is_empty_v<Adaptor>
                  && std::is_trivial_v<Adaptor>
        struct partial<Adaptor, BoundedArgs...> : std::views::__adaptor::_RangeAdaptorClosure<partial<Adaptor, BoundedArgs...>> {
            [[no_unique_address]] std::tuple<BoundedArgs...> m_args_ = std::tuple<BoundedArgs...>();

            inline static constexpr bool _S_has_simple_call_op =
                   __ranges::adaptor_has_simple_extra_args<
                     __ranges::defined_has_simple_extra_args_base<Adaptor>,
                     BoundedArgs...
                   >
                && std::is_trivially_copyable_v<BoundedArgs...>;

            template <typename Self, typename R>
            static constexpr bool adaptor_invocable_from_this =
                std::is_same_v<std::remove_cvref_t<Self>, partial> &&
                __ranges::adaptor_invocable<Adaptor, R, __util::merge_cvref_t<BoundedArgs, Self>...>;

            constexpr partial() noexcept
                requires (std::default_initializable<BoundedArgs> && ...)
            = default;

            constexpr partial(BoundedArgs... args)
            : m_args_(std::move(args)...) {}

            // Invoke Adaptor with arguments r, m_args_...
            // regardless of the value category of this partial object
            template <typename R>
                requires _S_has_simple_call_op
                      && __ranges::adaptor_invocable<Adaptor, R, const BoundedArgs&...>
            constexpr auto operator()(R&& r) const {
                auto forwarder = [&r](const auto&... args) {
                    return Adaptor{}(std::forward<R>(r), args...);
                };
                return std::apply(forwarder, m_args_);
            }

            // Invoke Adaptor with arguments r, m_args_...
            // according to the value category of this partial object
            template <typename Self, typename R>
                requires adaptor_invocable_from_this<Self, R>
            constexpr auto operator()(this Self&& self, R&& r) {
                auto forwarder = [&r](auto&... args) {
                    return Adaptor{}(std::forward<R>(r), std::forward_like<Self>(args)...);
                };
                return std::apply(forwarder, self.m_args_);
            }
        };

        template <typename Adaptor, typename BoundedArg>
            requires std::is_empty_v<Adaptor>
                  && std::is_trivial_v<Adaptor>
        struct partial<Adaptor, BoundedArg> : std::views::__adaptor::_RangeAdaptorClosure<partial<Adaptor, BoundedArg>> {
            [[no_unique_address]] BoundedArg m_arg_ = BoundedArg();

            inline static constexpr bool _S_has_simple_call_op =
                   __ranges::adaptor_has_simple_extra_args<
                     __ranges::defined_has_simple_extra_args_base<Adaptor>,
                     BoundedArg
                   >
                && std::is_trivially_copyable_v<BoundedArg>;

            template <typename Self, typename R>
            static constexpr bool adaptor_invocable_from_this =
                std::is_same_v<std::remove_cvref_t<Self>, partial> &&
                __ranges::adaptor_invocable<Adaptor, R, __util::merge_cvref_t<BoundedArg, Self>>;

            constexpr partial() noexcept requires std::default_initializable<BoundedArg> = default;

            constexpr partial(BoundedArg arg)
            : m_arg_(std::move(arg)) {}

            template <typename R>
                requires _S_has_simple_call_op
                      && __ranges::adaptor_invocable<Adaptor, R, const BoundedArg&>
            constexpr auto operator()(R&& r) const {
                return Adaptor{}(std::forward<R>(r), m_arg_);
            }

            template <typename Self, typename R>
                requires adaptor_invocable_from_this<Self, R>
            constexpr auto operator()(this Self&& self, R&& r) {
                return Adaptor{}(std::forward<R>(r), std::forward_like<Self>(self.m_arg_));
            }
        };

        template <typename Adaptor, typename BoundedArg>
            requires (!std::is_empty_v<Adaptor> || !std::is_trivial_v<Adaptor>)
        struct partial<Adaptor, BoundedArg> : std::views::__adaptor::_RangeAdaptorClosure<partial<Adaptor, BoundedArg>> {
            Adaptor m_adaptor_ = Adaptor();
            [[no_unique_address]] BoundedArg m_arg_ = BoundedArg();

            inline static constexpr bool _S_has_simple_call_op = false;

            template <typename Self, typename R>
            static constexpr bool adaptor_invocable_from_this =
                std::is_same_v<std::remove_cvref_t<Self>, partial> &&
                __ranges::adaptor_invocable<__util::merge_cvref_t<Adaptor, Self>, R,
                                            __util::merge_cvref_t<BoundedArg, Self>>;

            constexpr partial() noexcept
                requires std::default_initializable<Adaptor>
                      && std::default_initializable<BoundedArg>
            = default;

            constexpr partial(Adaptor adaptor, BoundedArg arg)
            : m_adaptor_(std::move(adaptor)), m_arg_(std::move(arg)) {}

            template <typename Self, typename R>
                requires adaptor_invocable_from_this<Self, R>
            constexpr auto operator()(this Self&& self, R&& r) {
                return std::forward_like<Self>(self.m_adaptor_)(std::forward<r>(r), std::forward_like<Self>(self.m_arg_));
            }
        };

        /// ranges::__ranges::adaptor_partial_app_viable_impl
        template <typename Adaptor, typename... Args>
        concept adaptor_partial_app_viable_impl =
             requires { { Adaptor::arity } -> std::convertible_to<int>; }
             && (Adaptor::arity > 1)
             && (sizeof...(Args) == Adaptor::arity - 1)
             && (std::constructible_from<std::decay_t<Args>, Args> && ...);

        /// ranges::__ranges::adaptor_partial_app_viable
        // True if the range adaptor non-closure Adaptor can be partially applied
        // with Args.
        template <typename Adaptor, typename... Args>
        concept adaptor_partial_app_viable =
            adaptor_partial_app_viable_impl<std::remove_cvref_t<Adaptor>, Args...>
         || __ranges::adaptor_invocable<__ranges::partial<Adaptor, Args...>, __ranges::uneval_range&>;

    } // namespace __ranges

    /// ranges::range_adaptor_closure
    // this is not a base class but solely a class template,
    // to wrap closures that satisfies gold::ranges::range_adaptor_closure_object

    // the template parameter Closure may define optional static data member 'has_simple_call_op'
    // if the behavior of this adaptor is independent of the value category
    // of the adaptor object
    // if 'Closure::has_simple_call_op' is not valid, then
    // its 'has_simple_call_op' is true when Closure is both empty and trivial type.

    // every specialiation of ranges::range_adaptor_closure
    // satisfies concept ranges::range_adaptor_closure_object

    // every specialization of this type is also final
    // which means you cannot derive from it
    template <typename Closure>
    struct range_adaptor_closure final : std::views::__adaptor::_RangeAdaptorClosure<Closure> {
        Closure m_closure_ = Closure();

        inline static constexpr bool _S_has_simple_call_op = [] {
            if constexpr (requires { { Closure::has_simple_call_op } -> std::convertible_to<bool>; })
                return Closure::has_simple_call_op;
            else if constexpr (requires { Closure::_S_has_simple_call_op; })
                return Closure::_S_has_simple_call_op;
            else
                return false;
        }();
        inline static constexpr bool has_simple_call_op = _S_has_simple_call_op;

        template <typename Self, typename R>
        static constexpr bool adaptor_invocable_from_this =
            std::is_same_v<std::remove_cvref_t<Self>, range_adaptor_closure> &&
            __ranges::adaptor_invocable<__util::merge_cvref_t<Closure, Self>, R>;

        constexpr range_adaptor_closure() requires std::default_initializable<Closure> = default;

        constexpr range_adaptor_closure(Closure closure)
        : m_closure_(std::move(closure)) {}

        template <typename... Args>
            requires std::constructible_from<Closure, Args...>
        explicit constexpr range_adaptor_closure(std::in_place_t, Args&&... args)
        : m_closure_(std::forward<Args>(args)...) {}

        template <typename R>
            requires _S_has_simple_call_op && __ranges::adaptor_invocable<const Closure&, R>
        constexpr auto operator()(R&& r) const {
            return m_closure_(std::forward<R>(r));
        }

        template <typename Self, typename R>
            requires adaptor_invocable_from_this<Self, R>
        constexpr auto operator()(this Self&& self, R&& r) {
            return std::forward_like<Self>(self.m_closure_)(std::forward<R>(r));
        }
    };

    template <typename Closure>
        requires std::is_empty_v<Closure>
              && std::is_trivial_v<Closure>
    struct range_adaptor_closure<Closure> final : std::views::__adaptor::_RangeAdaptorClosure<Closure> {
        inline static constexpr bool _S_has_simple_call_op = true;
        inline static constexpr bool has_simple_call_op = _S_has_simple_call_op;

        constexpr range_adaptor_closure() noexcept = default;

        explicit constexpr range_adaptor_closure(std::in_place_t) noexcept {}

        template <typename R>
            requires __ranges::adaptor_invocable<Closure, R>
        static constexpr auto operator()(R&& r) {
            return Closure{}(std::forward<R>(r));
        }
    };

    template <typename Closure>
    range_adaptor_closure(Closure) -> range_adaptor_closure<Closure>;

    /// ranges::range_adaptor
    // the class template of range adaptor non-closure

    // the template paramater Adaptor may define 'Adaptor::arity' that must
    // contain the total number of arguments the adaptor takes

    //// [TODO]
    // if 'Adaptor::arity' is equal to 1,
    // then the adaptor object is range adaptor closure

    // for partial application with extra arguments, the result satisfies
    // ranges::range_adaptor_closure_object

    // if 'Adaptor::arity' is not present, then it checks
    // with '__ranges::adaptor_invocable<__ranges::partial<Adaptor, Args...>, __ranges::uneval_range&>'
    // for partial application

    // 'Adaptor' may define optional static data member 'Adaptor::has_simple_extra_args'.
    // If it is true, then the behavior of this adaptor is independent of the
    // value category of the extra arguments. This data member could also
    // be defined as a variable template parameterized by the types of
    // the extra arguments. If 'Adaptor::has_simple_extra_args' is not present,
    // then it checks all Args to be trivially copyable in order
    // to yield true
    template <typename Adaptor>
    struct range_adaptor final
    : __ranges::defined_has_simple_extra_args_base<Adaptor>,
      __ranges::defined_arity_base<Adaptor> {
        Adaptor m_adaptor_ = Adaptor();

        constexpr range_adaptor() requires std::default_initializable<Adaptor> = default;

        constexpr range_adaptor(Adaptor adaptor)
        : m_adaptor_(std::move(adaptor)) {}

        template <typename... Args>
            requires __ranges::adaptor_partial_app_viable<Adaptor&, Args...>
        constexpr auto operator()(Args&&... args) & {
            return __ranges::partial<std::reference_wrapper<Adaptor>, std::decay_t<Args>...>{
                std::ref(m_adaptor_), std::forward<Args>(args)...
            };
        }

        template <typename... Args>
            requires __ranges::adaptor_partial_app_viable<const Adaptor&, Args...>
        constexpr auto operator()(Args&&... args) const& {
            return __ranges::partial<std::reference_wrapper<const Adaptor>, std::decay_t<Args>...>{
                std::cref(m_adaptor_), std::forward<Args>(args)...
            };
        }

        template <typename... Args>
            requires __ranges::adaptor_partial_app_viable<Adaptor, Args...>
        constexpr auto operator()(Args&&... args) && {
            return __ranges::partial<Adaptor, std::decay_t<Args>...>{
                std::move(m_adaptor_), std::forward<Args>(args)...
            };
        }

        template <typename... Args>
            requires __ranges::adaptor_partial_app_viable<const Adaptor, Args...>
        constexpr auto operator()(Args&&... args) const&& {
            return __ranges::partial<const Adaptor, std::decay_t<Args>...>{
                std::move(m_adaptor_), std::forward<Args>(args)...
            };
        }

        template <typename... Args>
            requires __ranges::adaptor_invocable<Adaptor&, Args...>
        constexpr auto operator()(Args&&... args) & {
            return m_adaptor_(std::forward<Args>(args)...);
        }

        template <typename... Args>
            requires __ranges::adaptor_invocable<const Adaptor&, Args...>
        constexpr auto operator()(Args&&... args) const& {
            return m_adaptor_(std::forward<Args>(args)...);
        }

        template <typename... Args>
            requires __ranges::adaptor_invocable<Adaptor, Args...>
        constexpr auto operator()(Args&&... args) && {
            return std::move(m_adaptor_)(std::forward<Args>(args)...);
        }

        template <typename... Args>
            requires __ranges::adaptor_invocable<const Adaptor, Args...>
        constexpr auto operator()(Args&&... args) const&& {
            return std::move(m_adaptor_)(std::forward<Args>(args)...);
        }

    };

    template <typename Adaptor>
        requires std::is_empty_v<Adaptor>
              && std::is_trivial_v<Adaptor>
    struct range_adaptor<Adaptor> final
    : __ranges::defined_has_simple_extra_args_base<Adaptor>,
      __ranges::defined_arity_base<Adaptor> {

        template <typename... Args>
            requires __ranges::adaptor_invocable<Adaptor, Args...>
        constexpr auto operator()(Args&&... args) const {
            return Adaptor{}(std::forward<Args>(args)...);
        }

        template <typename... Args>
            requires __ranges::adaptor_partial_app_viable<const Adaptor&, Args...>
        constexpr auto operator()(Args&&... args) const {
            return __ranges::partial<Adaptor, std::decay_t<Args>...>{
                std::forward<Args>(args)...
            };
        }

    };

//    template <typename Adaptor>
//    range_adaptor(Adaptor) -> range_adaptor<Adaptor>;

} // namespace gold::ranges

#endif // __GOLD_BITS_RANGES_RANGE_ADAPTOR_HPP
