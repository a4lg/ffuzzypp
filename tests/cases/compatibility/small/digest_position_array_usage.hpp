/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/compatibility/small/digest_position_array_usage.hpp
	Tests for position array-based digest class (basic usage)

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
#ifndef FFUZZYPP_TESTCASES_COMPATIBILITY_SMALL_DIGEST_POSITION_ARRAY_USAGE_HPP
#define FFUZZYPP_TESTCASES_COMPATIBILITY_SMALL_DIGEST_POSITION_ARRAY_USAGE_HPP

#include <functional>
#include <string>

#ifndef FFUZZYPP_DISABLE_POSITION_ARRAY

template <typename T>
class DigestPositionArrayUsageTestsGeneric : public ::testing::Test {};

template <bool IsNonRAAvailable>
struct DigestPositionArrayUsageTestTypes_impl
{
	typedef ::testing::Types<digest_position_array_t> type;
};

template <>
struct DigestPositionArrayUsageTestTypes_impl<true>
{
	typedef ::testing::Types<
		digest_position_array_t,
		digest_position_array_non_ra_t
	> type;
};

typedef typename DigestPositionArrayUsageTestTypes_impl<
	digest_position_array_params<false>::is_available
>::type DigestPositionArrayUsageTestTypes;

TYPED_TEST_CASE(DigestPositionArrayUsageTestsGeneric, DigestPositionArrayUsageTestTypes);

TYPED_TEST(DigestPositionArrayUsageTestsGeneric, BasicInitialization)
{
	TypeParam var1;
	TypeParam var2("3:A:");
	TypeParam var3(string("3:B:"));
	TypeParam var4(var3);
	var1 = var4;
}

TYPED_TEST(DigestPositionArrayUsageTestsGeneric, InitializationByDigest)
{
	digest<TypeParam::is_alphabet_restricted,  true,  true> d0("3:ABCDEFG:HIJKLMN");
	digest<TypeParam::is_alphabet_restricted,  true, false> d1(d0);
	digest<TypeParam::is_alphabet_restricted, false,  true> d2(d0);
	digest<TypeParam::is_alphabet_restricted, false, false> d3(d0);
	TypeParam var0(d0); TypeParam::construct(var0, d0);
	TypeParam var1(d1); TypeParam::construct(var1, d1);
	TypeParam var2(d2); TypeParam::construct(var2, d2);
	TypeParam var3(d3); TypeParam::construct(var3, d3);
}

