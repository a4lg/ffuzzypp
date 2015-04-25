/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/compatibility/small/digest_normalization.hpp
	Tests for digest normalization

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
#ifndef FFUZZYPP_TESTCASES_COMPATIBILITY_SMALL_DIGEST_NORMALIZATION_HPP
#define FFUZZYPP_TESTCASES_COMPATIBILITY_SMALL_DIGEST_NORMALIZATION_HPP

#include <utility>


class DigestNormalizationTests
	: public ::testing::TestWithParam<pair<string, string>> {};

TEST_P(DigestNormalizationTests, StringizationTest)
{
	string unorm_str = GetParam().first;
	string pnorm_str = GetParam().second;
	char digestbuf[digest_long_t::max_natural_chars];
	digest_long_t d(unorm_str);
	d.pretty_unsafe(digestbuf);
	ASSERT_STREQ(pnorm_str.c_str(), digestbuf);
	ASSERT_STREQ(digestbuf, d.pretty().c_str());
}

TEST_P(DigestNormalizationTests, ComparisonTest)
{
	string unorm_str = GetParam().first;
	string pnorm_str = GetParam().second;
	digest_long_t d1(unorm_str);
	digest_long_t d2(pnorm_str);
	ASSERT_EQ(d1, d2);
}

TEST_P(DigestNormalizationTests, NormalizationComparisonTest)
{
	string unorm_str = GetParam().first;
	string pnorm_str = GetParam().second;
	digest_long_t d(unorm_str);
	digest_long_unorm_t u(unorm_str);
	u.normalize();
	ASSERT_EQ(d, u);
}

INSTANTIATE_TEST_CASE_P(
	MinimumTests,
	DigestNormalizationTests,
	::testing::Values(
		pair<string, string>("196608:ABCDEFG:abcdefg", "196608:ABCDEFG:abcdefg"),
		pair<string, string>("3072:AAAAAAAABBBBBBBBCCCCCCCC:", "3072:AAABBBCCC:"),
		pair<string, string>("3072::AAAAAAAABBBBBBBBCCCCCCCC", "3072::AAABBBCCC"),
		pair<string, string>("3072:AAAAAABBBBBBCCCCCC:CCCCCCDDDDDDEEEEEE", "3072:AAABBBCCC:CCCDDDEEE")
	));

#endif
