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


class EditDistanceTestParam
{
private:
	EditDistanceTestParam(void) = delete;
	EditDistanceTestParam(const EditDistanceTestParam&) = delete;
public:
	static const size_t MaxSize = 32;
};


template <typename T>
class EditDistanceTests : public ::testing::Test {};

typedef ::testing::Types<
	strings::edit_dist_fast<unsigned, EditDistanceTestParam::MaxSize>,
	strings::edit_dist_dp<unsigned, EditDistanceTestParam::MaxSize>,
	strings::edit_dist_bitparallel_wrapper<unsigned, unsigned long, '0', '9', EditDistanceTestParam::MaxSize>
> EditDistanceTypes;
TYPED_TEST_CASE(EditDistanceTests, EditDistanceTypes);

TYPED_TEST(EditDistanceTests, BothNull)
{
	EXPECT_EQ(0, TypeParam::cost(nullptr, 0, nullptr, 0));
}

TYPED_TEST(EditDistanceTests, AdditionFromNull)
{
	EXPECT_EQ(1, TypeParam::cost(nullptr, 0, "0", 1));
	for (unsigned i = 2; i <= TypeParam::max_size; i++)
	{
		string str(i, '0');
		EXPECT_EQ(i, TypeParam::cost(nullptr, 0, str.data(), str.size()))
			<< "edit_dist addition (from null) test failed on length " << i << ".";
	}
}

TYPED_TEST(EditDistanceTests, RemovalToNull)
{
	EXPECT_EQ(1, TypeParam::cost("0", 1, nullptr, 0));
	for (unsigned i = 2; i <= TypeParam::max_size; i++)
	{
		string str(i, '0');
		EXPECT_EQ(i, TypeParam::cost(str.data(), str.size(), nullptr, 0))
			<< "edit_dist removal (to null) test failed on length " << i << ".";
	}
}


template <typename T>
class EditDistanceNonemptyTestsSimple : public ::testing::Test {};

typedef ::testing::Types<
	strings::edit_dist_fast<unsigned, EditDistanceTestParam::MaxSize>,
	strings::edit_dist_nonempty_fast<unsigned, EditDistanceTestParam::MaxSize>,
	strings::edit_dist_dp<unsigned, EditDistanceTestParam::MaxSize>,
	strings::edit_dist_nonempty_dp<unsigned, EditDistanceTestParam::MaxSize>,
	strings::edit_dist_bitparallel_wrapper<unsigned, unsigned long, '0', '9', EditDistanceTestParam::MaxSize>,
	strings::edit_dist_nonempty_bitparallel_wrapper<unsigned, unsigned long, '0', '9', EditDistanceTestParam::MaxSize>
> EditDistanceNonemptyTypesSimple;
TYPED_TEST_CASE(EditDistanceNonemptyTestsSimple, EditDistanceNonemptyTypesSimple);

TYPED_TEST(EditDistanceNonemptyTestsSimple, Addition)
{
	for (unsigned l1 = 1; l1 <= TypeParam::max_size; l1++)
	{
		string str1(l1, '0');
		for (unsigned l2 = l1; l2 <= TypeParam::max_size; l2++)
		{
			string str2(l2, '0');
			EXPECT_EQ(l2 - l1, TypeParam::cost(str1.data(), str1.size(), str2.data(), str2.size()))
				<< "edit_dist addition test failed on length from " << l1 << " to " << l2 << ".";
		}
	}
}

TYPED_TEST(EditDistanceNonemptyTestsSimple, Removal)
{
	for (unsigned l2 = 1; l2 <= TypeParam::max_size; l2++)
	{
		string str2(l2, '0');
		for (unsigned l1 = l2; l1 <= TypeParam::max_size; l1++)
		{
			string str1(l1, '0');
			EXPECT_EQ(l1 - l2, TypeParam::cost(str1.data(), str1.size(), str2.data(), str2.size()))
				<< "edit_dist removal test failed on length from " << l1 << " to " << l2 << ".";
		}
	}
}

TYPED_TEST(EditDistanceNonemptyTestsSimple, ReplaceOnIdenticalLength)
{
	EXPECT_EQ(2, TypeParam::cost("0", 1, "1", 1));
	for (unsigned i = 2; i <= TypeParam::max_size; i++)
	{
		string strA(i, '0');
		string strB(i, '1');
		EXPECT_EQ(i * 2, TypeParam::cost(strA.data(), strA.size(), strB.data(), strB.size()))
			<< "edit_dist replace test failed on length " << i << ".";
	}
}

TYPED_TEST(EditDistanceNonemptyTestsSimple, IdenticalStrings)
{
	for (unsigned i = 1; i <= TypeParam::max_size; i++)
	{
		string strA(i, '0');
		EXPECT_EQ(0, TypeParam::cost(strA.data(), strA.size(), strA.data(), strA.size()))
			<< "edit_dist identical strings test failed on length " << i << ".";
	}
}

TYPED_TEST(EditDistanceNonemptyTestsSimple, CompletelyDifferentStrings)
{
	for (unsigned i = 1; i <= TypeParam::max_size; i++)
	{
		string strA(i, '0');
		for (unsigned j = 1; j <= TypeParam::max_size; j++)
		{
			string strB(j, '1');
			EXPECT_EQ(i + j, TypeParam::cost(strA.data(), strA.size(), strB.data(), strB.size()))
				<< "edit_dist completely different strings test failed on length from "
				<< i << " to " << j << ".";
		}
	}
}


template <typename T>
class EditDistanceNonemptyTestsComplex : public ::testing::Test {};

typedef ::testing::Types<
	strings::edit_dist_fast<unsigned, EditDistanceTestParam::MaxSize>,
	strings::edit_dist_nonempty_fast<unsigned, EditDistanceTestParam::MaxSize>,
	strings::edit_dist_dp<unsigned, EditDistanceTestParam::MaxSize>,
	strings::edit_dist_nonempty_dp<unsigned, EditDistanceTestParam::MaxSize>
> EditDistanceNonemptyTypesComplex;
TYPED_TEST_CASE(EditDistanceNonemptyTestsComplex, EditDistanceNonemptyTypesComplex);

TYPED_TEST(EditDistanceNonemptyTestsComplex, OnRealExamples)
{
	EXPECT_EQ(5, TypeParam::cost("kitten", 6, "sitting", 7));
	EXPECT_EQ(7, TypeParam::cost("A character sequence", 20, "Other character sequence!", 25));
}

#endif
