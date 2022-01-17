#ifndef PCOMB_PRIVATES_ADAPTIVE_H_
#define PCOMB_PRIVATES_ADAPTIVE_H_

#include <functional>
#include <tuple>
#include <utility>

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"
#include "pcomb/trace.h"

#include "pcomb/privates/strings.h"

namespace pcomb::privates {

template <typename P, typename F>
class AdaptedValue {
  template <typename V>
  struct UnwrappedValue {
    using Type = std::result_of_t<F(V)>;

    static Type invoke(const F& func, V&& value) {
      return func(std::forward<V>(value));
    }
  };

  template <typename... VS>
  struct UnwrappedValue<std::tuple<VS...>> {
   private:
     using TV = std::tuple<VS...>;

   public:
    using Type = std::result_of_t<F(VS...)>;

    static Type invoke(const F& func, TV&& values) {
      return std::apply(func, std::forward<TV>(values));
    }
  };

  using ParserValueType = typename P::ValueType;

 public:
  using Type = typename UnwrappedValue<ParserValueType>::Type;

  static Type invoke(const F& func, ParserValueType&& value) {
    return UnwrappedValue<ParserValueType>::invoke(
        func, std::forward<ParserValueType>(value));
  }
};

template <typename P, typename F>
class AdaptiveParser
    : public Parser<typename P::CharType, typename AdaptedValue<P, F>::Type> {
 public:
  using CharType = typename P::CharType;
  using ValueType = typename AdaptedValue<P, F>::Type;

 private:
  using ResultType = Result<ValueType>;
  using StreamType = IStream<CharType>;

 public:
  explicit AdaptiveParser(ParserPointer<P>&& p, F&& f)
      : parser_(std::forward<ParserPointer<P>>(p))
      , func_(std::forward<F>(f)) {
    this->name_ = ADAPTIVE_PARSER_NAME(parser_);
  }

  ResultType parse(StreamType* stream) const override {
    auto result = parser_->parse(stream);
    if (!result.success()) {
      return ResultType(Trace(this, stream, EMPTY_MESSAGE,
                              {std::move(result).get_trace()}));
    }

    size_t consumed_number = result.get_consumed_number();
    return ResultType(consumed_number, AdaptedValue<P, F>::invoke(
        func_, std::move(result).get_value()));
  }

 private:
  ParserPointer<P> parser_;
  F func_;
};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_ADAPTIVE_H_
