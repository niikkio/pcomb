#ifndef PCOMB_PRIVATE_SEQUENCE_H_
#define PCOMB_PRIVATE_SEQUENCE_H_

#include <tuple>
#include <type_traits>
#include <utility>

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"
#include "pcomb/trace.h"
#include "pcomb/privates/common.h"
#include "pcomb/privates/skipped.h"

namespace pcomb::privates {

template <typename P1, typename... PS>
using SequenceBaseType = Parser<
    CommonCharType<P1, PS...>,
    ExtractedType<WithoutSkippedType<
        std::tuple<typename P1::ValueType, typename PS::ValueType...>>>>;

template <typename P1, typename... PS>
class SequenceParser : public SequenceBaseType<P1, PS...> {
 public:
  using CharType = typename SequenceBaseType<P1, PS...>::CharType;
  using ValueType = typename SequenceBaseType<P1, PS...>::ValueType;

 private:
  using ResultType = Result<ValueType>;
  using StreamType = IStream<CharType>;
  using ParsersType = std::tuple<P1, PS...>;
  using StorageType = std::tuple<ParserPointer<P1>, ParserPointer<PS>...>;
  static constexpr size_t StorageSize = 1 + sizeof...(PS);

 public:
  explicit SequenceParser(ParserPointer<P1>&& p1, ParserPointer<PS>&&... ps)
      : parsers_(std::forward_as_tuple(p1, ps...)) {
  }

  ResultType parse(StreamType* stream) const override {
    auto stream_copy = stream->clone();

    using RootParser =
        RecursiveSequenceParser<0, IsSkippedParser<0, ParsersType>>;

    auto result = RootParser::parse(parsers_, stream_copy.get());

    if (result.success()) {
      using TempType = typename RootParser::ValueType;
      constexpr size_t TempSize = std::tuple_size_v<TempType>;

      size_t consumed_number = result.get_consumed_number();
      stream->consume(consumed_number);

      return ResultType(
          consumed_number,
          Extract<TempSize, TempType>::from(std::move(result).get_value()));
    }

    return ResultType(
        Trace("Sequence", stream, "", {std::move(result).get_trace()}));
  }

 private:
  template <size_t I, bool Skip = false, bool Dummy = true>
  class RecursiveSequenceParser {
    static constexpr bool SkipNext = IsSkippedParser<I+1, ParsersType>;

   public:
    using ValueType = ConcatedType<
        typename std::tuple_element_t<I, ParsersType>::ValueType,
        typename RecursiveSequenceParser<I+1, SkipNext>::ValueType>;

   private:
    using ResultType = Result<ValueType>;

   public:
    static ResultType parse(const StorageType& parsers, StreamType* stream) {
      auto result = std::get<I>(parsers)->parse(stream);
      if (!result.success()) {
        return ResultType(
            Trace("Sequence", stream, "", {std::move(result).get_trace()}));
      }

      auto next_result =
          RecursiveSequenceParser<I+1, SkipNext>::parse(parsers, stream);
      if (!next_result.success()) {
        return ResultType(
            Trace("Sequence", stream, "",
                  {std::move(next_result).get_trace()}));
      }

      size_t consumed = result.get_consumed_number() +
          next_result.get_consumed_number();

      return ResultType(consumed, std::tuple_cat(
          WrappedValueType<I, ParsersType>(std::move(result).get_value()),
          std::move(next_result).get_value()));
    }
  };

  template <size_t I, bool Dummy>
  class RecursiveSequenceParser<I, true, Dummy> {
    static constexpr bool SkipNext = IsSkippedParser<I+1, ParsersType>;

   public:
    using ValueType =
        typename RecursiveSequenceParser<I+1, SkipNext>::ValueType;

   private:
    using ResultType = Result<ValueType>;

   public:
    static ResultType parse(const StorageType& parsers, StreamType* stream) {
      auto result = std::get<I>(parsers)->parse(stream);
      if (!result.success()) {
        return ResultType(Trace("Sequence",
                                stream,
                                "",
                                {std::move(result).get_trace()}));
      }

      auto next_result =
          RecursiveSequenceParser<I+1, SkipNext>::parse(parsers, stream);
      if (!next_result.success()) {
        return ResultType(Trace("Sequence",
                                stream,
                                "",
                                {std::move(next_result).get_trace()}));
      }

      size_t consumed = result.get_consumed_number() +
          next_result.get_consumed_number();
      return ResultType(consumed, std::move(next_result).get_value());
    }
  };

  template <bool Dummy>
  class RecursiveSequenceParser<StorageSize-1, false, Dummy> {
   public:
    using ValueType = WrappedValueType<StorageSize-1, ParsersType>;

   private:
    using ResultType = Result<ValueType>;

   public:
    static ResultType parse(const StorageType& parsers, StreamType* stream) {
      auto result = std::get<StorageSize-1>(parsers)->parse(stream);
      if (!result.success()) {
        return ResultType(Trace("Sequence",
                                stream,
                                "",
                                {std::move(result).get_trace()}));
      }

      size_t consumed = result.get_consumed_number();
      return ResultType(consumed, ValueType(std::move(result).get_value()));
    }
  };

  template <bool Dummy>
  class RecursiveSequenceParser<StorageSize-1, true, Dummy> {
   public:
    using ValueType = std::tuple<>;

   private:
    using ResultType = Result<ValueType>;

   public:
    static ResultType parse(const StorageType& parsers, StreamType* stream) {
      auto result = std::get<StorageSize-1>(parsers)->parse(stream);
      if (!result.success()) {
        return ResultType(Trace("Sequence",
                                stream,
                                "",
                                {std::move(result).get_trace()}));
      }

      size_t consumed = result.get_consumed_number();
      return ResultType(consumed, ValueType());
    }
  };

  StorageType parsers_;
};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_SEQUENCE_H_
