#ifndef PCOMB_EXACT_H_
#define PCOMB_EXACT_H_

#include <memory>
#include <utility>

#include "pcomb/privates/exact.h"

namespace pcomb {

template <typename P>
inline auto Exact(std::shared_ptr<P>&& parser) {
  return std::make_shared<privates::ExactParser<P>>(
      std::forward<std::shared_ptr<P>>(parser));
}

}  // namespace pcomb
#endif  // PCOMB_EXACT_H_
