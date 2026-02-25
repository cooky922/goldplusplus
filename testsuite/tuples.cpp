
#include <gold/tuples>
#include <string_view>
#include <array>

struct A {
    char c;
    template <std::size_t I>
        requires (I == 0)
    friend constexpr bool get(A) noexcept { return true; }
};

struct B {
    template <std::size_t>
    constexpr char get() const noexcept { return ':'; }
};

template <>
struct gold::tuples::tuple_size<A> {
    inline static constexpr std::size_t value = 1;
};

template <std::size_t I>
struct gold::tuples::tuple_element<I, A> {
    using type = char;
};

template <>
struct gold::tuples::tuple_size<B> {
    inline static constexpr std::size_t value = 1;
};

template <std::size_t I>
struct gold::tuples::tuple_element<I, B> {
    using type = char;
};

struct Empty {};

struct ArrayLike {
    // using element_type = char;
    inline static constexpr std::size_t extent = 5;

    static constexpr char operator[](std::size_t) noexcept { return 'A'; }
};

template <>
inline constexpr bool gold::tuples::enable_array_like<ArrayLike> = true;

int main() {
    static_assert(!gold::tuples::gettable<int[]>);
    // static_assert(!gold::tuples::gettable<int[0]>);
    static_assert(!gold::tuples::gettable<std::tuple<>>);
    static_assert(!gold::tuples::gettable<std::array<int, 0>>);
    static_assert(!gold::tuples::gettable<A>);
    static_assert(!gold::tuples::gettable_at<int[1], 1>);
    static_assert(gold::tuples::gettable_at<int[1], 0>);
    static_assert(gold::tuples::gettable<int[1]>);
    static_assert(gold::tuples::gettable<std::array<int, 10>>);
    static_assert(gold::tuples::gettable<std::tuple<int, char, bool>>);
    static_assert(gold::tuples::gettable<B>);

    static_assert(!gold::tuples::array_like<int[]>);
    static_assert(gold::tuples::array_like<int[3]>);
    static_assert(gold::tuples::array_like<std::array<int, 5>>);
    static_assert(!gold::tuples::array_like<std::tuple<int, double>>);
    static_assert(gold::tuples::array_like<ArrayLike>);

    // ArrayLike also models gold::tuples::tuple_like
    static_assert(gold::tuples::tuple_like<ArrayLike>);

    static_assert(std::same_as<char, gold::tuples::tuple_element_t<0, ArrayLike>>);

    static_assert(std::same_as<int&, decltype(gold::tuples::get<0>(std::declval<int(&)[1]>()))>);
    static_assert(std::same_as<const int&, decltype(gold::tuples::get<0>(std::declval<const int(&)[1]>()))>);
    static_assert(std::same_as<int&&, decltype(gold::tuples::get<0>(std::declval<int[1]>()))>);
    static_assert(std::same_as<const int&&, decltype(gold::tuples::get<0>(std::declval<const int[1]>()))>);
    static_assert(std::same_as<int&&, decltype(gold::tuples::get<0>(std::declval<int(&&)[1]>()))>);
    static_assert(std::same_as<const int&&, decltype(gold::tuples::get<0>(std::declval<const int(&&)[1]>()))>);

    static_assert(std::same_as<int&, decltype(std::get<0>(std::declval<std::tuple<int&>>()))>);
    static_assert(std::same_as<int&, decltype(gold::tuples::get<0>(std::declval<std::tuple<int&>>()))>);
    static_assert(std::same_as<int&, decltype(gold::tuples::get<0>(std::declval<std::tuple<int>&>()))>);

    static_assert(gold::tuples::applicable<void(int, int), int[2]>);
    static_assert(gold::tuples::applicable<void(int, int), std::tuple<int, int>>);
    static_assert(!gold::tuples::applicable<void(int, int), int[3]>);
    static_assert(gold::tuples::applicable<void(char), B>);

    static_assert(gold::tuples::tuple_like<std::tuple<>>);
    static_assert(gold::tuples::tuple_like<std::tuple<int>>);

    static_assert(gold::tuples::empty_like<std::tuple<>>);
    static_assert(gold::tuples::single_like<std::tuple<int>>);
    static_assert(gold::tuples::pair_like<std::tuple<int, char>>);

    static_assert(gold::tuples::applicable<void(std::string_view, const int&), std::tuple<const char*, int>>);
    static_assert(gold::tuples::applicable<int(), std::tuple<>>);

    static_assert(std::same_as<int, gold::tuples::apply_each_result_t<int()>>);
    static_assert(std::same_as<int&, gold::tuples::apply_each_result_t<int&()>>);
    static_assert(std::same_as<void, gold::tuples::apply_each_result_t<void()>>);
    static_assert(std::same_as<void, gold::tuples::apply_each_result_t<void(), std::tuple<>>>);
    static_assert(std::same_as<void, gold::tuples::apply_each_result_t<void(int), std::tuple<int>>>);
    static_assert(std::same_as<void, gold::tuples::apply_each_result_t<void(int, char), std::tuple<int>, std::tuple<char>>>);
    static_assert(std::same_as<void, gold::tuples::apply_each_result_t<void(int, char), int[2], char[2]>>);
    static_assert(std::same_as<void, gold::tuples::apply_each_result_t<void(int&, char&), int(&)[2], char(&)[2]>>);
    static_assert(std::same_as<std::tuple<int, int>, gold::tuples::apply_each_result_t<int(int, char), int[2], char[2]>>);
    static_assert(std::same_as<std::tuple<int&, int&>, gold::tuples::apply_each_result_t<int&(int, char), int[2], char[2]>>);
    static_assert(std::same_as<std::tuple<int&&, int&&>, gold::tuples::apply_each_result_t<int&&(int, char), int[2], char[2]>>);
    static_assert(std::same_as<std::tuple<float, float, float, float>, gold::tuples::apply_each_result_t<float(int, char), int[4], char[4]>>);

    static_assert(!noexcept(gold::tuples::fold_left_first(std::declval<int(int, int)>(), std::declval<std::tuple<int, int>>())));
    static_assert(noexcept(gold::tuples::fold_left_first(std::declval<int(int, int) noexcept>(), std::declval<std::tuple<int, int>>())));

    static_assert(std::same_as<std::tuple<>, decltype(gold::tuples::zip())>);
    static_assert(std::same_as<std::tuple<>, decltype(gold::tuples::zip(std::declval<std::tuple<>>()))>);
    static_assert(std::same_as<std::tuple<int>, decltype(gold::tuples::zip(std::declval<std::tuple<int>>()))>);

}
