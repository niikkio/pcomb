#ifndef PCOMB_CHAIN_H_
#define PCOMB_CHAIN_H_

#include <list>
#include <memory>
#include <utility>

#include "pcomb/privates/dynamic_sequence.h"

namespace pcomb {

// template <typename PS>
// inline auto Chain(PS&& ps) {
//   return std::make_shared<privates::DynamicSequenceParser<
//       typename PS::value_type::element_type>>(std::forward<PS>(ps));
// }

template <typename P>
inline auto Chain(std::list<std::shared_ptr<P>>&& ps) {
  return std::make_shared<privates::DynamicSequenceParser<P>>(
      std::forward<std::list<std::shared_ptr<P>>>(ps));
}

}  // namespace pcomb
#endif  // PCOMB_CHAIN_H_
