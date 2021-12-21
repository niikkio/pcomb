#ifndef PCOMB_UNTIL_H_
#define PCOMB_UNTIL_H_

#include <memory>
#include <utility>

#include "pcomb/privates/until.h"

namespace pcomb {

template <typename P>
inline auto Until(std::shared_ptr<P>&& parser) {
  return std::make_shared<privates::UntilParser<P>>(
      std::forward<std::shared_ptr<P>>(parser));
}

}  // namespace pcomb
#endif  // PCOMB_UNTIL_H_
