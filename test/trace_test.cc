#include <gtest/gtest.h>

#include "pcomb/stream_position.h"
#include "pcomb/string_stream.h"
#include "pcomb/trace.h"

using pcomb::StreamPosition;
using pcomb::StringStream;
using pcomb::Trace;

TEST(TraceTest, Alternative) {
  StringStream s("hello\nworld!\n");
  s.consume(8);

  auto trace = Trace(
    "Alternative", &s, "",
    {
      Trace("Char", &s, "A != B"),
      Trace("Char", &s, "A != C")
    });

  auto expected = "Alternative failed at [8,1,2]\n"
                  "\tChar failed at [8,1,2] [A != B]\n"
                  "\tChar failed at [8,1,2] [A != C]\n";

  EXPECT_EQ(trace.to_string(), expected);
}
