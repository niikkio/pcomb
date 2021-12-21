#ifndef PCOMB_SEQUENCE_H_
#define PCOMB_SEQUENCE_H_

#include <memory>
#include <utility>

#include "pcomb/privates/sequence.h"
#include "pcomb/privates/strict_sequence.h"

namespace pcomb {

template <typename P1, typename... PS>
inline auto Seq(std::shared_ptr<P1>&& p1, std::shared_ptr<PS>&&... ps) {
  return std::make_shared<privates::SequenceParser<P1, PS...>>(
          std::forward<std::shared_ptr<P1>>(p1),
          std::forward<std::shared_ptr<PS>>(ps)...);
}

template <typename P1, typename... PS>
inline auto StrictSeq(std::shared_ptr<P1>&& p1, std::shared_ptr<PS>&&... ps) {
  return std::make_shared<privates::StrictSequenceParser<P1, PS...>>(
          std::forward<std::shared_ptr<P1>>(p1),
          std::forward<std::shared_ptr<PS>>(ps)...);
}

}  // namespace pcomb
#endif  // PCOMB_SEQUENCE_H_
