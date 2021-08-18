// <gold/bits/type_traits/detection.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TYPE_TRAITS_DETECTION_HPP
#define __GOLD_BITS_TYPE_TRAITS_DETECTION_HPP

#include <type_traits>

namespace gold {

    namespace __detail {

        // detector_
        template <typename DefaultT, typename AlwaysVoid,
                  template <typename...> typename Op, typename... Args>
        struct detector_ {
            using value_t = std::false_type;
            using type    = DefaultT;
        };

        template <typename DefaultT, template <typename...> typename Op,
                  typename... Args>
        struct detector_<DefaultT, std::void_t<Op<Args...>>, Op, Args...> {
            using value_t = std::true_type;
            using type    = Op<Args...>;
        };

        // nonesuch_
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
        struct nonesuchbase_ {};
        struct nonesuch_ : private nonesuchbase_ {
            ~nonesuch_() = delete;
            nonesuch_(const nonesuch_&) = delete;
            void operator=(const nonesuch_&) = delete;
        };
        #pragma GCC diagnostic pop

        // detected_or_
        template <typename DefaultT, template <typename...> typename Op,
                  typename... Args>
        using detected_or_ = detector_<DefaultT, void, Op, Args...>;

        // detected_or_t_
        template <typename DefaultT, template <typename...> typename Op,
                  typename... Args>
        using detected_or_t_ = typename detected_or_<DefaultT, Op, Args...>::type;

    } // namespace __detail

    /// is_detected
    template <template <typename...> typename Op, typename... Args>
    using is_detected
    = typename __detail::detector_<__detail::nonesuch_, void, Op, Args...>::value_t;

    /// is_detected_v
    template <template <typename...> typename Op, typename... Args>
    inline constexpr bool is_detected_v = is_detected<Op, Args...>::value;

    /// detected_t
    template <template <typename...> typename Op, typename... Args>
    using detected_t
    = typename __detail::detector_<__detail::nonesuch_, void, Op, Args...>::type;

    /// detected_or
    template <typename DefaultT, template <typename...> typename Op,
              typename... Args>
    using detected_or = __detail::detected_or_<DefaultT, Op, Args...>;

    /// detected_or_t
    template <typename DefaultT, template <typename...> typename Op,
              typename... Args>
    using detected_or_t = typename detected_or<DefaultT, Op, Args...>::type;

    /// is_detected_exact
    template <typename ExpectedT, template <typename...> typename Op,
              typename... Args>
    using is_detected_exact = std::is_same<ExpectedT, detected_t<Op, Args...>>;

    /// is_detected_exact_v
    template <typename ExpectedT, template <typename...> typename Op,
              typename... Args>
    inline constexpr bool is_detected_exact_v = is_detected_exact<ExpectedT, Op, Args...>::value;

    /// is_detected_convertible
    template <typename ToT, template <typename...> typename Op,
              typename... Args>
    using is_detected_convertible = std::is_convertible<detected_t<Op, Args...>, ToT>;

    /// is_detected_convertible_v
    template <typename ToT, template <typename...> typename Op,
              typename... Args>
    inline constexpr bool is_detected_convertible_v = is_detected_convertible<ToT, Op, Args...>::value;
}

#endif // __GOLD_BITS_TYPE_TRAITS_DETECTION_HPP
