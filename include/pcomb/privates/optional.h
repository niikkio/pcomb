#ifndef PCOMB_PRIVATES_OPTIONAL_H_
#define PCOMB_PRIVATES_OPTIONAL_H_

#include <optional>
#include <utility>

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"

namespace pcomb::privates {

template <typename P>
class OptionalParser : public Parser<typename P::CharType,
                                     std::optional<typename P::ValueType>> {
 public:
  using CharType = typename P::CharType;
  using ValueType = std::optional<typename P::ValueType>;

 private:
  using ResultType = Result<ValueType>;
  using StreamType = IStream<CharType>;

 public:
  explicit OptionalParser(ParserPointer<P>&& p)
      : parser_(std::forward<ParserPointer<P>>(p)) { }

  ResultType parse(StreamType* stream) const override {
    auto result = parser_->parse(stream);
    if (!result.success()) {
      return ResultType(0, ValueType());
    }

    size_t consumed_number = result.get_consumed_number();
    return ResultType(consumed_number, std::move(result).get_value());
  }

 private:
  ParserPointer<P> parser_;
};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_OPTIONAL_H_
