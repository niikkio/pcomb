#ifndef PCOMB_PREDICATE_H_
#define PCOMB_PREDICATE_H_

#include <type_traits>

#include "pcomb/privates/predicate.h"

namespace pcomb {

template <typename Ch>
inline auto Char(Ch ch) {
  return privates::PredicateParser<Ch>(
      [ch](Ch c) { return c == ch; });
}

template <typename Ch>
using CharParserType = std::invoke_result_t<decltype(Char<Ch>), Ch>;

}  // namespace pcomb
#endif  // PCOMB_PREDICATE_H_
