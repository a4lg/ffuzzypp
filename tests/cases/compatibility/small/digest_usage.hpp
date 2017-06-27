/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/compatibility/small/digest_usage.hpp
	Tests for digest class (basic usage)

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
#ifndef FFUZZYPP_TESTCASES_COMPATIBILITY_SMALL_DIGEST_USAGE_HPP
#define FFUZZYPP_TESTCASES_COMPATIBILITY_SMALL_DIGEST_USAGE_HPP

#include <functional>
#include <string>


template <typename T>
class DigestUsageTestsGeneric : public ::testing::Test {};

typedef ::testing::Types<
	digest_t,
	digest_unorm_t,
	digest_long_t,
	digest_long_unorm_t,
	digest_ra_t,
	digest_ra_unorm_t,
	digest_ra_long_t,
	digest_ra_long_unorm_t
> DigestUsageTestsGenericTypes;
TYPED_TEST_CASE(DigestUsageTestsGeneric, DigestUsageTestsGenericTypes);

TYPED_TEST(DigestUsageTestsGeneric, BasicInitialization)
{
	TypeParam var1;
	TypeParam var2("3:A:");
	TypeParam var3(string("3:B:"));
	TypeParam var4(var3);
	var1 = var4;
}

TYPED_TEST(DigestUsageTestsGeneric, Pretty)
{
	char pbuffer[TypeParam::max_natural_width];
	TypeParam var1("3:ABC:DEF");
	TypeParam var2(string("3:012:345"));
	TypeParam var3(var2);
	EXPECT_EQ("3:ABC:DEF", var1.pretty());
	EXPECT_EQ("3:012:345", var2.pretty());
	EXPECT_EQ("3:012:345", var3.pretty());
	ASSERT_TRUE(var1.pretty_unsafe(pbuffer));
	EXPECT_STREQ("3:ABC:DEF", pbuffer);
	ASSERT_TRUE(var2.pretty_unsafe(pbuffer));
	EXPECT_STREQ("3:012:345", pbuffer);
	ASSERT_TRUE(var3.pretty_unsafe(pbuffer));
	EXPECT_STREQ("3:012:345", pbuffer);
	ASSERT_TRUE(var1.pretty(pbuffer, sizeof(pbuffer)));
	EXPECT_STREQ("3:ABC:DEF", pbuffer);
	ASSERT_TRUE(var2.pretty(pbuffer, sizeof(pbuffer)));
	EXPECT_STREQ("3:012:345", pbuffer);
	ASSERT_TRUE(var3.pretty(pbuffer, sizeof(pbuffer)));
	EXPECT_STREQ("3:012:345", pbuffer);
	/*
		"3:ABC:DEF" and "3:012:345" require 10 character-sized buffer
		(including terminating null character)
	*/
	EXPECT_FALSE(var1.pretty(pbuffer, 8));
	EXPECT_FALSE(var2.pretty(pbuffer, 8));
	EXPECT_FALSE(var3.pretty(pbuffer, 8));
	EXPECT_FALSE(var1.pretty(pbuffer, 9));
	EXPECT_FALSE(var2.pretty(pbuffer, 9));
	EXPECT_FALSE(var3.pretty(pbuffer, 9));
	EXPECT_TRUE(var1.pretty(pbuffer, 10));
	EXPECT_TRUE(var2.pretty(pbuffer, 10));
	EXPECT_TRUE(var3.pretty(pbuffer, 10));
}

TYPED_TEST(DigestUsageTestsGeneric, HashAndSwap)
{
	TypeParam value("3::");
	EXPECT_EQ(::std::hash<TypeParam>()(value), value.hash());
	TypeParam d0("3:012:345");
	TypeParam d1("3:ABC:DEF");
	::std::swap(d0, d1);
	EXPECT_EQ("3:ABC:DEF", d0.pretty());
	EXPECT_EQ("3:012:345", d1.pretty());
	decltype(d0)::swap(d0, d1);
	EXPECT_EQ("3:012:345", d0.pretty());
	EXPECT_EQ("3:ABC:DEF", d1.pretty());
}

