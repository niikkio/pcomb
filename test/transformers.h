#ifndef PCOMB_TEST_TRANSFORMERS_
#define PCOMB_TEST_TRANSFORMERS_

#include <list>
#include <memory>
#include <string>
#include <tuple>
#include <utility>
#include <variant>

#include "dummy.h"

int char2int(char c);

char variant2char(std::variant<char, char>&& v);

std::string list2string(std::list<char>&& chars);

int chars2int(char d1, char d0);

std::shared_ptr<Tree> int2tree(int v);

std::shared_ptr<Tree> seq2tree(
    std::shared_ptr<Tree> lhs, int v, std::shared_ptr<Tree> rhs);

std::pair<char, char> tuple2pair(std::tuple<char, char>&& t);

#endif  // PCOMB_TEST_TRANSFORMERS_
