#ifndef PCOMB_PRIVATES_STRICT_ADAPTIVE_H_
#define PCOMB_PRIVATES_STRICT_ADAPTIVE_H_

#include <functional>
#include <tuple>
#include <utility>

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"

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

 public:
  explicit StrictAdaptiveParser(const P& p, const F& f)
      : parser_(p), func_(f) {
  }

  explicit StrictAdaptiveParser(P&& p, F&& f)
      : parser_(std::forward<P>(p)), func_(std::forward<F>(f)) {
  }

  ResultType parse(StreamType* stream) const override {
    auto result = parser_.parse(stream);
    if (!result.success()) {
      return ResultType();
    }

    int consumed_number = result.get_consumed_number();
    return ResultType(consumed_number, func_(std::move(result).get_value()));
  }

 private:
  P parser_;
  F func_;
};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_STRICT_ADAPTIVE_H_
