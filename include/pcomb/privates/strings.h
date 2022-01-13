#ifndef PCOMB_PRIVATES_STRINGS_H_
#define PCOMB_PRIVATES_STRINGS_H_

#include <sstream>
#include <string>

namespace pcomb::privates {
  inline const std::string EMPTY_MESSAGE {""};

  inline const std::string ADAPTIVE_PARSER_NAME {"Adaptive Parser"};
  inline const std::string ALTERNATIVE_PARSER_NAME {"Alternative Parser"};
  inline const std::string CONSTRUCTIVE_PARSER_NAME {"Constructive Parser"};
  inline const std::string DYNAMIC_SEQUENCE_PARSER_NAME {
      "Dynamic Sequence Parser"};
  inline const std::string END_PARSER_NAME {"End Parser"};
  inline const std::string EXACT_PARSER_NAME {"Exact Parser"};
  inline const std::string MANY_PARSER_NAME {"Many Parser"};
  inline const std::string OPTIONAL_PARSER_NAME {"Optional Parser"};
  inline const std::string PREDICATE_PARSER_NAME {"Predicate Parser"};
  inline const std::string SEQUENCE_PARSER_NAME {"Sequence Parser"};
  inline const std::string SKIPPED_PARSER_NAME {"Skipped Parser"};
  inline const std::string STRICT_ADAPTIVE_PARSER_NAME {
      "Strict Adaptive Parser"};
  inline const std::string STRICT_ALTERNATIVE_PARSER_NAME {
      "Strict Alternative Parser"};
  inline const std::string STRICT_SEQUENCE_PARSER_NAME {
      "Strict Sequence Parser"};
  inline const std::string UNTIL_PARSER_NAME {"Until Parser"};

  inline const std::string END_PARSER_ERROR_MESSAGE {"EOF expected"};

  inline std::string EOF_WAS_EXPECTED_ERROR_MESSAGE() {
    return "unexpected characters at the end";
  }

  template <typename StreamPointer>
  inline std::string EOF_WAS_EXPECTED_ERROR_MESSAGE(
      const StreamPointer& stream_pointer) {
    std::stringstream ss;
    ss << "unexpected characters at the end: \'"
       << stream_pointer->head() << "...\' at "
       << stream_pointer->position().to_string();
    return ss.str();
  }

  inline std::string NOT_ENOUGH_RESULTS_ERROR_MESSAGE(size_t n,
                                                      size_t expected) {
    std::stringstream ss;
    ss << "not enough results: " << expected << " > " << n;
    return ss.str();
  }

  template <typename CharType>
  inline std::string UNEXPECTED_CHAR_ERROR_MESSAGE(CharType ch) {
    std::stringstream ss;
    ss << "unexpected character: \'" << ch << "\'";
    return ss.str();
  }

  inline const std::string EMPTY_STREAM_ERROR_MESSAGE {"characters not found"};

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_STRINGS_H_
