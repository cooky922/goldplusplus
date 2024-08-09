// <gold/bits/mdspan/mdarray.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_MDSPAN_MDARRAY_HPP
#define __GOLD_BITS_MDSPAN_MDARRAY_HPP

#include <vector>
#include <gold/bits/mdspan/mdspan.hpp>
#include <gold/bits/mdspan/index_cartesian_product.hpp>
#include <gold/bits/type_traits/conditional.hpp>

namespace gold {

    namespace __mdspan {

        /// __mdspan::static_extents_size
        template <typename>
        inline constexpr std::size_t static_extents_size = 0;

        template <typename IndexType, std::size_t... Extents>
        inline constexpr std::size_t static_extents_size<extents<IndexType, Extents...>> = (Extents * ... * 1);

        /// __mdspan::choose_container_type
        template <typename T, typename Extents, typename DynamicContainer>
        using choose_container_type = gold::conditional_t<
            Extents::rank_dynamic() == 0,
            std::array<T, __mdspan::static_extents_size<Extents>>,
            DynamicContainer
        >;

    } // namespace __mdspan

    /// mdarray
    template <typename T,
              typename Extents,
              typename LayoutPolicy = layout_right,
              typename DynamicContainer = std::vector<T>>
    class mdarray {
      public:
        using extents_type      = Extents;
        using layout_type       = LayoutPolicy;
        using container_type    = __mdspan::choose_container_type<T, extents_type, DynamicContainer>;
        using mapping_type      = layout_type::template mapping<extents_type>;
        using element_type      = T;
        using mdspan_type       = mdspan<element_type, extents_type, layout_type>;
        using const_mdspan_type = mdspan<const element_type, extents_type, layout_type>;
        using value_type        = std::remove_cv_t<T>;
        using index_type        = extents_type::index_type;
        using size_type         = extents_type::size_type;
        using rank_type         = extents_type::rank_type;
        using pointer           = decltype(std::to_address(std::declval<container_type>().begin()));
        using reference         = container_type::reference;
        using const_pointer     = decltype(std::to_address(std::declval<container_type>().cbegin()));
        using const_reference   = container_type::const_reference;

        static constexpr rank_type rank() noexcept { return extents_type::rank(); }
        static constexpr rank_type rank_dynamic() noexcept { return extents_type::rank_dynamic(); }
        static constexpr std::size_t static_extent(rank_type r) noexcept { return extents_type::static_extent(r); }
        constexpr index_type extent(rank_type r) const noexcept { return extents().extent(r); }

      private:
        [[no_unique_address]] mapping_type m_map_;
        container_type                     m_ctr_;

        static constexpr bool s_is_container_array_ = (rank_dynamic() == 0);

        static constexpr auto sf_val_to_array_(const value_type& val) requires s_is_container_array_ {
            constexpr std::size_t N = __mdspan::static_extents_size<extents_type>;
            return [&]<std::size_t... Is>(gold::index_seq_t<Is...>*) {
                return container_type { (Is, val) ... };
            }(static_cast<gold::index_seq_t<__integer_pack(N)...>*>(nullptr));
        }

      public:
        constexpr mdarray() requires (rank_dynamic() > 0) = default;
        constexpr mdarray(const mdarray&) = default;
        constexpr mdarray(mdarray&&) = default;

        template <typename... OtherIndexTypes>
            requires ((std::is_convertible_v<OtherIndexTypes, index_type> && ...) &&
                      (std::is_nothrow_constructible_v<index_type, OtherIndexTypes> && ...) &&
                      (sizeof...(OtherIndexTypes) == rank() || sizeof...(OtherIndexTypes) == rank_dynamic()) &&
                       std::is_constructible_v<mapping_type, extents_type> &&
                       !s_is_container_array_ && std::is_constructible_v<container_type, std::size_t>)
        constexpr explicit mdarray(OtherIndexTypes... exts)
        : m_map_(extents_type(static_cast<index_type>(std::move(exts))...)),
          m_ctr_(m_map_.required_span_size()) {
            if consteval { [[assume(m_map_.required_span_size() <= m_ctr_.size())]]; }
        }

