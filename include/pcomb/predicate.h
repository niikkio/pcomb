#ifndef PCOMB_PREDICATE_H_
#define PCOMB_PREDICATE_H_

#include <memory>
#include <sstream>
#include <type_traits>

#include "pcomb/privates/predicate.h"

namespace pcomb {

template <typename Ch>
inline auto Char(Ch ch) {
  std::stringstream ss;
  ss << "Char(" << ch << ") Parser";
  return with_name(std::make_shared<privates::PredicateParser<Ch>>(
      [ch](Ch c) { return c == ch; }), ss.str());
}

template <typename Ch = char>
inline auto AnyChar() {
  return std::make_shared<privates::PredicateParser<Ch>>(
      [](Ch) { return true; });
}

inline auto LatinChar() {
  return std::make_shared<privates::PredicateParser<char>>(
      [](char c) { return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'); });
}

template <typename Ch>
using CharParserType = typename std::invoke_result_t<
    decltype(Char<Ch>), Ch>::element_type;

}  // namespace pcomb
#endif  // PCOMB_PREDICATE_H_