TYPED_TEST(DigestUsageTestsGeneric, BasicComparison)
{
	TypeParam digest1("3:ABCDEFG:HIJKLMN");
	TypeParam digest2("3:HIJKLMN:ABCDEFG");
	/*
		Note:
		compare<Version> in dependent type needs `template` keyword first.
		So, digest_t is tested separately on BasicComparisonNonTemplate.
	*/
	EXPECT_EQ(0, TypeParam::compare(digest1, digest2));
	EXPECT_EQ(0, TypeParam::template compare<comparison_version::latest>(digest1, digest2));
	EXPECT_EQ(0, TypeParam::template compare<comparison_version::v2_9>(digest1, digest2));
	EXPECT_EQ(0, digest1.compare(digest2));
	EXPECT_EQ(0, digest1.template compare<comparison_version::latest>(digest2));
	EXPECT_EQ(0, digest1.template compare<comparison_version::v2_9>(digest2));
}


template <typename T>
class DigestUsageTestsGenericNormalized : public ::testing::Test {};

typedef ::testing::Types<
	digest_t,
	digest_long_t,
	digest_ra_t,
	digest_ra_long_t
> DigestUsageTestsGenericNormalizedTypes;
TYPED_TEST_CASE(DigestUsageTestsGenericNormalized, DigestUsageTestsGenericNormalizedTypes);

TYPED_TEST(DigestUsageTestsGenericNormalized, BasicComparisonEq)
{
	TypeParam digest1("3:ABCDEFG:HIJKLMN");
	TypeParam digest2("3:HIJKLMN:ABCDEFG");
	/*
		Note:
		compare<Version> in dependent type needs `template` keyword first.
		So, digest_t is tested separately on BasicComparisonNonTemplate.
	*/
	// compare_diff
	EXPECT_EQ(0, TypeParam::compare_diff(digest1, digest2));
	EXPECT_EQ(0, TypeParam::template compare_diff<comparison_version::latest>(digest1, digest2));
	EXPECT_EQ(0, TypeParam::template compare_diff<comparison_version::v2_9>(digest1, digest2));
	EXPECT_EQ(0, digest1.compare_diff(digest2));
	EXPECT_EQ(0, digest1.template compare_diff<comparison_version::latest>(digest2));
	EXPECT_EQ(0, digest1.template compare_diff<comparison_version::v2_9>(digest2));
	// compare_near
	EXPECT_EQ(0, TypeParam::compare_near(digest1, digest2));
	EXPECT_EQ(0, TypeParam::template compare_near<comparison_version::latest>(digest1, digest2));
	EXPECT_EQ(0, TypeParam::template compare_near<comparison_version::v2_9>(digest1, digest2));
	EXPECT_EQ(0, digest1.compare_near(digest2));
	EXPECT_EQ(0, digest1.template compare_near<comparison_version::latest>(digest2));
	EXPECT_EQ(0, digest1.template compare_near<comparison_version::v2_9>(digest2));
	// compare_near_diff
	EXPECT_EQ(0, TypeParam::compare_near_diff(digest1, digest2));
	EXPECT_EQ(0, TypeParam::template compare_near_diff<comparison_version::latest>(digest1, digest2));
	EXPECT_EQ(0, TypeParam::template compare_near_diff<comparison_version::v2_9>(digest1, digest2));
	EXPECT_EQ(0, digest1.compare_near_diff(digest2));
	EXPECT_EQ(0, digest1.template compare_near_diff<comparison_version::latest>(digest2));
	EXPECT_EQ(0, digest1.template compare_near_diff<comparison_version::v2_9>(digest2));
	// compare_near_eq
	EXPECT_EQ(0, TypeParam::compare_near_eq(digest1, digest2));
	EXPECT_EQ(0, TypeParam::template compare_near_eq<comparison_version::latest>(digest1, digest2));
	EXPECT_EQ(0, TypeParam::template compare_near_eq<comparison_version::v2_9>(digest1, digest2));
	EXPECT_EQ(0, digest1.compare_near_eq(digest2));
	EXPECT_EQ(0, digest1.template compare_near_eq<comparison_version::latest>(digest2));
	EXPECT_EQ(0, digest1.template compare_near_eq<comparison_version::v2_9>(digest2));
	// compare_near_eq_diff
	EXPECT_EQ(0, TypeParam::compare_near_eq_diff(digest1, digest2));
	EXPECT_EQ(0, TypeParam::template compare_near_eq_diff<comparison_version::latest>(digest1, digest2));
	EXPECT_EQ(0, TypeParam::template compare_near_eq_diff<comparison_version::v2_9>(digest1, digest2));
	EXPECT_EQ(0, digest1.compare_near_eq_diff(digest2));
	EXPECT_EQ(0, digest1.template compare_near_eq_diff<comparison_version::latest>(digest2));
	EXPECT_EQ(0, digest1.template compare_near_eq_diff<comparison_version::v2_9>(digest2));
	// compare_identical
	EXPECT_EQ(100, TypeParam::compare_identical(digest1));
	EXPECT_EQ(100, TypeParam::template compare_identical<comparison_version::latest>(digest1));
	EXPECT_EQ( 14, TypeParam::template compare_identical<comparison_version::v2_9>(digest1));
	EXPECT_EQ(100, digest1.compare_identical());
	EXPECT_EQ(100, digest1.template compare_identical<comparison_version::latest>());
	EXPECT_EQ( 14, digest1.template compare_identical<comparison_version::v2_9>());
}

