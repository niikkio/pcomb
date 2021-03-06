#ifndef PCOMB_PRIVATES_SKIPPED_H_
#define PCOMB_PRIVATES_SKIPPED_H_

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
class SkippedParser : public Parser<typename P::CharType, SkippedValue> {
 public:
  using CharType = typename P::CharType;
  using ValueType = SkippedValue;

 private:
  using ResultType = Result<ValueType>;
  using StreamType = IStream<CharType>;

 protected:
  std::string to_string_without_name() const override {
    return "Skipped " + wrapped(parser_);
  }

 public:
  explicit SkippedParser(ParserPointer<P>&& parser)
      : parser_(std::forward<ParserPointer<P>>(parser)) {
  }

  ResultType parse(StreamType* stream) const override {
    auto result = parser_->parse(stream);
    if (result.success()) {
      size_t consumed_number = result.get_consumed_number();
      return ResultType(consumed_number, ValueType());
    }

    return ResultType(Trace(this, stream, messages::NO_MESSAGE,
                            {std::move(result).get_trace()}));
  }

 private:
  ParserPointer<P> parser_;
};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_SKIPPED_H_
