#include <gold/utility>

constexpr int square(int n) {
    gold::assume(n >= 0);
    return n * n;
}

int main() {
    (void) square(1);
    (void) square(123);
    // constexpr int n = square(-1);
}
