#ifndef PCOMB_PREDICATE_H
#define PCOMB_PREDICATE_H

#include <functional>

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"

namespace pcomb {
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

        }

        ResultType parse(StreamType* stream) const override {
            if (stream->empty()) {
                return ResultType();
            }

            CharType ch(stream->head());
            if (predicate_(ch)) {
                stream->consume(1);
                return ResultType(1, ch);
            } else {
                return ResultType();
            }
        }

    private:
        PredicateType predicate_;
    };

}  // namespace pcomb
#endif  // PCOMB_PREDICATE_H
