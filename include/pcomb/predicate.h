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

template <typename Ch = char>
inline auto AnyChar() {
  return privates::PredicateParser<Ch>(
      [](Ch) { return true; });
}

inline auto LatinChar() {
  return privates::PredicateParser<char>(
      [](char c) { return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'); });
}

template <typename Ch>
using CharParserType = std::invoke_result_t<decltype(Char<Ch>), Ch>;

}  // namespace pcomb
#endif  // PCOMB_PREDICATE_H_
