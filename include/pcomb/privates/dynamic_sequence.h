#ifndef PCOMB_PRIVATES_DYNAMIC_SEQUENCE_H_
#define PCOMB_PRIVATES_DYNAMIC_SEQUENCE_H_

#include <list>
#include <memory>
#include <utility>

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"

namespace pcomb::privates {

template <typename P>
class DynamicSequenceParser : public Parser<
    typename P::CharType, std::list<typename P::ValueType>> {
 public:
  using CharType = typename P::CharType;
  using ValueType = std::list<typename P::ValueType>;

 private:
  using ResultType = Result<ValueType>;
  using StreamType = IStream<CharType>;
  using StorageType = std::list<P>;

 public:
  explicit DynamicSequenceParser(const StorageType& ps)
      : parsers_(ps) {
  }

  explicit DynamicSequenceParser(StorageType&& ps)
      : parsers_(std::forward<StorageType>(ps)) {
  }

  ResultType parse(StreamType* stream) const override {
    auto stream_copy = std::unique_ptr<StreamType>(stream->clone());

    ValueType values;
    int consumed_number = 0;
    for (auto it = parsers_.cbegin(); it != parsers_.cend(); ++it) {
      auto result = it->parse(stream_copy.get());
      if (!result.success()) {
        return ResultType();
      }

      consumed_number += result.get_consumed_number();
      values.push_back(std::move(result).get_value());
    }

    stream->consume(consumed_number);
    return ResultType(consumed_number, std::move(values));
  }

 private:
  StorageType parsers_;
};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_DYNAMIC_SEQUENCE_H_
