/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/compatibility/small/digest_comparison_identical_2_9_2.hpp
	Tests for identical digest comparison (compare with version 2.9; part 2)

	Copyright (C) 2017 Tsukasa OI <floss_ssdeep@irq.a4lg.com>


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
#ifndef FFUZZYPP_TESTCASES_COMPATIBILITY_SMALL_DIGEST_COMPARISON_IDENTICAL_2_9_2_HPP
#define FFUZZYPP_TESTCASES_COMPATIBILITY_SMALL_DIGEST_COMPARISON_IDENTICAL_2_9_2_HPP

#include <string>


// Data used in tests are retrieved from ssdeep 2.9, x86_64

TEST_F(DigestComparisonIdenticalTests_2_9, MinimumNormalizationComparisonTest)
{
	EXPECT_EQ(  0, digest_comparison<comparison_version::v2_9>::compare("3:AAAAAAA:", "3:AAAAAAAAAAAAAA:"));
	EXPECT_EQ(  0, digest_comparison<comparison_version::v2_9>::compare("3::AAAAAAA", "3::AAAAAAAAAAAAAA"));
	EXPECT_EQ(  7, digest_comparison<comparison_version::v2_9>::compare("3:AAAAAABCCCCCC:", "3:AAABCCC:"));
	EXPECT_EQ(  7, digest_comparison<comparison_version::v2_9>::compare("3:AAAAAABCCCCCC:D", "3:AAABCCC:E"));
	EXPECT_EQ( 14, digest_comparison<comparison_version::v2_9>::compare("3::AAAAAABCCCCCC", "3::AAABCCC"));
	EXPECT_EQ( 14, digest_comparison<comparison_version::v2_9>::compare("3:D:AAAAAABCCCCCC", "3:E:AAABCCC"));
	EXPECT_EQ( 32, digest_comparison<comparison_version::v2_9>::compare(
		"3:aaaaaaaabbbbbbbbccccccccddddddddeeeeeeeeffffffffgggggggghhhhhhhh01234567:",
		"3:aaabbbcccdddeeefffggghhh01234567:"
	));
}

#endif
