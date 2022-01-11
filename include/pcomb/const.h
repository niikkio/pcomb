#ifndef PCOMB_CONST_H_
#define PCOMB_CONST_H_

#include <string>

namespace pcomb {
  extern const std::string ADAPTIVE_PARSER_NAME;
  extern const std::string ALTERNATIVE_PARSER_NAME;
  extern const std::string CONSTRUCTIVE_PARSER_NAME;
  extern const std::string DYNAMIC_SEQUENCE_PARSER_NAME;
  extern const std::string END_PARSER_NAME;
  extern const std::string EXACT_PARSER_NAME;
  extern const std::string MANY_PARSER_NAME;
  extern const std::string OPTIONAL_PARSER_NAME;

  extern const std::string END_PARSER_ERROR_MESSAGE;

  template <typename StreamPointer>
  std::string EXACT_PARSER_ERROR_MESSAGE(const StreamPointer& stream_pointer) {
    return "unexpected characters at the end: \'" +
           std::string(1, stream_pointer->head()) + "...\' at " +
           stream_pointer->position().to_string();
  }
}  // namespace pcomb
#endif  // PCOMB_CONST_H_
