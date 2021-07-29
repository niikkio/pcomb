#ifndef PCOMB_MANY_H
#define PCOMB_MANY_H

#include <list>
#include <memory>

#include "pcomb/parser.h"
#include "pcomb/result.h"
#include "pcomb/stream.h"

namespace pcomb {
    template <typename BaseParser>
    class ManyParser : public Parser<
                            typename BaseParser::CharType,
                            std::list<typename BaseParser::ValueType> > {
    public:
        using CharType = typename BaseParser::CharType;
        using ValueType = std::list<typename BaseParser::ValueType>;

        explicit ManyParser(const BaseParser& parser, int min_count = 0, int max_count = -1)
                : parser_(parser)
                , min_count_(min_count)
                , max_count_(max_count) {
            // TODO: copy-constructors ???
        }

        explicit ManyParser(BaseParser&& parser, int min_count = 0, int max_count = -1)
                : parser_(std::forward<BaseParser>(parser))
                , min_count_(min_count)
                , max_count_(max_count) {

        }

        Result<ValueType> parse(IStream<CharType>* stream) const override {
            int consumed_number = 0;
            auto values = ValueType();

            auto stream_copy = std::unique_ptr<IStream<CharType> >(stream->clone());

            while (max_count_ < 0 || values.size() < max_count_) {
                auto result = parser_.parse(stream_copy.get());
                if (!result.success()) {
                    break;
                }

                consumed_number += result.get_consumed_number();
                values.emplace_back(std::move(result).get_value());
                // TODO: call stream->consume if values.size() >= min_count_ ?
            }

            if (values.size() >= min_count_) {
                stream->consume(consumed_number);
                return Result<ValueType>(consumed_number, std::move(values));
            }

            return Result<ValueType>();
        }

    private:
        BaseParser parser_;
        int min_count_;
        int max_count_;
    };

}  // namespace pcomb
#endif  // PCOMB_MANY_H