TYPED_TEST(DigestPositionArrayUsageTestsGeneric, ComparisonEq)
{
	digest<TypeParam::is_alphabet_restricted,  true, true> digest1("3:ABCDEFG:HIJKLMN");
	digest<TypeParam::is_alphabet_restricted, false, true> digest2(digest1);
	TypeParam parray("3:HIJKLMN:ABCDEFG");
	/*
		Note:
		compare<Version> in dependent type needs `template` keyword first.
		So, digest_position_array_t is tested separately on ComparisonNonTemplate.
	*/
	// compare
	EXPECT_EQ(0, TypeParam::compare(digest1, parray));
	EXPECT_EQ(0, TypeParam::compare(digest2, parray));
	EXPECT_EQ(0, TypeParam::compare(parray, digest1));
	EXPECT_EQ(0, TypeParam::compare(parray, digest2));
	EXPECT_EQ(0, TypeParam::template compare<comparison_version::latest>(digest1, parray));
	EXPECT_EQ(0, TypeParam::template compare<comparison_version::latest>(digest2, parray));
	EXPECT_EQ(0, TypeParam::template compare<comparison_version::latest>(parray, digest1));
	EXPECT_EQ(0, TypeParam::template compare<comparison_version::latest>(parray, digest2));
	EXPECT_EQ(0, TypeParam::template compare<comparison_version::v2_9>(digest1, parray));
	EXPECT_EQ(0, TypeParam::template compare<comparison_version::v2_9>(digest2, parray));
	EXPECT_EQ(0, TypeParam::template compare<comparison_version::v2_9>(parray, digest1));
	EXPECT_EQ(0, TypeParam::template compare<comparison_version::v2_9>(parray, digest2));
	EXPECT_EQ(0, parray.compare(digest1));
	EXPECT_EQ(0, parray.compare(digest2));
	EXPECT_EQ(0, parray.template compare<comparison_version::latest>(digest1));
	EXPECT_EQ(0, parray.template compare<comparison_version::latest>(digest2));
	EXPECT_EQ(0, parray.template compare<comparison_version::v2_9>(digest1));
	EXPECT_EQ(0, parray.template compare<comparison_version::v2_9>(digest2));
	// compare_near
	EXPECT_EQ(0, TypeParam::compare_near(digest1, parray));
	EXPECT_EQ(0, TypeParam::compare_near(digest2, parray));
	EXPECT_EQ(0, TypeParam::compare_near(parray, digest1));
	EXPECT_EQ(0, TypeParam::compare_near(parray, digest2));
	EXPECT_EQ(0, TypeParam::template compare_near<comparison_version::latest>(digest1, parray));
	EXPECT_EQ(0, TypeParam::template compare_near<comparison_version::latest>(digest2, parray));
	EXPECT_EQ(0, TypeParam::template compare_near<comparison_version::latest>(parray, digest1));
	EXPECT_EQ(0, TypeParam::template compare_near<comparison_version::latest>(parray, digest2));
	EXPECT_EQ(0, TypeParam::template compare_near<comparison_version::v2_9>(digest1, parray));
	EXPECT_EQ(0, TypeParam::template compare_near<comparison_version::v2_9>(digest2, parray));
	EXPECT_EQ(0, TypeParam::template compare_near<comparison_version::v2_9>(parray, digest1));
	EXPECT_EQ(0, TypeParam::template compare_near<comparison_version::v2_9>(parray, digest2));
	EXPECT_EQ(0, parray.compare_near(digest1));
	EXPECT_EQ(0, parray.compare_near(digest2));
	EXPECT_EQ(0, parray.template compare_near<comparison_version::latest>(digest1));
	EXPECT_EQ(0, parray.template compare_near<comparison_version::latest>(digest2));
	EXPECT_EQ(0, parray.template compare_near<comparison_version::v2_9>(digest1));
	EXPECT_EQ(0, parray.template compare_near<comparison_version::v2_9>(digest2));
	// compare_near_eq
	EXPECT_EQ(0, TypeParam::compare_near_eq(digest1, parray));
	EXPECT_EQ(0, TypeParam::compare_near_eq(digest2, parray));
	EXPECT_EQ(0, TypeParam::compare_near_eq(parray, digest1));
	EXPECT_EQ(0, TypeParam::compare_near_eq(parray, digest2));
	EXPECT_EQ(0, TypeParam::template compare_near_eq<comparison_version::latest>(digest1, parray));
	EXPECT_EQ(0, TypeParam::template compare_near_eq<comparison_version::latest>(digest2, parray));
	EXPECT_EQ(0, TypeParam::template compare_near_eq<comparison_version::latest>(parray, digest1));
	EXPECT_EQ(0, TypeParam::template compare_near_eq<comparison_version::latest>(parray, digest2));
	EXPECT_EQ(0, TypeParam::template compare_near_eq<comparison_version::v2_9>(digest1, parray));
	EXPECT_EQ(0, TypeParam::template compare_near_eq<comparison_version::v2_9>(digest2, parray));
	EXPECT_EQ(0, TypeParam::template compare_near_eq<comparison_version::v2_9>(parray, digest1));
	EXPECT_EQ(0, TypeParam::template compare_near_eq<comparison_version::v2_9>(parray, digest2));
	EXPECT_EQ(0, parray.compare_near_eq(digest1));
	EXPECT_EQ(0, parray.compare_near_eq(digest2));
	EXPECT_EQ(0, parray.template compare_near_eq<comparison_version::latest>(digest1));
	EXPECT_EQ(0, parray.template compare_near_eq<comparison_version::latest>(digest2));
	EXPECT_EQ(0, parray.template compare_near_eq<comparison_version::v2_9>(digest1));
	EXPECT_EQ(0, parray.template compare_near_eq<comparison_version::v2_9>(digest2));
	// compare_diff
	EXPECT_EQ(0, TypeParam::compare_diff(digest1, parray));
	EXPECT_EQ(0, TypeParam::compare_diff(digest2, parray));
	EXPECT_EQ(0, TypeParam::compare_diff(parray, digest1));
	EXPECT_EQ(0, TypeParam::compare_diff(parray, digest2));
	EXPECT_EQ(0, TypeParam::template compare_diff<comparison_version::latest>(digest1, parray));
	EXPECT_EQ(0, TypeParam::template compare_diff<comparison_version::latest>(digest2, parray));
	EXPECT_EQ(0, TypeParam::template compare_diff<comparison_version::latest>(parray, digest1));
	EXPECT_EQ(0, TypeParam::template compare_diff<comparison_version::latest>(parray, digest2));
	EXPECT_EQ(0, TypeParam::template compare_diff<comparison_version::v2_9>(digest1, parray));
	EXPECT_EQ(0, TypeParam::template compare_diff<comparison_version::v2_9>(digest2, parray));
	EXPECT_EQ(0, TypeParam::template compare_diff<comparison_version::v2_9>(parray, digest1));
	EXPECT_EQ(0, TypeParam::template compare_diff<comparison_version::v2_9>(parray, digest2));
	EXPECT_EQ(0, parray.compare_diff(digest1));
	EXPECT_EQ(0, parray.compare_diff(digest2));
	EXPECT_EQ(0, parray.template compare_diff<comparison_version::latest>(digest1));
	EXPECT_EQ(0, parray.template compare_diff<comparison_version::latest>(digest2));
	EXPECT_EQ(0, parray.template compare_diff<comparison_version::v2_9>(digest1));
	EXPECT_EQ(0, parray.template compare_diff<comparison_version::v2_9>(digest2));
	// compare_near_diff
	EXPECT_EQ(0, TypeParam::compare_near_diff(digest1, parray));
	EXPECT_EQ(0, TypeParam::compare_near_diff(digest2, parray));
	EXPECT_EQ(0, TypeParam::compare_near_diff(parray, digest1));
	EXPECT_EQ(0, TypeParam::compare_near_diff(parray, digest2));
	EXPECT_EQ(0, TypeParam::template compare_near_diff<comparison_version::latest>(digest1, parray));
	EXPECT_EQ(0, TypeParam::template compare_near_diff<comparison_version::latest>(digest2, parray));
	EXPECT_EQ(0, TypeParam::template compare_near_diff<comparison_version::latest>(parray, digest1));
	EXPECT_EQ(0, TypeParam::template compare_near_diff<comparison_version::latest>(parray, digest2));
	EXPECT_EQ(0, TypeParam::template compare_near_diff<comparison_version::v2_9>(digest1, parray));
	EXPECT_EQ(0, TypeParam::template compare_near_diff<comparison_version::v2_9>(digest2, parray));
	EXPECT_EQ(0, TypeParam::template compare_near_diff<comparison_version::v2_9>(parray, digest1));
	EXPECT_EQ(0, TypeParam::template compare_near_diff<comparison_version::v2_9>(parray, digest2));
	EXPECT_EQ(0, parray.compare_near_diff(digest1));
	EXPECT_EQ(0, parray.compare_near_diff(digest2));
	EXPECT_EQ(0, parray.template compare_near_diff<comparison_version::latest>(digest1));
	EXPECT_EQ(0, parray.template compare_near_diff<comparison_version::latest>(digest2));
	EXPECT_EQ(0, parray.template compare_near_diff<comparison_version::v2_9>(digest1));
	EXPECT_EQ(0, parray.template compare_near_diff<comparison_version::v2_9>(digest2));
	// compare_near_eq_diff
	EXPECT_EQ(0, TypeParam::compare_near_eq_diff(digest1, parray));
	EXPECT_EQ(0, TypeParam::compare_near_eq_diff(digest2, parray));
	EXPECT_EQ(0, TypeParam::compare_near_eq_diff(parray, digest1));
	EXPECT_EQ(0, TypeParam::compare_near_eq_diff(parray, digest2));
	EXPECT_EQ(0, TypeParam::template compare_near_eq_diff<comparison_version::latest>(digest1, parray));
	EXPECT_EQ(0, TypeParam::template compare_near_eq_diff<comparison_version::latest>(digest2, parray));
	EXPECT_EQ(0, TypeParam::template compare_near_eq_diff<comparison_version::latest>(parray, digest1));
	EXPECT_EQ(0, TypeParam::template compare_near_eq_diff<comparison_version::latest>(parray, digest2));
	EXPECT_EQ(0, TypeParam::template compare_near_eq_diff<comparison_version::v2_9>(digest1, parray));
	EXPECT_EQ(0, TypeParam::template compare_near_eq_diff<comparison_version::v2_9>(digest2, parray));
	EXPECT_EQ(0, TypeParam::template compare_near_eq_diff<comparison_version::v2_9>(parray, digest1));
	EXPECT_EQ(0, TypeParam::template compare_near_eq_diff<comparison_version::v2_9>(parray, digest2));
	EXPECT_EQ(0, parray.compare_near_eq_diff(digest1));
	EXPECT_EQ(0, parray.compare_near_eq_diff(digest2));
	EXPECT_EQ(0, parray.template compare_near_eq_diff<comparison_version::latest>(digest1));
	EXPECT_EQ(0, parray.template compare_near_eq_diff<comparison_version::latest>(digest2));
	EXPECT_EQ(0, parray.template compare_near_eq_diff<comparison_version::v2_9>(digest1));
	EXPECT_EQ(0, parray.template compare_near_eq_diff<comparison_version::v2_9>(digest2));
}

