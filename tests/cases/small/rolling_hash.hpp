/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/small/rolling_hash.hpp
	Tests for rolling_hash and rolling_hash_ssdeep

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
#ifndef FFUZZYPP_TESTCASES_SMALL_ROLLING_HASH_HPP
#define FFUZZYPP_TESTCASES_SMALL_ROLLING_HASH_HPP

#include <cstdint>


template <typename T>
class RollingHashGenericTests : public ::testing::Test {};

typedef ::testing::Types<rolling_hash, rolling_hash_ssdeep> RollingHashTypes;
TYPED_TEST_CASE(RollingHashGenericTests, RollingHashTypes);

TYPED_TEST(RollingHashGenericTests, ShortResetTest)
{
	typedef TypeParam rolling_hash_t;
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
				rolling_hash_t h; // initialized
				h.update(c1); h.update(c2); h.update(c3);
				uint_least32_t saved_sum = h.sum();
				h.reset();
				h.update(c1); h.update(c2); h.update(c3);
				ASSERT_EQ(saved_sum, h.sum())
					<< "3-byte rolling_hash short reset test failed on bytes "
					<< "[" << i1 << ", " << i2 << ", " << i3 << "].";
			}
			rolling_hash_t h; // initialized
			h.update(c1); h.update(c2);
			uint_least32_t saved_sum = h.sum();
			h.reset();
			h.update(c1); h.update(c2);
			ASSERT_EQ(saved_sum, h.sum())
				<< "2-byte rolling_hash short reset test failed on bytes "
				<< "[" << i1 << ", " << i2 << "].";
		}
		rolling_hash_t h; // initialized
		h.update(c1);
		uint_least32_t saved_sum = h.sum();
		h.reset();
		h.update(c1);
		ASSERT_EQ(saved_sum, h.sum())
			<< "1-byte rolling_hash short reset test failed on byte " << i1 << ".";
	}
}

#endif
