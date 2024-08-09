// <gold/bits/mdspan/index_cartesian_product.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_MDSPAN_INDEX_CARTESIAN_PRODUCT_HPP
#define __GOLD_BITS_MDSPAN_INDEX_CARTESIAN_PRODUCT_HPP

#include <gold/bits/mdspan/extents.hpp>

namespace gold::__mdspan {

    /// __mdspan::index_cartesian_product
    template <typename IndexType, std::size_t... Extents>
    struct index_cartesian_product {
        using extents_type = gold::extents<IndexType, Extents...>;

        const extents_type* m_exts_;

        constexpr index_cartesian_product(const extents_type& exts) noexcept
        : m_exts_(&exts) {}

        struct iterator {
            using iterator_concept = std::input_iterator_tag;
            using value_type       = std::array<IndexType, extents_type::rank()>;
            using reference        = const value_type&;
            using difference_type  = std::ptrdiff_t;

            using Parent = index_cartesian_product;

            const Parent* m_parent_  = nullptr;
            // TODO: zero fill m_current_
            value_type    m_current_ = value_type();
            bool          m_done_    = false;

            constexpr iterator& operator++() {
                using rank_type = extents_type::rank_type;

                for (rank_type r = extents_type::rank() - 1;; --r) {
                    auto& val = m_current_[r];
                    auto  ext = m_parent_->m_exts_->extent(r);
                    ++val;
                    if (val == ext) {
                        val = 0;
                        if (r == 0) {
                            m_done_ = true;
                            break;
                        }
                    } else {
                        break;
                    }
                }

                return *this;
            }

            constexpr iterator operator++(int) {
                auto self = *this;
                ++*this;
                return self;
            }

            constexpr reference operator*() const noexcept {
                return m_current_;
            }

            constexpr bool operator==(std::default_sentinel_t) const noexcept {
                return m_done_;
            }

        };

        constexpr iterator begin() const noexcept {
            return iterator { .m_parent_ = this };
        }

        static constexpr std::default_sentinel_t end() noexcept {
            return std::default_sentinel;
        }
    };

    template <typename IndexType, std::size_t... Extents>
    index_cartesian_product(const gold::extents<IndexType, Extents...>&)
    -> index_cartesian_product<IndexType, Extents...>;

} // namespace gold::__mdspan

#endif // __GOLD_BITS_MDSPAN_INDEX_CARTESIAN_PRODUCT_HPP
