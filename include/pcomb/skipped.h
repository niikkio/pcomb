#ifndef PCOMB_SKIPPED_H_
#define PCOMB_SKIPPED_H_

#include <utility>

#include "pcomb/parser.h"
#include "pcomb/privates/skipped.h"

namespace pcomb {

template <typename P>
inline auto Skip(ParserPointer<P>&& parser) {
  return make<privates::SkippedParser<P>>(
      std::forward<ParserPointer<P>>(parser));
}

}  // namespace pcomb
#endif  // PCOMB_SKIPPED_H_
