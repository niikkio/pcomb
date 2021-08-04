#ifndef PCOMB_PARSER_H_
#define PCOMB_PARSER_H_

#include <type_traits>

#include "pcomb/result.h"
#include "pcomb/stream.h"

namespace pcomb {

template <typename C, typename V>
class Parser {
 public:
  using CharType = C;
  using ValueType = V;

  virtual ~Parser() = default;

  virtual Result<ValueType> parse(IStream<CharType>* stream) const = 0;
};

}  // namespace pcomb
#endif  // PCOMB_PARSER_H_
