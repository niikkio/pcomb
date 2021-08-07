#ifndef PCOMB_LAZY_H_
#define PCOMB_LAZY_H_

#include "pcomb/privates/lazy.h"

namespace pcomb {

template <typename P>
inline auto Lazy(const P* parser_pointer) {
  return privates::LazyParser<P>(parser_pointer);
}

}  // namespace pcomb
#endif  // PCOMB_LAZY_H_
