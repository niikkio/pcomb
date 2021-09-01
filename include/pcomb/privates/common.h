#ifndef PCOMB_PRIVATES_COMMON_H_
#define PCOMB_PRIVATES_COMMON_H_

#include <tuple>
#include <utility>

namespace pcomb::privates {

template <typename P1, typename... PS>
using CommonCharType = std::enable_if_t<
    std::conjunction_v<std::is_same<typename P1::CharType,
                                    typename PS::CharType>...>,
    typename P1::CharType>;

class SkippedValue {
 public:
  inline bool operator==(const SkippedValue&) const {
    return true;
  }
};

class NoValue {
 public:
  inline bool operator==(const NoValue&) const {
    return true;
  }
};

template <typename V, typename TV>
using ConcatedType = decltype(std::tuple_cat(
    std::declval<std::tuple<V>>(),
    std::declval<TV>()));

template <size_t I, typename TP>
using WrappedValueType = std::tuple<
    typename std::tuple_element_t<I, TP>::ValueType>;

template <size_t I, typename TV>
inline constexpr bool IsSkipped =
    std::is_same_v<SkippedValue, std::tuple_element_t<I, TV>>;

template <size_t I, typename TP>
inline constexpr bool IsSkippedParser =
    std::is_same_v<SkippedValue,
                   typename std::tuple_element_t<I, TP>::ValueType>;

template <size_t I, bool Skip, typename TV>
struct RecursiveWithoutSkipped {
  using Type = ConcatedType<
      std::tuple_element_t<I, TV>,
      typename RecursiveWithoutSkipped<I-1, IsSkipped<I-1, TV>, TV>::Type>;
};

template <size_t I, typename TV>
struct RecursiveWithoutSkipped<I, true, TV> {
  using Type =
      typename RecursiveWithoutSkipped<I-1, IsSkipped<I-1, TV>, TV>::Type;
};

template <typename TV>
struct RecursiveWithoutSkipped<0, false, TV> {
  using Type = typename std::tuple<std::tuple_element_t<0, TV>>;
};

template <typename TV>
struct RecursiveWithoutSkipped<0, true, TV> {
  using Type = std::tuple<>;
};

template <typename TV>
struct WithoutSkipped {
 private:
  static constexpr size_t Size = std::tuple_size_v<TV>;

 public:
  using Type = typename RecursiveWithoutSkipped<
      Size-1, IsSkipped<Size-1, TV>, TV>::Type;
};

template <typename TV>
using WithoutSkippedType = typename WithoutSkipped<TV>::Type;

template <size_t I, typename TV>
struct RecursiveExtracted {
  using Type = TV;
};

template <typename TV>
struct RecursiveExtracted<0, TV> {
  using Type = SkippedValue;
};

template <typename TV>
struct RecursiveExtracted<1, TV> {
  using Type = typename std::tuple_element_t<0, TV>;
};

template <typename TV>
struct Extracted {
 private:
  static constexpr size_t Size = std::tuple_size_v<TV>;

 public:
  using Type = typename RecursiveExtracted<Size, TV>::Type;
};

template <typename TV>
using ExtractedType = typename Extracted<TV>::Type;

template <size_t I, typename TV>
struct Extract {
  static ExtractedType<TV> from(TV&& values) {
    return values;
  }
};

template <typename TV>
struct Extract<0, TV> {
  static ExtractedType<TV> from(TV&&) {
    return SkippedValue();
  }
};

template <typename TV>
struct Extract<1, TV> {
  static ExtractedType<TV> from(TV&& values) {
    return std::get<0>(std::forward<TV>(values));
  }
};

template <typename V, size_t I, typename TV>
struct RecursiveNotIn {
  static constexpr bool value =
      !std::is_same_v<V, std::tuple_element_t<I, TV>> &&
      RecursiveNotIn<V, I-1, TV>::value;
};

template <typename V, typename TV>
struct RecursiveNotIn<V, 0, TV> {
  static constexpr bool value =
      !std::is_same_v<V, std::tuple_element_t<0, TV>>;
};

template <typename V, typename TV>
struct NotIn {
  static constexpr bool value = RecursiveNotIn<
      V, std::tuple_size_v<TV>-1, TV>::value;
};

template <typename V, typename TV, bool Take>
struct Append;

template <typename V, typename TV>
struct Append<V, TV, false> {
  using Type = TV;
};

template <typename V, typename TV>
struct Append<V, TV, true> {
  using Type = decltype(std::tuple_cat(std::declval<TV>(),
                                       std::declval<std::tuple<V>>()));
};

template <size_t I, typename TV>
struct RecursiveWithoutDuplicates {
 private:
  using SET = typename RecursiveWithoutDuplicates<I-1, TV>::Type;
  using V = typename std::tuple_element_t<I, TV>;

 public:
  using Type = typename Append<V, SET, NotIn<V, SET>::value>::Type;
};

template <typename TV>
struct RecursiveWithoutDuplicates<0, TV> {
  using Type = std::tuple<typename std::tuple_element_t<0, TV>>;
};

template <typename TV>
struct WithoutDuplicates {
  using Type = typename RecursiveWithoutDuplicates<
      std::tuple_size_v<TV>-1, TV>::Type;
};

template <typename TV>
using WithoutDuplicatesType = typename WithoutDuplicates<TV>::Type;

};  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_COMMON_H_
