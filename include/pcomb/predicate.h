#ifndef PCOMB_PREDICATE_H_
#define PCOMB_PREDICATE_H_

#include <sstream>
#include <type_traits>

#include "pcomb/parser.h"

#include "pcomb/privates/char.h"
#include "pcomb/privates/digit.h"
#include "pcomb/privates/predicate.h"

namespace pcomb {

template <typename Ch>
inline auto Char(Ch ch) {
  std::stringstream name;
  name << "Char(\'" << ch << "\')";
  return with_name(make<privates::CharParser<Ch>>(ch), name.str());
}

template <typename Ch = char>
inline auto AnyChar() {
  return with_name(
      make<privates::PredicateParser<Ch>>([](Ch) { return true; }), "AnyChar");
}

inline auto LatinChar() {
  return with_name(
      make<privates::PredicateParser<char>>(
          [](char c) {
            return ('a' <= c && c <= 'z') ||
                   ('A' <= c && c <= 'Z'); }),
      "LatinChar");
}

inline auto Digit() {
  return with_name(make<privates::DigitParser<char>>(), "Digit");
}

template <typename Ch>
using CharParserType = typename std::invoke_result_t<
    decltype(Char<Ch>), Ch>::element_type;

}  // namespace pcomb
#endif  // PCOMB_PREDICATE_H_
