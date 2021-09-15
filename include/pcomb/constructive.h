#ifndef PCOMB_CONSTRUCTIVE_H_
#define PCOMB_CONSTRUCTIVE_H_

#include <memory>
#include <utility>

#include "pcomb/parser.h"
#include "pcomb/privates/constructive.h"

namespace pcomb {

template <typename T, typename P>
inline auto Construct(P&& parser) {
  return privates::ConstructiveParser<T, std::remove_reference_t<P>>(
      std::forward<P>(parser));
}

template <typename T, typename P>
inline auto ConstructPointer(P&& parser) {
  using ParserType = std::remove_reference_t<P>;
  return ParserPointerType<T, typename ParserType::CharType>(
      new privates::ConstructiveParser<T, ParserType>(std::forward<P>(parser)));
}

}  // namespace pcomb
#endif  // PCOMB_CONSTRUCTIVE_H_
