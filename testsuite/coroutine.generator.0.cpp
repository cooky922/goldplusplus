#include <gold/generator>

#include <iostream>
//#include <array>
//

gold::generator<int> ints(int n) {
    for (;;)
        co_yield n;
}

gold::generator<int> count_from(int n) {
    co_yield n;
    co_yield count_from(n + 1) | gold::ranges::to_yieldable;
}

gold::generator<int> count_down(int n) {
    while (n != 0)
        co_yield n--;
}

gold::generator<int> gen() {
    co_yield count_down(10) | gold::ranges::to_yieldable;
}

gold::generator<int> gen_alloc(std::allocator_arg_t, std::allocator<int>& b) {
    co_yield gen() | gold::ranges::to_yieldable;
}

//gold::generator<std::array<int, 2>> yield_twice() {
//    co_yield { 101, 69 };
//    co_yield { 43, 99 };
//}
//

gold::generator<int> fib() {
    auto a = 0, b = 1;
    while (true)
        co_yield std::exchange(a, std::exchange(b, a + b));
}

int main() {

    static_assert(std::ranges::input_range<gold::generator<int>>);
    static_assert(!std::ranges::forward_range<gold::generator<int>>);
    static_assert(std::ranges::view<gold::generator<int>>);


    for (const auto& elem : fib()) {
        std::cout << elem;
        std::cin.get();
        if (std::cin.eof())
            break;
    }
}
