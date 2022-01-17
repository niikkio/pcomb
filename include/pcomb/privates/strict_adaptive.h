#ifndef PCOMB_PRIVATES_STRICT_ADAPTIVE_H_
#define PCOMB_PRIVATES_STRICT_ADAPTIVE_H_

#include <functional>
#include <string>
#include <tuple>
#include <utility>

#include "pcomb/messages.h"
#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"
#include "pcomb/trace.h"

#include "pcomb/privates/common.h"

namespace pcomb::privates {

template <typename P, typename F>
class StrictAdaptiveParser
    : public Parser<typename P::CharType,
                    std::result_of_t<F(typename P::ValueType)>> {
 public:
  using CharType = typename P::CharType;
  using ValueType = std::result_of_t<F(typename P::ValueType)>;

 private:
  using ResultType = Result<ValueType>;
  using StreamType = IStream<CharType>;

 protected:
  std::string to_string_without_name() const override {
    return "Strict Adaptive " + wrapped(parser_);
  }

 public:
  explicit StrictAdaptiveParser(ParserPointer<P>&& p, F&& f)
      : parser_(std::forward<ParserPointer<P>>(p))
      , func_(std::forward<F>(f)) {
  }

  ResultType parse(StreamType* stream) const override {
    auto result = parser_->parse(stream);
    if (!result.success()) {
      return ResultType(Trace(this, stream, messages::NO_MESSAGE,
                              {std::move(result).get_trace()}));
    }

    size_t consumed_number = result.get_consumed_number();
    return ResultType(consumed_number, func_(std::move(result).get_value()));
  }

 private:
  ParserPointer<P> parser_;
  F func_;
};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_STRICT_ADAPTIVE_H_
