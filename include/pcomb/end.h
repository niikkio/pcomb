#ifndef PCOMB_END_H_
#define PCOMB_END_H_

#include <memory>

#include "pcomb/privates/end.h"

namespace pcomb {

template <typename CharType = char>
inline auto End() {
  return std::make_shared<privates::EndParser<CharType>>();
}

}  // namespace pcomb
#endif  // PCOMB_END_H_
