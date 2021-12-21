#ifndef PCOMB_PRIVATES_MANY_H_
#define PCOMB_PRIVATES_MANY_H_

#include <list>
#include <memory>
#include <utility>

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"
#include "pcomb/trace.h"

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
  explicit ManyParser(std::shared_ptr<P>&& parser, size_t min_count = 0)
      : parser_(std::forward<std::shared_ptr<P>>(parser)),
        min_count_(min_count),
        max_count_(0),
        is_unlimited_(true) {
  }

  ManyParser(std::shared_ptr<P>&& parser, size_t min_count, size_t max_count)
      : parser_(std::forward<std::shared_ptr<P>>(parser)),
        min_count_(min_count),
        max_count_(max_count),
        is_unlimited_(false) {
  }

  ResultType parse(StreamType* stream) const override {
    auto values = ValueType();
    auto stream_copy = std::unique_ptr<StreamType>(stream->clone());
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
    auto message = "not enough results: " +
                   std::to_string(min_count_) + " > " +
                   std::to_string(values.size());

    return ResultType(Trace("Many", stream->position(), std::move(message)));
  }

 private:
  std::shared_ptr<P> parser_;
  size_t min_count_;
  size_t max_count_;
  bool is_unlimited_;
};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_MANY_H_
