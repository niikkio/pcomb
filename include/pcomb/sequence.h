#ifndef PCOMB_SEQUENCE_H_
#define PCOMB_SEQUENCE_H_

#include <utility>

#include "pcomb/parser.h"
#include "pcomb/privates/sequence.h"
#include "pcomb/privates/strict_sequence.h"

namespace pcomb {

template <typename P1, typename... PS>
inline auto Seq(ParserPointer<P1>&& p1, ParserPointer<PS>&&... ps) {
  return make<privates::SequenceParser<P1, PS...>>(
          std::forward<ParserPointer<P1>>(p1),
          std::forward<ParserPointer<PS>>(ps)...);
}

template <typename P1, typename... PS>
inline auto StrictSeq(ParserPointer<P1>&& p1, ParserPointer<PS>&&... ps) {
  return make<privates::StrictSequenceParser<P1, PS...>>(
          std::forward<ParserPointer<P1>>(p1),
          std::forward<ParserPointer<PS>>(ps)...);
}

}  // namespace pcomb
#endif  // PCOMB_SEQUENCE_H_
