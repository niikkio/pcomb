#ifndef PCOMB_PRIVATES_CHAR_H_
#define PCOMB_PRIVATES_CHAR_H_

#include <sstream>
#include <string>

#include "pcomb/privates/predicate.h"

namespace pcomb::privates {

template <typename T>
class CharParser : public PredicateParser<T> {
 public:
  using CharType = typename PredicateParser<T>::CharType;
  using ValueType = typename PredicateParser<T>::ValueType;

  explicit CharParser(const T& ch)
      : PredicateParser<T>([ch](T c) { return c == ch; })
      , ch_(ch) {
  }

 protected:
  std::string to_string_without_name() const override {
    std::stringstream ss;
    ss << "Char('" << ch_ << "')";
    return ss.str();
  }

 private:
  T ch_;
};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_CHAR_H_
