#ifndef PCOMB_PRIVATES_NUMBER_H_
#define PCOMB_PRIVATES_NUMBER_H_

#include <functional>
#include <string>
#include <utility>

#include "pcomb/messages.h"
#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"
#include "pcomb/trace.h"

#include "pcomb/privates/common.h"
#include "pcomb/privates/digit.h"

namespace pcomb::privates {

template <typename C, typename V>
class NumberParser : public Parser<C, V> {
 public:
  using CharType = C;
  using ValueType = V;

 private:
  using ResultType = Result<ValueType>;
  using StreamType = IStream<CharType>;

  using LogType = Trace::LogType;
  using TraceBuilderType = std::function<Trace(LogType&&)>;

 public:
  ResultType parse(StreamType* stream) const override {
    auto trace_builder = TraceBuilderType(
      [this, stream](LogType&& log) {
        return Trace(this, stream, messages::NO_MESSAGE,
                     std::forward<LogType>(log));
      });
    return InnerNumberParser<ValueType>::parse(trace_builder, stream);
  }

 protected:
  std::string to_string_without_name() const override {
    return InnerNumberParser<ValueType>::to_string_without_name();
  }

 private:
  template <typename NumberType, bool Dummy = true>
  struct InnerNumberParser {
    static ResultType parse(const TraceBuilderType& trace_builder,
                            StreamType* stream);
    static std::string to_string_without_name();
  };

  template <bool Dummy>
  struct InnerNumberParser<int, Dummy> {
    static ResultType parse(const TraceBuilderType& trace_builder,
                            StreamType* stream) {
      auto result = base_parser()->parse(stream);
      if (!result.success()) {
        return ResultType(trace_builder({std::move(result).get_trace()}));
      }

      size_t consumed_number = result.get_consumed_number();
      return ResultType(consumed_number, std::move(result).get_value() - '0');
    }

    static std::string to_string_without_name() {
      return "Number(int) " + wrapped(base_parser());
    }
   private:
    static auto base_parser() {
      return make<DigitParser<CharType>>();
    }
  };
};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_NUMBER_H_
