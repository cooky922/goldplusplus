#include <gold/pack>
#include <gold/bits/assume.hpp>
#include <bits/move.h>
#include <gold/bits/__util/uneval.hpp>

#include <string_view>

consteval void test0() {
    int x, y, z;

    gold::pack_get<2>(x, y, z) = 69;
    gold::pack_get<1>(x, y, z) = 42;
    gold::pack_get<0>(x, y, z) = 11;

    gold::assume(x == 11);
    gold::assume(y == 42);
    gold::assume(z == 69);

    std::string_view v;
    bool b;

    gold::pack_get<1>(true, v) = "Hello";

    gold::assume(gold::pack_get<0>(v) == "Hello");
}

consteval void test1() {
    using gold::__util::uneval;

    static_assert(__is_same(int&&, decltype(gold::pack_get<0>(uneval<int>(), uneval<float>()))));
    static_assert(__is_same(bool&&, decltype(gold::pack_get<2>(uneval<int>(), uneval<float>(), uneval<bool>()))));
    static_assert(__is_same(int&, decltype(gold::pack_get<0>(uneval<int&>(), uneval<float&>()))));
    static_assert(__is_same(float&, decltype(gold::pack_get<1>(uneval<int&>(), uneval<float&>()))));

    static_assert(__is_same(int&&, decltype(gold::pack_get<0>(uneval<int&&>(), uneval<float&&>()))));
    static_assert(__is_same(float&&, decltype(gold::pack_get<1>(uneval<int&&>(), uneval<float&&>()))));

    static_assert(__is_same(const int&, decltype(gold::pack_get<0>(uneval<const int&>(), uneval<float&>()))));
    static_assert(__is_same(const float&, decltype(gold::pack_get<1>(uneval<int&>(), uneval<const float&>()))));

    static_assert(__is_same(const int&&, decltype(gold::pack_get<0>(uneval<const int&&>(), uneval<float&>()))));
    static_assert(__is_same(const float&&, decltype(gold::pack_get<1>(uneval<int&>(), uneval<const float&&>()))));

    static_assert(__is_same(int&&, decltype(gold::pack_get<0>(uneval<int>(), uneval<int>()))));
    static_assert(__is_same(int&&, decltype(gold::pack_get<0>(uneval<const int>(), uneval<int>()))));
    static_assert(__is_same(const int&, decltype(gold::pack_get<0>(uneval<const int&>(), uneval<int>()))));
}

#include <gold/value_sequence>
#include <gold/bits/__util/mini_array.hpp>


template <std::size_t N, std::size_t... Is>
auto test(gold::value_seq_t<Is...>) {
    return gold::pack_get<N>(gold::constant<Is>...);
    // return c_array<std::size_t, sizeof...(Is)>{Is...}[N];

    // return gold::__util::mini_array{Is...}[N];
}

int main() {
    static_assert(gold::pack_get<0>(true, 3, 6u, 'A') == true);
    static_assert(gold::pack_get<1>(true, 3, 6u, 'A') == 3);
    static_assert(gold::pack_get<2>(3, true, 6u, 'A') == 6u);
    static_assert(gold::pack_get<3>(true, 3, 6u, 'A') == 'A');

    auto i = test<419>(gold::make_index_seq<1000>);

}
