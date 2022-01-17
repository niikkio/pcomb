#ifndef PCOMB_PRIVATES_PREDICATE_H_
#define PCOMB_PRIVATES_PREDICATE_H_

#include <functional>
#include <string>
#include <utility>

#include "pcomb/messages.h"
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

 protected:
  std::string to_string_without_name() const override {
    return "Predicate";
  }

 public:
  explicit PredicateParser(PredicateType&& predicate)
      : predicate_(std::forward<PredicateType>(predicate)) {
  }

  ResultType parse(StreamType* stream) const override {
    if (stream->empty()) {
      return ResultType(Trace(this, stream, messages::EMPTY_STREAM));
    }

    CharType ch(stream->head());
    if (predicate_(ch)) {
      stream->consume(1);
      return ResultType(1, std::move(ch));
    }

    return ResultType(Trace(this, stream, messages::UNEXPECTED_CHAR(ch)));
  }

 private:
  PredicateType predicate_;
};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_PREDICATE_H_
