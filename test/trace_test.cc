#include <gtest/gtest.h>

#include "pcomb/stream_position.h"
#include "pcomb/trace.h"

using pcomb::StreamPosition;
using pcomb::Trace;

TEST(TraceTest, Alternative) {
  auto trace = Trace(
    "Alternative",
    StreamPosition{12, 2, 3},
    "",
    {
      Trace(
        "Char",
        StreamPosition{12, 2, 3},
        "A != B"),
      Trace(
        "Char",
        StreamPosition{12, 2, 3},
        "A != C")
    });

  auto expected = "Alternative failed at [12,2,3]\n"
                  "\tChar failed at [12,2,3] [A != B]\n"
                  "\tChar failed at [12,2,3] [A != C]\n";

  EXPECT_EQ(trace.to_string(), expected);
}