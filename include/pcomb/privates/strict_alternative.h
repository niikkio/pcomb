#ifndef PCOMB_PRIVATES_STRICT_ALTERNATIVE_H_
#define PCOMB_PRIVATES_STRICT_ALTERNATIVE_H_

#include <list>
#include <tuple>
#include <utility>
#include <variant>

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"
#include "pcomb/trace.h"

#include "pcomb/privates/magic.h"
#include "pcomb/privates/strings.h"

namespace pcomb::privates {

template <typename P1, typename... PS>
using StrictAlternativeBaseType = Parser<
    CommonCharType<P1, PS...>,
    std::variant<typename P1::ValueType,
                 typename PS::ValueType...>>;

template <typename P1, typename... PS>
class StrictAlternativeParser : public StrictAlternativeBaseType<P1, PS...> {
 public:
  using CharType = typename StrictAlternativeBaseType<P1, PS...>::CharType;
  using ValueType = typename StrictAlternativeBaseType<P1, PS...>::ValueType;

 private:
  using ResultType = Result<ValueType>;
  using StreamType = IStream<CharType>;
  using StorageType = std::tuple<ParserPointer<P1>, ParserPointer<PS>...>;
  using LogType = std::list<Trace>;
  static constexpr size_t StorageSize = 1 + sizeof...(PS);

 public:
  explicit StrictAlternativeParser(
      ParserPointer<P1>&& p1, ParserPointer<PS>&&... ps)
          : parsers_(std::forward_as_tuple(p1, ps...)) {
    this->name_ = STRICT_ALTERNATIVE_PARSER_NAME;
  }

  ResultType parse(StreamType* stream) const override {
    LogType log;
    return RecursiveAlternativeParser<0>::parse(this, stream, &log);
  }

 private:
  template <size_t I, bool Dummy = true>
  struct RecursiveAlternativeParser {
    static ResultType parse(const StrictAlternativeParser* owner,
                            StreamType* stream, LogType* log) {
      auto result = std::get<I>(owner->parsers_)->parse(stream);
      if (result.success()) {
        size_t consumed = result.get_consumed_number();
        return ResultType(consumed, ValueType(std::in_place_index<I>,
                                              std::move(result).get_value()));
      }
      log->push_back(std::move(result).get_trace());
      return RecursiveAlternativeParser<I+1>::parse(owner, stream, log);
    }
  };

  template <bool Dummy>
  struct RecursiveAlternativeParser<StorageSize-1, Dummy> {
    static ResultType parse(const StrictAlternativeParser* owner,
                            StreamType* stream, LogType* log) {
      auto result = std::get<StorageSize-1>(owner->parsers_)->parse(stream);
      if (result.success()) {
        size_t consumed = result.get_consumed_number();
        return ResultType(consumed,
                          ValueType(std::in_place_index<StorageSize-1>,
                                    std::move(result).get_value()));
      }
      log->push_back(std::move(result).get_trace());
      return ResultType(Trace(owner, stream, EMPTY_MESSAGE, std::move(*log)));
    }
  };

  StorageType parsers_;
};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_STRICT_ALTERNATIVE_H_
