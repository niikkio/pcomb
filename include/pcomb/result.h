#ifndef PCOMB_RESULT_H_
#define PCOMB_RESULT_H_

#include <optional>
#include <utility>

namespace pcomb {

template <typename ValueType>
class Result {
 public:
  Result() : storage_(std::nullopt), consumed_(0) { }

  template <typename T>
  explicit Result(int consumed, T&& value)
      : storage_(std::forward<T>(value)), consumed_(consumed) {
  }

  bool success() const {
    return storage_.has_value();
  }

  const ValueType& get_value() const& {
    return storage_.value();
  }

  ValueType&& get_value() && {
    return std::move(storage_.value());
  }

  int get_consumed_number() const {
    return consumed_;
  }

 private:
  std::optional<ValueType> storage_;
  int consumed_;
};

}  // namespace pcomb
#endif  // PCOMB_RESULT_H_
