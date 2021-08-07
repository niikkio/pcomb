#ifndef PCOMB_SEQUENCE_H_
#define PCOMB_SEQUENCE_H_

#include <utility>

#include "pcomb/privates/sequence.h"
#include "pcomb/privates/strict_sequence.h"

namespace pcomb {

template <typename P1, typename... PS>
inline auto Seq(P1&& p1, PS&&... ps) {
  return privates::SequenceParser<
      std::remove_reference_t<P1>, std::remove_reference_t<PS>...>(
          std::forward<P1>(p1), std::forward<PS>(ps)...);
}

template <typename P1, typename... PS>
inline auto StrictSeq(P1&& p1, PS&&... ps) {
  return privates::StrictSequenceParser<
      std::remove_reference_t<P1>, std::remove_reference_t<PS>...>(
          std::forward<P1>(p1), std::forward<PS>(ps)...);
}


}  // namespace pcomb
#endif  // PCOMB_SEQUENCE_H_
