#ifndef PCOMB_SKIPPED_H_
#define PCOMB_SKIPPED_H_

#include <memory>
#include <utility>

#include "pcomb/privates/skipped.h"

namespace pcomb {

template <typename P>
inline auto Skip(std::shared_ptr<P>&& parser) {
  return std::make_shared<privates::SkippedParser<P>>(
      std::forward<std::shared_ptr<P>>(parser));
}

}  // namespace pcomb
#endif  // PCOMB_SKIPPED_H_
