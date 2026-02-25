/// type traits
#include <gold/type_traits>
#include <gold/concepts>

template <typename>
struct A {};

template <typename T>
using B = A<T>;

template <typename, std::size_t>
struct Array {};

template <std::size_t N>
using ByteArray = Array<std::byte, N>;

/// gold::byte_array
/// gold::to_bytes
/// gold::to_writable_bytes
/// gold::from_bytes

int main() {
    static_assert(gold::is_specialization_of_v<A<int>, A>());
    static_assert(gold::is_specialization_of_v<B<int>, A>());
    static_assert(gold::is_specialization_of_v<B<int>, B>());
    static_assert(gold::is_specialization_of_v<A<int>, B>());

    static_assert(gold::is_specialization_of_v<Array<int, 1>, Array>());
    static_assert(gold::is_specialization_of_v<ByteArray<1>, Array>());
    static_assert(!gold::is_specialization_of_v<ByteArray<1>, ByteArray>());
    static_assert(!gold::is_specialization_of_v<Array<std::byte, 1>, ByteArray>());

}
