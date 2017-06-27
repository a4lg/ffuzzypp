/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/compatibility/small/digest_generator_forms.hpp
	Tests for digest generator (result forms)

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
#ifndef FFUZZYPP_TESTCASES_COMPATIBILITY_SMALL_DIGEST_GENERATOR_FORMS_HPP
#define FFUZZYPP_TESTCASES_COMPATIBILITY_SMALL_DIGEST_GENERATOR_FORMS_HPP

#include <string>

#include "digest_generator_initial_guess.hpp"


TEST(DigestGeneratorResultFormTests, DigestReturningVariantsTest)
{
	digest_generator gen;
	DigestGeneratorInitialGuessTests::fill_seq(gen, 64, 1536);
	EXPECT_EQ("24"
			":0000000000000000000000000000000000000000000000000000000000000000"
			":0000000000000000000000000000000U",
		gen.digest_str()
	);
	EXPECT_EQ("24"
			":0000000000000000000000000000000000000000000000000000000000000000"
			":0000000000000000000000000000000U",
		gen.digest().pretty()
	);
	EXPECT_EQ("24:000:000U", gen.digest_normalized().pretty());
	// expect digest_long to return non-shortened form
	EXPECT_EQ("24"
			":0000000000000000000000000000000000000000000000000000000000000000"
			":0000000000000000000000000000000000000000000000000000000000000000",
		gen.digest_long().pretty()
	);
	// expect no 'U' at the tail
	EXPECT_EQ("24:000:000", gen.digest_long_normalized().pretty());
}

TEST(DigestGeneratorResultFormTests, CopyDigestTest)
{
	digest_generator gen;
	DigestGeneratorInitialGuessTests::fill_seq(gen, 64, 1536);
	digest_t dsn;
	digest_unorm_t dsu;
	digest_long_t dln;
	digest_long_unorm_t dlu;
	EXPECT_TRUE(gen.copy_digest(dsn));
	EXPECT_EQ("24:000:000U", dsn.pretty());
	EXPECT_TRUE(gen.copy_digest(dsu));
	EXPECT_EQ("24"
			":0000000000000000000000000000000000000000000000000000000000000000"
			":0000000000000000000000000000000U",
		dsu.pretty()
	);
	// expect copy_digest to return shortened form
	EXPECT_TRUE(gen.copy_digest(dln));
	EXPECT_EQ("24:000:000U", dln.pretty());
	EXPECT_TRUE(gen.copy_digest(dlu));
	EXPECT_EQ("24"
			":0000000000000000000000000000000000000000000000000000000000000000"
			":0000000000000000000000000000000U",
		dlu.pretty()
	);
}

TEST(DigestGeneratorResultFormTests, CopyDigestLongNormalizedTest)
{
	digest_generator gen;
	DigestGeneratorInitialGuessTests::fill_seq(gen, 64, 1536);
	digest_long_t dln;
	// expect copy_digest_long_normalized to return non-shortened form
	EXPECT_TRUE(gen.copy_digest_long_normalized(dln));
	EXPECT_EQ("24:000:000", dln.pretty());
}

TEST(DigestGeneratorResultFormTests, CopyDigestLongTest)
{
	digest_generator gen;
	DigestGeneratorInitialGuessTests::fill_seq(gen, 64, 1536);
	digest_long_t dln;
	digest_long_unorm_t dlu;
	// expect copy_digest_long to return non-shortened form
	EXPECT_TRUE(gen.copy_digest_long(dln));
	EXPECT_EQ("24:000:000", dln.pretty());
	EXPECT_TRUE(gen.copy_digest_long(dlu));
	EXPECT_EQ("24"
			":0000000000000000000000000000000000000000000000000000000000000000"
			":0000000000000000000000000000000000000000000000000000000000000000",
		dlu.pretty()
	);
}

TEST(DigestGeneratorResultFormTests, CopyDigestNormalizedTest)
{
	digest_generator gen;
	DigestGeneratorInitialGuessTests::fill_seq(gen, 64, 1536);
	digest_unorm_t ds;
	digest_long_unorm_t dl;
	EXPECT_TRUE((gen.copy_digest_normalized<false, true>(ds)));
	EXPECT_EQ("24:000:000U", ds.pretty());
	EXPECT_TRUE((gen.copy_digest_normalized<false, false>(dl)));
	EXPECT_EQ("24:000:000U", dl.pretty());
	EXPECT_TRUE((gen.copy_digest_normalized<false, false, true>(dl)));
	EXPECT_EQ("24:000:000U", dl.pretty());
	// expect no 'U' at the tail
	EXPECT_TRUE((gen.copy_digest_normalized<false, false, false>(dl)));
	EXPECT_EQ("24:000:000", dl.pretty());
}

#endif
