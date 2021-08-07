#ifndef PCOMB_PREDICATE_H_
#define PCOMB_PREDICATE_H_

#include "pcomb/privates/predicate.h"

namespace pcomb {

inline auto Char(char ch) {
  return privates::PredicateParser<char>(
      [ch](char c) { return c == ch; });
}

inline auto Digit() {
  return privates::PredicateParser<char>(
      [](char c) { return '0' <= c && c <= '9'; });
}

}  // namespace pcomb
#endif  // PCOMB_PREDICATE_H_
