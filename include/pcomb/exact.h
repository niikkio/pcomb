#ifndef PCOMB_EXACT_H_
#define PCOMB_EXACT_H_

#include <memory>
#include <utility>

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"

namespace pcomb {

template <typename P>
class ExactParser : public Parser<typename P::CharType, typename P::ValueType> {
 public:
  using CharType = typename P::CharType;
  using ValueType = typename P::ValueType;

 private:
  using ResultType = Result<ValueType>;
  using StreamType = IStream<CharType>;

 public:
  explicit ExactParser(const P& p) : parser_(p) { }

  explicit ExactParser(P&& p) : parser_(std::forward<P>(p)) { }

  ResultType parse(StreamType* stream) const override {
    auto stream_copy = std::unique_ptr<StreamType>(stream->clone());

    auto result = parser_.parse(stream_copy.get());
    if (!result.success() || !stream_copy->empty()) {
      return ResultType();
    }

    stream->consume(result.get_consumed_number());
    return result;
  }

 private:
  P parser_;
};

}  // namespace pcomb
#endif  // PCOMB_EXACT_H_
