#ifndef PCOMB_LEXEME_H_
#define PCOMB_LEXEME_H_

#include <algorithm>
#include <cctype>
#include <iterator>
#include <list>
#include <string>
#include <utility>

#include "pcomb/chain.h"
#include "pcomb/skipped.h"
#include "pcomb/sequence.h"
#include "pcomb/predicate.h"

#include "pcomb/privates/predicate.h"

namespace pcomb {

inline auto Digit() {
  return privates::PredicateParser<char>(
      [](char c) { return '0' <= c && c <= '9'; });
}

inline auto NewLine() {
  return Char('\n');
}

inline auto Space() {
  return privates::PredicateParser<char>(
      [](unsigned char c) { return std::isspace(c); });
}

template <typename P>
inline auto Inside(char ob, P&& parser, char cb) {
  return Seq(Skip(Char(ob)), std::forward<P>(parser), Skip(Char(cb)));
}

inline auto String(const std::string& s) {
  std::list<CharParserType<char>> parsers;
  std::transform(s.cbegin(), s.cend(),
                 std::inserter(parsers, parsers.begin()),
                 Char<char>);
  return Chain(std::move(parsers));
}

}  // namespace pcomb
#endif  // PCOMB_LEXEME_H_
