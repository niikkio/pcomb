#ifndef PCOMB_PRIVATES_UNTIL_H_
#define PCOMB_PRIVATES_UNTIL_H_

#include <list>
#include <string>
#include <utility>

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"

#include "pcomb/privates/common.h"

namespace pcomb::privates {

template <typename P>
using UntilBaseType = Parser<typename P::CharType,
                             std::list<typename P::CharType>>;

template <typename P>
class UntilParser : public UntilBaseType<P> {
 public:
  using CharType = typename UntilBaseType<P>::CharType;
  using ValueType = typename UntilBaseType<P>::ValueType;

 private:
  using ResultType = Result<ValueType>;
  using StreamType = IStream<CharType>;

 protected:
  std::string to_string_without_name() const override {
    return "Until " + wrapped(parser_);
  }

 public:
  explicit UntilParser(ParserPointer<P>&& parser)
      : parser_(std::forward<ParserPointer<P>>(parser)) {
  }

  ResultType parse(StreamType* stream) const override {
    auto stream_copy = stream->clone();

    ValueType values;
    size_t consumed_number = 0;
    while (!parser_->parse(stream_copy.get()).success()) {
      if (stream_copy->empty()) {
        break;
      }

      values.push_back(stream_copy->head());
      stream_copy->consume(1);
      consumed_number += 1;
    }

    stream->consume(consumed_number);
    return ResultType(consumed_number, values);
  }

 private:
  ParserPointer<P> parser_;
};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_UNTIL_H_
