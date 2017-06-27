/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/compatibility/small/blockhash_comparison_min_matching.hpp
	Tests for minimum matching scores (small)

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
#ifndef FFUZZYPP_TESTCASES_COMPATIBILITY_SMALL_BLOCKHASH_COMPARISON_MIN_MATCHING_HPP
#define FFUZZYPP_TESTCASES_COMPATIBILITY_SMALL_BLOCKHASH_COMPARISON_MIN_MATCHING_HPP

#include <cassert>
#include <algorithm>
#include <string>

#include "../common/blockhash_comparison_min_matching.hpp"


TEST_F(BlockhashComparisonMinMatchingTests, TemplateSanity)
{
	if (s1template.size() >= 2)
	{
		EXPECT_EQ("01", s1template.substr(0, 2));
		EXPECT_EQ("23", s2template.substr(0, 2));
	}
	if (s1template.size() >= 8)
	{
		EXPECT_EQ("01010101", s1template.substr(0, 8));
		EXPECT_EQ("23232323", s2template.substr(0, 8));
	}

	EXPECT_EQ("ABCDEFG", MinMatchingTemplate1(7, 0));
	EXPECT_EQ("ABCDEFG", MinMatchingTemplate2(7, 0));

	EXPECT_EQ("ABCDEFG1010101010101", MinMatchingTemplate1(20,  0));
	EXPECT_EQ("0ABCDEFG010101010101", MinMatchingTemplate1(20,  1));
	EXPECT_EQ("01ABCDEFG10101010101", MinMatchingTemplate1(20,  2));
	EXPECT_EQ("010ABCDEFG0101010101", MinMatchingTemplate1(20,  3));
	EXPECT_EQ("0101ABCDEFG101010101", MinMatchingTemplate1(20,  4));
	EXPECT_EQ("01010ABCDEFG01010101", MinMatchingTemplate1(20,  5));
	EXPECT_EQ("010101ABCDEFG1010101", MinMatchingTemplate1(20,  6));
	EXPECT_EQ("0101010ABCDEFG010101", MinMatchingTemplate1(20,  7));
	EXPECT_EQ("01010101ABCDEFG10101", MinMatchingTemplate1(20,  8));
	EXPECT_EQ("010101010ABCDEFG0101", MinMatchingTemplate1(20,  9));
	EXPECT_EQ("0101010101ABCDEFG101", MinMatchingTemplate1(20, 10));
	EXPECT_EQ("01010101010ABCDEFG01", MinMatchingTemplate1(20, 11));
	EXPECT_EQ("010101010101ABCDEFG1", MinMatchingTemplate1(20, 12));
	EXPECT_EQ("0101010101010ABCDEFG", MinMatchingTemplate1(20, 13));

	EXPECT_EQ("ABCDEFG3232323232323", MinMatchingTemplate2(20,  0));
	EXPECT_EQ("2ABCDEFG232323232323", MinMatchingTemplate2(20,  1));
	EXPECT_EQ("23ABCDEFG32323232323", MinMatchingTemplate2(20,  2));
	EXPECT_EQ("232ABCDEFG2323232323", MinMatchingTemplate2(20,  3));
	EXPECT_EQ("2323ABCDEFG323232323", MinMatchingTemplate2(20,  4));
	EXPECT_EQ("23232ABCDEFG23232323", MinMatchingTemplate2(20,  5));
	EXPECT_EQ("232323ABCDEFG3232323", MinMatchingTemplate2(20,  6));
	EXPECT_EQ("2323232ABCDEFG232323", MinMatchingTemplate2(20,  7));
	EXPECT_EQ("23232323ABCDEFG32323", MinMatchingTemplate2(20,  8));
	EXPECT_EQ("232323232ABCDEFG2323", MinMatchingTemplate2(20,  9));
	EXPECT_EQ("2323232323ABCDEFG323", MinMatchingTemplate2(20, 10));
	EXPECT_EQ("23232323232ABCDEFG23", MinMatchingTemplate2(20, 11));
	EXPECT_EQ("232323232323ABCDEFG3", MinMatchingTemplate2(20, 12));
	EXPECT_EQ("2323232323232ABCDEFG", MinMatchingTemplate2(20, 13));

	EXPECT_EQ("ABCDEFG101010101010101010101010101010101010101010101010101010101", MinMatchingTemplate1(64,  0));
	EXPECT_EQ("010101010101010101010101010101010101010101010101010101010ABCDEFG", MinMatchingTemplate1(64, 57));
	EXPECT_EQ("ABCDEFG323232323232323232323232323232323232323232323232323232323", MinMatchingTemplate2(64,  0));
	EXPECT_EQ("232323232323232323232323232323232323232323232323232323232ABCDEFG", MinMatchingTemplate2(64, 57));
}


TEST_F(BlockhashComparisonMinMatchingTests, MinMatchingScoreSmall)
{
	for (blockhash_len_t s1len = blockhash_comparison_params::min_match_len;
		s1len <= digest_params::max_blockhash_len; s1len++)
	{
		string s1 = MinMatchingTemplate1(s1len, 0);
		for (blockhash_len_t s2len = blockhash_comparison_params::min_match_len;
			s2len <= digest_params::max_blockhash_len; s2len++)
		{
			string s2 = MinMatchingTemplate2(s2len, s2len - blockhash_comparison_params::min_match_len);
			for (unsigned bi = 0; bi < digest_blocksize::number_of_blockhashes; bi++)
			{
				digest_blocksize_t bs = digest_blocksize::at(bi);
				ASSERT_EQ(
					blockhash_comparison<>::uncapped_min_matching_score(s1len, s2len),
					blockhash_comparison<>::uncapped_score(s1.data(), s1len, s2.data(), s2len)
				);
				ASSERT_EQ(
					blockhash_comparison<>::min_matching_score(bs, s1len, s2len),
					blockhash_comparison<>::score(s1.data(), s1len, s2.data(), s2len, bs)
				);
				ASSERT_EQ(
					blockhash_comparison<>::min_matching_score(bs, s1len, s2len),
					std::min(
						blockhash_comparison<>::uncapped_min_matching_score(s1len, s2len),
						blockhash_comparison<>::score_cap(bs, s1len, s2len)
					)
				);
			}
		}
	}
}

TEST_F(BlockhashComparisonMinMatchingTests, MinMatchingScoreLargeSubset)
{
	for (blockhash_len_t s1len = blockhash_comparison_params::min_match_len;
		s1len <= digest_params::max_blockhash_len; s1len++)
	{
		for (blockhash_len_t s1idx = 0; s1idx <= s1len - blockhash_comparison_params::min_match_len; s1idx++)
		{
			string s1 = MinMatchingTemplate1(s1len, s1idx);
			for (blockhash_len_t s2len = blockhash_comparison_params::min_match_len;
				s2len <= digest_params::max_blockhash_len; s2len++)
			{
				for (blockhash_len_t s2idx = 0; s2idx <= s2len - blockhash_comparison_params::min_match_len; s2idx++)
				{
					string s2 = MinMatchingTemplate2(s2len, s2idx);
					ASSERT_EQ(
						blockhash_comparison<>::uncapped_min_matching_score(s1len, s2len),
						blockhash_comparison<>::uncapped_score(s1.data(), s1len, s2.data(), s2len)
					);
				}
			}
		}
	}
}

#endif
