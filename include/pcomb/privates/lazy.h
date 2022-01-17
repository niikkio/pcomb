#ifndef PCOMB_PRIVATES_LAZY_H_
#define PCOMB_PRIVATES_LAZY_H_

#include <string>

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"

#include "pcomb/privates/common.h"

namespace pcomb::privates {

template <typename P>
class LazyParser : public Parser<typename P::CharType, typename P::ValueType> {
 public:
  using CharType = typename P::CharType;
  using ValueType = typename P::ValueType;

 private:
  using ResultType = Result<ValueType>;
  using StreamType = IStream<CharType>;

 protected:
  std::string to_string_without_name() const override {
    return "Lazy " + wrapped(parser_);
  }

 public:
  explicit LazyParser(const P* p) : parser_(p) { }

  ResultType parse(StreamType* stream) const override {
    return parser_->parse(stream);
  }

 private:
  const P* parser_;
};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_LAZY_H_
