#ifndef PCOMB_PRIVATES_PREDICATE_H_
#define PCOMB_PRIVATES_PREDICATE_H_

#include <functional>
#include <string>
#include <utility>

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"
#include "pcomb/trace.h"

namespace pcomb::privates {

template <typename T>
class PredicateParser : public Parser<T, T> {
 public:
  using CharType = typename Parser<T, T>::CharType;
  using ValueType = typename Parser<T, T>::ValueType;

 private:
  using ResultType = Result<ValueType>;
  using StreamType = IStream<CharType>;
  using PredicateType = std::function<bool(const CharType&)>;

 public:
  explicit PredicateParser(PredicateType&& predicate)
      : predicate_(std::forward<PredicateType>(predicate)) {
    this->name_ = "Predicate Parser";
  }

  ResultType parse(StreamType* stream) const override {
    if (stream->empty()) {
      auto message = "character not found";
      auto trace = Trace(this->name(), stream, std::move(message));
      return ResultType(std::move(trace));
    }

    CharType ch(stream->head());
    if (predicate_(ch)) {
      stream->consume(1);
      return ResultType(1, std::move(ch));
    }

    auto message = "unexpected character: \'" + std::string(1, ch) + "\'";
    auto trace = Trace(this->name(), stream, std::move(message));
    return ResultType(std::move(trace));
  }

 private:
  PredicateType predicate_;
};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_PREDICATE_H_
