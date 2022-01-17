#ifndef PCOMB_PRIVATES_STRINGS_H_
#define PCOMB_PRIVATES_STRINGS_H_

#include <list>
#include <sstream>
#include <string>
#include <tuple>

namespace pcomb::privates {
  template <typename... PS>
  std::string to_string(const PS&... ps) {
    std::stringstream ss;
    const char* sep = "";
    ((static_cast<void>(ss << sep << ps->name()), sep = ", "), ...);
    return ss.str();
  }

  template <typename P>
  std::string to_string(const std::list<P>& ps) {
    std::stringstream ss;
    const char* sep = "";
    for (const auto& p : ps) {
      ss << sep << p->name();
      sep = ", ";
    }
    return ss.str();
  }

  template <typename... PS>
  std::string to_string(const std::tuple<PS...>& ps) {
    return std::apply([](const auto&... ts) { return to_string(ts...); }, ps);
  }

  inline const std::string EMPTY_MESSAGE {""};

  template <typename Parsers>
  inline auto WRAPPED_PARSER_NAME(const std::string& prefix,
                                  const Parsers& parsers,
                                  const std::string& suffix = "") {
    std::stringstream ss;
    ss << prefix << to_string(parsers) << suffix;
    return ss.str();
  }

  template <typename Parser>
  inline std::string ADAPTIVE_PARSER_NAME(const Parser& p) {
    return WRAPPED_PARSER_NAME("Adaptive [", p, "]");
  }

  template <typename Parsers>
  inline std::string ALTERNATIVE_PARSER_NAME(const Parsers& ps) {
    return WRAPPED_PARSER_NAME("Alternative [", ps, "]");
  }

  template <typename Parser>
  inline std::string CONSTRUCTIVE_PARSER_NAME(const Parser& p) {
    return WRAPPED_PARSER_NAME("Constructive [", p, "]");
  }

  template <typename Parsers>
  inline std::string DYNAMIC_SEQUENCE_PARSER_NAME(const Parsers& ps) {
    return WRAPPED_PARSER_NAME("Dynamic Sequence [", ps, "]");
  }

  inline const std::string END_PARSER_NAME {"End"};

  template <typename Parser>
  inline std::string EXACT_PARSER_NAME(const Parser& p) {
    return WRAPPED_PARSER_NAME("Exact ", p);
  }

  template <typename Parser>
  inline std::string LAZY_PARSER_NAME(const Parser* p) {
    return WRAPPED_PARSER_NAME("Lazy ", p);
  }

  template <typename Parser>
  inline std::string MANY_PARSER_NAME(const Parser& p, size_t min_count) {
    std::stringstream prefix;
    prefix << "Many [" << min_count << ".. x ";
    return WRAPPED_PARSER_NAME(prefix.str(), p, "]");
  }

  template <typename Parser>
  inline std::string MANY_PARSER_NAME(const Parser& p,
                                      size_t min_count, size_t max_count) {
    std::stringstream prefix;
    prefix << "Many [" << min_count << ".." << max_count << " x ";
    return WRAPPED_PARSER_NAME(prefix.str(), p, "]");
  }

  template <typename Parser>
  inline std::string OPTIONAL_PARSER_NAME(const Parser& p) {
    return WRAPPED_PARSER_NAME("Optional ", p);
  }

  inline const std::string PREDICATE_PARSER_NAME {"Predicate"};

  template <typename Parsers>
  inline std::string SEQUENCE_PARSER_NAME(const Parsers& ps) {
    return WRAPPED_PARSER_NAME("Sequence [", ps, "]");
  }

  template <typename Parser>
  inline std::string SKIPPED_PARSER_NAME(const Parser& p) {
    return WRAPPED_PARSER_NAME("Skipped ", p);
  }

  template <typename Parser>
  inline const std::string STRICT_ADAPTIVE_PARSER_NAME(const Parser& p) {
    return WRAPPED_PARSER_NAME("Strict Adaptive [", p, "]");
  }

  template <typename Parsers>
  inline std::string STRICT_ALTERNATIVE_PARSER_NAME(const Parsers& ps) {
    return WRAPPED_PARSER_NAME("Stict Alternative [", ps, "]");
  }

  template <typename Parsers>
  inline const std::string STRICT_SEQUENCE_PARSER_NAME(const Parsers& ps) {
    return WRAPPED_PARSER_NAME("Strict Sequence [", ps, "]");
  }

  template <typename Parser>
  inline const std::string UNTIL_PARSER_NAME(const Parser& p) {
    return WRAPPED_PARSER_NAME("Until ", p);
  }

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
