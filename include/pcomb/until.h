#ifndef PCOMB_UNTIL_H_
#define PCOMB_UNTIL_H_

#include <utility>

#include "pcomb/parser.h"

#include "pcomb/privates/until.h"

namespace pcomb {

template <typename P>
inline auto Until(ParserPointer<P>&& parser) {
  return with_name(
      make<privates::UntilParser<P>>(std::forward<ParserPointer<P>>(parser)),
      "Until");
}

}  // namespace pcomb
#endif  // PCOMB_UNTIL_H_
