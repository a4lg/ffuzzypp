/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/small/position_array.hpp
	Position array tests

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
#ifndef FFUZZYPP_TESTCASES_SMALL_POSITION_ARRAY_HPP
#define FFUZZYPP_TESTCASES_SMALL_POSITION_ARRAY_HPP

#include <cstddef>

class PositionArrayTests : public ::testing::Test
{
public:
	static const char Digits[11];
	static const char TestString[65];
	static const unsigned long long DigitPositions[10];
};
const char PositionArrayTests::TestString[65] =
	"02122121"
	"12122121"
	"34344343"
	"34344343"
	"56566565"
	"56566565"
	"78788787"
	"78788789";
const unsigned long long PositionArrayTests::DigitPositions[10] =
{
	0x0000000000000001ull,
	0x000000000000a5a4ull,
	0x0000000000005a5aull,
	0x00000000a5a50000ull,
	0x000000005a5a0000ull,
	0x0000a5a500000000ull,
	0x00005a5a00000000ull,
	0x25a5000000000000ull,
	0x5a5a000000000000ull,
	0x8000000000000000ull,
};
const char PositionArrayTests::Digits[11] = "0123456789";


TEST_F(PositionArrayTests, InitialStateAfterReset)
{
	strings::position_array<unsigned long long, char, '0', '9'> parray;
	parray.reset();
	for (int i = 0; i < 10; i++)
		ASSERT_EQ(0, parray['0' + i]);
}

TEST_F(PositionArrayTests, InitializationProperties)
{
	strings::position_array<unsigned long long, char, '0', '9'> parray1;
	parray1.construct(TestString, 64);
	strings::position_array<unsigned long long, char, '0', '9'> parray2(parray1);
	strings::position_array<unsigned long long, char, '0', '9'> parray3(TestString, 64);
	for (int i = 0; i < 10; i++)
	{
		ASSERT_EQ(parray1[Digits[i]], parray2[Digits[i]]);
		ASSERT_EQ(parray1[Digits[i]], parray3[Digits[i]]);
	}
}

TEST_F(PositionArrayTests, BasicProperties)
{
	strings::position_array<unsigned long long, char, '0', '9'> parray(TestString, 64);
	for (int i = 0; i < 10; i++)
	{
		ASSERT_EQ(DigitPositions[i], parray[Digits[i]])
			<< "position array for '" << Digits[i] << "' "
			<< "had unexpected value.";
		ASSERT_EQ(parray.bitmap_for(Digits[i]), parray[Digits[i]]);
	}
	EXPECT_EQ(0, parray.bitmap_for_safe('\0'));
	EXPECT_EQ(0, parray.bitmap_for_safe('A'));
	EXPECT_EQ(0, parray.bitmap_for_safe('z'));
}

TEST_F(PositionArrayTests, SafeProperties)
{
	strings::position_array<unsigned long long, char, '0', '9'> parray;
	char teststr[65];
	memcpy(teststr, TestString, 65);
	// Insert out-of-range characters
	teststr[ 0] = 'A';
	teststr[21] = '\0';
	teststr[63] = 'z';
	unsigned long long mask = 0xffffffffffffffffull;
	mask &= ~(1ull <<  0);
	mask &= ~(1ull << 21);
	mask &= ~(1ull << 63);
	// ...then test using the mask
	parray.construct_safe(teststr, 64);
	for (int i = 0; i < 10; i++)
	{
		ASSERT_EQ(DigitPositions[i] & mask, parray[Digits[i]])
			<< "position array for '" << Digits[i] << "' "
			<< "had unexpected value.";
		ASSERT_EQ(parray.bitmap_for_safe(Digits[i]), parray.bitmap_for(Digits[i]));
	}
	// No matter the value is set, bitmap is not updated for those out-of-range characters.
	EXPECT_EQ(0, parray.bitmap_for_safe('\0'));
	EXPECT_EQ(0, parray.bitmap_for_safe('A'));
	EXPECT_EQ(0, parray.bitmap_for_safe('z'));
}

TEST_F(PositionArrayTests, ResetProperties)
{
	strings::position_array<unsigned long long, char, '0', '9'> parray(TestString, 64);
	parray.reset();
	for (int i = 0; i < 10; i++)
		ASSERT_EQ(0, parray[Digits[i]]);
}

#endif
