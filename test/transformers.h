#ifndef PCOMB_TEST_TRANSFORMERS_
#define PCOMB_TEST_TRANSFORMERS_

#include <list>
#include <string>
#include <variant>

int char2int(char c);

char variant2char(std::variant<char, char>&& v);

std::string list2string(std::list<char>&& chars);

int chars2int(char d1, char d0);

#endif  // PCOMB_TEST_TRANSFORMERS_
