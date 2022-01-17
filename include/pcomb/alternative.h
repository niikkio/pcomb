#ifndef PCOMB_ALTERNATIVE_H_
#define PCOMB_ALTERNATIVE_H_

#include <utility>

#include "pcomb/parser.h"

#include "pcomb/privates/alternative.h"
#include "pcomb/privates/strict_alternative.h"

namespace pcomb {

template <typename P1, typename... PS>
inline auto Any(ParserPointer<P1>&& p1, ParserPointer<PS>&&... ps) {
  return with_name(
      make<privates::AlternativeParser<P1, PS...>>(
          std::forward<ParserPointer<P1>>(p1),
          std::forward<ParserPointer<PS>>(ps)...),
      "Any");
}

template <typename P1, typename... PS>
inline auto StrictAny(ParserPointer<P1>&& p1, ParserPointer<PS>&&... ps) {
  return with_name(
      make<privates::StrictAlternativeParser<P1, PS...>>(
          std::forward<ParserPointer<P1>>(p1),
          std::forward<ParserPointer<PS>>(ps)...),
      "StrictAny");
}

}  // namespace pcomb
#endif  // PCOMB_ALTERNATIVE_H_
