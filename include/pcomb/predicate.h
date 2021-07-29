#ifndef PCOMB_PREDICATE_H
#define PCOMB_PREDICATE_H

#include <functional>

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"

namespace pcomb {
    template <typename CharType>
    class PredicateParser : public Parser<CharType, CharType> {
    public:
        using StreamType = typename Parser<CharType, CharType>::StreamType;
        using ResultType = typename Parser<CharType, CharType>::ResultType;

        using PredicateType = std::function<bool(const CharType&)>;

        explicit PredicateParser(const PredicateType& predicate)
                : predicate_(predicate) {

        }

        ResultType parse(StreamType* stream) const override {
            if (stream->empty()) {
                return ResultType(0);
            }

            CharType ch = stream->head();
            if (predicate_(ch)) {
                stream->consume(1);
                return ResultType(1, ch);
            } else {
                return ResultType(0);
            }
        }

    private:
        PredicateType predicate_;
    };
}
#endif
