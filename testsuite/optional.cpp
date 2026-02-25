#include <optional>

int main() {
    int x = 10;
    std::optional<int&> a = x;
    std::optional<const int&> b = x;
}
