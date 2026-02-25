#include <gold/expected>
#include <utility>

int main() {
    auto unex = gold::unexpected(123);
    int& a = unex.error();
    const int& b = std::as_const(unex).error();
    int&& c = std::move(unex).error();
    const int&& d = std::move(std::as_const(unex)).error();
}