        template <typename... OtherIndexTypes>
            requires ((std::is_convertible_v<OtherIndexTypes, index_type> && ...) &&
                      (std::is_nothrow_constructible_v<index_type, OtherIndexTypes> && ...) &&
                      (sizeof...(OtherIndexTypes) == rank() || sizeof...(OtherIndexTypes) == rank_dynamic()) &&
                       s_is_container_array_ && std::is_constructible_v<mapping_type, extents_type>)
        constexpr explicit mdarray(OtherIndexTypes... exts)
        : m_map_(extents_type(static_cast<index_type>(std::move(exts))...)) {
            if consteval { [[assume(m_map_.required_span_size() <= m_ctr_.size())]]; }
        }

        constexpr explicit mdarray(const extents_type& exts)
            requires (std::is_constructible_v<mapping_type, const extents_type&> &&
                      !s_is_container_array_ && std::is_constructible_v<container_type, std::size_t>)
        : m_map_(exts), m_ctr_(m_map_.required_span_size()) {
            if consteval { [[assume(m_map_.required_span_size() <= m_ctr_.size())]]; }
        }

        constexpr explicit mdarray(const extents_type& exts)
            requires (std::is_constructible_v<mapping_type, const extents_type&> &&
                      s_is_container_array_)
        : m_map_(exts), m_ctr_() {
            if consteval { [[assume(m_map_.required_span_size() <= m_ctr_.size())]]; }
        }

        constexpr explicit mdarray(const mapping_type& m)
            requires (!s_is_container_array_ && std::is_constructible_v<container_type, std::size_t>)
        : m_map_(m), m_ctr_(m_map_.required_span_size()) {
            if consteval { [[assume(m_map_.required_span_size() <= m_ctr_.size())]]; }
        }

        constexpr explicit mdarray(const mapping_type& m)
            requires s_is_container_array_
        : m_map_(m), m_ctr_() {
            if consteval { [[assume(m_map_.required_span_size() <= m_ctr_.size())]]; }
        }

        constexpr mdarray(const extents_type& exts, const value_type& val)
            requires (std::is_constructible_v<mapping_type, const extents_type&> &&
                      !s_is_container_array_ && std::is_constructible_v<container_type, std::size_t, value_type>)
        : m_map_(exts), m_ctr_(m_map_.required_span_size(), val) {
            if consteval { [[assume(m_map_.required_span_size() <= m_ctr_.size())]]; }
        }

        constexpr mdarray(const extents_type& exts, const value_type& val)
            requires (std::is_constructible_v<mapping_type, const extents_type&> &&
                      s_is_container_array_)
        : m_map_(exts), m_ctr_(sf_val_to_array_(val)) {
            if consteval { [[assume(m_map_.required_span_size() <= m_ctr_.size())]]; }
        }

        constexpr mdarray(const mapping_type& m, const value_type& val)
            requires (!s_is_container_array_ && std::is_constructible_v<container_type, std::size_t, value_type>)
        : m_map_(m), m_ctr_(m_map_.required_span_size(), val) {
            if consteval { [[assume(m_map_.required_span_size() <= m_ctr_.size())]]; }
        }

        constexpr mdarray(const mapping_type& m, const value_type& val)
            requires s_is_container_array_
        : m_map_(m), m_ctr_(sf_val_to_array_(val)) {
            if consteval { [[assume(m_map_.required_span_size() <= m_ctr_.size())]]; }
        }

        constexpr mdarray(const extents_type& exts, const container_type& c)
            requires std::is_constructible_v<mapping_type, const extents_type&>
        : m_map_(exts), m_ctr_(c) {
            if consteval { [[assume(m_map_.required_span_size() <= m_ctr_.size())]]; }
        }

        constexpr mdarray(const mapping_type& m, const container_type& c)
        : m_map_(m), m_ctr_(c) {
            if consteval { [[assume(m_map_.required_span_size() <= m_ctr_.size())]]; }
        }

        constexpr mdarray(const extents_type& exts, container_type&& c)
            requires std::is_constructible_v<mapping_type, const extents_type&>
        : m_map_(exts), m_ctr_(std::move(c)) {
            if consteval { [[assume(m_map_.required_span_size() <= m_ctr_.size())]]; }
        }

