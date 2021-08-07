#ifndef PCOMB_ALTERNATIVE_H_
#define PCOMB_ALTERNATIVE_H_

#include <utility>

#include "pcomb/privates/alternative.h"
#include "pcomb/privates/strict_alternative.h"

namespace pcomb {

template <typename P1, typename... PS>
inline auto Any(P1&& p1, PS&&... ps) {
  return privates::AlternativeParser<
      std::remove_reference_t<P1>, std::remove_reference_t<PS>...>(
          std::forward<P1>(p1), std::forward<PS>(ps)...);
}

template <typename P1, typename... PS>
inline auto StrictAny(P1&& p1, PS&&... ps) {
  return privates::StrictAlternativeParser<
      std::remove_reference_t<P1>, std::remove_reference_t<PS>...>(
          std::forward<P1>(p1), std::forward<PS>(ps)...);
}

}  // namespace pcomb
#endif  // PCOMB_ALTERNATIVE_H_
