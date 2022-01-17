#ifndef PCOMB_PRIVATES_EXACT_H_
#define PCOMB_PRIVATES_EXACT_H_

#include <utility>

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"
#include "pcomb/trace.h"

#include "pcomb/privates/strings.h"

namespace pcomb::privates {

template <typename P>
class ExactParser : public Parser<typename P::CharType, typename P::ValueType> {
 public:
  using CharType = typename P::CharType;
  using ValueType = typename P::ValueType;

 private:
  using ResultType = Result<ValueType>;
  using StreamType = IStream<CharType>;

 public:
  explicit ExactParser(ParserPointer<P>&& p)
      : parser_(std::forward<ParserPointer<P>>(p)) {
    this->name_ = EXACT_PARSER_NAME(parser_);
  }

  ResultType parse(StreamType* stream) const override {
    auto stream_copy = stream->clone();

    auto result = parser_->parse(stream_copy.get());
    if (!result.success()) {
      return ResultType(Trace(this, stream, EMPTY_MESSAGE,
                              {std::move(result).get_trace()}));
    }

    if (!stream_copy->empty()) {
      return ResultType(Trace(this, stream,
                              EOF_WAS_EXPECTED_ERROR_MESSAGE(stream_copy)));
    }

    stream->consume(result.get_consumed_number());
    return result;
  }

 private:
  ParserPointer<P> parser_;
};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_EXACT_H_