        constexpr mdarray(const mapping_type& m, container_type&& c)
        : m_map_(m), m_ctr_(std::move(c)) {
            if consteval { [[assume(m_map_.required_span_size() <= m_ctr_.size())]]; }
        }

        template <typename U, typename OtherExtents,
                  typename OtherLayoutPolicy, typename OtherContainer>
            requires (std::is_constructible_v<mapping_type, const typename OtherLayoutPolicy::template mapping<OtherExtents>&> &&
                      std::is_constructible_v<container_type, OtherContainer> &&
                      std::is_constructible_v<container_type, const OtherContainer&> &&
                      std::is_constructible_v<extents_type, OtherExtents>)
        constexpr explicit(!std::is_convertible_v<const typename OtherLayoutPolicy::template mapping<OtherExtents>&, mapping_type> ||
                           !std::is_convertible_v<const OtherContainer&, container_type>)
        mdarray(const mdarray<U, OtherExtents, OtherLayoutPolicy, OtherContainer>& other)
        : m_map_(other.m_map_), m_ctr_(other.m_ctr_) {
            if consteval {
                for (rank_type r = 0; r < rank(); ++r)
                    [[assume(static_extent(r) == std::dynamic_extent ||
                             static_extent(r) == other.extent(r))]];
            }
        }

        template <typename U, typename OtherExtents,
                  typename OtherLayoutPolicy, typename Accessor>
            requires (std::is_constructible_v<value_type, typename Accessor::reference> &&
                      std::is_assignable_v<typename Accessor::reference, value_type> &&
                      std::is_default_constructible_v<value_type> &&
                      std::is_constructible_v<mapping_type, const typename OtherLayoutPolicy::template mapping<OtherExtents>&> &&
                      !s_is_container_array_ && std::is_constructible_v<container_type, std::size_t> &&
                      std::is_constructible_v<extents_type, OtherExtents>)
        constexpr explicit(!std::is_convertible_v<const typename OtherLayoutPolicy::template mapping<OtherExtents>&, mapping_type> ||
                           !std::is_convertible_v<typename Accessor::reference, value_type>)
        mdarray(const mdspan<U, OtherExtents, OtherLayoutPolicy, Accessor>& other)
        : m_map_(other.mapping()), m_ctr_(other.mapping().required_span_size()) {

            // precondition
            if consteval {
                for (rank_type r = 0; r < rank(); ++r)
                    [[assume(static_extent(r) == std::dynamic_extent ||
                             static_extent(r) == other.extent(r))]];
            }

            // for all unique multidimensional indices is... in other.extents()
            // assigns other[is...] to m_ctr_[m_map_(i...)]

            // preview:
            //      auto [... exts] = other.extents();
            //      for (auto [... is] : std::views::cartesian_product(std::views::iota(0, exts)...))
            //          m_ctr_[m_map_(is...)] = other[is...];

            for (const auto& indices : __mdspan::index_cartesian_product(other.extents()))
                (*this)[indices] = other[indices];
        }

        template <typename U, typename OtherExtents,
                  typename OtherLayoutPolicy, typename Accessor>
            requires (std::is_constructible_v<value_type, typename Accessor::reference> &&
                      std::is_assignable_v<typename Accessor::reference, value_type> &&
                      std::is_default_constructible_v<value_type> &&
                      std::is_constructible_v<mapping_type, const typename OtherLayoutPolicy::template mapping<OtherExtents>&> &&
                      s_is_container_array_ && std::is_constructible_v<extents_type, OtherExtents>)
        constexpr explicit(!std::is_convertible_v<const typename OtherLayoutPolicy::template mapping<OtherExtents>&, mapping_type> ||
                           !std::is_convertible_v<typename Accessor::reference, value_type>)
        mdarray(const mdspan<U, OtherExtents, OtherLayoutPolicy, Accessor>& other)
        : m_map_(other.mapping()) {
            if consteval {
                for (rank_type r = 0; r < rank(); ++r)
                    [[assume(static_extent(r) == std::dynamic_extent ||
                             static_extent(r) == other.extent(r))]];
                [[assume(other.mapping().required_span_size() <= container_size())]];
            }

            for (const auto& indices : __mdspan::index_cartesian_product(other.extents()))
                (*this)[indices] = other[indices];
        }

