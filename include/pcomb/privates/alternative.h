#ifndef PCOMB_PRIVATES_ALTERNATIVE_H_
#define PCOMB_PRIVATES_ALTERNATIVE_H_

#include <list>
#include <tuple>
#include <utility>
#include <variant>

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"
#include "pcomb/trace.h"
#include "pcomb/privates/common.h"

namespace pcomb::privates {

template <typename P1, typename... PS>
class AlternativeValue {
  using TV = std::tuple<typename P1::ValueType, typename PS::ValueType...>;
  using UTV = WithoutDuplicatesType<TV>;

  template <typename V>
  struct WrappedValue {
    using Type = V;
  };

  template <typename... VS>
  struct WrappedValue<std::tuple<VS...>> {
    using Type = std::variant<VS...>;
  };

 public:
  using Type = typename WrappedValue<ExtractedType<UTV>>::Type;
};

template <typename P1, typename... PS>
using AlternativeValueType = typename AlternativeValue<P1, PS...>::Type;

template <typename P1, typename... PS>
using AlternativeBaseType = Parser<CommonCharType<P1, PS...>,
                                   AlternativeValueType<P1, PS...>>;

template <typename P1, typename... PS>
class AlternativeParser : public AlternativeBaseType<P1, PS...> {
 public:
  using CharType = typename AlternativeBaseType<P1, PS...>::CharType;
  using ValueType = typename AlternativeBaseType<P1, PS...>::ValueType;

 private:
  using ResultType = Result<ValueType>;
  using StreamType = IStream<CharType>;
  using StorageType = std::tuple<P1, PS...>;
  using LogType = std::list<Trace>;
  static constexpr size_t StorageSize = 1 + sizeof...(PS);

 public:
  explicit AlternativeParser(const P1& p1, const PS&... ps)
      : parsers_(std::make_tuple(p1, ps...)) {
  }

  explicit AlternativeParser(P1&& p1, PS&&... ps)
      : parsers_(std::forward_as_tuple(p1, ps...)) {
  }

  ResultType parse(StreamType* stream) const override {
    LogType log;
    return RecursiveAlternativeParser<0>::parse(parsers_, stream, &log);
  }

 private:
  template <size_t I, bool Dummy = true>
  struct RecursiveAlternativeParser {
    static ResultType parse(
          const StorageType& parsers, StreamType* stream, LogType* log) {
      auto result = std::get<I>(parsers).parse(stream);
      if (result.success()) {
        size_t consumed = result.get_consumed_number();
        return ResultType(consumed, ValueType(std::move(result).get_value()));
      }
      log->push_back(std::move(result).get_trace());
      return RecursiveAlternativeParser<I+1>::parse(parsers, stream, log);
    }
  };

  template <bool Dummy>
  struct RecursiveAlternativeParser<StorageSize-1, Dummy> {
    static ResultType parse(
        const StorageType& parsers, StreamType* stream, LogType* log) {
      auto result = std::get<StorageSize-1>(parsers).parse(stream);
      if (result.success()) {
        size_t consumed = result.get_consumed_number();
        return ResultType(consumed, ValueType(std::move(result).get_value()));
      }
      log->push_back(std::move(result).get_trace());
      return ResultType(Trace("Alternative",
                              stream->position(),
                              "",
                              std::move(*log)));
    }
  };

  StorageType parsers_;
};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_ALTERNATIVE_H_
