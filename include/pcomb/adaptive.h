#ifndef PCOMB_ADAPTIVE_H_
#define PCOMB_ADAPTIVE_H_

#include <functional>
#include <utility>

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"

namespace pcomb {

template <typename P, typename F>
class AdaptiveParser
    : public Parser<typename P::CharType,
                    std::result_of_t<F(typename P::ValueType)>> {
 public:
  using CharType = typename P::CharType;
  using ValueType = std::result_of_t<F(typename P::ValueType)>;

 private:
  using ResultType = Result<ValueType>;
  using StreamType = IStream<CharType>;

 public:
  explicit AdaptiveParser(const P& p, const F& f)
      : parser_(p), func_(f) {
  }

  explicit AdaptiveParser(P&& p, F&& f)
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

}  // namespace pcomb

#endif  // PCOMB_ADAPRIVE_H_