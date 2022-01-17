#ifndef PCOMB_CONSTRUCTIVE_H_
#define PCOMB_CONSTRUCTIVE_H_

#include <utility>

#include "pcomb/parser.h"

#include "pcomb/privates/constructive.h"

namespace pcomb {

template <typename T, typename P>
inline auto Construct(ParserPointer<P>&& parser) {
  return with_name(
      make<privates::ConstructiveParser<T, P>>(
          std::forward<ParserPointer<P>>(parser)),
      "Construct");
}

}  // namespace pcomb
#endif  // PCOMB_CONSTRUCTIVE_H_
