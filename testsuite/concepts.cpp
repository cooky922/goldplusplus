#include <gold/bits/concepts/string.hpp>
#include <string>

int main() {
    static_assert(gold::string_like<char*>);
    static_assert(gold::string_like<const char*>);
    static_assert(gold::string_like<char[]>);
    static_assert(gold::string_like<const char[]>);
    static_assert(gold::string_like<char[5]>);
    static_assert(gold::string_like<const char[5]>);
    static_assert(!gold::string_like<char[0]>);
    static_assert(!gold::string_like<const char[0]>);
    static_assert(!gold::string_like<const char[5][2]>);
    static_assert(gold::string_like<std::string_view>);
    static_assert(gold::string_like<std::string>);
}
