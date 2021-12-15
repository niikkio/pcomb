#ifndef PCOMB_PRIVATES_SKIPPED_H_
#define PCOMB_PRIVATES_SKIPPED_H_

#include <utility>

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"
#include "pcomb/trace.h"
#include "pcomb/privates/common.h"

namespace pcomb::privates {

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
      size_t consumed_number = result.get_consumed_number();
      return ResultType(consumed_number, ValueType());
    }

    return ResultType(Trace("Skipped",
                            stream->position(),
                            "",
                            {std::move(result).get_trace()}));
  }

 private:
  P parser_;
};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_SKIPPED_H_
