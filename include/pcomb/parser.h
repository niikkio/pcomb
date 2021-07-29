#ifndef PCOMB_PARSER_H
#define PCOMB_PARSER_H

#include "pcomb/result.h"
#include "pcomb/stream.h"

namespace pcomb {
    template <typename CharType, typename ValueType>
    class Parser {
    public:
        using StreamType = IStream<CharType>;
        using ResultType = Result<ValueType>;

        virtual ~Parser() = default;

        virtual ResultType parse(StreamType* stream) const = 0;
    };

}  // namespace pcomb
#endif  // PCOMB_PARSER_H
