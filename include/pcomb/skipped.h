#ifndef PCOMB_SKIPPED_H_
#define PCOMB_SKIPPED_H_

#include <utility>

#include "pcomb/common.h"
#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"

namespace pcomb {

template <typename P>
class SkippedParser : public Parser<typename P::CharType, SkippedValue> {
 public:
  using CharType = typename P::CharType;
  using ValueType = SkippedValue;

 private:
  using ResultType = Result<ValueType>;
  using StreamType = IStream<CharType>;

 public:
  explicit SkippedParser(const P& parser) : parser_(parser) { }
  explicit SkippedParser(P&& parser) : parser_(std::forward<P>(parser)) { }

  ResultType parse(StreamType* stream) const override {
    auto result = parser_.parse(stream);
    if (result.success()) {
      int consumed_number = result.get_consumed_number();
      return ResultType(consumed_number, ValueType());
    }
    return ResultType();
  }

 private:
  P parser_;
};

}  // namespace pcomb
#endif  // PCOMB_SKIPPED_H_
