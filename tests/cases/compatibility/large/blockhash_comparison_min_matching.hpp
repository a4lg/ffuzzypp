/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/compatibility/large/blockhash_comparison_min_matching.hpp
	Tests for minimum matching scores (large)

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
#ifndef FFUZZYPP_TESTCASES_COMPATIBILITY_LARGE_BLOCKHASH_COMPARISON_MIN_MATCHING_HPP
#define FFUZZYPP_TESTCASES_COMPATIBILITY_LARGE_BLOCKHASH_COMPARISON_MIN_MATCHING_HPP

#include <cassert>
#include <algorithm>
#include <string>

#include "../common/blockhash_comparison_min_matching.hpp"


class BlockhashComparisonMinMatchingTestsWithBlocksizeIndex
	: public BlockhashComparisonMinMatchingTests
	, public ::testing::WithParamInterface<unsigned>
{
};

TEST_P(BlockhashComparisonMinMatchingTestsWithBlocksizeIndex, MinMatchingScore)
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
					unsigned bi = GetParam();
					digest_blocksize_t bs = digest_blocksize::at(bi);
					ASSERT_EQ(
						blockhash_comparison<>::min_matching_score(bs, s1len, s2len),
						blockhash_comparison<>::score(s1.data(), s1len, s2.data(), s2len, bs)
					);
				}
			}
		}
	}
}

INSTANTIATE_TEST_CASE_P(BlocksizeCases, BlockhashComparisonMinMatchingTestsWithBlocksizeIndex,
	::testing::Range<unsigned>(0u, digest_blocksize::number_of_blockhashes));


#endif