        // constructors with allocator construction
        template <typename Alloc>
            requires (std::is_constructible_v<mapping_type, const extents_type&> &&
                      !s_is_container_array_ && std::is_constructible_v<container_type, std::size_t, const Alloc&>)
        constexpr explicit mdarray(const extents_type& exts, const Alloc& alloc)
        : m_map_(exts), m_ctr_(m_map_.required_span_size(), alloc) {
            if consteval { [[assume(m_map_.required_span_size() <= m_ctr_.size())]]; }
        }

        template <typename Alloc>
            requires (!s_is_container_array_ && std::is_constructible_v<container_type, std::size_t, const Alloc&>)
        constexpr explicit mdarray(const mapping_type& m, const Alloc& alloc)
        : m_map_(m), m_ctr_(m_map_.required_span_size(), alloc) {
            if consteval { [[assume(m_map_.required_span_size() <= m_ctr_.size())]]; }
        }

        template <typename Alloc>
            requires (std::is_constructible_v<mapping_type, const extents_type&> &&
                      !s_is_container_array_ && std::is_constructible_v<container_type, std::size_t, value_type, const Alloc&>)
        constexpr mdarray(const extents_type& exts, const value_type& val, const Alloc& alloc)
        : m_map_(exts), m_ctr_(m_map_.required_span_size(), val, alloc) {
            if consteval { [[assume(m_map_.required_span_size() <= m_ctr_.size())]]; }
        }

        template <typename Alloc>
            requires (!s_is_container_array_ && std::is_constructible_v<container_type, std::size_t, value_type, const Alloc&>)
        constexpr mdarray(const mapping_type& m, const value_type& val, const Alloc& alloc)
        : m_map_(m), m_ctr_(m_map_.required_span_size(), val, alloc) {
            if consteval { [[assume(m_map_.required_span_size() <= m_ctr_.size())]]; }
        }

        template <typename Alloc>
            requires (!s_is_container_array_ &&
                      std::is_constructible_v<mapping_type, const extents_type&> &&
                      std::is_constructible_v<container_type, const container_type&, const Alloc&>)
        constexpr mdarray(const extents_type& exts, const container_type& c, const Alloc& alloc)
        : m_map_(exts), m_ctr_(c, alloc) {
            if consteval { [[assume(m_map_.required_span_size() <= m_ctr_.size())]]; }
        }

        template <typename Alloc>
            requires (!s_is_container_array_ &&
                      std::is_constructible_v<container_type, const container_type&, const Alloc&>)
        constexpr mdarray(const mapping_type& m, const container_type& c, const Alloc& alloc)
        : m_map_(m), m_ctr_(c, alloc) {
            if consteval { [[assume(m_map_.required_span_size() <= m_ctr_.size())]]; }
        }

        template <typename Alloc>
            requires (!s_is_container_array_ &&
                      std::is_constructible_v<mapping_type, const extents_type&> &&
                      std::is_constructible_v<container_type, container_type, const Alloc&>)
        constexpr mdarray(const extents_type& exts, container_type&& c, const Alloc& alloc)
        : m_map_(exts), m_ctr_(std::move(c), alloc) {
            if consteval { [[assume(m_map_.required_span_size() <= m_ctr_.size())]]; }
        }

        template <typename Alloc>
            requires (!s_is_container_array_ &&
                      std::is_constructible_v<container_type, container_type, const Alloc&>)
        constexpr mdarray(const mapping_type& m, container_type&& c, const Alloc& alloc)
        : m_map_(m), m_ctr_(std::move(c), alloc) {
            if consteval { [[assume(m_map_.required_span_size() <= m_ctr_.size())]]; }
        }

        template <typename U, typename OtherExtents,
                  typename OtherLayoutPolicy, typename OtherContainer,
                  typename Alloc>
            requires (std::is_constructible_v<mapping_type, const typename OtherLayoutPolicy::template mapping<OtherExtents>&> &&
                      std::is_constructible_v<container_type, OtherContainer, const Alloc&> &&
                      std::is_constructible_v<container_type, const OtherContainer&, const Alloc&> &&
                      std::is_constructible_v<extents_type, OtherExtents>)
        constexpr explicit(!std::is_convertible_v<const typename OtherLayoutPolicy::template mapping<OtherExtents>&, mapping_type> ||
                           !std::is_convertible_v<const OtherContainer&, container_type>)
        mdarray(const mdarray<U, OtherExtents, OtherLayoutPolicy, OtherContainer>& other, const Alloc& alloc)
        : m_map_(other.m_map_), m_ctr_(other.m_ctr_, alloc) {
            if consteval {
                for (rank_type r = 0; r < rank(); ++r)
                    [[assume(static_extent(r) == std::dynamic_extent ||
                             static_extent(r) == other.extent(r))]];
            }
        }

