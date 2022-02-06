// <gold/bits/min.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_MIN_HPP
#define __GOLD_BITS_MIN_HPP

#include <initializer_list>
#include <concepts>

namespace gold::__detail {

    /// min_
    template <std::copyable T>
    constexpr T min_(std::initializer_list<T> list_) {
        auto iter_begin_   = list_.begin();
        auto&& iter_end_   = list_.end();

        auto result_ = *iter_begin_;
        while (++iter_begin_ != iter_end_) {
            auto&& temp_ = *iter_begin_;
            if (temp_ < result_)
                result_ = temp_;
        }

        return result_;
    }

} // namespace gold::__detail

#endif // __GOLD_BITS_MIN_HPP
