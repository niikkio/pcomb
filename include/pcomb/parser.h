#ifndef PCOMB_PARSER_H
#define PCOMB_PARSER_H

#include <type_traits>

#include "pcomb/result.h"
#include "pcomb/stream.h"

namespace pcomb {
    template <typename P1, typename...PS>
    using CommonCharType = std::enable_if_t<
                                std::conjunction_v<
                                    std::is_same<typename P1::CharType,
                                                 typename PS::CharType>...>,
                                typename P1::CharType>;

    template <typename C, typename V>
    class Parser {
    public:
        using CharType = C;
        using ValueType = V;

        virtual ~Parser() = default;

        virtual Result<ValueType> parse(IStream<CharType>* stream) const = 0;
    };

}  // namespace pcomb
#endif  // PCOMB_PARSER_H
