#include <gold/generator>
#include <ranges>

using u64 = unsigned long long;

struct Triple {
    u64 a;
    u64 b;
    u64 c;
};

gold::generator<Triple> pythagorean_triples(u64 n) {
    for (auto [a, b, c] : std::views::cartesian_product(std::views::iota(u64(1), n + 1),
                                                        std::views::iota(u64(1), n + 1),
                                                        std::views::iota(u64(1), n + 1)))
        if (a * a + b * b == c * c)
            co_yield {a, b, c};
}

#include <cstdio>

int main() {
    for (auto [a, b, c] : pythagorean_triples(50)) {
        std::printf("a: %zu, b: %zu, c: %zu", a, b, c);
        std::getchar();
    }
}
