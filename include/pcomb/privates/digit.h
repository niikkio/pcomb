#ifndef PCOMB_PRIVATES_DIGIT_H_
#define PCOMB_PRIVATES_DIGIT_H_

#include <string>

#include "pcomb/privates/predicate.h"

namespace pcomb::privates {

template <typename T>
class DigitParser : public PredicateParser<T> {
 public:
  using CharType = typename PredicateParser<T>::CharType;
  using ValueType = typename PredicateParser<T>::ValueType;

  DigitParser() : PredicateParser<T>([](T c) { return '0' <= c && c <= '9'; }) {
  }

 protected:
  std::string to_string_without_name() const override {
    return "Digit";
  }
};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_DIGIT_H_
