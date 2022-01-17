#ifndef PCOMB_END_H_
#define PCOMB_END_H_

#include "pcomb/parser.h"

#include "pcomb/privates/end.h"

namespace pcomb {

template <typename CharType = char>
inline auto End() {
  return with_name(make<privates::EndParser<CharType>>(), "End");
}

}  // namespace pcomb
#endif  // PCOMB_END_H_