TYPED_TEST(DigestUsageTestsGenericNormalized, BasicComparisonLt)
{
	TypeParam digest1("6:ABCDEFG:HIJKLMN");
	TypeParam digest2("12:HIJKLMN:OPQRSTU");
	// compare_near_lt
	EXPECT_EQ(28, TypeParam::compare_near_lt(digest1, digest2));
	EXPECT_EQ(28, TypeParam::template compare_near_lt<comparison_version::latest>(digest1, digest2));
	EXPECT_EQ(28, TypeParam::template compare_near_lt<comparison_version::v2_9>(digest1, digest2));
	EXPECT_EQ(28, digest1.compare_near_lt(digest2));
	EXPECT_EQ(28, digest1.template compare_near_lt<comparison_version::latest>(digest2));
	EXPECT_EQ(28, digest1.template compare_near_lt<comparison_version::v2_9>(digest2));
}


TEST(DigestUsageTests, Conversion)
{
	// non-RA to non-RA conversions
	{
		digest_t            d0("3:ABC:DEF");
		digest_unorm_t      d1(d0); d1 = d0;
		digest_long_t       d2(d0); d2 = d0;
		digest_long_unorm_t d3(d0); d3 = d0;
		digest_long_unorm_t d4(d1); d4 = d1;
		digest_long_unorm_t d5(d2); d5 = d2;
		EXPECT_EQ("3:ABC:DEF", d1.pretty());
		EXPECT_EQ("3:ABC:DEF", d2.pretty());
		EXPECT_EQ("3:ABC:DEF", d3.pretty());
		EXPECT_EQ("3:ABC:DEF", d4.pretty());
		EXPECT_EQ("3:ABC:DEF", d5.pretty());
	}
	// RA to RA conversions
	{
		digest_ra_t            d0("3:ABC:DEF");
		digest_ra_unorm_t      d1(d0); d1 = d0;
		digest_ra_long_t       d2(d0); d2 = d0;
		digest_ra_long_unorm_t d3(d0); d3 = d0;
		digest_ra_long_unorm_t d4(d1); d4 = d1;
		digest_ra_long_unorm_t d5(d2); d5 = d2;
		EXPECT_EQ("3:ABC:DEF", d1.pretty());
		EXPECT_EQ("3:ABC:DEF", d2.pretty());
		EXPECT_EQ("3:ABC:DEF", d3.pretty());
		EXPECT_EQ("3:ABC:DEF", d4.pretty());
		EXPECT_EQ("3:ABC:DEF", d5.pretty());
	}
	// RA to non-RA conversions (same sort)
	{
		digest_ra_t            d0("3:ABC:DEF");
		digest_ra_unorm_t      d1(d0); d1 = d0;
		digest_ra_long_t       d2(d0); d2 = d0;
		digest_ra_long_unorm_t d3(d0); d3 = d0;
		digest_t               d4(d0); d4 = d0;
		digest_unorm_t         d5(d1); d5 = d1;
		digest_long_t          d6(d2); d6 = d2;
		digest_long_unorm_t    d7(d3); d7 = d3;
		EXPECT_EQ("3:ABC:DEF", d1.pretty());
		EXPECT_EQ("3:ABC:DEF", d2.pretty());
		EXPECT_EQ("3:ABC:DEF", d3.pretty());
		EXPECT_EQ("3:ABC:DEF", d4.pretty());
		EXPECT_EQ("3:ABC:DEF", d5.pretty());
		EXPECT_EQ("3:ABC:DEF", d6.pretty());
		EXPECT_EQ("3:ABC:DEF", d7.pretty());
	}
	// RA to non-RA conversions (from short normalized digest)
	{
		digest_ra_t         d0("3:ABC:DEF");
		digest_unorm_t      d1(d0); d1 = d0;
		digest_long_t       d2(d0); d2 = d0;
		digest_long_unorm_t d3(d0); d3 = d0;
		EXPECT_EQ("3:ABC:DEF", d1.pretty());
		EXPECT_EQ("3:ABC:DEF", d2.pretty());
		EXPECT_EQ("3:ABC:DEF", d3.pretty());
	}
	// RA to non-RA conversions
	// (to long unnormalized digest and NOT from short normalized digest)
	{
		digest_ra_long_t    d0("3:ABC:DEF");
		digest_ra_unorm_t   d1("3:ABC:DEF");
		digest_long_unorm_t d2(d0); d2 = d0;
		digest_long_unorm_t d3(d1); d3 = d1;
		EXPECT_EQ("3:ABC:DEF", d2.pretty());
		EXPECT_EQ("3:ABC:DEF", d3.pretty());
	}
}

