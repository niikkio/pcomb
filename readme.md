C++ [combinatory parsing](https://en.wikipedia.org/wiki/Parser_combinator) library

<ins>build & run tests</ins>
```Bash
git clone git@github.com:niikkio/pcomb.git pcomb
cd pcomb

cmake -S . -B build -D pcomb_build_tests=ON
cmake --build build -- -j4

cd build && ctest
```

<ins>*CMakeLists.txt.in* for custom boost installation</ins>
```Bash
set(Boost_INCLUDE_DIR /usr/local/src/boost_1_76_0)
set(Boost_LIBRARY_DIR /usr/local/src/boost_1_76_0/libs)
```

<ins>using</ins>
```Bash
# CMakeLists.txt:
FetchContent_Declare(
  pcomb
  GIT_REPOSITORY https://github.com/niikkio/pcomb.git
)
FetchContent_MakeAvailable(pcomb)
```

<ins>building tests</ins>
```Bash
# running cmake:
cmake -D pcomb_build_tests=ON  # ...
# ... or CMakeLists.txt:
set(pcomb_build_tests ON CACHE INTERNAL "")
```
