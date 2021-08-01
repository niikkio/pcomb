#ifndef PCOMB_SEQUENCE_H_
#define PCOMB_SEQUENCE_H_

#include <memory>
#include <tuple>
#include <utility>

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"

namespace pcomb {

template <typename P1, typename... PS>
using SequenceBaseType = Parser<CommonCharType<P1, PS...>,
                                std::tuple<typename P1::ValueType,
                                           typename PS::ValueType...>>;

template <typename P1, typename... PS>
class SequenceParser : public SequenceBaseType<P1, PS...> {
 public:
  using CharType = typename SequenceBaseType<P1, PS...>::CharType;
  using ValueType = typename SequenceBaseType<P1, PS...>::ValueType;

 private:
  using ResultType = Result<ValueType>;
  using StreamType = IStream<CharType>;
  using StorageType = std::tuple<P1, PS...>;
  static constexpr size_t StorageSize = 1 + sizeof...(PS);

 public:
  explicit SequenceParser(const P1& p1, const PS&... ps)
      : parsers_(std::make_tuple(p1, ps...)) {
  }

  explicit SequenceParser(P1&& p1, PS&&... ps)
      : parsers_(std::forward_as_tuple(p1, ps...)) {
  }

  ResultType parse(StreamType* stream) const override {
    auto stream_copy = std::unique_ptr<StreamType>(stream->clone());

    auto result = Sequence<0>::parse(parsers_, stream_copy.get());
    if (result.success()) {
        stream->consume(result.get_consumed_number());
    }
    return result;
  }

 private:
  template <size_t I, bool Dummy = true>
  class Sequence {
    using CurrentWrappedValueType = std::tuple<
        typename std::tuple_element_t<I, StorageType>::ValueType>;

   public:
    using RecursiveValueType = decltype(std::tuple_cat(
        std::declval<CurrentWrappedValueType>(),
        std::declval<typename Sequence<I+1>::RecursiveValueType>()));

    static Result<RecursiveValueType> parse(const StorageType& parsers,
                                            StreamType* stream) {
      auto result = std::get<I>(parsers).parse(stream);
      if (!result.success()) {
        return Result<RecursiveValueType>();
      }

      auto next_result = Sequence<I+1>::parse(parsers, stream);
      if (!next_result.success()) {
        return Result<RecursiveValueType>();
      }

      int consumed = result.get_consumed_number() +
          next_result.get_consumed_number();
      return Result<RecursiveValueType>(
          consumed,
          std::tuple_cat(
              CurrentWrappedValueType(std::move(result).get_value()),
              std::move(next_result).get_value()));
    }
  };

  template <bool Dummy>
  class Sequence<StorageSize-1, Dummy> {
   public:
    using RecursiveValueType = std::tuple<
        typename std::tuple_element_t<StorageSize-1, StorageType>::ValueType>;

    static Result<RecursiveValueType> parse(const StorageType& parsers,
                                            StreamType* stream) {
      auto result = std::get<StorageSize-1>(parsers).parse(stream);
      if (!result.success()) {
        return Result<RecursiveValueType>();
      }

      int consumed = result.get_consumed_number();
      return Result<RecursiveValueType>(
          consumed, RecursiveValueType(std::move(result).get_value()));
    }
  };

  StorageType parsers_;
};

}  // namespace pcomb
#endif  // PCOMB_SEQUENCE_H_
