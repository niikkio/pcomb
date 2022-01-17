#ifndef PCOMB_ADAPTIVE_H_
#define PCOMB_ADAPTIVE_H_

#include <utility>

#include "pcomb/parser.h"

#include "pcomb/privates/adaptive.h"
#include "pcomb/privates/strict_adaptive.h"

namespace pcomb {

template <typename P, typename F>
inline auto Adapted(ParserPointer<P>&& parser, F&& func) {
  return with_name(
      make<privates::AdaptiveParser<P, std::remove_reference_t<F>>>(
          std::forward<ParserPointer<P>>(parser), std::forward<F>(func)),
      "Adapted");
}

template <typename P, typename F>
inline auto StrictAdapted(ParserPointer<P>&& parser, F&& func) {
  return with_name(
      make<privates::StrictAdaptiveParser<P, std::remove_reference_t<F>>>(
          std::forward<ParserPointer<P>>(parser), std::forward<F>(func)),
      "StrictAdapted");
}

}  // namespace pcomb
#endif  // PCOMB_ADAPTIVE_H_
