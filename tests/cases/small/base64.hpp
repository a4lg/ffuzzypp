/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/small/base64.hpp
	Base64 tests

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
#ifndef FFUZZYPP_TESTCASES_SMALL_BASE64_HPP
#define FFUZZYPP_TESTCASES_SMALL_BASE64_HPP

#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <errno.h>


TEST(Base64Tests, BasicPropertiesOfBase64Values)
{
	ASSERT_EQ(64, sizeof(base64::values) / sizeof(base64::values[0]));
	ASSERT_EQ(1, sizeof(base64::values[0]));
	ASSERT_EQ('+', base64::values[62]);
	ASSERT_EQ('/', base64::values[63]);
}

TEST(Base64Tests, Base64ValuesHaveNoDuplicates)
{
	for (size_t i = 0; i < sizeof(base64::values); i++)
	{
		for (size_t j = 0; j < sizeof(base64::values); j++)
		{
			if (i == j)
				continue;
			ASSERT_NE(base64::values[i], base64::values[j])
				<< "base64::values at " << i << " and " << j
				<< " are same ('" << base64::values[i] << "')";
		}
	}
}

TEST(Base64Tests, Base64ValuesSatisfiesIsBase64)
{
	for (size_t i = 0; i < sizeof(base64::values); i++)
	{
		EXPECT_TRUE(base64::isbase64(base64::values[i]));
	}
}

static void Base64SpecForOneChar(char ch)
{
	const char* ch_in_values = find(
		base64::values,
		base64::values + sizeof(base64::values),
		ch);
	if (ch_in_values != base64::values+sizeof(base64::values))
	{
		EXPECT_TRUE(base64::isbase64(ch));
	}
	else
	{
		EXPECT_FALSE(base64::isbase64(ch));
	}
}

TEST(Base64Tests, Base64SpecForAllChars)
{
	for (char i : ranges::inclusive<char>())
		Base64SpecForOneChar(i);
}

TEST(Base64Tests, Base64ValueOrdinals)
{
	// Expected orders and ordinals by base 36
	//  0 - 25 : 'A' - 'Z' (10 to 35)
	// 26 - 51 : 'a' - 'z' (10 to 35)
	// 52 - 61 : '0' - '9' ( 0 to  9)
	char debugstr[4] = {'\'', '\0', '\'', '\0'};
	char buf[2];
	char* rem;
	buf[1] = '\0';
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 26; i++)
		{
			int idx = i + j * 26;
			int ord = i + 10;
			debugstr[1] = buf[0] = base64::values[idx];
			for (int base = 2; base <= ord; base++)
			{
				ASSERT_EQ(0, strtoul(buf, &rem, base))
					<< debugstr << " (ordinal " << ord
					<< ") should fail at strtoul with base == " << base << ".";
				ASSERT_EQ(&buf[0], rem)
					<< debugstr << " (ordinal " << ord
					<< ") should fail at strtoul with base == " << base << ".";
			}
			for (int base = ord + 1; base < 36; base++)
			{
				ASSERT_EQ(static_cast<unsigned long>(ord), strtoul(buf, &rem, base))
					<< debugstr << " (ordinal " << ord
					<< ") should succeed and match given ordinal at strtoul with base == "
					<< base << ".";
				ASSERT_EQ(&buf[1], rem)
					<< debugstr << " (ordinal " << ord
					<< ") should succeed at strtoul with base == " << base << ".";
			}
		}
	}
	for (int i = 0; i < 10; i++)
	{
		int ord = i;
		debugstr[1] = buf[0] = base64::values[52 + i];
		ASSERT_EQ(static_cast<unsigned long>(ord), strtoul(buf, &rem, 10))
			<< debugstr << " (ordinal " << ord
			<< ") should succeed and match given ordinal at strtoul with base == 10.";
		ASSERT_EQ(&buf[1], rem)
			<< debugstr << " (ordinal " << ord
			<< ") should succeed at strtoul with base == 10.";
	}
}

TEST(Base64Tests, Base64ValueTypes)
{
	//  0 - 25 : 'A' - 'Z' (upper)
	// 26 - 51 : 'a' - 'z' (lower)
	for (int i = 0; i < 26; i++)
	{
		EXPECT_TRUE(isupper(base64::values[i]))
			<< "Base64 character at " << i << " should be upper "
			<< "(changing locale to C may make the test pass).";
	}
	for (int i = 26; i < 52; i++)
	{
		EXPECT_TRUE(islower(base64::values[i]))
			<< "Base64 character at " << i << " should be upper "
			<< "(changing locale to C may make the test pass).";
	}
}

TEST(Base64Tests, Base64Indices)
{
	// base64::toindex should return index of corresponding character
	for (int i = 0; i < 64; i++)
	{
		EXPECT_EQ(i, base64::toindex(base64::values[i]))
			<< "Base 64 character at " << i << " did not converted back to its index.";
	}
}

TEST(Base64Tests, Base64Transformations)
{
	static const char base64_chars[12] =
	{
		'a', 'c', 'b',
		'C', 'A', 'B',
		'0', '1', '2',
		'+', '/', ':',
	};
	static const char base64_indices[12] =
	{
		26, 28, 27,
		2,  0,  1,
		52, 53, 54,
		62, 63, base64::invalid_index,
	};
	char buf1[12], buf2[12];
	// indices to chars
	memcpy(buf1, base64_indices, 12);
	strings::nosequences<base64::transform_to_b64>::copy_raw(buf2, buf1, 11);
	buf2[11] = base64_chars[11];
	EXPECT_EQ(0, memcmp(buf2, base64_chars, 12));
	// chars to indices
	strings::nosequences<base64::transform_from_b64>::copy_raw(buf1, buf2, 12);
	EXPECT_EQ(0, memcmp(buf1, base64_indices, 12));
}

#endif