TYPED_TEST(DigestPositionArrayUsageTestsGeneric, ComparisonLt)
{
	TypeParam parray("6:ABCDEFG:HIJKLMN");
	digest<TypeParam::is_alphabet_restricted,  true, true> digest1("12:HIJKLMN:OPQRSTU");
	digest<TypeParam::is_alphabet_restricted, false, true> digest2(digest1);
	// compare_near_lt (position array < regular)
	EXPECT_EQ(28, TypeParam::compare_near_lt(parray, digest1));
	EXPECT_EQ(28, TypeParam::compare_near_lt(parray, digest2));
	EXPECT_EQ(28, TypeParam::template compare_near_lt<comparison_version::latest>(parray, digest1));
	EXPECT_EQ(28, TypeParam::template compare_near_lt<comparison_version::latest>(parray, digest2));
	EXPECT_EQ(28, TypeParam::template compare_near_lt<comparison_version::v2_9>(parray, digest1));
	EXPECT_EQ(28, TypeParam::template compare_near_lt<comparison_version::v2_9>(parray, digest2));
	// compare_near_lt
	EXPECT_EQ(28, parray.compare_near_lt(digest1));
	EXPECT_EQ(28, parray.compare_near_lt(digest2));
}

TYPED_TEST(DigestPositionArrayUsageTestsGeneric, ComparisonGt)
{
	digest<TypeParam::is_alphabet_restricted,  true, true> digest1("6:ABCDEFG:HIJKLMN");
	digest<TypeParam::is_alphabet_restricted, false, true> digest2(digest1);
	TypeParam parray("12:HIJKLMN:OPQRSTU");
	// compare_near_lt (regular < position_array)
	EXPECT_EQ(28, TypeParam::compare_near_lt(digest1, parray));
	EXPECT_EQ(28, TypeParam::compare_near_lt(digest2, parray));
	EXPECT_EQ(28, TypeParam::template compare_near_lt<comparison_version::latest>(digest1, parray));
	EXPECT_EQ(28, TypeParam::template compare_near_lt<comparison_version::latest>(digest2, parray));
	EXPECT_EQ(28, TypeParam::template compare_near_lt<comparison_version::v2_9>(digest1, parray));
	EXPECT_EQ(28, TypeParam::template compare_near_lt<comparison_version::v2_9>(digest2, parray));
	// compare_near_gt
	EXPECT_EQ(28, parray.compare_near_gt(digest1));
	EXPECT_EQ(28, parray.compare_near_gt(digest2));
}

