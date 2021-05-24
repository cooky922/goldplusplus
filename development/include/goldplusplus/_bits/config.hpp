// config.hpp under gold++._bits (exposition)

#ifndef __GOLDPLUSPLUS_LIBBITS_CONFIG_HPP__
#define __GOLDPLUSPLUS_LIBBITS_CONFIG_HPP__

#define __GOLDLIB 202105L

#define NS_BEGIN namespace gold {
#define NS_END }

#define __GOLD_STATIC_ASSERT(p) static_assert(p, "Gold Assertion Failed!")
#define __GOLD_THROW throw
#define __GOLD_NOTHROW noexcept

#define __GOLD_NORETURN [[noreturn]]
#define __GOLD_DEPRECATED [[deprecated]]
#define __GOLD_DEPRECATEDR(r) [[deprecated(r)]]
#define __GOLD_NODISCARD [[nodiscard]]
#define __GOLD_NODISCARDR(r) [[nodiscard(r)]]


#endif // __GOLDPLUSPLUS_LIBBITS_CONFIG_HPP__
