#ifndef PCOMB_ALTERNATIVE_H
#define PCOMB_ALTERNATIVE_H

#include <tuple>
#include <type_traits>
#include <variant>

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"

namespace pcomb {
    template <typename P1, typename... PS>
    using AlternativeBaseType = Parser<
            std::enable_if_t<
                std::conjunction_v<
                    std::is_same<typename P1::CharType,
                                 typename PS::CharType>...>,
                    typename P1::CharType>,
            std::variant<typename P1::ValueType,
                         typename PS::ValueType...>>;

    template <typename P1, typename... PS>
    class AlternativeParser : public AlternativeBaseType<P1, PS...> {
    public:
        using CharType = typename AlternativeBaseType<P1, PS...>::CharType;
        using ValueType = typename AlternativeBaseType<P1, PS...>::ValueType;

    private:
        using ResultType = Result<ValueType>;
        using StreamType = IStream<CharType>;
        using StorageType = std::tuple<P1, PS...>;
        static constexpr size_t StorageSize = 1 + sizeof...(PS);

    public:
        explicit AlternativeParser(const P1& p1, const PS&... ps) : parsers_(std::make_tuple(p1, ps...)) {

        }

        explicit AlternativeParser(P1&& p1, PS&&... ps) : parsers_(std::forward_as_tuple(p1, ps...)) {

        }

        ResultType parse(StreamType* stream) const override {
            return Alternative<StorageSize>::parse(parsers_, stream);
        }

    private:
        template <size_t I, bool Dummy = true>
        struct Alternative {
            static ResultType parse(const StorageType& parsers, StreamType* stream) {
                auto prevResult = Alternative<I-1>::parse(parsers, stream);
                if (prevResult.success()) {
                    return prevResult;
                }

                auto result = std::get<I-1>(parsers).parse(stream);
                if (result.success()) {
                    int consumed = result.get_consumed_number();
                    return ResultType(consumed, ValueType(std::in_place_index<I-1>,
                                                          std::move(result).get_value()));
                }

                return ResultType();
            }
        };

        template <bool Dummy>
        struct Alternative<1, Dummy> {
            static ResultType parse(const StorageType& parsers, StreamType* stream) {
                auto result = std::get<0>(parsers).parse(stream);
                if (result.success()) {
                    int consumed = result.get_consumed_number();
                    return ResultType(consumed, ValueType(std::in_place_index<0>,
                                                          std::move(result).get_value()));
                }

                return ResultType();
            }
        };

        StorageType parsers_;
    };

}  // namespace pcomb
#endif  // PCOMB_ALTERNATIVE_H
