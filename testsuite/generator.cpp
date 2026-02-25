#include <iostream>
#include <gold/math>
#include <gold/generator>
#include <ranges>
#include <vector>

using i32 = int;
using u32 = unsigned int;
using i64 = long long;
using u64 = unsigned long long;

// generate primes up until 'n'
// - space complexity O(n)
// - time complexity O(n * log(log n))
gold::generator<i32> primes_upto(i32 n) {
    if (n <= 1)
        co_return;
    std::vector<bool> primes (n + 1, true);
    primes[0] = false;
    primes[1] = false;
    for (const i32 p : std::views::iota(2, gold::math::square_root(n) + 1)) {
        if (!primes[p])
            continue;
        for (const i32 i : std::views::iota(p * p, n + 1) | std::views::stride(p))
            primes[i] = false;
    }
    for (const i32 i : std::views::iota(2, n + 1))
        if (primes[i])
            co_yield i;
}

int main() {
    for (const auto& elem : primes_upto(50)) {
        std::cout << elem;
        std::cin.get();
        if (std::cin.eof()) break;
    }
}
