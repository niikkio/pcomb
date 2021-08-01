#ifndef PCOMB_MANY_H_
#define PCOMB_MANY_H_

#include <list>
#include <memory>
#include <utility>

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"

namespace pcomb {
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
  explicit ManyParser(const P& parser, int min_count = 0, int max_count = -1)
      : parser_(parser),
        min_count_(min_count),
        max_count_(max_count) {
  }

  explicit ManyParser(P&& parser, int min_count = 0, int max_count = -1)
      : parser_(std::forward<P>(parser)),
        min_count_(min_count),
        max_count_(max_count) {
  }

  ResultType parse(StreamType* stream) const override {
    auto values = ValueType();
    auto stream_copy = std::unique_ptr<StreamType>(stream->clone());
    int consumed_number = 0;

    while (max_count_ < 0 || values.size() < max_count_) {
      auto result = parser_.parse(stream_copy.get());
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
    return ResultType();
  }

 private:
  P parser_;
  int min_count_;
  int max_count_;
};

}  // namespace pcomb
#endif  // PCOMB_MANY_H_
