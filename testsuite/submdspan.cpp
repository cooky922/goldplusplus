#include <gold/mdspan>
#include <gold/mdarray>
#include <gold/bits/assume.hpp>

consteval void test0() {
    int d [] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    auto m = gold::mdspan(d, 12);
    gold::assume(m.extent(0) == 12);
}

consteval void test1() {
    int d[] { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    // [ 1 2 3 ]
    // [ 4 5 6 ]
    // [ 7 8 9 ]
    auto m = gold::mdspan(d, gold::extents<std::size_t, 3, 3>());
    gold::assume(m.rank() == 2);
    gold::assume(m.extent(0) == 3);
    gold::assume(m.extent(1) == 3);
    gold::assume(gold::equal_mdspan(m, m));

    auto v1 = gold::submdspan(m, gold::ranged_slice(0, 2), gold::ranged_slice(1, 3));
    // v1:
    // [ 2 3 ]
    // [ 5 6 ]
    gold::assume(v1.rank() == 2);
    gold::assume(v1.extent(0) == 2);
    gold::assume(v1.extent(1) == 2);

    gold::mdarray a (v1);
    gold::assume(gold::equal_mdspan(a.to_mdspan(), v1));

    {
        int d2 [] { 2, 3, 5, 6 };
        auto m2 = gold::mdspan(d2, gold::extents<std::size_t, 2, 2>());
        gold::assume(gold::equal_mdspan(m2, v1));
    }
}

void test2() {
    int d[] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

    auto m = gold::mdspan(d, gold::extents(12));
    auto print_view = [](auto v) {
        static_assert(v.rank() == 1);
        for (std::size_t i = 0; i < v.extent(0); ++i)
            __builtin_printf("%i ", v[i]);
        __builtin_printf("\n");
    };

    print_view(m);
    // these are all equivalent below to the first line
    // print_view(gold::submdspan(m, gold::full_extent));
    // print_view(gold::submdspan(m, gold::ranged_slice(0, 12)));

    // strided_slice { offset, extent, stride }
    print_view(gold::submdspan(m, gold::strided_slice(0, 10, 1)));
    print_view(gold::submdspan(m, gold::strided_slice(1, 10, 1)));
    print_view(gold::submdspan(m, gold::strided_slice(3, 8, 2)));
    print_view(gold::submdspan(m, gold::strided_slice(5, 0, 1)));

    // strided_slice(0, 10, 1) => [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
    // strided_slice(1, 10, 1) => [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    // strided_slice(2, 10, 1) => [2, 3, 4, 5, 6, 7, 8, 9, 10, 11]
}

int main() {
    test0();
    test1();
    test2();
}
