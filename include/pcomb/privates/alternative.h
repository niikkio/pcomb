#ifndef PCOMB_PRIVATES_ALTERNATIVE_H_
#define PCOMB_PRIVATES_ALTERNATIVE_H_

#include <functional>
#include <list>
#include <string>
#include <tuple>
#include <utility>
#include <variant>

#include "pcomb/messages.h"
#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"
#include "pcomb/trace.h"

#include "pcomb/privates/common.h"
#include "pcomb/privates/magic.h"

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
  using StorageType = std::tuple<ParserPointer<P1>, ParserPointer<PS>...>;
  using ParsersType = std::tuple<P1, PS...>;
  using LogType = Trace::LogType;
  using TraceBuilderType = std::function<Trace(StreamType*, LogType*)>;
  static constexpr size_t StorageSize = 1 + sizeof...(PS);

 protected:
  std::string to_string_without_name() const override {
    return "Alternative " + wrapped(parsers_);
  }

 public:
  explicit AlternativeParser(ParserPointer<P1>&& p1, ParserPointer<PS>&&... ps)
      : parsers_(std::forward_as_tuple(p1, ps...)) {
  }

  ResultType parse(StreamType* stream) const override {
    LogType log;
    auto trace_builder = TraceBuilderType(
        [this](StreamType* stream, LogType* log) {
          return Trace(this, stream, messages::NO_MESSAGE, std::move(*log));
        });
    return RecursiveAlternativeParser<0>::parse(
        parsers_, trace_builder, stream, &log);
  }

 private:
  template <size_t I, bool Dummy = true>
  struct RecursiveAlternativeParser {
    static ResultType parse(const StorageType& parsers,
                            const TraceBuilderType& trace_builder,
                            StreamType* stream, LogType* log) {
      auto result = std::get<I>(parsers)->parse(stream);
      if (result.success()) {
        size_t consumed = result.get_consumed_number();
        return ResultType(consumed, ValueType(std::move(result).get_value()));
      }
      log->push_back(std::move(result).get_trace());
      return RecursiveAlternativeParser<I+1>::parse(
          parsers, trace_builder, stream, log);
    }
  };

  template <bool Dummy>
  struct RecursiveAlternativeParser<StorageSize-1, Dummy> {
    static ResultType parse(const StorageType& parsers,
                            const TraceBuilderType& trace_builder,
                            StreamType* stream, LogType* log) {
      auto result = std::get<StorageSize-1>(parsers)->parse(stream);
      if (result.success()) {
        size_t consumed = result.get_consumed_number();
        return ResultType(consumed, ValueType(std::move(result).get_value()));
      }
      log->push_back(std::move(result).get_trace());

      auto trace = trace_builder(stream, log);
      return ResultType(std::move(trace));
    }
  };

  StorageType parsers_;
};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_ALTERNATIVE_H_
