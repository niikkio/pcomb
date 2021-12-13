#include "transformers.h"

int char2int(char c) {
  return c - '0';
}

char variant2char(std::variant<char, char>&& v) {
  auto value = std::get_if<0>(&v);
  if (value != nullptr) return *value;
  return *std::get_if<1>(&v);
}

std::string list2string(std::list<char>&& chars) {
  return std::string(chars.cbegin(), chars.cend());
}

int chars2int(char d1, char d0) {
  return 10 * char2int(d1) + char2int(d0);
}

std::shared_ptr<Tree> int2tree(int v) {
  return std::make_shared<Tree>(v);
}

std::shared_ptr<Tree> seq2tree(
    std::shared_ptr<Tree> lhs, int v, std::shared_ptr<Tree> rhs) {
  return std::make_shared<Tree>(lhs, v, rhs);
}

std::pair<char, char> tuple2pair(std::tuple<char, char>&& t) {
  return std::make_pair(std::get<0>(t), std::get<1>(t));
}
