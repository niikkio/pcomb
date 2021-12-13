#ifndef PCOMB_MANY_H_
#define PCOMB_MANY_H_

#include <utility>

#include "pcomb/privates/many.h"

namespace pcomb {

template <typename P>
inline auto Many(P&& parser) {
  return privates::ManyParser<std::remove_reference_t<P>>(
      std::forward<P>(parser));
}

template <typename P>
inline auto Some(P&& parser) {
  return privates::ManyParser<std::remove_reference_t<P>>(
      std::forward<P>(parser), 1);
}

template <typename P>
inline auto Maybe(P&& parser) {
  return privates::ManyParser<std::remove_reference_t<P>>(
      std::forward<P>(parser), 0, 1);
}

template <typename P>
inline auto Repeat(P&& parser, size_t count) {
  return privates::ManyParser<std::remove_reference_t<P>>(
      std::forward<P>(parser), count, count);
}

}  // namespace pcomb
#endif  // PCOMB_MANY_H_
