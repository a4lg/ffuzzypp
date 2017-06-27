/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/small/digest_comparison_score_cap.hpp
	Tests for score capping (which prevents match exaggerations)

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
#ifndef FFUZZYPP_TESTCASES_SMALL_DIGEST_COMPARISON_SCORE_CAP_HPP
#define FFUZZYPP_TESTCASES_SMALL_DIGEST_COMPARISON_SCORE_CAP_HPP

#include <cstdint>
#include <algorithm>


TEST(DigestComparisonScoreCapTests, IsSafeForScoreCappingTrueSpec)
{
	static constexpr digest_blocksize_t ubound =
		digest_blocksize_t(99u + blockhash_comparison_params::min_match_len) /
		digest_blocksize_t(blockhash_comparison_params::min_match_len) *
		digest_blocksize::min_blocksize;
	for (digest_blocksize_t s = 0; s < ubound; s++)
	{
		EXPECT_TRUE(blockhash_comparison<>::is_safe_for_score_capping(s))
			<< "is_safe_for_score_capping test (to true) failed at " << s << ".";
	}
}

TEST(DigestComparisonScoreCapTests, IsSafeForScoreCappingFalseSpec)
{
	// Test for limited range (in 32-bit)
	for (
		uint_least64_t i = digest_blocksize::min_blocksize * 100 + 1;
		i <= 0xfffffffful; i++
	)
	{
		digest_blocksize_t s(i);
		ASSERT_FALSE(blockhash_comparison<>::is_safe_for_score_capping(s))
			<< "is_safe_for_score_capping test (to false) failed at " << s << ".";
	}
}

TEST(DigestComparisonScoreCapTests, IsSafeForScoreCappingIndeterminateSpecMin)
{
	static constexpr digest_blocksize_t ubound =
		digest_blocksize::min_blocksize * 100;
	static constexpr digest_blocksize_t lbound =
		digest_blocksize_t(99u + blockhash_comparison_params::min_match_len) /
		digest_blocksize_t(blockhash_comparison_params::min_match_len) *
		digest_blocksize::min_blocksize;
	/*
		Tests values not tested in the following two tests
		*	IsSafeForScoreCappingTrueSpec (0 <= x < lbound)
		*	IsSafeForScpreCapFalseSpec (ubound < x [< 2^32])

		That means, tests range of (lbound <= x <= ubound).
	*/
	for (uint_least64_t i = lbound; i <= ubound; i++)
	{
		digest_blocksize_t s(i);
		ASSERT_LE(100,
			blockhash_comparison<>::score_cap(s,
				blockhash_comparison_params::min_match_len,
				blockhash_comparison_params::min_match_len
			))
			<< "score_cap test for indeterminate is_safe_for_score_capping range "
			<< "failed at " << s << ".";
	}
}

TEST(DigestComparisonScoreCapTests, ScoreCapSpecSmall)
{
	for (digest_blocksize_t s = 0; s < digest_blocksize::min_blocksize * 100; s++)
	{
		for (
			blockhash_len_t l = blockhash_comparison_params::min_match_len;
			l <= digest_params::max_blockhash_len; l++
		)
		{
			if (blockhash_comparison<>::is_safe_for_score_capping(s))
			{
				digest_comparison_score_t expected_cap
					= std::min<digest_comparison_score_t>(
						digest_comparison_score_t(s / digest_blocksize::min_blocksize)
							* digest_comparison_score_t(l),
						100);
				digest_comparison_score_t actual_cap
					= blockhash_comparison<>::score_cap(s, l, l);
				ASSERT_TRUE(expected_cap == actual_cap || 100 <= actual_cap)
					<< "score cap test failed at block size " << s
					<< " and blockhash length " << l << ".";
			}
			else
			{
				ASSERT_EQ(100, blockhash_comparison<>::score_cap(s, l, l))
					<< "score cap test failed at block size " << s
					<< " and blockhash length " << l << ".";
			}
		}
	}
}

#endif