TYPED_TEST(DigestPositionArrayUsageTestsGeneric, ComparisonIdentical)
{
	digest<TypeParam::is_alphabet_restricted,  true, true> digest1("3:ABCDEFG:HIJKLMN");
	digest<TypeParam::is_alphabet_restricted, false, true> digest2(digest1);
	TypeParam parray(digest1);
	// compare (identical digest but different types)
	EXPECT_EQ(100, TypeParam::compare(digest1, parray));
	EXPECT_EQ(100, TypeParam::compare(digest2, parray));
	EXPECT_EQ(100, TypeParam::compare(parray, digest1));
	EXPECT_EQ(100, TypeParam::compare(parray, digest2));
	EXPECT_EQ(100, TypeParam::template compare<comparison_version::latest>(digest1, parray));
	EXPECT_EQ(100, TypeParam::template compare<comparison_version::latest>(digest2, parray));
	EXPECT_EQ(100, TypeParam::template compare<comparison_version::latest>(parray, digest1));
	EXPECT_EQ(100, TypeParam::template compare<comparison_version::latest>(parray, digest2));
	EXPECT_EQ( 14, TypeParam::template compare<comparison_version::v2_9>(digest1, parray));
	EXPECT_EQ( 14, TypeParam::template compare<comparison_version::v2_9>(digest2, parray));
	EXPECT_EQ( 14, TypeParam::template compare<comparison_version::v2_9>(parray, digest1));
	EXPECT_EQ( 14, TypeParam::template compare<comparison_version::v2_9>(parray, digest2));
	EXPECT_EQ(100, parray.compare(digest1));
	EXPECT_EQ(100, parray.compare(digest2));
	EXPECT_EQ(100, parray.template compare<comparison_version::latest>(digest1));
	EXPECT_EQ(100, parray.template compare<comparison_version::latest>(digest2));
	EXPECT_EQ( 14, parray.template compare<comparison_version::v2_9>(digest1));
	EXPECT_EQ( 14, parray.template compare<comparison_version::v2_9>(digest2));
}


