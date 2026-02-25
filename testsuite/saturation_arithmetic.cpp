#include <gold/bits/numbers/sat.hpp>
#include <gold/basic_types>

int main() {
    using u8 = unsigned char;
    using i8 = signed char;
    using u32 = unsigned int;
    using i32 = int;
    using u128 = gold::uint128_t;
    using i128 = gold::int128_t;

    static_assert(gold::sat_add(1, 2) == 3);
    static_assert(gold::sat_add(-1u, 0) == -1u);
    static_assert(gold::sat_add(-1u, 1) == -1u);
    static_assert(gold::sat_add(std::numeric_limits<int>::max(), 3) == std::numeric_limits<int>::max());
    static_assert(gold::sat_add(u8(178), u8(200)) == u8(255));
    static_assert(gold::sat_add(i8(127), i8(1)) == i8(127));
    static_assert(gold::sat_add(i8(-127), i8(1)) == i8(-126));
    static_assert(gold::sat_add(i8(-127), i8(-1)) == i8(-128));
    static_assert(gold::sat_add(i8(-127), i8(-2)) == i8(-128));
    static_assert(gold::sat_add(i8(-100), i8(-100)) == i8(-128));

    static_assert(gold::sat_sub(u8(4), u8(1)) == u8(3));
    static_assert(gold::sat_sub(u8(2), u8(3)) == u8(0));
    static_assert(gold::sat_sub(u8(0), u8(1)) == u8(0));
    static_assert(gold::sat_sub(u128(0), u128(1)) == u128(0));
    static_assert(gold::sat_sub(i8(-127), i8(1)) == i8(-128));
    static_assert(gold::sat_sub(i8(-127), i8(2)) == i8(-128));
    static_assert(gold::sat_sub(i8(126), i8(1)) == i8(125));
    static_assert(gold::sat_sub(i8(126), i8(0)) == i8(126));
    static_assert(gold::sat_sub(i8(126), i8(-1)) == i8(127));
    static_assert(gold::sat_sub(i8(126), i8(-2)) == i8(127));
    static_assert(gold::sat_sub(i8(100), i8(-50)) == i8(127));

    static_assert(gold::sat_mul(u8(5), u8(10)) == u8(50));
    static_assert(gold::sat_mul(u8(5), u8(100)) == u8(255));
    static_assert(gold::sat_mul(i8(5), i8(100)) == i8(127));
    static_assert(gold::sat_mul(i8(-5), i8(-100)) == i8(127));
    static_assert(gold::sat_mul(i8(-5), i8(100)) == i8(-128));
    static_assert(gold::sat_mul(i8(5), i8(-100)) == i8(-128));

    static_assert(gold::saturate_cast<u8>(255u) == u8(255));
    static_assert(gold::saturate_cast<u8>(256u) == u8(255));
    static_assert(gold::saturate_cast<u8>(300) == u8(255));
    static_assert(gold::saturate_cast<u8>(-1) == u8(0));
    static_assert(gold::saturate_cast<u128>(-1) == u128(0));
    static_assert(gold::saturate_cast<u128>(i128(-1)) == u128(0));

    static_assert(gold::saturate_cast<i8>(1) == i8(1));
    static_assert(gold::saturate_cast<i8>(127) == i8(127));
    static_assert(gold::saturate_cast<i8>(128) == i8(127));
    static_assert(gold::saturate_cast<i8>(255) == i8(127));
    static_assert(gold::saturate_cast<i8>(-101) == i8(-101));
    static_assert(gold::saturate_cast<i8>(-128) == i8(-128));
    static_assert(gold::saturate_cast<i8>(-129) == i8(-128));

}
