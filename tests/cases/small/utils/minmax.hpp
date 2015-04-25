/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/small/utils/minmax.hpp
	Tests for minimum / maximum library

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
#ifndef FFUZZYPP_TESTCASES_SMALL_UTILS_MINMAX_HPP
#define FFUZZYPP_TESTCASES_SMALL_UTILS_MINMAX_HPP

#include <algorithm>


class MinMaxTests : public ::testing::Test
{
public:
	template <class T>
	struct MinMax_DefaultComparator
	{
		bool operator ()(const T& a, const T& b)
		{
			return a < b;
		}
	};
	template <class T>
	struct MinMax_DefaultComparatorReversed
	{
		bool operator ()(const T& a, const T& b)
		{
			return a > b;
		}
	};
public:
	struct nt_equality_t
	{
		int a, b;
	};
};

static bool operator<(
	const MinMaxTests::nt_equality_t& a,
	const MinMaxTests::nt_equality_t& b
) noexcept
{
	return a.a < b.a;
}
static bool operator==(
	const MinMaxTests::nt_equality_t& a,
	const MinMaxTests::nt_equality_t& b
) noexcept
{
	return a.a == b.a && a.b == b.b;
}
static bool operator!=(
	const MinMaxTests::nt_equality_t& a,
	const MinMaxTests::nt_equality_t& b
) noexcept
{
	return !(a == b);
}


TEST_F(MinMaxTests, MinSpecWithoutComparator)
{
	for (int a = -1000; a < 1000; a++)
	{
		for (int b = -1000; b < 1000; b++)
		{
			ASSERT_EQ(std::min(a, b), ffuzzy::minmax::min(a, b))
				<< "ffuzzy::minmax::min test failed with " << a << " and " << b << ".";
		}
	}
}

TEST_F(MinMaxTests, MaxSpecWithoutComparator)
{
	for (int a = -1000; a < 1000; a++)
	{
		for (int b = -1000; b < 1000; b++)
		{
			ASSERT_EQ(std::max(a, b), ffuzzy::minmax::max(a, b))
				<< "ffuzzy::minmax::max test failed with " << a << " and " << b << ".";
		}
	}
}

TEST_F(MinMaxTests, MinSpecWithComparator)
{
	MinMax_DefaultComparator<int> cmp;
	for (int a = -1000; a < 1000; a++)
	{
		for (int b = -1000; b < 1000; b++)
		{
			ASSERT_EQ(std::min(a, b, cmp), ffuzzy::minmax::min(a, b, cmp))
				<< "ffuzzy::minmax::min test with comparator failed with " << a << " and " << b;
		}
	}
}

TEST_F(MinMaxTests, MaxSpecWithComparator)
{
	MinMax_DefaultComparator<int> cmp;
	for (int a = -1000; a < 1000; a++)
	{
		for (int b = -1000; b < 1000; b++)
		{
			ASSERT_EQ(std::max(a, b, cmp), ffuzzy::minmax::max(a, b, cmp))
				<< "ffuzzy::minmax::max test with comparator failed with " << a << " and " << b << ".";
		}
	}
}


TEST_F(MinMaxTests, MinSpecWithComparatorReversed)
{
	MinMax_DefaultComparatorReversed<int> cmp;
	for (int a = -1000; a < 1000; a++)
	{
		for (int b = -1000; b < 1000; b++)
		{
			ASSERT_EQ(std::max(a, b), ffuzzy::minmax::min(a, b, cmp))
				<< "ffuzzy::minmax::min test with reverse comparator failed with " << a << " and " << b << ".";
		}
	}
}

TEST_F(MinMaxTests, MaxSpecWithComparatorReversed)
{
	MinMax_DefaultComparatorReversed<int> cmp;
	for (int a = -1000; a < 1000; a++)
	{
		for (int b = -1000; b < 1000; b++)
		{
			ASSERT_EQ(std::min(a, b), ffuzzy::minmax::max(a, b, cmp))
				<< "ffuzzy::minmax::max test with reverse comparator failed with " << a << " and " << b << ".";
		}
	}
}

TEST_F(MinMaxTests, FirstArgumentOnEquivalent)
{
	typedef MinMaxTests::nt_equality_t eq_t;
	eq_t a = {5, 1};
	eq_t b = {5, 0};
	EXPECT_NE(a, b);
	EXPECT_FALSE(a < b);
	EXPECT_FALSE(b < a);
	EXPECT_EQ(a, ffuzzy::minmax::max(a, b));
	EXPECT_NE(b, ffuzzy::minmax::max(a, b));
	EXPECT_EQ(b, ffuzzy::minmax::max(b, a));
	EXPECT_NE(a, ffuzzy::minmax::max(b, a));
	EXPECT_EQ(a, ffuzzy::minmax::min(a, b));
	EXPECT_NE(b, ffuzzy::minmax::min(a, b));
	EXPECT_EQ(b, ffuzzy::minmax::min(b, a));
	EXPECT_NE(a, ffuzzy::minmax::min(b, a));
}

#endif
