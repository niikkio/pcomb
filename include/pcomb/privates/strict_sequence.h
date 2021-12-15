#ifndef PCOMB_PRIVATE_STRICT_SEQUENCE_H_
#define PCOMB_PRIVATE_STRICT_SEQUENCE_H_

#include <memory>
#include <tuple>
#include <utility>

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"
#include "pcomb/privates/common.h"

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
  using StorageType = std::tuple<P1, PS...>;
  static constexpr size_t StorageSize = 1 + sizeof...(PS);

 public:
  explicit StrictSequenceParser(const P1& p1, const PS&... ps)
      : parsers_(std::make_tuple(p1, ps...)) {
  }

  explicit StrictSequenceParser(P1&& p1, PS&&... ps)
      : parsers_(std::forward_as_tuple(p1, ps...)) {
  }

  ResultType parse(StreamType* stream) const override {
    auto stream_copy = std::unique_ptr<StreamType>(stream->clone());

    auto result =
        RecursiveSequenceParser<0>::parse(parsers_, stream_copy.get());
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
        typename std::tuple_element_t<I, StorageType>::ValueType,
        typename RecursiveSequenceParser<I+1>::ValueType>;

   private:
    using ResultType = Result<ValueType>;

   public:
    static ResultType parse(const StorageType& parsers, StreamType* stream) {
      auto result = std::get<I>(parsers).parse(stream);
      if (!result.success()) {
        return ResultType(Trace("StrictSequence",
                                stream->position(),
                                "",
                                {std::move(result).get_trace()}));
      }

      auto next_result = RecursiveSequenceParser<I+1>::parse(parsers, stream);
      if (!next_result.success()) {
        return ResultType(Trace("StrictSequence",
                                stream->position(),
                                "",
                                {std::move(next_result).get_trace()}));
      }

      size_t consumed = result.get_consumed_number() +
          next_result.get_consumed_number();
      return ResultType(consumed, std::tuple_cat(
          WrappedValueType<I, StorageType>(std::move(result).get_value()),
          std::move(next_result).get_value()));
    }
  };

  template <bool Dummy>
  class RecursiveSequenceParser<StorageSize-1, Dummy> {
   public:
    using ValueType = WrappedValueType<StorageSize-1, StorageType>;

   private:
    using ResultType = Result<ValueType>;

   public:
    static ResultType parse(const StorageType& parsers, StreamType* stream) {
      auto result = std::get<StorageSize-1>(parsers).parse(stream);
      if (!result.success()) {
        return ResultType(Trace("StrictSequence",
                                stream->position(),
                                "",
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
