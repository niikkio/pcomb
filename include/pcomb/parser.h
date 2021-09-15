#ifndef PCOMB_PARSER_H_
#define PCOMB_PARSER_H_

#include <memory>

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

template <typename T, typename C = char>
using ParserType = Parser<C, T>;

template <typename T, typename C = char>
using ParserPointerType = std::shared_ptr<ParserType<T, C>>;

}  // namespace pcomb
#endif  // PCOMB_PARSER_H_
