#ifndef PCOMB_PRIVATE_STRICT_SEQUENCE_H_
#define PCOMB_PRIVATE_STRICT_SEQUENCE_H_

#include <tuple>
#include <utility>

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"
#include "pcomb/trace.h"

#include "pcomb/privates/magic.h"
#include "pcomb/privates/strings.h"

namespace pcomb::privates {

template <typename P1, typename... PS>
using StrictSequenceBaseType = Parser<CommonCharType<P1, PS...>,
                                      std::tuple<typename P1::ValueType,
                                                 typename PS::ValueType...>>;

template <typename P1, typename... PS>
class StrictSequenceParser : public StrictSequenceBaseType<P1, PS...> {
 public:
  using CharType = typename StrictSequenceBaseType<P1, PS...>::CharType;
  using ValueType = typename StrictSequenceBaseType<P1, PS...>::ValueType;

 private:
  using ResultType = Result<ValueType>;
  using StreamType = IStream<CharType>;
  using ParsersType = std::tuple<P1, PS...>;
  using StorageType = std::tuple<ParserPointer<P1>, ParserPointer<PS>...>;
  static constexpr size_t StorageSize = 1 + sizeof...(PS);

 public:
  explicit StrictSequenceParser(
      ParserPointer<P1>&& p1, ParserPointer<PS>&&... ps)
          : parsers_(std::forward_as_tuple(p1, ps...)) {
    this->name_ = STRICT_SEQUENCE_PARSER_NAME;
  }

  ResultType parse(StreamType* stream) const override {
    auto stream_copy = stream->clone();

    auto result =
        RecursiveSequenceParser<0>::parse(this, stream_copy.get());
    if (result.success()) {
      stream->consume(result.get_consumed_number());
    }
    return result;
  }

 private:
  template <size_t I, bool Dummy = true>
  class RecursiveSequenceParser {
   public:
    using ValueType = ConcatedType<
        typename std::tuple_element_t<I, ParsersType>::ValueType,
        typename RecursiveSequenceParser<I+1>::ValueType>;

   private:
    using ResultType = Result<ValueType>;

   public:
    static ResultType parse(const StrictSequenceParser* owner,
                            StreamType* stream) {
      auto result = std::get<I>(owner->parsers_)->parse(stream);
      if (!result.success()) {
        return ResultType(Trace(owner, stream, EMPTY_MESSAGE,
                                {std::move(result).get_trace()}));
      }

      auto next_result = RecursiveSequenceParser<I+1>::parse(owner, stream);
      if (!next_result.success()) {
        return ResultType(Trace(owner, stream, EMPTY_MESSAGE,
                                {std::move(next_result).get_trace()}));
      }

      size_t consumed = result.get_consumed_number() +
          next_result.get_consumed_number();
      return ResultType(consumed, std::tuple_cat(
          WrappedValueType<I, ParsersType>(std::move(result).get_value()),
          std::move(next_result).get_value()));
    }
  };

  template <bool Dummy>
  class RecursiveSequenceParser<StorageSize-1, Dummy> {
   public:
    using ValueType = WrappedValueType<StorageSize-1, ParsersType>;

   private:
    using ResultType = Result<ValueType>;

   public:
    static ResultType parse(const StrictSequenceParser* owner,
                            StreamType* stream) {
      auto result = std::get<StorageSize-1>(owner->parsers_)->parse(stream);
      if (!result.success()) {
        return ResultType(Trace(owner, stream, EMPTY_MESSAGE,
                                {std::move(result).get_trace()}));
      }

      size_t consumed = result.get_consumed_number();
      return ResultType(consumed, ValueType(std::move(result).get_value()));
    }
  };

  StorageType parsers_;
};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_STRICT_SEQUENCE_H_
