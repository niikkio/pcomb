#ifndef PCOMB_ADAPTIVE_H_
#define PCOMB_ADAPTIVE_H_

#include <memory>
#include <utility>

#include "pcomb/privates/adaptive.h"
#include "pcomb/privates/strict_adaptive.h"

namespace pcomb {

template <typename P, typename F>
inline auto Adapted(std::shared_ptr<P>&& parser, F&& func) {
  return std::make_shared<privates::AdaptiveParser<
      P, std::remove_reference_t<F>>>(
          std::forward<std::shared_ptr<P>>(parser), std::forward<F>(func));
}

template <typename P, typename F>
inline auto StrictAdapted(std::shared_ptr<P>&& parser, F&& func) {
  return std::make_shared<privates::StrictAdaptiveParser<
      P, std::remove_reference_t<F>>>(
          std::forward<std::shared_ptr<P>>(parser), std::forward<F>(func));
}

}  // namespace pcomb
#endif  // PCOMB_ADAPTIVE_H_
