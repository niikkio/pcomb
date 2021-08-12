C++ [combinatory parsing](https://en.wikipedia.org/wiki/Parser_combinator) library

<ins>build & run tests</ins>
```Bash
git clone git@github.com:niikkio/pcomb.git pcomb
cd pcomb

cmake -S . -B build 
cmake --build build -- -j4

cd build && ctest
```

<ins>*CMakeLists.txt.in* for custom boost installation</ins>
```Bash
set(Boost_INCLUDE_DIR /usr/local/src/boost_1_76_0)
set(Boost_LIBRARY_DIR /usr/local/src/boost_1_76_0/libs)
```
