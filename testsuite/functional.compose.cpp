#include <gold/bits/functional/compose.hpp>

constexpr int plus_one(int n) {
    return n + 1;
}

constexpr int times_two(int n) {
    return n * 2;
}

int main() {
    constexpr auto p1 = gold::compose(plus_one, times_two);
    static_assert(p1(3) == 7);

    constexpr auto p2 = gold::compose(times_two, plus_one);
    static_assert(p2(3) == 8);

    constexpr auto p3 = gold::compose(times_two, plus_one, times_two);
    static_assert(p3(3) == 14);
}
