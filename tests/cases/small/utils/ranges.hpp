/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/small/utils/ranges.hpp
	Tests for value range library

	Copyright (C) 2015 Tsukasa OI <floss_ssdeep@irq.a4lg.com>


	Permission to use, copy, modify, and/or distribute this software for
	any purpose with or without fee is hereby granted, provided that the
	above copyright notice and this permission notice appear in all copies.

	THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
	WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
	MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
	ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
	WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
	ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
	OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

*/
#ifndef FFUZZYPP_TESTCASES_SMALL_UTILS_RANGES_HPP
#define FFUZZYPP_TESTCASES_SMALL_UTILS_RANGES_HPP

#include <algorithm>


TEST(InclusiveRangeTests, SameValue)
{
	ranges::inclusive<int> a(5, 5);
	int count_of_elems = 0;
	for (int i : a)
	{
		count_of_elems++;
		EXPECT_EQ(1, count_of_elems);
		EXPECT_EQ(5, i);
	}
}

TEST(InclusiveRangeTests, BasicRange)
{
	ranges::inclusive<int> a(0, 100);
	int count_of_elems = 0;
	int prev = -1;
	for (int i : a)
	{
		count_of_elems++;
		ASSERT_GE(i, 0);
		ASSERT_LE(i, 100);
		ASSERT_EQ(1, i - prev);
		prev = i;
	}
	EXPECT_EQ(101, count_of_elems);
}

/*
	Following tests are optional.
	If you fail to compile here, disable these tests.
*/
#if 1
TEST(InclusiveRangeTests, Int8Range)
{
	ranges::inclusive<int8_t> a;
	int count_of_elems = 0;
	int prev = -129;
	for (int8_t i : a)
	{
		count_of_elems++;
		ASSERT_GE(int(i), std::numeric_limits<int8_t>::min());
		ASSERT_LE(int(i), std::numeric_limits<int8_t>::max());
		ASSERT_EQ(1, int(i) - prev);
		prev = int(i);
	}
	EXPECT_EQ(256, count_of_elems);
	EXPECT_EQ(127, prev);
}

TEST(InclusiveRangeTests, UInt8Range)
{
	ranges::inclusive<uint8_t> a;
	int count_of_elems = 0;
	int prev = -1;
	for (uint8_t i : a)
	{
		count_of_elems++;
		ASSERT_GE(int(i), std::numeric_limits<uint8_t>::min());
		ASSERT_LE(int(i), std::numeric_limits<uint8_t>::max());
		ASSERT_EQ(1, int(i) - prev);
		prev = int(i);
	}
	EXPECT_EQ(256, count_of_elems);
	EXPECT_EQ(255, prev);
}
#endif

#endif
