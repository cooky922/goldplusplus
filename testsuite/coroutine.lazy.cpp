#include <gold/lazy>

gold::lazy<int> create_value(int n) {
    co_return n;
}

gold::lazy<int> process_value(int a, int b) {
    const int m = co_await create_value(a);
    const int n = co_await create_value(b);
    co_return m + n;
}

int main() {
    return process_value(3, 4).sync_await();
}
