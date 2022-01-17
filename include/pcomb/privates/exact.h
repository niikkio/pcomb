#ifndef PCOMB_PRIVATES_EXACT_H_
#define PCOMB_PRIVATES_EXACT_H_

#include <string>
#include <utility>

#include "pcomb/messages.h"
#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"
#include "pcomb/trace.h"

#include "pcomb/privates/common.h"

namespace pcomb::privates {

template <typename P>
class ExactParser : public Parser<typename P::CharType, typename P::ValueType> {
 public:
  using CharType = typename P::CharType;
  using ValueType = typename P::ValueType;

 private:
  using ResultType = Result<ValueType>;
  using StreamType = IStream<CharType>;

 protected:
  std::string to_string_without_name() const override {
    return "Exact " + wrapped(parser_);
  }

 public:
  explicit ExactParser(ParserPointer<P>&& p)
      : parser_(std::forward<ParserPointer<P>>(p)) {
  }

  ResultType parse(StreamType* stream) const override {
    auto stream_copy = stream->clone();

    auto result = parser_->parse(stream_copy.get());
    if (!result.success()) {
      return ResultType(Trace(this, stream, messages::NO_MESSAGE,
                              {std::move(result).get_trace()}));
    }

    if (!stream_copy->empty()) {
      return ResultType(Trace(this, stream,
                              messages::END_WAS_EXPECTED(
                                  stream_copy->head(),
                                  stream_copy->position())));
    }

    stream->consume(result.get_consumed_number());
    return result;
  }

 private:
  ParserPointer<P> parser_;
};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_EXACT_H_
