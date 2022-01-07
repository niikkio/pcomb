#ifndef PCOMB_CHAIN_H_
#define PCOMB_CHAIN_H_

#include <list>
#include <utility>

#include "pcomb/parser.h"
#include "pcomb/privates/dynamic_sequence.h"

namespace pcomb {

template <typename P>
inline auto Chain(std::list<ParserPointer<P>>&& ps) {
  return make<privates::DynamicSequenceParser<P>>(
      std::forward<std::list<ParserPointer<P>>>(ps));
}

}  // namespace pcomb
#endif  // PCOMB_CHAIN_H_
