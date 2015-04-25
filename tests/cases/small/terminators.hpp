/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/small/terminators.hpp
	Tests for terminators

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
#ifndef FFUZZYPP_TESTCASES_SMALL_TERMINATORS_HPP
#define FFUZZYPP_TESTCASES_SMALL_TERMINATORS_HPP

#include <unordered_set>


class TerminatorTests : public ::testing::Test
{
public:
	template <char... Chars>
	static void TerminatorTestForOne(char ch, const unordered_set<char> expected_terms)
	{
		if (expected_terms.find(ch) != expected_terms.end())
			EXPECT_TRUE(strings::terminators<Chars...>::isterm(ch));
		else
			EXPECT_FALSE(strings::terminators<Chars...>::isterm(ch));
	}
};

TEST_F(TerminatorTests, NullTest)
{
	unordered_set<char> expected_terms;
	expected_terms.insert('\0');
	for (char i : ranges::inclusive<char>())
		TerminatorTestForOne<>(i, expected_terms);
}

TEST_F(TerminatorTests, Params1Test)
{
	unordered_set<char> expected_terms;
	expected_terms.insert('\0');
	expected_terms.insert('1');
	for (char i : ranges::inclusive<char>())
		TerminatorTestForOne<'1'>(i, expected_terms);
}

TEST_F(TerminatorTests, Params2Test)
{
	unordered_set<char> expected_terms;
	expected_terms.insert('\0');
	expected_terms.insert('2');
	expected_terms.insert('B');
	for (char i : ranges::inclusive<char>())
		TerminatorTestForOne<'2', 'B'>(i, expected_terms);
}

TEST_F(TerminatorTests, Params3Test)
{
	unordered_set<char> expected_terms;
	expected_terms.insert('\0');
	expected_terms.insert('3');
	expected_terms.insert('c');
	expected_terms.insert(',');
	for (char i : ranges::inclusive<char>())
		TerminatorTestForOne<'3', 'c', ','>(i, expected_terms);
}

TEST_F(TerminatorTests, Params3DupTest)
{
	unordered_set<char> expected_terms;
	expected_terms.insert('\0');
	expected_terms.insert('4');
	expected_terms.insert('D');
	for (char i : ranges::inclusive<char>())
		TerminatorTestForOne<'D', '4', 'D'>(i, expected_terms);
}

#endif
