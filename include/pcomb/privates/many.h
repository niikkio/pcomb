#ifndef PCOMB_PRIVATES_MANY_H_
#define PCOMB_PRIVATES_MANY_H_

#include <list>
#include <utility>

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"
#include "pcomb/trace.h"

#include "pcomb/privates/strings.h"

namespace pcomb::privates {

template <typename P>
using ManyBaseType = Parser<typename P::CharType,
                            std::list<typename P::ValueType>>;

template <typename P>
class ManyParser : public ManyBaseType<P> {
 public:
  using CharType = typename ManyBaseType<P>::CharType;
  using ValueType = typename ManyBaseType<P>::ValueType;

 private:
  using ResultType = Result<ValueType>;
  using StreamType = IStream<CharType>;

 public:
  explicit ManyParser(ParserPointer<P>&& parser, size_t min_count = 0)
      : parser_(std::forward<ParserPointer<P>>(parser)),
        min_count_(min_count),
        max_count_(0),
        is_unlimited_(true) {
    this->name_ = MANY_PARSER_NAME;
  }

  ManyParser(ParserPointer<P>&& parser, size_t min_count, size_t max_count)
      : parser_(std::forward<ParserPointer<P>>(parser)),
        min_count_(min_count),
        max_count_(max_count),
        is_unlimited_(false) {
    this->name_ = MANY_PARSER_NAME;
  }

  ResultType parse(StreamType* stream) const override {
    auto values = ValueType();
    auto stream_copy = stream->clone();
    size_t consumed_number = 0;

    while (is_unlimited_ || values.size() < max_count_) {
      auto result = parser_->parse(stream_copy.get());
      if (!result.success()) {
          break;
      }

      consumed_number += result.get_consumed_number();
      values.emplace_back(std::move(result).get_value());
      // TODO: call stream->consume if values.size() >= min_count_ ?
    }

    if (values.size() >= min_count_) {
      stream->consume(consumed_number);
      return ResultType(consumed_number, std::move(values));
    }

    return ResultType(Trace(this, stream,
                            NOT_ENOUGH_RESULTS_ERROR_MESSAGE(values.size(),
                                                             min_count_)));
  }

 private:
  ParserPointer<P> parser_;
  size_t min_count_;
  size_t max_count_;
  bool is_unlimited_;
};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_MANY_H_
