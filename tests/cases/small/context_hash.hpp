/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/small/context_hash.hpp
	Tests for context_hash and context_hash_fast

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
#ifndef FFUZZYPP_TESTCASES_SMALL_CONTEXT_HASH_HPP
#define FFUZZYPP_TESTCASES_SMALL_CONTEXT_HASH_HPP

#include <cstdint>

#include <limits>


TEST(ContextHashTests, PortableEnvironmentSpec)
{
	// This will fail on non-8bit environments.
	// Failing this test has no problem except portability.
	EXPECT_EQ(0x00, numeric_limits<unsigned char>::min());
	EXPECT_EQ(0xff, numeric_limits<unsigned char>::max());
}

template <typename T>
class ContextHashGenericTests : public ::testing::Test {};

typedef ::testing::Types<context_hash, context_hash_fast> ContextHashTypes;
TYPED_TEST_CASE(ContextHashGenericTests, ContextHashTypes);

TYPED_TEST(ContextHashGenericTests, ResetTest)
{
	typedef TypeParam context_hash_t;
	// Test for bytes (max of 3)
	for (unsigned i1 = 0x00; i1 <= 0xff; i1++)
	{
		unsigned char c1(i1);
		for (unsigned i2 = 0x00; i2 <= 0xff; i2++)
		{
			unsigned char c2(i2);
			for (unsigned i3 = 0x00; i3 <= 0xff; i3++)
			{
				unsigned char c3(i3);
				context_hash_t h; // uninitialized
				h.reset(); h.update(c1); h.update(c2); h.update(c3);
				char saved_sum = h.sum_in_base64();
				h.reset(); h.update(c1); h.update(c2); h.update(c3);
				ASSERT_EQ(int(saved_sum), int(h.sum_in_base64()))
					<< "3-byte context_hash reset test failed on bytes "
					<< "[" << i1 << ", " << i2 << ", " << i3 << "]";
			}
			context_hash_t h; // uninitialized
			h.reset(); h.update(c1); h.update(c2);
			char saved_sum = h.sum_in_base64();
			h.reset(); h.update(c1); h.update(c2);
			ASSERT_EQ(int(saved_sum), int(h.sum_in_base64()))
				<< "2-byte context_hash reset test failed on bytes "
				<< "[" << i1 << ", " << i2 << "]";
		}
		context_hash_t h; // uninitialized
		h.reset(); h.update(c1);
		char saved_sum = h.sum_in_base64();
		h.reset(); h.update(c1);
		ASSERT_EQ(int(saved_sum), int(h.sum_in_base64()))
			<< "1-byte context_hash reset test failed on byte " << i1;
	}
}

TEST(ContextHashTests, FastImplCompareTest)
{
	// Test for bytes (max of 3)
	for (unsigned i1 = 0x00; i1 <= 0xff; i1++)
	{
		unsigned char c1(i1);
		for (unsigned i2 = 0x00; i2 <= 0xff; i2++)
		{
			unsigned char c2(i2);
			for (unsigned i3 = 0x00; i3 <= 0xff; i3++)
			{
				unsigned char c3(i3);
				context_hash      h1;
				context_hash_fast h2;
				h1.reset();
				h2.reset();
				h1.update(c1); h1.update(c2); h1.update(c3);
				h2.update(c1); h2.update(c2); h2.update(c3);
				ASSERT_EQ(int(h1.sum_in_base64()), int(h2.sum_in_base64()))
					<< "3-byte context_hash insertion test failed on bytes "
					<< "[" << i1 << ", " << i2 << ", " << i3 << "]";
			}
			context_hash      h1;
			context_hash_fast h2;
			h1.reset();
			h2.reset();
			h1.update(c1); h1.update(c2);
			h2.update(c1); h2.update(c2);
			ASSERT_EQ(int(h1.sum_in_base64()), int(h2.sum_in_base64()))
				<< "2-byte context_hash insertion test failed on bytes "
				<< "[" << i1 << ", " << i2 << "]";
		}
		context_hash      h1;
		context_hash_fast h2;
		h1.reset();
		h2.reset();
		h1.update(c1);
		h2.update(c1);
		ASSERT_EQ(int(h1.sum_in_base64()), int(h2.sum_in_base64()))
			<< "1-byte context_hash insertion test failed on byte " << i1;
	}
}

#endif
