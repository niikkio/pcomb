#ifndef PCOMB_SKIPPED_H_
#define PCOMB_SKIPPED_H_

#include <utility>

#include "pcomb/privates/skipped.h"

namespace pcomb {

template <typename P>
inline auto Skip(P&& parser) {
  return privates::SkippedParser<std::remove_reference_t<P>>(
      std::forward<P>(parser));
}

}  // namespace pcomb
#endif  // PCOMB_SKIPPED_H_
