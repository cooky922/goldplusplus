#include <gold/bits/ranges/cycle_view.hpp>

/////////

#include <iostream>
#include <gold/bits/assume.hpp>

consteval void test0() {
    int arr[] { 6, 9 };
    auto cycled = gold::views::cycle_n(arr, 3);
    gold::assume(cycled.size() == 6);
    gold::assume(cycled.end() - cycled.begin() == 6);
}

consteval void test1() {
    char arr[] { 'A', 'B', 'C', 'D', 'E' };
    auto cycled = gold::views::cycle_n(arr, 2);
    gold::assume(cycled.size() == 10);
    gold::assume(cycled.begin()[0] == 'A');
    gold::assume(cycled.begin()[1] == 'B');
    gold::assume(cycled.begin()[2] == 'C');
    gold::assume(cycled.begin()[3] == 'D');
    gold::assume(cycled.begin()[4] == 'E');
    gold::assume(cycled.begin()[5] == 'A');
    gold::assume(cycled.begin()[6] == 'B');
    gold::assume(cycled.begin()[7] == 'C');
    gold::assume(cycled.begin()[8] == 'D');
    gold::assume(cycled.begin()[9] == 'E');

    auto reversed_cycled = cycled | std::views::reverse;
    gold::assume(reversed_cycled.end() - reversed_cycled.begin() == 10);
    auto rc_iter = reversed_cycled.begin();

    gold::assume(rc_iter[0] == 'E');
    gold::assume(rc_iter[1] == 'D');
    gold::assume(rc_iter[2] == 'C');
    gold::assume(rc_iter[3] == 'B');
    gold::assume(rc_iter[4] == 'A');
    gold::assume(rc_iter[5] == 'E');
    gold::assume(rc_iter[6] == 'D');
    gold::assume(rc_iter[7] == 'C');
    gold::assume(rc_iter[8] == 'B');
    gold::assume(rc_iter[9] == 'A');
}

int main() {
    test0();
    test1();

    int arr[] { 6, 9 };
    for (int e : gold::views::cycle_n(arr, 5)) {
        std::cout << e;
        std::cin.get();
    }
}
