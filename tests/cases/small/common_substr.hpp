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


class CommonSubstringTests : public ::testing::Test
{
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

	typedef strings::common_substr_fast<MaxSize, SubstringSizeToTest> test_type;
};
const CommonSubstringTests::known_entry CommonSubstringTests::KnownEntries[] =
{
	{"empty", "", "", false},
	{"identical length 1", "1", "1", false},
	{"identical length 2", "12", "12", false},
	{"identical length 3", "123", "123", false},
	{"identical length 4", "1234", "1234", false},
	{"identical length 5", "12345", "12345", false},
	{"identical length 6", "123456", "123456", false},
	{"identical length 7", "1234567", "1234567", true},
	{"identical length 8", "12345678", "12345678", true},
	{"identical length 9", "123456789", "123456789", true},
	{"identical length 10", "1234567890", "1234567890", true},
	{"different length 1", ".", ",", false},
	{"different length 2", "..", ",,", false},
	{"different length 3", "...", ",,,", false},
	{"different length 4", "....", ",,,,", false},
	{"different length 5", ".....", ",,,,,", false},
	{"different length 6", "......", ",,,,,,", false},
	{"different length 7", ".......", ",,,,,,,", false},
	{"different length 8", "........", ",,,,,,,,", false},
	{"different length 9", ".........", ",,,,,,,,,", false},
	{"different length 10", "..........", ",,,,,,,,,,", false},
	{"mostly identical length 7", "123456.", ",123456", false},
	{"mostly identical length 8", "1234567.", ",1234567", true},
	{"mostly identical length 9 A", "1234567..", ",,1234567", true},
	{"mostly identical length 9 B", ".1234567.", ",1234567,", true},
	{"mostly identical length 10 A", "1234567...", ",,,1234567", true},
	{"mostly identical length 10 B", ".1234567..", ",,1234567,", true},
	{"mostly identical length 10 C", "..1234567.", ",1234567,,", true},
};


TEST_F(CommonSubstringTests, MatchTest)
{
	for (const auto& entry : KnownEntries)
	{
		size_t l1 = strlen(entry.str1);
		size_t l2 = strlen(entry.str2);
		EXPECT_EQ(entry.has_common_substring,
			test_type::match(entry.str1, l1, entry.str2, l2))
			<< "match test failed on <" << entry.description << ">";
	}
}

TEST_F(CommonSubstringTests, MatchCommutativityTest)
{
	for (const auto& entry : KnownEntries)
	{
		size_t l1 = strlen(entry.str1);
		size_t l2 = strlen(entry.str2);
		EXPECT_EQ(
			test_type::match(entry.str1, l1, entry.str2, l2),
			test_type::match(entry.str2, l2, entry.str1, l1))
			<< "match commutativity test failed on <" << entry.description << ">";
	}
}

#endif
