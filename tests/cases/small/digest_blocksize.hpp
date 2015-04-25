/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/small/digest_blocksize.hpp
	Tests for digest_blocksize

	Copyright (C) 2014 Tsukasa OI <floss_ssdeep@irq.a4lg.com>


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
#ifndef FFUZZYPP_TESTCASES_SMALL_DIGEST_BLOCKSIZE_HPP
#define FFUZZYPP_TESTCASES_SMALL_DIGEST_BLOCKSIZE_HPP

#include <cstdint>
#include <cstddef>
#include <string>


TEST(DigestBlocksizeTests, MaxNaturalDigitsSpec)
{
	size_t max_natural_digits = digest_blocksize::max_natural_digits;
	EXPECT_LE(
		to_string(static_cast<unsigned long>(digest_blocksize::max_blocksize)).size(),
		max_natural_digits
	);
}

TEST(DigestBlocksizeTests, IsSafeToDoubleSpec)
{
	for (digest_blocksize_t i = 0; i < 0x80000000ul; i++)
	{
		ASSERT_TRUE(digest_blocksize::is_safe_to_double(i))
			<< "is_safe_to_double test failed on " << i << ".";
	}
}

TEST(DigestBlocksizeTests, IsNaturalSpec)
{
	for (digest_blocksize_t i = 0; i < digest_blocksize::min_blocksize; i++)
	{
		EXPECT_FALSE(digest_blocksize::is_natural(i))
			<< "is_natural test (to false) failed on " << i << ".";
	}
	digest_blocksize_t s = digest_blocksize::min_blocksize;
	while (true)
	{
		ASSERT_TRUE(digest_blocksize::is_natural(s))
			<< "is_natural test (to true) failed on " << s << ".";
		if (s == digest_blocksize::max_blocksize)
			break;
		digest_blocksize_t s2 = s * 2;
		for (digest_blocksize_t i = s + 1; i < s2; i++)
		{
			ASSERT_FALSE(digest_blocksize::is_natural(i))
				<< "is_natural test (to false) failed on " << i << ".";
		}
		s = s2;
	}
	// Additional test for limited range (in 32-bit)
	if (digest_blocksize::max_blocksize != 0xfffffffful)
	{
		for (uint_least64_t i = digest_blocksize::max_blocksize + 1; i <= 0xfffffffful; i++)
		{
			ASSERT_FALSE(digest_blocksize::is_natural(i))
				<< "is_natural test (to false) failed on " << i << ".";
		}
	}
}

TEST(DigestBlocksizeTests, IsNaturalAndIndexSpec)
{
	for (unsigned i = 0; i < digest_blocksize::number_of_blockhashes; i++)
	{
		EXPECT_TRUE(digest_blocksize::is_natural(digest_blocksize::at(i)))
			<< "natural index test failed at index " << i << ".";
	}
}

TEST(DigestBlocksizeTests, NaturalIndexSpec)
{
	for (unsigned i = 0; i < digest_blocksize::number_of_blockhashes; i++)
	{
		EXPECT_EQ(i, digest_blocksize::natural_to_index(digest_blocksize::at(i)))
			<< "natural log test failed at index " << i << ".";
		EXPECT_EQ(i, digest_blocksize::natural_to_index<true>(digest_blocksize::at(i)))
			<< "natural log test (debrujin) failed at index " << i << ".";
	}
}

TEST(DigestBlocksizeTests, IsNearEqTrueSpec)
{
	// For limited range (in 32-bit)
	for (uint_least64_t i = 0; i <= 0xfffffffful; i++)
	{
		ASSERT_TRUE(digest_blocksize::is_near_eq(i, i))
			<< "is_near_eq(" << i << ", " << i << ") should be true.";
	}
}

TEST(DigestBlocksizeTests, IsNearLtGtFalseSpecAtZero)
{
	EXPECT_FALSE(digest_blocksize::is_near_gt(0, 0));
	EXPECT_FALSE(digest_blocksize::is_near_lt(0, 0));
}

TEST(DigestBlocksizeTests, IsNearLtGtAndIndexSpec)
{
	for (unsigned i = 0; i < digest_blocksize::number_of_blockhashes; i++)
	{
		for (unsigned j = 0; j < digest_blocksize::number_of_blockhashes; j++)
		{
			EXPECT_EQ(i == j,
				digest_blocksize::is_near_eq(digest_blocksize::at(i), digest_blocksize::at(j)))
				<< "is_near_eq index test failed at indices " << i << " " << j << ".";
			EXPECT_EQ(i + 1 == j,
				digest_blocksize::is_near_lt(digest_blocksize::at(i), digest_blocksize::at(j)))
				<< "is_near_lt index test failed at indices " << i << " " << j << ".";
			EXPECT_EQ(i == j + 1,
				digest_blocksize::is_near_gt(digest_blocksize::at(i), digest_blocksize::at(j)))
				<< "is_near_gt index test failed at indices " << i << " " << j << ".";
			EXPECT_EQ(i == j || i + 1 == j || i == j + 1,
				digest_blocksize::is_near(digest_blocksize::at(i), digest_blocksize::at(j)))
				<< "is_near index test failed at indices " << i << " " << j << ".";
			// composite tests
			if (digest_blocksize::is_near_lt(digest_blocksize::at(i), digest_blocksize::at(j)))
			{
				ASSERT_TRUE(digest_blocksize::is_safe_to_double(digest_blocksize::at(i)));
				ASSERT_EQ(digest_blocksize::at(i) * 2, digest_blocksize::at(j));
				ASSERT_EQ(digest_blocksize::at(i + 1), digest_blocksize::at(j));
			}
			if (digest_blocksize::is_near_gt(digest_blocksize::at(i), digest_blocksize::at(j)))
			{
				ASSERT_TRUE(digest_blocksize::is_safe_to_double(digest_blocksize::at(j)));
				ASSERT_EQ(digest_blocksize::at(j) * 2, digest_blocksize::at(i));
				ASSERT_EQ(digest_blocksize::at(j + 1), digest_blocksize::at(i));
			}
		}
	}
}

TEST(DigestBlocksizeTests, IsNearLtGtTrueSpec1)
{
	// For limited range (in 32-bit)
	for (uint_least32_t i = 0; i < 0xfffffffful; i++)
	{
		digest_blocksize_t s(i + 1);
		if (!digest_blocksize::is_safe_to_double(s))
			break;
		ASSERT_TRUE(digest_blocksize::is_near_lt(s, s * 2))
			<< "is_near_lt(" << s << ", " << (i * 2) << ") should be true.";
		ASSERT_TRUE(digest_blocksize::is_near_gt(s * 2, s))
			<< "is_near_gt(" << (s * 2) << ", " << s << ") should be true.";
	}
}

TEST(DigestBlocksizeTests, IsNearLtGtTrueSpec2)
{
	// For limited range (in 32-bit)
	for (uint_least32_t i = 1; i < 0x80000000ul; i++)
	{
		digest_blocksize_t s(i * 2);
		ASSERT_TRUE(digest_blocksize::is_near_gt(s, s / 2))
			<< "is_near_gt(" << s << ", " << (s / 2) << ") should be true.";
		ASSERT_TRUE(digest_blocksize::is_near_lt(s / 2, s))
			<< "is_near_lt(" << (s / 2) << ", " << s << ") should be true.";
	}
}

#endif
