#ifndef PCOMB_PARSER_H_
#define PCOMB_PARSER_H_

#include <memory>
#include <sstream>
#include <string>
#include <utility>

#include "pcomb/result.h"
#include "pcomb/stream.h"

namespace pcomb {

template <typename P>
using ParserPointer = std::shared_ptr<P>;

template <typename P, typename... Args>
inline ParserPointer<P> make(Args&&... args) {
  return std::make_shared<P>(std::forward<Args>(args)...);
}

template <typename C, typename V>
class Parser {
 public:
  using CharType = C;
  using ValueType = V;

  virtual ~Parser() = default;

  virtual Result<ValueType> parse(IStream<CharType>* stream) const = 0;

  std::string to_string(bool with_name = true) const {
    std::stringstream ss;
    if (with_name) ss << name_ << " <";
    ss << to_string_without_name();
    if (with_name) ss << ">";
    return ss.str();
  }

  friend ParserPointer<Parser>&& with_name(ParserPointer<Parser>&& p,
                                           const std::string& name) {
    p->name_ = name;
    return std::forward<ParserPointer<Parser>>(p);
  }

 protected:
  virtual std::string to_string_without_name() const = 0;

  std::string name_ = "Parser";
};

}  // namespace pcomb
#endif  // PCOMB_PARSER_H_
