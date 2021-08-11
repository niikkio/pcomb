#ifndef PCOMB_PRIVATES_END_H_
#define PCOMB_PRIVATES_END_H_

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"
#include "pcomb/privates/common.h"

namespace pcomb::privates {

template <typename C>
class EndParser : public Parser<C, NoValue> {
 public:
  using CharType = C;
  using ValueType = NoValue;

 private:
  using ResultType = Result<ValueType>;
  using StreamType = IStream<CharType>;

 public:
  ResultType parse(StreamType* stream) const override {
    if (!stream->empty()) {
      return ResultType();
    }

    return ResultType(0, ValueType());
  }
};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_END_H_
