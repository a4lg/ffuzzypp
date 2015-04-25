/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/compatibility/small/digest_generator_rolling_hash_overflow.hpp
	Tests for digest generator (regarding rolling_hash overflow)

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
#ifndef FFUZZYPP_TESTCASES_COMPATIBILITY_SMALL_DIGEST_GENERATOR_ROLLING_HASH_OVERFLOW_HPP
#define FFUZZYPP_TESTCASES_COMPATIBILITY_SMALL_DIGEST_GENERATOR_ROLLING_HASH_OVERFLOW_HPP

#include <string>


// This test case is inspired by fail case from Digest::ssdeep 0.9.3, x86_64.

TEST(DigestGeneratorTests, RollingHashOverflowTest)
{
	digest_generator gen;
	// if no mod 2^32 on final hash addition, it makes 0x100000004 (blocksize 3)
	static const unsigned char ovf1[] = {0x78, 0xfa, 0xb8, 0xfd, 0x22, 0x00};
	// if no mod 2^32 on final hash addition, it makes 0x100000001 (blocksize 6)
	static const unsigned char ovf2[] = {0xf9, 0xdf, 0x1d, 0x5a, 0xcb, 0x00};
	// if no mod 2^32 on final hash addition and XOR-SHIFT hash, it makes a blocksize 3 match
	static const unsigned char ovf3[] = {0xb2, 0xe9, 0x10, 0x5b, 0x38, 0x00};
	// if no mod 2^32 on final hash addition and XOR-SHIFT hash, it makes a blocksize 3 match
	static const unsigned char ovf4[] = {0xff, 0x00, 0x30, 0x43, 0x7a, 0x27, 0x00};
	// 6-bytes to wipe rolling hash (7 bytes requied but I chose sequences with last byte zero)
	static const unsigned char nil6[6] = {0};

	gen.reset();
	gen.update(ovf1, sizeof(ovf1));
	EXPECT_EQ("3:2:2", gen.digest_str());

	gen.reset();
	gen.update(ovf1, sizeof(ovf1));
	gen.update(nil6, sizeof(nil6));
	EXPECT_EQ("3::", gen.digest_str());

	gen.reset();
	gen.update(ovf2, sizeof(ovf2));
	EXPECT_EQ("3:D:D", gen.digest_str());

	gen.reset();
	gen.update(ovf2, sizeof(ovf2));
	gen.update(nil6, sizeof(nil6));
	EXPECT_EQ("3::", gen.digest_str());

	gen.reset();
	gen.update(ovf3, sizeof(ovf3));
	EXPECT_EQ("3:3:3", gen.digest_str());

	gen.reset();
	gen.update(ovf3, sizeof(ovf3));
	gen.update(nil6, sizeof(nil6));
	EXPECT_EQ("3::", gen.digest_str());

	gen.reset();
	gen.update(ovf4, sizeof(ovf4));
	EXPECT_EQ("3:Q:Q", gen.digest_str());

	gen.reset();
	gen.update(ovf4, sizeof(ovf4));
	gen.update(nil6, sizeof(nil6));
	EXPECT_EQ("3::", gen.digest_str());
}

#endif
