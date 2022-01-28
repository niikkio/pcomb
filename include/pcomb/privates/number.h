#ifndef PCOMB_PRIVATES_NUMBER_H_
#define PCOMB_PRIVATES_NUMBER_H_

#include <functional>
#include <list>
#include <numeric>
#include <optional>
#include <string>
#include <utility>

#include "pcomb/messages.h"
#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"
#include "pcomb/trace.h"

#include "pcomb/adaptive.h"
#include "pcomb/alternative.h"
#include "pcomb/many.h"
#include "pcomb/optional.h"
#include "pcomb/predicate.h"
#include "pcomb/sequence.h"
#include "pcomb/skipped.h"

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
      auto result = parser()->parse(stream);
      if (!result.success()) {
        return ResultType(trace_builder({std::move(result).get_trace()}));
      }

      size_t consumed_number = result.get_consumed_number();
      return ResultType(consumed_number, std::move(result).get_value());
    }

    static std::string to_string_without_name() {
      return "Number(int)";  //  + wrapped(parser());
    }

   private:
    static auto parser() {
      return Adapted(
          Seq(WithDefault(Any(Char('+'), Char('-')), '+'), Some(Digit())),
          [](const char sign, const std::list<char>& digits) {
            int v = std::reduce(digits.cbegin(), digits.cend(), 0,
                                [](int acc, char ch) {
                                  return 10 * acc + (ch - '0');
                                });
            return (sign == '-') ? -v : v;
          });
    }
  };

  template <bool Dummy>
  struct InnerNumberParser<uint64_t, Dummy> {
    static ResultType parse(const TraceBuilderType& trace_builder,
                            StreamType* stream) {
      auto result = parser()->parse(stream);
      if (!result.success()) {
        return ResultType(trace_builder({std::move(result).get_trace()}));
      }

      size_t consumed_number = result.get_consumed_number();
      return ResultType(consumed_number, std::move(result).get_value());
    }

    static std::string to_string_without_name() {
      return "Number(unsigned long)";  //  + wrapped(parser());
    }

   private:
    static auto parser() {
      return Adapted(
          Some(Digit()),
          [](const std::list<char>& digits) {
            return std::reduce(
                digits.cbegin(), digits.cend(), 0,
                [](uint64_t acc, char ch) {
                  return 10 * acc + static_cast<uint64_t>(ch - '0');
                });
          });
    }
  };

  template <bool Dummy>
  struct InnerNumberParser<double, Dummy> {
    static ResultType parse(const TraceBuilderType& trace_builder,
                            StreamType* stream) {
      auto result = parser()->parse(stream);
      if (!result.success()) {
        return ResultType(trace_builder({std::move(result).get_trace()}));
      }

      size_t consumed_number = result.get_consumed_number();
      return ResultType(consumed_number, std::move(result).get_value());
    }

    static std::string to_string_without_name() {
      return "Number(double)";  //  + wrapped(parser());
    }

   private:
    static auto parser() {
      return Adapted(
          Seq(WithDefault(Any(Char('+'), Char('-')), '+'),
              Some(Digit()),
              Opt(Seq(Skip(Char('.')), Some(Digit())))),
          [](const char sign, const std::list<char>& digits,
             const std::optional<std::list<char>>& mantissa) {
            double v = std::reduce(digits.cbegin(), digits.cend(), 0,
                                   [](double acc, char ch) {
                                     return 10 * acc + (ch - '0');
                                  });


            double m = 0;
            if (mantissa.has_value()) {
              m = std::reduce(
                  mantissa.value().crbegin(), mantissa.value().crend(), 0.0,
                  [](double acc, char ch) {
                    return 0.1 * (acc + static_cast<double>(ch - '0'));
                  });
            }
            return (sign == '-') ? -(v + m) : (v + m);
          });
    }
  };
};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_NUMBER_H_
