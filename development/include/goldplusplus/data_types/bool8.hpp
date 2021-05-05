// bool8.hpp
#ifndef __GOLDPLUSPLUS_DATA_TYPES_BOOL8_HPP__
#define __GOLDPLUSPLUS_DATA_TYPES_BOOL8_HPP__

#define NS_BEGIN namespace gold {
#define NS_END }

#include <cstdint>

NS_BEGIN 
inline namespace data_type {
namespace bool8 {
  using bool8_t = unsigned char;
  
  bool8_t packbools(const bool[8]);
  void unpackbools(const bool8_t&, bool[8]);
  
  template <uint8_t N>
  bool readbool(bool8_t);
  
  template <uint8_t N>
  void writebool(bool8_t&, const bool&);

}}
#undef NS_BEGIN
#undef NS_END
#endif //  __GOLDPLUSPLUS_DATA_TYPES_BOOL8_HPP__
