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

        using PredicateType = std::function<bool(const CharType&)>;

        explicit PredicateParser(PredicateType&& predicate)
                : predicate_(std::forward<PredicateType>(predicate)) {

        }

        Result<ValueType> parse(IStream<CharType>* stream) const override {
            if (stream->empty()) {
                return Result<ValueType>();
            }

            CharType ch(stream->head());
            if (predicate_(ch)) {
                stream->consume(1);
                return Result<ValueType>(1, ch);
            } else {
                return Result<ValueType>();
            }
        }

    private:
        PredicateType predicate_;
    };

}  // namespace pcomb
#endif  // PCOMB_PREDICATE_H
