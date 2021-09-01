#ifndef PCOMB_CHAIN_H_
#define PCOMB_CHAIN_H_

#include <list>
#include <utility>

#include "pcomb/privates/dynamic_sequence.h"

namespace pcomb {

template <typename PS>
inline auto Chain(PS&& ps) {
  return privates::DynamicSequenceParser<typename PS::value_type>(
      std::forward<PS>(ps));
}

}  // namespace pcomb
#endif  // PCOMB_CHAIN_H_
