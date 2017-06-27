/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/small/common_substr.hpp
	Common substring tests

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
#ifndef FFUZZYPP_TESTCASES_SMALL_COMMON_SUBSTR_HPP
#define FFUZZYPP_TESTCASES_SMALL_COMMON_SUBSTR_HPP

#include <cstring>


class CommonSubstringTestParams
{
private:
	CommonSubstringTestParams(void) = delete;
	CommonSubstringTestParams(const CommonSubstringTestParams&) = delete;
public:
	static const size_t MaxSize = 64;
	static const size_t SubstringSizeToTest = 7;
	struct known_entry
	{
		const char* description;
		const char* str1;
		const char* str2;
		bool has_common_substring;
	};
	static const known_entry KnownEntries[];
};

const CommonSubstringTestParams::known_entry CommonSubstringTestParams::KnownEntries[] =
{
	{"empty", "", "", false},
	{"identical length 1", "1", "1", false},
	{"identical length 2", "12", "12", false},
	{"identical length 3", "123", "123", false},
	{"identical length 4", "1234", "1234", false},
	{"identical length 5", "12345", "12345", false},
	{"identical length 6", "123456", "123456", false},
	{"identical length 7", "1234567", "1234567", true},
	{"identical length 8", "12345679", "12345678", true},
	{"identical length 9", "123456796", "123456796", true},
	{"identical length 10", "1234567962", "1234567962", true},
	{"different length 1", "8", "0", false},
	{"different length 2", "88", "00", false},
	{"different length 3", "888", "000", false},
	{"different length 4", "8888", "0000", false},
	{"different length 5", "88888", "00000", false},
	{"different length 6", "888888", "000000", false},
	{"different length 7", "8888888", "0000000", false},
	{"different length 8", "88888888", "00000000", false},
	{"different length 9", "888888888", "000000000", false},
	{"different length 10", "8888888888", "0000000000", false},
	{"mostly identical length 7", "1234568", "0123456", false},
	{"mostly identical length 8", "12345678", "01234567", true},
	{"mostly identical length 9 A", "123456788", "001234567", true},
	{"mostly identical length 9 B", "812345678", "012345670", true},
	{"mostly identical length 10 A", "1234567888", "0001234567", true},
	{"mostly identical length 10 B", "8123456788", "0012345670", true},
	{"mostly identical length 10 C", "8812345678", "0123456700", true},
};


template <typename T>
class CommonSubstringTests : public ::testing::Test {};

typedef ::testing::Types<
	strings::common_substr_fast<
		CommonSubstringTestParams::MaxSize,
		CommonSubstringTestParams::SubstringSizeToTest
	>,
	strings::common_substr_hasharray<
		CommonSubstringTestParams::MaxSize,
		CommonSubstringTestParams::SubstringSizeToTest
	>,
	strings::common_substr_bitparallel_wrapper<
		unsigned long long, '0', '9',
		CommonSubstringTestParams::MaxSize,
		CommonSubstringTestParams::SubstringSizeToTest
	>
> CommonSubstringTypes;
TYPED_TEST_CASE(CommonSubstringTests, CommonSubstringTypes);

TYPED_TEST(CommonSubstringTests, MatchTest)
{
	for (const auto& entry : CommonSubstringTestParams::KnownEntries)
	{
		size_t l1 = strlen(entry.str1);
		size_t l2 = strlen(entry.str2);
		EXPECT_EQ(entry.has_common_substring,
			TypeParam::match(entry.str1, l1, entry.str2, l2))
			<< "match test failed on <" << entry.description << ">";
	}
}

TYPED_TEST(CommonSubstringTests, MatchCommutativityTest)
{
	for (const auto& entry : CommonSubstringTestParams::KnownEntries)
	{
		size_t l1 = strlen(entry.str1);
		size_t l2 = strlen(entry.str2);
		EXPECT_EQ(
			TypeParam::match(entry.str1, l1, entry.str2, l2),
			TypeParam::match(entry.str2, l2, entry.str1, l1))
			<< "match commutativity test failed on <" << entry.description << ">";
	}
}

#endif
