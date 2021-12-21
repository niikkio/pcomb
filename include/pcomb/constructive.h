#ifndef PCOMB_CONSTRUCTIVE_H_
#define PCOMB_CONSTRUCTIVE_H_

#include <memory>
#include <utility>

#include "pcomb/privates/constructive.h"

namespace pcomb {

template <typename T, typename P>
inline auto Construct(std::shared_ptr<P>&& parser) {
  return std::make_shared<privates::ConstructiveParser<T, P>>(
      std::forward<std::shared_ptr<P>>(parser));
}

}  // namespace pcomb
#endif  // PCOMB_CONSTRUCTIVE_H_
