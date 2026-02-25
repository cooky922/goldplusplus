#include <gold/ascii>

int main() {
    static_assert(gold::is_ascii('A'));
    static_assert(gold::is_ascii(U'B'));
    static_assert(!gold::is_ascii(U'な'));

    static_assert(gold::ascii_to_lower(U'Z') == 'z');
}
