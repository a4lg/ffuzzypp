/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/precond/utils/static_assert_query.hpp
	Precondition tests for static assertion queries

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
#ifndef FFUZZYPP_TESTCASES_PRECOND_UTILS_STATIC_ASSERT_QUERY_HPP
#define FFUZZYPP_TESTCASES_PRECOND_UTILS_STATIC_ASSERT_QUERY_HPP

namespace static_assert_query_tests
{

template <uintmax_t Limit>
struct q_is_less_than_limit
{
	template <uintmax_t Value>
	struct check
	{
		static const bool value = Value < Limit;
	};
};

static_assert(static_assert_query::is_all<q_is_less_than_limit<5>::check, 5>::value,
	"values out of range [0, 5] (inclusive) must not be checked through is_all.");
static_assert(!static_assert_query::is_all<q_is_less_than_limit<5>::check, 5+1>::value,
	"make sure that is_all checks all [0, 5] (inclusive) range.");
static_assert(static_assert_query::is_all<q_is_less_than_limit<6>::check, 6>::value,
	"values out of range [0, 6] (inclusive) must not be checked through is_all.");
static_assert(!static_assert_query::is_all<q_is_less_than_limit<6>::check, 6+1>::value,
	"make sure that is_all checks all [0, 6] (inclusive) range.");
static_assert(static_assert_query::is_all<q_is_less_than_limit<7>::check, 7>::value,
	"values out of range [0, 7] (inclusive) must not be checked through is_all.");
static_assert(!static_assert_query::is_all<q_is_less_than_limit<7>::check, 7+1>::value,
	"make sure that is_all checks all [0, 7] (inclusive) range.");
static_assert(static_assert_query::is_all<q_is_less_than_limit<8>::check, 8>::value,
	"values out of range [0, 8] (inclusive) must not be checked through is_all.");
static_assert(!static_assert_query::is_all<q_is_less_than_limit<8>::check, 8+1>::value,
	"make sure that is_all checks all [0, 8] (inclusive) range.");
static_assert(static_assert_query::is_all<q_is_less_than_limit<9>::check, 9>::value,
	"values out of range [0, 9] (inclusive) must not be checked through is_all.");
static_assert(!static_assert_query::is_all<q_is_less_than_limit<9>::check, 9+1>::value,
	"make sure that is_all checks all [0, 9] (inclusive) range.");


template <uintmax_t Target>
struct q_is_equal_to_target
{
	template <uintmax_t Value>
	struct check
	{
		static const bool value = Value == Target;
	};
};

static_assert(!static_assert_query::is_any<q_is_equal_to_target<5>::check, 5-1>::value,
	"values out of range [0, 4] (inclusive) must not be checked through is_any.");
static_assert(static_assert_query::is_any<q_is_less_than_limit<5>::check, 5>::value,
	"make sure that is_any checks all [0, 5] (inclusive) range.");
static_assert(!static_assert_query::is_any<q_is_equal_to_target<6>::check, 6-1>::value,
	"values out of range [0, 5] (inclusive) must not be checked through is_any.");
static_assert(static_assert_query::is_any<q_is_less_than_limit<6>::check, 6>::value,
	"make sure that is_any checks all [0, 6] (inclusive) range.");
static_assert(!static_assert_query::is_any<q_is_equal_to_target<7>::check, 7-1>::value,
	"values out of range [0, 6] (inclusive) must not be checked through is_any.");
static_assert(static_assert_query::is_any<q_is_less_than_limit<7>::check, 7>::value,
	"make sure that is_any checks all [0, 7] (inclusive) range.");
static_assert(!static_assert_query::is_any<q_is_equal_to_target<8>::check, 8-1>::value,
	"values out of range [0, 7] (inclusive) must not be checked through is_any.");
static_assert(static_assert_query::is_any<q_is_less_than_limit<8>::check, 8>::value,
	"make sure that is_any checks all [0, 8] (inclusive) range.");
static_assert(!static_assert_query::is_any<q_is_equal_to_target<9>::check, 9-1>::value,
	"values out of range [0, 8] (inclusive) must not be checked through is_any.");
static_assert(static_assert_query::is_any<q_is_less_than_limit<9>::check, 9>::value,
	"make sure that is_any checks all [0, 9] (inclusive) range.");


template <bool Constant>
struct q_constant
{
	template <uintmax_t Value>
	struct check
	{
		static const bool value = Constant;
	};
};

static_assert(static_assert_query::is_all<q_constant<false>::check, 0>::value,
	"empty bounds of is_all must return true.");
static_assert(!static_assert_query::is_any<q_constant<true>::check, 0>::value,
	"empty bounds of is_any must return false.");

}

#endif
