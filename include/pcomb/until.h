#ifndef PCOMB_UNTIL_H_
#define PCOMB_UNTIL_H_

#include <utility>

#include "pcomb/privates/until.h"

namespace pcomb {

template <typename P>
inline auto Until(P&& parser) {
  return privates::UntilParser<std::remove_reference_t<P>>(
      std::forward<P>(parser));
}

}  // namespace pcomb
#endif  // PCOMB_UNTIL_H_
