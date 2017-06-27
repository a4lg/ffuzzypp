/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/compatibility/small/blockhash_comparison_max_matching.hpp
	Tests for maximum matching scores

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
#ifndef FFUZZYPP_TESTCASES_COMPATIBILITY_SMALL_BLOCKHASH_COMPARISON_MAX_MATCHING_HPP
#define FFUZZYPP_TESTCASES_COMPATIBILITY_SMALL_BLOCKHASH_COMPARISON_MAX_MATCHING_HPP

#include <cassert>
#include <string>


class BlockhashComparisonMaxMatchingTests : public ::testing::Test
{
protected:
	const string template_str;
public:
	BlockhashComparisonMaxMatchingTests(void)
		: ::testing::Test()
		, template_str("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/")
	{
	}
protected:
	string MaxMatchingTemplate(blockhash_len_t len)
	{
		return template_str.substr(0, len);
	}
	string MaxMatchingTemplateSubstring(
		blockhash_len_t len_max,
		blockhash_len_t len_min,
		blockhash_len_t index
	)
	{
		assert(len_max > len_min);
		assert(index <= len_max - len_min);
		return template_str.substr(index, len_min);
	}
};


TEST_F(BlockhashComparisonMaxMatchingTests, TemplateSanity)
{
	EXPECT_EQ("A", MaxMatchingTemplate(1));
	EXPECT_EQ("AB", MaxMatchingTemplate(2));
	EXPECT_EQ("ABC", MaxMatchingTemplate(3));
	EXPECT_EQ("ABCD", MaxMatchingTemplate(4));
	EXPECT_EQ("ABCDE", MaxMatchingTemplate(5));
	EXPECT_EQ("ABCDEF", MaxMatchingTemplate(6));
	EXPECT_EQ("ABCDEFG", MaxMatchingTemplate(7));
	EXPECT_EQ("ABCDEFGH", MaxMatchingTemplate(8));
	EXPECT_EQ("ABCDEFGHI", MaxMatchingTemplate(9));
	EXPECT_EQ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/", MaxMatchingTemplate(64));
	EXPECT_EQ("AB", MaxMatchingTemplateSubstring(3, 2, 0));
	EXPECT_EQ("BC", MaxMatchingTemplateSubstring(3, 2, 1));
	EXPECT_EQ("ABC", MaxMatchingTemplateSubstring(7, 3, 0));
	EXPECT_EQ("BCD", MaxMatchingTemplateSubstring(7, 3, 1));
	EXPECT_EQ("CDE", MaxMatchingTemplateSubstring(7, 3, 2));
	EXPECT_EQ("DEF", MaxMatchingTemplateSubstring(7, 3, 3));
	EXPECT_EQ("EFG", MaxMatchingTemplateSubstring(7, 3, 4));
}


TEST_F(BlockhashComparisonMaxMatchingTests, CommutativeProperty)
{
	for (blockhash_len_t s1len = blockhash_comparison_params::min_match_len;
		s1len <= digest_params::max_blockhash_len; s1len++)
	{
		for (blockhash_len_t s2len = blockhash_comparison_params::min_match_len;
			s2len <= digest_params::max_blockhash_len; s2len++)
		{
			ASSERT_EQ(
				blockhash_comparison<>::uncapped_max_matching_score(s1len, s2len),
				blockhash_comparison<>::uncapped_max_matching_score(s2len, s1len)
			);
			for (unsigned bi = 0; bi < digest_blocksize::number_of_blockhashes; bi++)
			{
				digest_blocksize_t bs = digest_blocksize::at(bi);
				ASSERT_EQ(
					blockhash_comparison<>::capped_max_matching_score(bs, s1len, s2len),
					blockhash_comparison<>::capped_max_matching_score(bs, s2len, s1len)
				);
				ASSERT_EQ(
					blockhash_comparison<>::max_matching_score(bs, s1len, s2len),
					blockhash_comparison<>::max_matching_score(bs, s2len, s1len)
				);
			}
		}
	}
}

TEST_F(BlockhashComparisonMaxMatchingTests, MaxMatchingScoreNotEq)
{
	for (blockhash_len_t s1len = blockhash_comparison_params::min_match_len;
		s1len <= digest_params::max_blockhash_len; s1len++)
	{
		string s1 = MaxMatchingTemplate(s1len);
		for (blockhash_len_t s2len = blockhash_comparison_params::min_match_len; s2len < s1len; s2len++)
		{
			ASSERT_NE(s1len, s2len);
			ASSERT_EQ(
				blockhash_comparison<>::uncapped_max_matching_score_le(s2len, s1len),
				blockhash_comparison<>::uncapped_max_matching_score(s2len, s1len)
			);
			for (blockhash_len_t index = 0; index <= s1len - s2len; index++)
			{
				string s2 = MaxMatchingTemplateSubstring(s1len, s2len, index);
				ASSERT_EQ(
					blockhash_comparison<>::uncapped_max_matching_score_le(s2len, s1len),
					blockhash_comparison<>::uncapped_score(s1.data(), s1len, s2.data(), s2len)
				);
				for (unsigned bi = 0; bi < digest_blocksize::number_of_blockhashes; bi++)
				{
					digest_blocksize_t bs = digest_blocksize::at(bi);
					ASSERT_EQ(
						blockhash_comparison<>::capped_max_matching_score_le(bs, s2len, s1len),
						blockhash_comparison<>::capped_max_matching_score(bs, s2len, s1len)
					);
					ASSERT_EQ(
						blockhash_comparison<>::max_matching_score_le(bs, s2len, s1len),
						blockhash_comparison<>::max_matching_score(bs, s2len, s1len)
					);
					ASSERT_EQ(
						blockhash_comparison<>::max_matching_score_le(bs, s2len, s1len),
						blockhash_comparison<>::score(s1.data(), s1len, s2.data(), s2len, bs)
					);
					ASSERT_EQ(
						blockhash_comparison<>::max_matching_score_le(bs, s2len, s1len),
						blockhash_comparison<>::capped_max_matching_score_le(bs, s2len, s1len)
					);
				}
			}
		}
	}
}

TEST_F(BlockhashComparisonMaxMatchingTests, MaxMatchingScoreEq)
{
	for (blockhash_len_t slen = blockhash_comparison_params::min_match_len;
		slen <= digest_params::max_blockhash_len; slen++)
	{
		ASSERT_EQ(
			blockhash_comparison<>::uncapped_max_matching_score_le(slen, slen),
			blockhash_comparison<>::uncapped_max_matching_score(slen, slen)
		);
		string s = MaxMatchingTemplate(slen);
		ASSERT_EQ(
			blockhash_comparison<>::uncapped_max_matching_score_le(slen, slen),
			blockhash_comparison<>::uncapped_score(s.data(), slen, s.data(), slen)
		);
		for (unsigned bi = 0; bi < digest_blocksize::number_of_blockhashes; bi++)
		{
			digest_blocksize_t bs = digest_blocksize::at(bi);
			ASSERT_EQ(
				blockhash_comparison<>::capped_max_matching_score_le(bs, slen, slen),
				blockhash_comparison<>::score(s.data(), slen, s.data(), slen, bs)
			);
			ASSERT_EQ(
				blockhash_comparison<>::max_matching_score_le(bs, slen, slen),
				blockhash_comparison<>::score_identical(slen, bs)
			);
		}
	}
}

#endif
