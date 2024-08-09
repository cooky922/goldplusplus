// <gold/bits/format/sink.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FORMAT_SINK_HPP
#define __GOLD_BITS_FORMAT_SINK_HPP

#include <string>
#include <string_view>
#include <gold/basic_types>
#include <gold/bits/__util/mini_span.hpp>
#include <gold/bits/algo/copy.hpp>
#include <gold/bits/type_traits/specialization_of.hpp>
#include <gold/bits/format/format_to_n_result.hpp>

namespace gold::__format {

    /// __format::sink
    // Abstract base class for type-erased character sinks.
    // All formatting and output is done via this type's iterator,
    // to reduce the number of different template instantiations.
    struct sink {
        using span_type = __util::mini_span<char>;
        using iterator  = span_type::iterator;

        span_type m_span_;
        iterator  m_next_;

        // called when the span is full, to make more space available
        // pre:  m_next_ != m_span.begin()
        // post: m_next_ != m_span.end()
        virtual void flush() = 0;

        // pre: m_span_.size() != 0
        [[gnu::always_inline]] constexpr explicit sink(span_type sp) noexcept
        : m_span_(sp), m_next_(sp.begin()) {}

        // the portion of the span that has been written to
        [[gnu::always_inline]] constexpr span_type used() const noexcept {
            return m_span_.first(m_next_ - m_span_.begin());
        }

        // the portion of the span that has not been written to
        [[gnu::always_inline]] constexpr span_type unused() const noexcept {
            return m_span_.subspan(m_next_ - m_span_.begin());
        }

        // use the start of the span as the next write position
        [[gnu::always_inline]] constexpr void rewind() noexcept {
            m_next_ = m_span_.begin();
        }

        // replace the current output range
        constexpr void reset(span_type s, std::size_t pos = 0) noexcept {
            m_span_ = s;
            m_next_ = s.begin() + pos;
        }

        // called by the iterator for *it++ = c
        constexpr void write(char c) {
            *m_next_++ = c;
            if (m_next_ - m_span_.begin() == static_cast<signed long long>(m_span_.size())) [[unlikely]]
                flush();
        }

        constexpr void write(std::string_view s) {
            span_type to = unused();
            while (to.size() <= s.size()) {
                s.copy(to.data(), to.size());
                m_next_ += to.size();
                s.remove_prefix(to.size());
                flush();
                to = unused();
            }
            if (s.size()) {
                s.copy(to.data(), s.size());
                m_next_ += s.size();
            }
        }

        sink(const sink&) = delete;
        sink& operator=(const sink&) = delete;

        [[gnu::always_inline]] constexpr sink_iterator out() noexcept;
    };

    /// __format::sink_iterator
    struct sink_iterator {
        __format::sink* m_sink_ = nullptr;

        using iterator_category = std::output_iterator_tag;
        using value_type = void;
        using difference_type = std::ptrdiff_t;
        using pointer = void;
        using reference = void;

        sink_iterator() = default;
        sink_iterator(const sink_iterator&) = default;
        sink_iterator(sink_iterator&&) = default;
        sink_iterator& operator=(const sink_iterator&) = default;
        sink_iterator& operator=(sink_iterator&&) = default;

        [[gnu::always_inline]] constexpr explicit sink_iterator(__format::sink& op)
        : m_sink_(__builtin_addressof(op)) {}

        [[gnu::always_inline]] constexpr sink_iterator& operator=(char c) {
            m_sink_->write(c);
            return *this;
        }

        [[gnu::always_inline]] constexpr sink_iterator& operator=(std::string_view s) {
            m_sink_->write(s);
            return *this;
        }

        [[gnu::always_inline]] constexpr sink_iterator& operator*() { return *this; }
        [[gnu::always_inline]] constexpr sink_iterator& operator++() { return *this; }
        [[gnu::always_inline]] constexpr sink_iterator operator++(int) { return *this; }
    };

    [[gnu::always_inline]] constexpr sink_iterator sink::out() noexcept {
        return sink_iterator(*this);
    }

    /// __format::buf_sink
    struct buf_sink : public sink {
        char m_buf_[32 * sizeof(void*)];

        [[gnu::always_inline]] constexpr buf_sink() noexcept
        : sink(m_buf_) {}
    };

    /// __format::seq_sink
    // Writes to a buffer then appends that to the sequence when it fills up
    template <typename Seq>
    struct seq_sink final : public buf_sink {
        Seq m_seq_;

        constexpr void flush() override {
            auto s = this->used();
            if (s.empty())
                return;
            if constexpr (gold::is_specialization_of_v<Seq, std::basic_string>())
                m_seq_.append(s.data(), s.size());
            else
                m_seq_.insert(m_seq_.end(), s.begin(), s.end());
            this->rewind();
        }

        [[gnu::always_inline]] constexpr seq_sink() noexcept(__is_nothrow_constructible(Seq)) {}
        constexpr seq_sink(Seq&& s) noexcept(__is_nothrow_constructible(Seq, Seq))
        : m_seq_(std::move(s)) {}

        using sink::out;

