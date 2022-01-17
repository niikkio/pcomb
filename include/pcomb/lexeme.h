#ifndef PCOMB_LEXEME_H_
#define PCOMB_LEXEME_H_

#include <algorithm>
#include <cctype>
#include <functional>
#include <iterator>
#include <list>
#include <string>
#include <utility>

#include "pcomb/adaptive.h"
#include "pcomb/chain.h"
#include "pcomb/skipped.h"
#include "pcomb/sequence.h"
#include "pcomb/parser.h"
#include "pcomb/predicate.h"
#include "pcomb/until.h"

#include "pcomb/privates/predicate.h"

namespace pcomb {

inline auto Digit() {
  return with_name(
      make<privates::PredicateParser<char>>(
          [](char c) { return '0' <= c && c <= '9'; }),
      "Digit");
}

inline auto NewLine() {
  return with_name(Char('\n'), "NewLine");
}

inline auto Space() {
  return with_name(
      make<privates::PredicateParser<char>>(
          [](unsigned char c) { return std::isspace(c); }),
      "Space");
}

template <typename P>
inline auto Inside(char ob, ParserPointer<P>&& parser, char cb) {
  std::stringstream name;
  name << "Inside('" << ob << "..." << cb << "')";
  return with_name(
      Seq(Skip(Char(ob)),
          std::forward<ParserPointer<P>>(parser),
          Skip(Char(cb))),
      name.str());
}

inline auto String(const std::string& s) {
  std::list<ParserPointer<CharParserType<char>>> parsers;
  std::transform(s.cbegin(), s.cend(),
                 std::inserter(parsers, parsers.begin()),
                 Char<char>);

  std::function<std::string(std::list<char>)> adapter =
      [](std::list<char>&& chars) {
        return std::string(chars.cbegin(), chars.cend());
      };

  std::stringstream name;
  name << "String('" << s << "')";
  return with_name(Adapted(Chain(std::move(parsers)), std::move(adapter)),
                   name.str());
}

inline auto Line() {
  return with_name(Until(NewLine()), "Line");
}

inline auto Word() {
  return with_name(Until(Space()), "Word");
}

}  // namespace pcomb
#endif  // PCOMB_LEXEME_H_
