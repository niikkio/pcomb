#ifndef PCOMB_EXACT_H_
#define PCOMB_EXACT_H_

#include <utility>

#include "pcomb/privates/exact.h"

namespace pcomb {

template <typename P>
inline auto Exact(P&& parser) {
  return privates::ExactParser<std::remove_reference_t<P>>(
      std::forward<P>(parser));
}

}  // namespace pcomb
#endif  // PCOMB_EXACT_H_