        template <typename U, typename OtherExtents,
                  typename OtherLayoutPolicy, typename Accessor,
                  typename Alloc>
            requires (std::is_constructible_v<value_type, typename Accessor::reference> &&
                      std::is_assignable_v<typename Accessor::reference, value_type> &&
                      std::is_default_constructible_v<value_type> &&
                      std::is_constructible_v<mapping_type, const typename OtherLayoutPolicy::template mapping<OtherExtents>&> &&
                      !s_is_container_array_ && std::is_constructible_v<container_type, std::size_t, const Alloc&> &&
                      std::is_constructible_v<extents_type, OtherExtents>)
        constexpr explicit(!std::is_convertible_v<const typename OtherLayoutPolicy::template mapping<OtherExtents>&, mapping_type> ||
                           !std::is_convertible_v<typename Accessor::reference, value_type>)
        mdarray(const mdspan<U, OtherExtents, OtherLayoutPolicy, Accessor>& other, const Alloc& alloc)
        : m_map_(other.mapping()), m_ctr_(other.mapping().required_span_size(), alloc) {
            if consteval {
                for (rank_type r = 0; r < rank(); ++r)
                    [[assume(static_extent(r) == std::dynamic_extent ||
                             static_extent(r) == other.extent(r))]];
            }
            for (const auto& indices : __mdspan::index_cartesian_product(other.extents()))
                (*this)[indices] = other[indices];
        }

        constexpr mdarray& operator=(const mdarray&) = default;
        constexpr mdarray& operator=(mdarray&&) = default;

        template <typename... OtherIndexTypes>
            requires ((std::is_convertible_v<OtherIndexTypes, index_type> && ...) &&
                      (std::is_nothrow_constructible_v<index_type, OtherIndexTypes> && ...) &&
                      (sizeof...(OtherIndexTypes) == rank()))
        constexpr decltype(auto) operator[](this auto& self, OtherIndexTypes... indices) {
            return self.m_ctr_[self.m_map_(static_cast<index_type>(std::move(indices))...)];
        }

        template <typename OtherIndexType>
        constexpr decltype(auto) operator[](this auto& self, std::span<OtherIndexType, rank()> indices) {
            return gold::pack_apply_index<rank()>([&]<std::size_t... Is> -> decltype(auto) {
                return self[static_cast<const OtherIndexType&>(indices[Is])...];
            });
        }

        template <typename OtherIndexType>
        constexpr decltype(auto) operator[](this auto& self, const std::array<OtherIndexType, rank()>& indices) {
            return gold::pack_apply_index<rank()>([&]<std::size_t... Is> -> decltype(auto) {
                return self[static_cast<const OtherIndexType&>(indices[Is])...];
            });
        }

        constexpr size_type size() const noexcept {
            index_type s = 1;
            for (std::size_t r = 0; r < extents_type::rank(); ++r)
                s *= extents().extent(r);
            return s;
        }

        [[nodiscard]] constexpr bool empty() const noexcept { return size() == 0; }
        constexpr size_type container_size() const { return m_ctr_.size(); }

        friend constexpr void swap(mdarray& lhs, mdarray& rhs) noexcept {
            std::ranges::swap(lhs.m_ctr_, rhs.m_ctr_);
            std::ranges::swap(lhs.m_map_, rhs.m_map_);
        }

        constexpr const extents_type& extents() const { return m_map_.extents(); }
        constexpr pointer container_data() { return std::to_address(m_ctr_.begin()); }
        constexpr const_pointer container_data() const { return std::to_address(m_ctr_.cbegin()); }
        constexpr const mapping_type& mapping() const { return m_map_; }

