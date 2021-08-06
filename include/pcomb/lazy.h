#ifndef PCOMB_LAZY_H_
#define PCOMB_LAZY_H_

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"

namespace pcomb {

template <typename P>
class LazyParser : public Parser<typename P::CharType, typename P::ValueType> {
 public:
  using CharType = typename P::CharType;
  using ValueType = typename P::ValueType;

 private:
  using ResultType = Result<ValueType>;
  using StreamType = IStream<CharType>;

 public:
  explicit LazyParser(const P* p) : parser_(p) { }

  ResultType parse(StreamType* stream) const override {
    return parser_->parse(stream);
  }

 private:
  const P* parser_;
};

}  // namespace pcomb
#endif  // PCOMB_LAZY_H_
