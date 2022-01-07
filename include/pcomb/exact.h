#ifndef PCOMB_EXACT_H_
#define PCOMB_EXACT_H_

#include <utility>

#include "pcomb/parser.h"
#include "pcomb/privates/exact.h"

namespace pcomb {

template <typename P>
inline auto Exact(ParserPointer<P>&& parser) {
  return make<privates::ExactParser<P>>(
      std::forward<ParserPointer<P>>(parser));
}

}  // namespace pcomb
#endif  // PCOMB_EXACT_H_
