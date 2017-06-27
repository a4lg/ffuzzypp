/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/compatibility/small/digest_data_naturality.hpp
	Tests for digest data naturality

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
#ifndef FFUZZYPP_TESTCASES_COMPATIBILITY_SMALL_DIGEST_DATA_NATURALITY_HPP
#define FFUZZYPP_TESTCASES_COMPATIBILITY_SMALL_DIGEST_DATA_NATURALITY_HPP

#include <string>


TEST(DigestDataNaturalityTests, NaturalCases)
{
	EXPECT_TRUE(digest_t("3::").is_natural());
	EXPECT_TRUE(digest_t("3:AA:A").is_natural());
}

TEST(DigestDataNaturalityTests, UnnaturalCases)
{
	// block size 4 is not natural
	EXPECT_FALSE(digest_t("4::").is_natural());
	// second block hash must not be longer than the first one
	EXPECT_FALSE(digest_t("3:A:AA").is_natural());
	// non-Base64 character may not be included.
	EXPECT_FALSE(digest_t("3:*:A").is_natural());
	EXPECT_FALSE(digest_t("3:A:*").is_natural());
}

#endif
