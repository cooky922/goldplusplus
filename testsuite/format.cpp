#include <gold/assertion>
#include <gold/format>
// #include <gold/print>

//// example:
// gold::format("{}", 1234) == "1234"
// gold::format("{0}, {1}", "Hello", "world") == "Hello, world!"
// gold::format("{0}{1}{0}", "abra", "cad") == "abracadabra"
// gold::format("{:#x}", 0xdeadbeef) == "0xdeadbeef"
// gold::format("{:#X}", 0xdeadbeef) == "0XDEADBEEF"
// gold::format("{:.2f}", 3.14f) == "3.14"
// gold::format("Wh{}? {} is this madness?", 'y', "Is") == "Why? Is this madness?"
// gold::format("{::^5}", 'A') == "::A::"
// gold::format("{0:.{1}f}", 3.1415926535, 2) == "3.14"
// gold::format("{:|^#{}x}", 0xdeadbeef, 30) == "||||||||||0xdeadbeef||||||||||"

// TODO: add format_as CPO

// [gold++.format.v2.5] changes
// + added more escape presentations for Basic Control Codes
// + removed 'format' and 'parse' member functions
//   in formatter specializations leaving only
//   'try_format' and 'parse'

// [gold++.format.v2.4] changes
// + added non-throwing variant of text formatting
//   with 'try_format' and 'try_parse'
// + gold::formattable will no longer require
//   having 'parse' and 'format' as member functions.
//   in fact, they are deprecated since they are replaced
//   with safe functions 'try_parse' and 'try_format'

// [gold++.format.v2.3] changes
// + added custom separator support as part of the format specs
//   for range and tuple formatters

// [gold++.format.v2.2] changes
// + added support for range and tuple formatter
// + added compile-time check for dynamic spec argument type
// + fixed some issues

// [gold++.format.v2] changes
// + followed GCC libstdc++'s <format>
// + disabled mixing automatic and manual argument indexing
// + added more options to floating-point formatter
//   at compile-time (still unstable)

// [gold++.format.v1]
// + initial support for text formatting
// + allows mixing automatic and manual argument indexing
// + awkward way of creating formatters
// + exposed 'gold::format_specs'
// + missing formatting options for some types
// + '{' and '}' are not counted as escape characters
//   since '$' introduction character is used to
//   start replacement field

struct Point {
    int x;
    int y;
};

template <>
struct gold::formatter<Point> : gold::formatter<std::string_view> {
    constexpr auto try_format(Point p, gold::format_context& ctx) const {
        return gold::formatter<std::string_view>::try_format(
            gold::format(gold::unchecked_format_string("({:d}, {:d})"), p.x, p.y)
        , ctx);
    }
};

// easy implementation
static_assert(gold::format("{}", 1234) == "1234");
static_assert(gold::format("{0}", 1234) == "1234");
static_assert(gold::format("{0}, {1}!", "Hello", "world") == "Hello, world!");
static_assert(gold::format("{0}{1}{0}", "abra", "cad") == "abracadabra");
static_assert(gold::format("{:#x}", 0xdeadbeef) == "0xdeadbeef");
static_assert(gold::format("{}", Point { 3, 4 }) == "(3, 4)");

int main() {

    int x = 10;

    gold::system_assert(gold::format("{}", 1234) == "1234");
    gold::system_assert(gold::format("{0}, {1}!", "Hello", "world") == "Hello, world!");
    gold::system_assert(gold::format("{0}{1}{0}", "abra", "cad") == "abracadabra");
    gold::system_assert(gold::format("{:#x}", 0xdeadbeef) == "0xdeadbeef");
    gold::system_assert(gold::format("{:.2f}", 3.14f) == "3.14");
    gold::system_assert(gold::format("Wh{1}? {0} this madness?", "Is", 'y') == "Why? Is this madness?");
    gold::system_assert(gold::format("{::^5}", 'A') == "::A::");
    gold::system_assert(gold::format("{0:.{1}f}", 3.1415926535, 2) == "3.14");
    gold::system_assert(gold::format("{:|^#{}x}", 0xdeadbeef, 30) == "||||||||||0xdeadbeef||||||||||");
    gold::system_assert(gold::format("{}", Point{.x = 1, .y = 2}) == "(1, 2)");

}
