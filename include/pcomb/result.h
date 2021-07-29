#ifndef PCOMB_RESULT_H
#define PCOMB_RESULT_H

#include <optional>

namespace pcomb {
    template <typename ValueType>
    class Result {
    public:
        explicit Result(int consumed)
                : storage_(std::nullopt)
                , consumed_(consumed) {
        }

        template <typename T>
        explicit Result(int consumed, T&& value)
                : storage_(std::forward<T>(value))
                , consumed_(consumed) {

        }

        bool success() const {
            return storage_.has_value();
        }

        const ValueType& get_value() const {
            return storage_.value();
        }

        int get_consumed() const {
            return consumed_;
        }

    private:
        std::optional<ValueType> storage_;
        int consumed_;
    };

}  // namespace pcomb
#endif  // PCOMB_RESULT_H
