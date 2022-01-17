#ifndef PCOMB_MANY_H_
#define PCOMB_MANY_H_

#include <sstream>
#include <utility>

#include "pcomb/parser.h"

#include "pcomb/privates/many.h"

namespace pcomb {

template <typename P>
inline auto Many(ParserPointer<P>&& parser) {
  return with_name(
      make<privates::ManyParser<P>>(
          std::forward<ParserPointer<P>>(parser)),
      "Many");
}

template <typename P>
inline auto Some(ParserPointer<P>&& parser) {
  return with_name(
      make<privates::ManyParser<P>>(
          std::forward<ParserPointer<P>>(parser), 1),
      "Some");
}

template <typename P>
inline auto Maybe(ParserPointer<P>&& parser) {
  return with_name(
      make<privates::ManyParser<P>>(
          std::forward<ParserPointer<P>>(parser), 0, 1),
      "Maybe");
}

template <typename P>
inline auto Repeat(ParserPointer<P>&& parser, size_t count) {
  std::stringstream name;
  name << "Repeat(" << count << ")";
  return with_name(
      make<privates::ManyParser<P>>(
          std::forward<ParserPointer<P>>(parser), count, count),
      name.str());
}

}  // namespace pcomb
#endif  // PCOMB_MANY_H_
