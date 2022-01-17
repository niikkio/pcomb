#ifndef PCOMB_CHAIN_H_
#define PCOMB_CHAIN_H_

#include <list>
#include <utility>

#include "pcomb/parser.h"

#include "pcomb/privates/dynamic_sequence.h"

namespace pcomb {

template <typename P>
inline auto Chain(std::list<ParserPointer<P>>&& ps) {
  return with_name(
      make<privates::DynamicSequenceParser<P>>(
          std::forward<std::list<ParserPointer<P>>>(ps)),
      "Chain");
}

}  // namespace pcomb
#endif  // PCOMB_CHAIN_H_
