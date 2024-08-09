// <gold/bits/concepts/stream.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_CONCEPTS_STREAM_HPP
#define __GOLD_BITS_CONCEPTS_STREAM_HPP

#include <iosfwd>
#include <bits/ranges_base.h>

#include <gold/bits/concepts/string.hpp>

namespace gold {

    /// basic_stream_insertable
    template <typename T, typename CharT, typename Traits = std::char_traits<CharT>>
    concept basic_stream_insertable = requires (std::basic_ostream<CharT, Traits>& os, const T& t) {
        {(os << t)} -> std::same_as<std::basic_ostream<CharT, Traits>&>;
    } && char_type<CharT>;

    /// basic_stream_extractable
    template <typename T, typename CharT, typename Traits = std::char_traits<CharT>>
    concept basic_stream_extractable = requires (std::basic_istream<CharT, Traits>& is, T& t) {
        {(is >> t)} -> std::same_as<std::basic_istream<CharT, Traits>&>;
    } && char_type<CharT>;

    /// stream_insertable
    template <typename T>
    concept stream_insertable = basic_stream_insertable<T, char>;

    /// wstream_insertable
    template <typename T>
    concept wstream_insertable = basic_stream_insertable<T, wchar_t>;

    /// stream_extractable
    template <typename T>
    concept stream_extractable = basic_stream_extractable<T, char>;

    /// wstream_extractable
    template <typename T>
    concept wstream_extractable = basic_stream_extractable<T, wchar_t>;

    /// range_basic_stream_insertable
    template <typename T, typename C>
    concept range_basic_stream_insertable =
        std::ranges::range<T>
     && std::ranges::sized_range<T>
     && !basic_string_like<T, C>
     && basic_stream_insertable<std::ranges::range_value_t<T>, C>
     && char_type<C>;

    /// range_stream_insertable
    template <typename T>
    concept range_stream_insertable = range_basic_stream_insertable<T, char>;

    /// range_wstream_insertable
    template <typename T>
    concept range_wstream_insertable = range_basic_stream_insertable<T, wchar_t>;


} // namespace gold

#endif // __GOLD_BITS_CONCEPTS_STREAM_HPP
