#ifndef PCOMB_LAZY_H_
#define PCOMB_LAZY_H_

#include "pcomb/parser.h"
#include "pcomb/privates/lazy.h"

namespace pcomb {

template <typename P>
inline auto Lazy(const P* parser_pointer) {
  return make<privates::LazyParser<P>>(parser_pointer);
}

}  // namespace pcomb
#endif  // PCOMB_LAZY_H_
