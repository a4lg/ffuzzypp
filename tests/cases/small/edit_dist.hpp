/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/small/edit_dist.hpp
	Edit distance tests

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
#ifndef FFUZZYPP_TESTCASES_SMALL_EDIT_DIST_HPP
#define FFUZZYPP_TESTCASES_SMALL_EDIT_DIST_HPP

#include <cstddef>
#include <string>


class EditDistanceFastTests : public ::testing::Test
{
public:
	static const size_t max_size = 32;
	typedef strings::edit_dist_fast<unsigned, max_size> test_type;
};

TEST_F(EditDistanceFastTests, BothNull)
{
	EXPECT_EQ(0, test_type::cost(nullptr, 0, nullptr, 0));
}

TEST_F(EditDistanceFastTests, AdditionFromNull)
{
	EXPECT_EQ(1, test_type::cost(nullptr, 0, "A", 1));
	for (unsigned i = 2; i <= max_size; i++)
	{
		string str(i, 'A');
		EXPECT_EQ(i, test_type::cost(nullptr, 0, str.data(), str.size()))
			<< "edit_dist addition (from null) test failed on length " << i << ".";
	}
}

TEST_F(EditDistanceFastTests, RemovalToNull)
{
	EXPECT_EQ(1, test_type::cost("A", 1, nullptr, 0));
	for (unsigned i = 2; i <= max_size; i++)
	{
		string str(i, 'A');
		EXPECT_EQ(i, test_type::cost(str.data(), str.size(), nullptr, 0))
			<< "edit_dist removal (to null) test failed on length " << i << ".";
	}
}


template <typename T>
class EditDistanceFastNonemptyTests : public ::testing::Test {};

typedef ::testing::Types<
	strings::edit_dist_fast<unsigned, EditDistanceFastTests::max_size>,
	strings::edit_dist_nonempty_fast<unsigned, EditDistanceFastTests::max_size>
> EditDistanceFastNonemptyTypes;
TYPED_TEST_CASE(EditDistanceFastNonemptyTests, EditDistanceFastNonemptyTypes);

TYPED_TEST(EditDistanceFastNonemptyTests, Addition)
{
	typedef TypeParam test_type;
	static const size_t max_size = test_type::max_size;
	for (unsigned l1 = 1; l1 <= max_size; l1++)
	{
		string str1(l1, 'A');
		for (unsigned l2 = l1; l2 <= max_size; l2++)
		{
			string str2(l2, 'A');
			EXPECT_EQ(l2 - l1, test_type::cost(str1.data(), str1.size(), str2.data(), str2.size()))
				<< "edit_dist addition test failed on length from " << l1 << " to " << l2 << ".";
		}
	}
}

TYPED_TEST(EditDistanceFastNonemptyTests, Removal)
{
	typedef TypeParam test_type;
	static const size_t max_size = test_type::max_size;
	for (unsigned l2 = 1; l2 <= max_size; l2++)
	{
		string str2(l2, 'A');
		for (unsigned l1 = l2; l1 <= max_size; l1++)
		{
			string str1(l1, 'A');
			EXPECT_EQ(l1 - l2, test_type::cost(str1.data(), str1.size(), str2.data(), str2.size()))
				<< "edit_dist removal test failed on length from " << l1 << " to " << l2 << ".";
		}
	}
}

TYPED_TEST(EditDistanceFastNonemptyTests, ReplaceOnIdenticalLength)
{
	typedef TypeParam test_type;
	static const size_t max_size = test_type::max_size;
	EXPECT_EQ(2, test_type::cost("A", 1, "B", 1));
	for (unsigned i = 2; i <= max_size; i++)
	{
		string strA(i, 'A');
		string strB(i, 'B');
		EXPECT_EQ(i * 2, test_type::cost(strA.data(), strA.size(), strB.data(), strB.size()))
			<< "edit_dist replace test failed on length " << i << ".";
	}
}

TYPED_TEST(EditDistanceFastNonemptyTests, IdenticalStrings)
{
	typedef TypeParam test_type;
	static const size_t max_size = test_type::max_size;
	for (unsigned i = 1; i <= max_size; i++)
	{
		string strA(i, 'A');
		EXPECT_EQ(0, test_type::cost(strA.data(), strA.size(), strA.data(), strA.size()))
			<< "edit_dist identical strings test failed on length " << i << ".";
	}
}

TYPED_TEST(EditDistanceFastNonemptyTests, CompletelyDifferentStrings)
{
	typedef TypeParam test_type;
	static const size_t max_size = test_type::max_size;
	for (unsigned i = 1; i <= max_size; i++)
	{
		string strA(i, 'A');
		for (unsigned j = 1; j <= max_size; j++)
		{
			string strB(j, 'B');
			EXPECT_EQ(i + j, test_type::cost(strA.data(), strA.size(), strB.data(), strB.size()))
				<< "edit_dist completely different strings test failed on length from "
				<< i << " to " << j << ".";
		}
	}
}

TYPED_TEST(EditDistanceFastNonemptyTests, OnRealExamples)
{
	typedef TypeParam test_type;
	EXPECT_EQ(5, test_type::cost("kitten", 6, "sitting", 7));
	EXPECT_EQ(7, test_type::cost("A character sequence", 20, "Other character sequence!", 25));
}

#endif
