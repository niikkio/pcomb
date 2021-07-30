#ifndef PCOMB_ALTERNATIVE_H
#define PCOMB_ALTERNATIVE_H

#include <tuple>
#include <type_traits>
#include <variant>

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"

namespace pcomb {
    template <typename P, typename... Parsers>
    struct AlternativeTypes {
        using CharType = std::enable_if_t<
                                         std::conjunction_v<
                                                           std::is_same<
                                                                       typename P::CharType,
                                                                       typename Parsers::CharType
                                                                       >...
                                                           >,
                                         typename P::CharType
                                         >;

        using ValueType = typename std::variant<
            typename P::ValueType, typename Parsers::ValueType...>;
    };

    template <typename ...Parsers>
    class AlternativeParser : public Parser<
            typename AlternativeTypes<Parsers...>::CharType,
            typename AlternativeTypes<Parsers...>::ValueType > {
    public:
        using CharType = typename AlternativeTypes<Parsers...>::CharType;
        using ValueType = typename AlternativeTypes<Parsers...>::ValueType;

        AlternativeParser(Parsers&... parsers) : parsers_(std::make_tuple(parsers...)) {

        }

        AlternativeParser(Parsers&&... parsers) : parsers_(std::forward_as_tuple(parsers...)) {

        }

        Result<ValueType> parse(IStream<CharType>* stream) const override {
            return Alternative<std::tuple<Parsers...>, sizeof...(Parsers)>::parse(parsers_, stream);
        }

    private:
        template <typename Tup, size_t I>
        struct Alternative {
            static Result<ValueType> parse(const Tup& parsers, IStream<CharType>* stream) {
                auto prevResult = Alternative<Tup, I-1>::parse(parsers, stream);
                if (prevResult.success()) {
                    return prevResult;
                }

                auto result = std::get<I-1>(parsers).parse(stream);
                if (result.success()) {
                    int consumed = result.get_consumed_number();
                    return Result<ValueType>(consumed,
                            ValueType(std::in_place_index<I-1>, std::move(result).get_value()));
                }

                return Result<ValueType>();
            }
        };

        template <typename Tup>
        struct Alternative<Tup, 1> {
            static Result<ValueType> parse(const Tup& parsers, IStream<CharType>* stream) {
                auto result = std::get<0>(parsers).parse(stream);
                if (result.success()) {
                    int consumed = result.get_consumed_number();
                    return Result<ValueType>(consumed,
                            ValueType(std::in_place_index<0>, std::move(result).get_value()));
                }

                return Result<ValueType>();
            }
        };

        std::tuple<Parsers...> parsers_;
    };

}  // namespace pcomb
#endif  // PCOMB_ALTERNATIVE_H
