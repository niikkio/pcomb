#ifndef PCOMB_PRIVATES_CONSTRUCTIVE_H_
#define PCOMB_PRIVATES_CONSTRUCTIVE_H_

#include <tuple>
#include <utility>

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"
#include "pcomb/trace.h"

namespace pcomb::privates {

template <class T, class Tuple, size_t... Is>
T construct_from_tuple(Tuple&& tuple, std::index_sequence<Is...> ) {
  return T{std::get<Is>(std::forward<Tuple>(tuple))...};
}

template <class T, class Tuple>
T construct_from_tuple(Tuple&& tuple) {
  return construct_from_tuple<T>(std::forward<Tuple>(tuple),
      std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
}


template <typename T, typename P>
class ConstructedValue {
  template <typename V>
  struct UnwrappedValue {
    static T construct(V&& value) {
      return T{std::forward<V>(value)};
    }
  };

  template <typename... VS>
  struct UnwrappedValue<std::tuple<VS...>> {
   private:
    using TV = std::tuple<VS...>;

   public:
    static T construct(TV&& values) {
      return construct_from_tuple<T>(std::forward<TV>(values));
    }
  };

  using ParserValueType = typename P::ValueType;

 public:
  static T construct(ParserValueType&& value) {
    return UnwrappedValue<ParserValueType>::construct(
        std::forward<ParserValueType>(value));
  }
};


template <typename T, typename P>
class ConstructiveParser : public Parser<typename P::CharType, T> {
 public:
  using CharType = typename P::CharType;
  using ValueType = T;

 private:
  using ResultType = Result<ValueType>;
  using StreamType = IStream<CharType>;

 public:
  explicit ConstructiveParser(ParserPointer<P>&& p)
      : parser_(std::forward<ParserPointer<P>>(p)) { }

  ResultType parse(StreamType* stream) const override {
    auto result = parser_->parse(stream);
    if (!result.success()) {
      return ResultType(Trace("Constructive",
                              stream->position(),
                              "",
                              {std::move(result).get_trace()}));
    }

    size_t consumed_number = result.get_consumed_number();
    return ResultType(consumed_number,
        ConstructedValue<T, P>::construct(std::move(result).get_value()));
  }

 private:
  ParserPointer<P> parser_;
};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_CONSTRUCTIVE_H_
