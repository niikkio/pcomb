#ifndef PCOMB_END_H_
#define PCOMB_END_H_

#include "pcomb/privates/end.h"

namespace pcomb {

template <typename Char>
inline auto End() {
  return privates::EndParser<Char>();
}

}  // namespace pcomb
#endif  // PCOMB_END_H_