TEST(DigestUsageTests, BasicComparisonNonTemplate)
{
	digest_t digest1("3:ABCDEFG:HIJKLMN");
	digest_t digest2("3:HIJKLMN:ABCDEFG");
	// compare
	EXPECT_EQ(0, digest_t::compare(digest1, digest2));
	EXPECT_EQ(0, digest_t::compare<comparison_version::latest>(digest1, digest2));
	EXPECT_EQ(0, digest_t::compare<comparison_version::v2_9>(digest1, digest2));
	EXPECT_EQ(0, digest1.compare(digest2));
	EXPECT_EQ(0, digest1.compare<comparison_version::latest>(digest2));
	EXPECT_EQ(0, digest1.compare<comparison_version::v2_9>(digest2));
	// compare_diff
	EXPECT_EQ(0, digest_t::compare_diff(digest1, digest2));
	EXPECT_EQ(0, digest_t::compare_diff<comparison_version::latest>(digest1, digest2));
	EXPECT_EQ(0, digest_t::compare_diff<comparison_version::v2_9>(digest1, digest2));
	EXPECT_EQ(0, digest1.compare_diff(digest2));
	EXPECT_EQ(0, digest1.compare_diff<comparison_version::latest>(digest2));
	EXPECT_EQ(0, digest1.compare_diff<comparison_version::v2_9>(digest2));
	// compare_near
	EXPECT_EQ(0, digest_t::compare_near(digest1, digest2));
	EXPECT_EQ(0, digest_t::compare_near<comparison_version::latest>(digest1, digest2));
	EXPECT_EQ(0, digest_t::compare_near<comparison_version::v2_9>(digest1, digest2));
	EXPECT_EQ(0, digest1.compare_near(digest2));
	EXPECT_EQ(0, digest1.compare_near<comparison_version::latest>(digest2));
	EXPECT_EQ(0, digest1.compare_near<comparison_version::v2_9>(digest2));
	// compare_near_diff
	EXPECT_EQ(0, digest_t::compare_near_diff(digest1, digest2));
	EXPECT_EQ(0, digest_t::compare_near_diff<comparison_version::latest>(digest1, digest2));
	EXPECT_EQ(0, digest_t::compare_near_diff<comparison_version::v2_9>(digest1, digest2));
	EXPECT_EQ(0, digest1.compare_near_diff(digest2));
	EXPECT_EQ(0, digest1.compare_near_diff<comparison_version::latest>(digest2));
	EXPECT_EQ(0, digest1.compare_near_diff<comparison_version::v2_9>(digest2));
	// compare_near_eq
	EXPECT_EQ(0, digest_t::compare_near_eq(digest1, digest2));
	EXPECT_EQ(0, digest_t::compare_near_eq<comparison_version::latest>(digest1, digest2));
	EXPECT_EQ(0, digest_t::compare_near_eq<comparison_version::v2_9>(digest1, digest2));
	EXPECT_EQ(0, digest1.compare_near_eq(digest2));
	EXPECT_EQ(0, digest1.compare_near_eq<comparison_version::latest>(digest2));
	EXPECT_EQ(0, digest1.compare_near_eq<comparison_version::v2_9>(digest2));
	// compare_near_eq_diff
	EXPECT_EQ(0, digest_t::compare_near_eq_diff(digest1, digest2));
	EXPECT_EQ(0, digest_t::compare_near_eq_diff<comparison_version::latest>(digest1, digest2));
	EXPECT_EQ(0, digest_t::compare_near_eq_diff<comparison_version::v2_9>(digest1, digest2));
	EXPECT_EQ(0, digest1.compare_near_eq_diff(digest2));
	EXPECT_EQ(0, digest1.compare_near_eq_diff<comparison_version::latest>(digest2));
	EXPECT_EQ(0, digest1.compare_near_eq_diff<comparison_version::v2_9>(digest2));
	// compare_identical
	EXPECT_EQ(100, digest_t::compare_identical(digest1));
	EXPECT_EQ(100, digest_t::compare_identical<comparison_version::latest>(digest1));
	EXPECT_EQ( 14, digest_t::compare_identical<comparison_version::v2_9>(digest1));
	EXPECT_EQ(100, digest1.compare_identical());
	EXPECT_EQ(100, digest1.compare_identical<comparison_version::latest>());
	EXPECT_EQ( 14, digest1.compare_identical<comparison_version::v2_9>());
	// compare_near_lt
	digest1 = digest_t("6:ABCDEFG:HIJKLMN");
	digest2 = digest_t("12:HIJKLMN:OPQRSTU");
	EXPECT_EQ(28, digest_t::compare_near_lt(digest1, digest2));
	EXPECT_EQ(28, digest_t::compare_near_lt<comparison_version::latest>(digest1, digest2));
	EXPECT_EQ(28, digest_t::compare_near_lt<comparison_version::v2_9>(digest1, digest2));
	EXPECT_EQ(28, digest1.compare_near_lt(digest2));
	EXPECT_EQ(28, digest1.compare_near_lt<comparison_version::latest>(digest2));
	EXPECT_EQ(28, digest1.compare_near_lt<comparison_version::v2_9>(digest2));
}

#endif
