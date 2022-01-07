#ifndef PCOMB_MANY_H_
#define PCOMB_MANY_H_

#include <utility>

#include "pcomb/parser.h"
#include "pcomb/privates/many.h"

namespace pcomb {

template <typename P>
inline auto Many(ParserPointer<P>&& parser) {
  return make<privates::ManyParser<P>>(
      std::forward<ParserPointer<P>>(parser));
}

template <typename P>
inline auto Some(ParserPointer<P>&& parser) {
  return make<privates::ManyParser<P>>(
      std::forward<ParserPointer<P>>(parser), 1);
}

template <typename P>
inline auto Maybe(ParserPointer<P>&& parser) {
  return make<privates::ManyParser<P>>(
      std::forward<ParserPointer<P>>(parser), 0, 1);
}

template <typename P>
inline auto Repeat(ParserPointer<P>&& parser, size_t count) {
  return make<privates::ManyParser<P>>(
      std::forward<ParserPointer<P>>(parser), count, count);
}

}  // namespace pcomb
#endif  // PCOMB_MANY_H_
