#ifndef PCOMB_END_H_
#define PCOMB_END_H_

#include "pcomb/parser.h"
#include "pcomb/privates/end.h"

namespace pcomb {

template <typename CharType = char>
inline auto End() {
  return make<privates::EndParser<CharType>>();
}

}  // namespace pcomb
#endif  // PCOMB_END_H_