        template <typename U,
                  typename OtherExtents,
                  typename OtherLayoutType,
                  typename OtherAccessorType>
            requires std::is_same_v<pointer, element_type*>
        constexpr operator mdspan<U, OtherExtents, OtherLayoutType, OtherAccessorType>() {
            return mdspan(container_data(), m_map_);
        }

        template <typename OtherAccessorType = default_accessor<element_type>>
        constexpr mdspan<element_type, extents_type, layout_type, OtherAccessorType>
        to_mdspan(const OtherAccessorType& a = {}) {
            return mdspan<element_type, extents_type, layout_type, OtherAccessorType>(
                container_data(), m_map_, a
            );
        }

        template <typename OtherAccessorType = default_accessor<const element_type>>
        constexpr mdspan<const element_type, extents_type, layout_type, OtherAccessorType>
        to_mdspan(const OtherAccessorType& a = {}) const {
            return mdspan<const element_type, extents_type, layout_type, OtherAccessorType>(
                container_data(), m_map_, a
            );
        }

        constexpr container_type&& extract_container() && { return std::move(m_ctr_); }

        static constexpr bool is_always_unique() noexcept { return mapping_type::is_always_unique(); }
        static constexpr bool is_always_exhaustive() noexcept { return mapping_type::is_always_exhaustive(); }
        static constexpr bool is_always_strided() noexcept { return mapping_type::is_always_strided(); }

        constexpr bool is_unique() const noexcept { return m_map_.is_unique(); }
        constexpr bool is_exhaustive() const noexcept { return m_map_.is_exhaustive(); }
        constexpr bool is_strided() const noexcept { return m_map_.is_strided(); }

        constexpr index_type stride(rank_type r) const { return m_map_.stride(r); }

    };

    // deduction guides for mdarray
    template <typename IndexType, std::size_t... Extents, typename Container>
    mdarray(const extents<IndexType, Extents...>&, const Container&)
    -> mdarray<typename Container::value_type, extents<IndexType, Extents...>, layout_right, Container>;

    template <typename Mapping, typename Container>
    mdarray(const Mapping&, const Container&)
    -> mdarray<typename Container::value_type,
               typename Mapping::extents_type,
               typename Mapping::layout_type,
               Container>;

    template <typename IndexType, std::size_t... Extents, typename Container>
    mdarray(const extents<IndexType, Extents...>&, Container&&)
    -> mdarray<typename Container::value_type, extents<IndexType, Extents...>, layout_right, Container>;

    template <typename Mapping, typename Container>
    mdarray(const Mapping&, Container&&)
    -> mdarray<typename Container::value_type,
               typename Mapping::extents_type,
               typename Mapping::layout_type,
               Container>;

    template <typename T, typename Extents, typename Layout, typename Accessor>
    mdarray(const mdspan<T, Extents, Layout, Accessor>&)
    -> mdarray<std::remove_cv_t<T>, Extents, Layout>;

    template <typename IndexType, std::size_t... Extents, typename Container, typename Alloc>
    mdarray(const extents<IndexType, Extents...>&, const Container&, const Alloc&)
    -> mdarray<typename Container::value_type, extents<IndexType, Extents...>, layout_right, Container>;

    template <typename Mapping, typename Container, typename Alloc>
    mdarray(const Mapping&, const Container&, const Alloc&)
    -> mdarray<typename Container::value_type,
               typename Mapping::extents_type,
               typename Mapping::layout_type,
               Container>;

    template <typename IndexType, std::size_t... Extents, typename Container, typename Alloc>
    mdarray(const extents<IndexType, Extents...>&, Container&&, const Alloc&)
    -> mdarray<typename Container::value_type, extents<IndexType, Extents...>, layout_right, Container>;

    template <typename Mapping, typename Container, typename Alloc>
    mdarray(const Mapping&, Container&&, const Alloc&)
    -> mdarray<typename Container::value_type,
               typename Mapping::extents_type,
               typename Mapping::layout_type,
               Container>;

    template <typename T, typename Extents, typename Layout, typename Accessor, typename Alloc>
    mdarray(const mdspan<T, Extents, Layout, Accessor>&, const Alloc&)
    -> mdarray<std::remove_cv_t<T>, Extents, Layout>;

} // namespace gold

#endif // __GOLD_BITS_MDSPAN_MDARRAY_HPP
