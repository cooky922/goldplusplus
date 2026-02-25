#include <gold/mdspan>
#include <gold/mdarray>
#include <gold/bits/assume.hpp>

// layout_right - row-major
// layout_left  - column-major

consteval void test0() {
    [] consteval {
        gold::dextents<std::size_t, 3> dexts {5, 6, 7};
        [[gnu::assume(dexts.rank() == 3)]];
        [[gnu::assume(dexts.rank_dynamic() == 3)]];
        [[gnu::assume(dexts.extent(0) == 5)]];
        [[gnu::assume(dexts.extent(1) == 6)]];
        [[gnu::assume(dexts.extent(2) == 7)]];
        [[gnu::assume(dexts.static_extent(0) == std::dynamic_extent)]];
        [[gnu::assume(dexts.static_extent(1) == std::dynamic_extent)]];
        [[gnu::assume(dexts.static_extent(2) == std::dynamic_extent)]];

        static constexpr std::size_t Dy = std::dynamic_extent;
        gold::extents<std::size_t, 2, Dy, 3, Dy> exts { 8, 6 };
        [[gnu::assume(exts.rank() == 4)]];
        [[gnu::assume(exts.rank_dynamic() == 2)]];
        [[gnu::assume(exts.extent(0) == 2)]];
        [[gnu::assume(exts.extent(1) == 8)]];
        [[gnu::assume(exts.extent(2) == 3)]];
        [[gnu::assume(exts.extent(3) == 6)]];
        [[gnu::assume(exts.static_extent(0) == 2)]];
        [[gnu::assume(exts.static_extent(1) == std::dynamic_extent)]];
        [[gnu::assume(exts.static_extent(2) == 3)]];
        [[gnu::assume(exts.static_extent(3) == std::dynamic_extent)]];

        // gold::extents<std::size_t, 3, 3> ext3x3 {};

        int underlying_data[] { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

        using matrix_3x3 = gold::mdspan<int, gold::extents<std::size_t, 3, 3>>;

        matrix_3x3 mat { underlying_data };

        [[gnu::assume(mat[0, 0] == 1)]];
        [[gnu::assume(mat[0, 1] == 2)]];
        [[gnu::assume(mat[0, 2] == 3)]];
        [[gnu::assume(mat[1, 0] == 4)]];
        [[gnu::assume(mat[1, 1] == 5)]];
        [[gnu::assume(mat[1, 2] == 6)]];
        [[gnu::assume(mat[2, 0] == 7)]];
        [[gnu::assume(mat[2, 1] == 8)]];
        [[gnu::assume(mat[2, 2] == 9)]];

        //       [ 1 2 3 ]
        // mat = [ 4 5 6 ]
        //       [ 7 8 9 ]

        using matrix_3x3_transposed = gold::mdspan<int, gold::extents<std::size_t, 3, 3>, gold::layout_left>;

        matrix_3x3_transposed mat_t { underlying_data };

        //         [ 1 4 7 ]
        // mat_t = [ 2 5 8 ]
        //         [ 3 6 9 ]

        [[gnu::assume(mat_t[0, 0] == 1)]];
        [[gnu::assume(mat_t[0, 1] == 4)]];
        [[gnu::assume(mat_t[0, 2] == 7)]];
        [[gnu::assume(mat_t[1, 0] == 2)]];
        [[gnu::assume(mat_t[1, 1] == 5)]];
        [[gnu::assume(mat_t[1, 2] == 8)]];
        [[gnu::assume(mat_t[2, 0] == 3)]];
        [[gnu::assume(mat_t[2, 1] == 6)]];
        [[gnu::assume(mat_t[2, 2] == 9)]];

        // summary: layout-left  -> column major
        //          layout-right -> row major

        auto md = gold::mdspan(underlying_data, gold::extents(3, 3));
        [[gnu::assume(md.extent(0) == 3)]];
        [[gnu::assume(md.extent(1) == 3)]];
        [[gnu::assume(md.static_extent(0) == std::dynamic_extent)]];
        [[gnu::assume(md.static_extent(1) == std::dynamic_extent)]];

        static_assert(__is_same(gold::mdspan<int, gold::dextents<std::size_t, 2>>, decltype(md)));
    }();

    [] consteval {
        gold::extents<int, 2, 3> exts;
        gold::dextents<int, 2> dexts = exts;
        [[gnu::assume(dexts.extent(0) == 2)]];
        [[gnu::assume(dexts.extent(1) == 3)]];
        [[gnu::assume(dexts.static_extent(0) == std::dynamic_extent)]];
        [[gnu::assume(dexts.static_extent(1) == std::dynamic_extent)]];
    }();
}

consteval void test1() {
    int arr[] {
        1, 2, 3, 4, 5, 6,
        7, 8, 9, 10, 11, 12,
        13, 14, 15, 16, 17, 18,
        19, 20, 21, 22, 23, 24
    };

    auto view_6x4   = gold::mdspan(arr, gold::extents(6, 4));
    auto view_2x3x4 = gold::mdspan(arr, gold::extents(2, 3, 4));

    gold::assume(view_6x4[0, 0] == 1);
    gold::assume(view_6x4[0, 1] == 2);
    gold::assume(view_6x4[0, 2] == 3);
    gold::assume(view_6x4[0, 3] == 4);
    gold::assume(view_6x4[1, 0] == 5);
    gold::assume(view_6x4[3, 3] == 16);
    gold::assume(view_6x4[5, 3] == 24);

    gold::assume(view_2x3x4[0, 0, 0] == 1);
    gold::assume(view_2x3x4[1, 1, 3] == 20);
    gold::assume(view_2x3x4[1, 2, 0] == 21);
    gold::assume(view_2x3x4[1, 2, 3] == 24);

}

consteval void test2() {
    using Md = gold::mdarray<int, gold::extents<std::size_t, 2, 2>>;
    Md md;
    md[0, 0] = 1;
    md[0, 1] = 2;
    md[1, 0] = 3;
    md[1, 1] = 4;
}

#include <gold/bits/mdspan/layout_left_padded.hpp>

consteval void test3() {
    using Extents     = gold::extents<std::size_t, 2, 3, 4>;
    using MappingLeft = gold::layout_left::mapping<Extents>;
    using MappingLeftPadded = gold::layout_left_padded<5>::mapping<Extents>;

    static_assert(MappingLeft().stride(0) == 1);
    static_assert(MappingLeft().stride(1) == 2);
    static_assert(MappingLeft().stride(2) == 6);
    static_assert(MappingLeft().required_span_size() == 24);

    static_assert(MappingLeftPadded().stride(0) == 1);
    static_assert(MappingLeftPadded().stride(1) == 5);
    static_assert(MappingLeftPadded().stride(2) == 15);
    static_assert(MappingLeftPadded().required_span_size() == 57);
    // 56 = 7 * 8
}

consteval void test4() {
    {
        constexpr auto m = gold::layout_left_padded<2>::mapping<gold::extents<std::size_t, 9, 2>>();
        static_assert(m.stride(0) == 1);
        static_assert(m.stride(1) == 10);
        static_assert(m.required_span_size() == 19);
    }

    {
        constexpr auto m = gold::layout_left_padded<4>::mapping<gold::extents<std::size_t, 9, 2>>();
        static_assert(m.stride(0) == 1);
        static_assert(m.stride(1) == 12);
        static_assert(m.required_span_size() == 21);
    }
}

int main() {
    test0();
    test1();
    test2();
}