        constexpr void reserve(std::size_t n) {
            if constexpr (requires { m_seq_.reserve(n); })
                m_seq_.reserve(n);
        }

        constexpr Seq get() && {
            if (this->used().size() != 0)
                seq_sink::flush();
            return std::move(m_seq_);
        }
    };

    // Adapts back_insert_iterator into seq_sink that refers to the existing sequence
    template <typename Seq>
    struct seq_sink<Seq&> final : public buf_sink {
        Seq* m_seq_;

        constexpr void flush() override {
            auto s = this->used();
            if (s.empty())
                return;
            if constexpr (gold::is_specialization_of_v<Seq, std::basic_string>())
                m_seq_->append(s.data(), s.size());
            else
                m_seq_->insert(m_seq_.end(), s.begin(), s.end());
            this->rewind();
        }

        constexpr seq_sink(std::back_insert_iterator<Seq> iter) noexcept
        : m_seq_(iter._M_underlying_container()) {}

        using sink::out;

        constexpr void reserve(std::size_t n) {
            if constexpr (requires { m_seq_->reserve(n); })
                m_seq_->reserve(n);
        }

        constexpr gold::format_to_n_result<std::back_insert_iterator<Seq>> finish() && {
            if (this->used().size() != 0)
                seq_sink::flush();
            std::ptrdiff_t c (m_seq_->size());
            return { std::back_insert_iterator<Seq>(*m_seq_), c };
        }
    };

    /// __format::str_sink
    using str_sink = seq_sink<std::string>;

    /// __format::iter_sink
    // A sink that writes to an output iterator.
    // Writes to a fixed-size buffer and then flushes to the output iterator
    // when the buffer fills up.
    template <typename Out>
    struct iter_sink : public buf_sink {
        Out m_out_;
        std::iter_difference_t<Out> m_max_;
        std::size_t m_count_ = 0;

        constexpr void flush() override {
            auto s = this->used();
            if (m_max_ < 0) // no maximum
                m_out_ = gold::__algo::copy(s, std::move(m_out_));
            else if (m_count_ < static_cast<std::size_t>(m_max_)) {
                auto m = m_max_ - m_count_;
                sink::span_type first;
                if (m < s.size())
                    first = s.first(static_cast<std::size_t>(m));
                else
                    first = s;
                m_out_ = gold::__algo::copy(first, std::move(m_out_));
            }
            this->rewind();
            m_count_ += s.size();
        }

        [[gnu::always_inline]] constexpr explicit iter_sink(Out out, std::iter_difference_t<Out> m = -1)
        : m_out_(std::move(out)), m_max_(m) {}

        using sink::out;

        constexpr gold::format_to_n_result<Out> finish() && {
            if (this->used().size() != 0)
                iter_sink::flush();
            std::iter_difference_t<Out> c (m_count_);
            return { std::move(m_out_), c };
        }
    };

    /// __format::contiguous_iter_sink
    // note: only at runtime
    template <typename Out>
    struct contiguous_iter_sink : public sink {
        Out m_first_;
        std::iter_difference_t<Out> m_max_ = -1;
        std::size_t m_count_ = 0;
        char m_buf_[64];

        void flush() override {
            if (this->unused().size() != 0)
                return; // no need to switch to internal buffer yet

            auto s = this->used();

            if (m_max_ >= 0) {
                m_count_ += s.size();
                this->reset(this->m_buf_);
            } else {
                this->reset({s.data(), s.size() + 1024}, s.size());
            }
        }

        static sink::span_type sf_make_span_(char* ptr, std::iter_difference_t<Out> n, sink::span_type buf) noexcept {
            if (n == 0)
                return buf;

            if (n > 0)
                return {ptr, std::size_t(n)};

            if (std::size_t bytes = __builtin_dynamic_object_size(ptr, 2))
                return {ptr, bytes};

            gold::uint64_t off = reinterpret_cast<gold::uint64_t>(ptr) % 1024;
            n = (1024 - off);

            if (n > 0) [[likely]]
                return {ptr, static_cast<std::size_t>(n)};
            else
                return {ptr, 1};
        }

        explicit contiguous_iter_sink(Out out, std::iter_difference_t<Out> n = -1) noexcept
        : sink(sf_make_span_(std::to_address(out), n, m_buf_)), m_first_(out), m_max_(n) {}

        gold::format_to_n_result<Out> finish() && {
            auto s = this->used();
            if (s.data() == m_buf_) {
                // switched to internal buffer, so must have written m_max_
                std::iter_difference_t<Out> c (m_count_ + s.size());
                return { m_first_ + m_max_, c };
            } else {
                // not using internal buffer yet
                std::iter_difference_t<Out> c (s.size());
                return { m_first_ + c, c };
            }
        }

    };

    /// __format::counting_sink
    struct counting_sink final : iter_sink<char*> {
        constexpr counting_sink() : iter_sink<char*>(nullptr, 0) {}

        [[gnu::always_inline]] constexpr std::size_t count() {
            return this->m_count_ + this->used().size();
        }
    };


} // namespace gold::__format

#endif // __GOLD_BITS_FORMAT_SINK_HPP
