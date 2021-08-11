#ifndef PCOMB_OPTIONAL_H_
#define PCOMB_OPTIONAL_H_

#include <utility>

#include "pcomb/privates/optional.h"

namespace pcomb {

template <typename P>
inline auto Opt(P&& parser) {
  return privates::OptionalParser<std::remove_reference_t<P>>(
      std::forward<P>(parser));
}

}  // namespace pcomb
#endif  // PCOMB_OPTIONAL_H_
