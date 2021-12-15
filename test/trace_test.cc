#include <gtest/gtest.h>

#include "pcomb/trace.h"

using pcomb::Trace;

TEST(TraceTest, Alternative) {
  auto trace = Trace(
    "Alternative",
    "[12, 2, 3]",
    "",
    {
      Trace(
        "Char",
        "[12, 2, 3]",
        "A != B"),
      Trace(
        "Char",
        "[12, 2, 3]",
        "A != C")
    });

  auto expected = "Alternative Parser failed at [12, 2, 3]\n"
                  "\tChar Parser failed at [12, 2, 3] [A != B]\n"
                  "\tChar Parser failed at [12, 2, 3] [A != C]\n";

  EXPECT_EQ(trace.to_string(), expected);
}
