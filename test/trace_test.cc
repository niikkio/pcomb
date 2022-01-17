#include <gtest/gtest.h>

#include "pcomb/alternative.h"
#include "pcomb/predicate.h"
#include "pcomb/stream_position.h"
#include "pcomb/string_stream.h"
#include "pcomb/trace.h"

using pcomb::Any;
using pcomb::Char;
using pcomb::StreamPosition;
using pcomb::StringStream;
using pcomb::Trace;

TEST(TraceTest, Alternative) {
  StringStream s("hello\nworld!\n");
  s.consume(8);

  auto pa = Char('A');
  auto pb = Char('B');
  auto pab = Any(Char('A'), Char('B'));

  auto trace = Trace(
    pab, &s, "",
    {
      Trace(pa, &s, "A != B"),
      Trace(pb, &s, "A != C")
    });

  auto expected = pab->name() + " failed at [8,1,2]\n"
                  "\t" + pa->name() + " failed at [8,1,2] [A != B]\n"
                  "\t" + pb->name() + " failed at [8,1,2] [A != C]\n";

  EXPECT_EQ(trace.to_string(), expected);
}
