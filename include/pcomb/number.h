#ifndef PCOMB_NUMBER_H_
#define PCOMB_NUMBER_H_

#include "pcomb/parser.h"

#include "pcomb/privates/number.h"

namespace pcomb {

template <typename CharType = char>
inline auto Int() {
  return with_name(make<privates::NumberParser<CharType, int>>(), "Int");
}

template <typename CharType = char>
inline auto UInt() {
  return with_name(
      make<privates::NumberParser<CharType, unsigned int>>(), "UInt");
}

template <typename CharType = char>
inline auto Double() {
  return with_name(make<privates::NumberParser<CharType, double>>(), "Double");
}

}  // namespace pcomb
#endif  // PCOMB_NUMBER_H_