TEST(DigestPositionArrayUsageTests, ComparisonNonTemplate)
{
	digest_ra_t      digest1("3:ABCDEFG:HIJKLMN");
	digest_ra_long_t digest2(digest1);
	digest_position_array_t parray("3:HIJKLMN:ABCDEFG");
	// compare
	EXPECT_EQ(0, digest_position_array_t::compare(digest1, parray));
	EXPECT_EQ(0, digest_position_array_t::compare(digest2, parray));
	EXPECT_EQ(0, digest_position_array_t::compare(parray, digest1));
	EXPECT_EQ(0, digest_position_array_t::compare(parray, digest2));
	EXPECT_EQ(0, digest_position_array_t::compare<comparison_version::latest>(digest1, parray));
	EXPECT_EQ(0, digest_position_array_t::compare<comparison_version::latest>(digest2, parray));
	EXPECT_EQ(0, digest_position_array_t::compare<comparison_version::latest>(parray, digest1));
	EXPECT_EQ(0, digest_position_array_t::compare<comparison_version::latest>(parray, digest2));
	EXPECT_EQ(0, digest_position_array_t::compare<comparison_version::v2_9>(digest1, parray));
	EXPECT_EQ(0, digest_position_array_t::compare<comparison_version::v2_9>(digest2, parray));
	EXPECT_EQ(0, digest_position_array_t::compare<comparison_version::v2_9>(parray, digest1));
	EXPECT_EQ(0, digest_position_array_t::compare<comparison_version::v2_9>(parray, digest2));
	EXPECT_EQ(0, parray.compare(digest1));
	EXPECT_EQ(0, parray.compare(digest2));
	EXPECT_EQ(0, parray.compare<comparison_version::latest>(digest1));
	EXPECT_EQ(0, parray.compare<comparison_version::latest>(digest2));
	EXPECT_EQ(0, parray.compare<comparison_version::v2_9>(digest1));
	EXPECT_EQ(0, parray.compare<comparison_version::v2_9>(digest2));
	// compare_near
	EXPECT_EQ(0, digest_position_array_t::compare_near(digest1, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_near(digest2, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_near(parray, digest1));
	EXPECT_EQ(0, digest_position_array_t::compare_near(parray, digest2));
	EXPECT_EQ(0, digest_position_array_t::compare_near<comparison_version::latest>(digest1, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_near<comparison_version::latest>(digest2, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_near<comparison_version::latest>(parray, digest1));
	EXPECT_EQ(0, digest_position_array_t::compare_near<comparison_version::latest>(parray, digest2));
	EXPECT_EQ(0, digest_position_array_t::compare_near<comparison_version::v2_9>(digest1, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_near<comparison_version::v2_9>(digest2, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_near<comparison_version::v2_9>(parray, digest1));
	EXPECT_EQ(0, digest_position_array_t::compare_near<comparison_version::v2_9>(parray, digest2));
	EXPECT_EQ(0, parray.compare_near(digest1));
	EXPECT_EQ(0, parray.compare_near(digest2));
	EXPECT_EQ(0, parray.compare_near<comparison_version::latest>(digest1));
	EXPECT_EQ(0, parray.compare_near<comparison_version::latest>(digest2));
	EXPECT_EQ(0, parray.compare_near<comparison_version::v2_9>(digest1));
	EXPECT_EQ(0, parray.compare_near<comparison_version::v2_9>(digest2));
	// compare_near_eq
	EXPECT_EQ(0, digest_position_array_t::compare_near_eq(digest1, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_near_eq(digest2, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_near_eq(parray, digest1));
	EXPECT_EQ(0, digest_position_array_t::compare_near_eq(parray, digest2));
	EXPECT_EQ(0, digest_position_array_t::compare_near_eq<comparison_version::latest>(digest1, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_near_eq<comparison_version::latest>(digest2, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_near_eq<comparison_version::latest>(parray, digest1));
	EXPECT_EQ(0, digest_position_array_t::compare_near_eq<comparison_version::latest>(parray, digest2));
	EXPECT_EQ(0, digest_position_array_t::compare_near_eq<comparison_version::v2_9>(digest1, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_near_eq<comparison_version::v2_9>(digest2, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_near_eq<comparison_version::v2_9>(parray, digest1));
	EXPECT_EQ(0, digest_position_array_t::compare_near_eq<comparison_version::v2_9>(parray, digest2));
	EXPECT_EQ(0, parray.compare_near_eq(digest1));
	EXPECT_EQ(0, parray.compare_near_eq(digest2));
	EXPECT_EQ(0, parray.compare_near_eq<comparison_version::latest>(digest1));
	EXPECT_EQ(0, parray.compare_near_eq<comparison_version::latest>(digest2));
	EXPECT_EQ(0, parray.compare_near_eq<comparison_version::v2_9>(digest1));
	EXPECT_EQ(0, parray.compare_near_eq<comparison_version::v2_9>(digest2));
	// compare_diff
	EXPECT_EQ(0, digest_position_array_t::compare_diff(digest1, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_diff(digest2, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_diff(parray, digest1));
	EXPECT_EQ(0, digest_position_array_t::compare_diff(parray, digest2));
	EXPECT_EQ(0, digest_position_array_t::compare_diff<comparison_version::latest>(digest1, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_diff<comparison_version::latest>(digest2, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_diff<comparison_version::latest>(parray, digest1));
	EXPECT_EQ(0, digest_position_array_t::compare_diff<comparison_version::latest>(parray, digest2));
	EXPECT_EQ(0, digest_position_array_t::compare_diff<comparison_version::v2_9>(digest1, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_diff<comparison_version::v2_9>(digest2, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_diff<comparison_version::v2_9>(parray, digest1));
	EXPECT_EQ(0, digest_position_array_t::compare_diff<comparison_version::v2_9>(parray, digest2));
	EXPECT_EQ(0, parray.compare_diff(digest1));
	EXPECT_EQ(0, parray.compare_diff(digest2));
	EXPECT_EQ(0, parray.compare_diff<comparison_version::latest>(digest1));
	EXPECT_EQ(0, parray.compare_diff<comparison_version::latest>(digest2));
	EXPECT_EQ(0, parray.compare_diff<comparison_version::v2_9>(digest1));
	EXPECT_EQ(0, parray.compare_diff<comparison_version::v2_9>(digest2));
	// compare_near_diff
	EXPECT_EQ(0, digest_position_array_t::compare_near_diff(digest1, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_near_diff(digest2, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_near_diff(parray, digest1));
	EXPECT_EQ(0, digest_position_array_t::compare_near_diff(parray, digest2));
	EXPECT_EQ(0, digest_position_array_t::compare_near_diff<comparison_version::latest>(digest1, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_near_diff<comparison_version::latest>(digest2, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_near_diff<comparison_version::latest>(parray, digest1));
	EXPECT_EQ(0, digest_position_array_t::compare_near_diff<comparison_version::latest>(parray, digest2));
	EXPECT_EQ(0, digest_position_array_t::compare_near_diff<comparison_version::v2_9>(digest1, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_near_diff<comparison_version::v2_9>(digest2, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_near_diff<comparison_version::v2_9>(parray, digest1));
	EXPECT_EQ(0, digest_position_array_t::compare_near_diff<comparison_version::v2_9>(parray, digest2));
	EXPECT_EQ(0, parray.compare_near_diff(digest1));
	EXPECT_EQ(0, parray.compare_near_diff(digest2));
	EXPECT_EQ(0, parray.compare_near_diff<comparison_version::latest>(digest1));
	EXPECT_EQ(0, parray.compare_near_diff<comparison_version::latest>(digest2));
	EXPECT_EQ(0, parray.compare_near_diff<comparison_version::v2_9>(digest1));
	EXPECT_EQ(0, parray.compare_near_diff<comparison_version::v2_9>(digest2));
	// compare_near_eq_diff
	EXPECT_EQ(0, digest_position_array_t::compare_near_eq_diff(digest1, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_near_eq_diff(digest2, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_near_eq_diff(parray, digest1));
	EXPECT_EQ(0, digest_position_array_t::compare_near_eq_diff(parray, digest2));
	EXPECT_EQ(0, digest_position_array_t::compare_near_eq_diff<comparison_version::latest>(digest1, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_near_eq_diff<comparison_version::latest>(digest2, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_near_eq_diff<comparison_version::latest>(parray, digest1));
	EXPECT_EQ(0, digest_position_array_t::compare_near_eq_diff<comparison_version::latest>(parray, digest2));
	EXPECT_EQ(0, digest_position_array_t::compare_near_eq_diff<comparison_version::v2_9>(digest1, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_near_eq_diff<comparison_version::v2_9>(digest2, parray));
	EXPECT_EQ(0, digest_position_array_t::compare_near_eq_diff<comparison_version::v2_9>(parray, digest1));
	EXPECT_EQ(0, digest_position_array_t::compare_near_eq_diff<comparison_version::v2_9>(parray, digest2));
	EXPECT_EQ(0, parray.compare_near_eq_diff(digest1));
	EXPECT_EQ(0, parray.compare_near_eq_diff(digest2));
	EXPECT_EQ(0, parray.compare_near_eq_diff<comparison_version::latest>(digest1));
	EXPECT_EQ(0, parray.compare_near_eq_diff<comparison_version::latest>(digest2));
	EXPECT_EQ(0, parray.compare_near_eq_diff<comparison_version::v2_9>(digest1));
	EXPECT_EQ(0, parray.compare_near_eq_diff<comparison_version::v2_9>(digest2));

	// preparation: compare_near_lt
	parray = digest_position_array_t("6:ABCDEFG:HIJKLMN");
	digest1 = digest_ra_t("12:HIJKLMN:OPQRSTU");
	digest2 = digest_ra_long_t("12:HIJKLMN:OPQRSTU");
	// compare_near_lt (position array < regular)
	EXPECT_EQ(28, digest_position_array_t::compare_near_lt(parray, digest1));
	EXPECT_EQ(28, digest_position_array_t::compare_near_lt(parray, digest2));
	EXPECT_EQ(28, digest_position_array_t::compare_near_lt<comparison_version::latest>(parray, digest1));
	EXPECT_EQ(28, digest_position_array_t::compare_near_lt<comparison_version::latest>(parray, digest2));
	EXPECT_EQ(28, digest_position_array_t::compare_near_lt<comparison_version::v2_9>(parray, digest1));
	EXPECT_EQ(28, digest_position_array_t::compare_near_lt<comparison_version::v2_9>(parray, digest2));
	// compare_near_lt
	EXPECT_EQ(28, parray.compare_near_lt(digest1));
	EXPECT_EQ(28, parray.compare_near_lt(digest2));

	// preparation: compare_near_gt
	digest1 = digest_ra_t("6:ABCDEFG:HIJKLMN");
	digest2 = digest_ra_long_t("6:ABCDEFG:HIJKLMN");
	parray = digest_position_array_t("12:HIJKLMN:OPQRSTU");
	// compare_near_lt (regular < position_array)
	EXPECT_EQ(28, digest_position_array_t::compare_near_lt(digest1, parray));
	EXPECT_EQ(28, digest_position_array_t::compare_near_lt(digest2, parray));
	EXPECT_EQ(28, digest_position_array_t::compare_near_lt<comparison_version::latest>(digest1, parray));
	EXPECT_EQ(28, digest_position_array_t::compare_near_lt<comparison_version::latest>(digest2, parray));
	EXPECT_EQ(28, digest_position_array_t::compare_near_lt<comparison_version::v2_9>(digest1, parray));
	EXPECT_EQ(28, digest_position_array_t::compare_near_lt<comparison_version::v2_9>(digest2, parray));
	// compare_near_gt
	EXPECT_EQ(28, parray.compare_near_gt(digest1));
	EXPECT_EQ(28, parray.compare_near_gt(digest2));

	// compare (identical digest but different types)
	digest1 = digest_ra_t("3:ABCDEFG:HIJKLMN");
	digest2 = digest_ra_long_t("3:ABCDEFG:HIJKLMN");
	parray = digest_position_array_t(digest1);
	EXPECT_EQ(100, digest_position_array_t::compare(digest1, parray));
	EXPECT_EQ(100, digest_position_array_t::compare(digest2, parray));
	EXPECT_EQ(100, digest_position_array_t::compare(parray, digest1));
	EXPECT_EQ(100, digest_position_array_t::compare(parray, digest2));
	EXPECT_EQ(100, digest_position_array_t::compare<comparison_version::latest>(digest1, parray));
	EXPECT_EQ(100, digest_position_array_t::compare<comparison_version::latest>(digest2, parray));
	EXPECT_EQ(100, digest_position_array_t::compare<comparison_version::latest>(parray, digest1));
	EXPECT_EQ(100, digest_position_array_t::compare<comparison_version::latest>(parray, digest2));
	EXPECT_EQ( 14, digest_position_array_t::compare<comparison_version::v2_9>(digest1, parray));
	EXPECT_EQ( 14, digest_position_array_t::compare<comparison_version::v2_9>(digest2, parray));
	EXPECT_EQ( 14, digest_position_array_t::compare<comparison_version::v2_9>(parray, digest1));
	EXPECT_EQ( 14, digest_position_array_t::compare<comparison_version::v2_9>(parray, digest2));
	EXPECT_EQ(100, parray.compare(digest1));
	EXPECT_EQ(100, parray.compare(digest2));
	EXPECT_EQ(100, parray.compare<comparison_version::latest>(digest1));
	EXPECT_EQ(100, parray.compare<comparison_version::latest>(digest2));
	EXPECT_EQ( 14, parray.compare<comparison_version::v2_9>(digest1));
	EXPECT_EQ( 14, parray.compare<comparison_version::v2_9>(digest2));
}

#endif
#endif
