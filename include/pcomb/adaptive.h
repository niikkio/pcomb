#ifndef PCOMB_ADAPTIVE_H_
#define PCOMB_ADAPTIVE_H_

#include <utility>

#include "pcomb/privates/adaptive.h"
#include "pcomb/privates/strict_adaptive.h"

namespace pcomb {

template <typename P, typename F>
inline auto Adapted(P&& parser, F&& func) {
  return privates::AdaptiveParser<
      std::remove_reference_t<P>, std::remove_reference_t<F>>(
          std::forward<P>(parser), std::forward<F>(func));
}

template <typename P, typename F>
inline auto StrictAdapted(P&& parser, F&& func) {
  return privates::StrictAdaptiveParser<
      std::remove_reference_t<P>, std::remove_reference_t<F>>(
          std::forward<P>(parser), std::forward<F>(func));
}

}  // namespace pcomb
#endif  // PCOMB_ADAPTIVE_H_
