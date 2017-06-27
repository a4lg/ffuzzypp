/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/precond/digest.hpp
	Digest type precondition tests

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
#ifndef FFUZZYPP_TESTCASES_PRECOND_DIGEST_HPP
#define FFUZZYPP_TESTCASES_PRECOND_DIGEST_HPP

#include <type_traits>


/*
	Test Case : Typedefs
*/

static_assert(is_same<digest_to_unorm<digest_t>, digest_unorm_t>::value,
	"unnormalized variant of digest_t must be digest_unorm_t.");
static_assert(is_same<digest_to_norm<digest_t>, digest_t>::value,
	"normalized variant of digest_t must be digest_t itself.");
static_assert(is_same<digest_to_short<digest_t>, digest_t>::value,
	"short variant of digest_t must be digest_t itself.");
static_assert(is_same<digest_to_long<digest_t>, digest_long_t>::value,
	"long variant of digest_t must be digest_long_t.");
static_assert(is_same<digest_to_ra<digest_t>, digest_ra_t>::value,
	"alphabet restricted variant of digest_t must be digest_ra_t.");
static_assert(is_same<digest_to_non_ra<digest_t>, digest_t>::value,
	"non alphabet restricted variant of digest_t must be digest_t itself.");

static_assert(is_same<digest_to_unorm<digest_unorm_t>, digest_unorm_t>::value,
	"unnormalized variant of digest_unorm_t must be digest_unorm_t itself.");
static_assert(is_same<digest_to_norm<digest_unorm_t>, digest_t>::value,
	"normalized variant of digest_unorm_t must be digest_t.");
static_assert(is_same<digest_to_short<digest_unorm_t>, digest_unorm_t>::value,
	"short variant of digest_unorm_t must be digest_unorm_t itself.");
static_assert(is_same<digest_to_long<digest_unorm_t>, digest_long_unorm_t>::value,
	"long variant of digest_unorm_t must be digest_long_unorm_t.");
static_assert(is_same<digest_to_ra<digest_unorm_t>, digest_ra_unorm_t>::value,
	"alphabet restricted variant of digest_unorm_t must be digest_ra_unorm_t.");
static_assert(is_same<digest_to_non_ra<digest_unorm_t>, digest_unorm_t>::value,
	"non alphabet restricted variant of digest_unorm_t must be digest_unorm_t itself.");

static_assert(is_same<digest_to_unorm<digest_long_t>, digest_long_unorm_t>::value,
	"unnormalized variant of digest_long_t must be digest_long_unorm_t.");
static_assert(is_same<digest_to_norm<digest_long_t>, digest_long_t>::value,
	"normalized variant of digest_long_t must be digest_long_t itself.");
static_assert(is_same<digest_to_short<digest_long_t>, digest_t>::value,
	"short variant of digest_long_t must be digest_t.");
static_assert(is_same<digest_to_long<digest_long_t>, digest_long_t>::value,
	"long variant of digest_long_t must be digest_long_t itself.");
static_assert(is_same<digest_to_ra<digest_long_t>, digest_ra_long_t>::value,
	"alphabet restricted variant of digest_long_t must be digest_ra_long_t.");
static_assert(is_same<digest_to_non_ra<digest_long_t>, digest_long_t>::value,
	"non alphabet restricted variant of digest_long_t must be digest_long_t itself.");

static_assert(is_same<digest_to_unorm<digest_long_unorm_t>, digest_long_unorm_t>::value,
	"unnormalized variant of digest_long_unorm_t must be digest_long_unorm_t itself.");
static_assert(is_same<digest_to_norm<digest_long_unorm_t>, digest_long_t>::value,
	"normalized variant of digest_long_unorm_t must be digest_long_t.");
static_assert(is_same<digest_to_short<digest_long_unorm_t>, digest_unorm_t>::value,
	"short variant of digest_long_unorm_t must be digest_unorm_t.");
static_assert(is_same<digest_to_long<digest_long_unorm_t>, digest_long_unorm_t>::value,
	"long variant of digest_long_unorm_t must be digest_long_unorm_t itself.");
static_assert(is_same<digest_to_ra<digest_long_unorm_t>, digest_ra_long_unorm_t>::value,
	"alphabet restricted variant of digest_long_unorm_t must be digest_ra_long_unorm_t.");
static_assert(is_same<digest_to_non_ra<digest_long_unorm_t>, digest_long_unorm_t>::value,
	"non alphabet restricted variant of digest_long_unorm_t must be digest_long_unorm_t itself.");

static_assert(is_same<digest_to_unorm<digest_ra_t>, digest_ra_unorm_t>::value,
	"unnormalized variant of digest_ra_t must be digest_ra_unorm_t.");
static_assert(is_same<digest_to_norm<digest_ra_t>, digest_ra_t>::value,
	"normalized variant of digest_ra_t must be digest_ra_t itself.");
static_assert(is_same<digest_to_short<digest_ra_t>, digest_ra_t>::value,
	"short variant of digest_ra_t must be digest_ra_t itself.");
static_assert(is_same<digest_to_long<digest_ra_t>, digest_ra_long_t>::value,
	"long variant of digest_ra_t must be digest_ra_long_t.");
static_assert(is_same<digest_to_ra<digest_ra_t>, digest_ra_t>::value,
	"alphabet restricted variant of digest_ra_t must be digest_ra_t itself.");
static_assert(is_same<digest_to_non_ra<digest_ra_t>, digest_t>::value,
	"non alphabet restricted variant of digest_ra_t must be digest_t.");

static_assert(is_same<digest_to_unorm<digest_ra_unorm_t>, digest_ra_unorm_t>::value,
	"unnormalized variant of digest_ra_unorm_t must be digest_ra_unorm_t itself.");
static_assert(is_same<digest_to_norm<digest_ra_unorm_t>, digest_ra_t>::value,
	"normalized variant of digest_ra_unorm_t must be digest_ra_t.");
static_assert(is_same<digest_to_short<digest_ra_unorm_t>, digest_ra_unorm_t>::value,
	"short variant of digest_ra_unorm_t must be digest_ra_unorm_t itself.");
static_assert(is_same<digest_to_long<digest_ra_unorm_t>, digest_ra_long_unorm_t>::value,
	"long variant of digest_ra_unorm_t must be digest_ra_long_unorm_t.");
static_assert(is_same<digest_to_ra<digest_ra_unorm_t>, digest_ra_unorm_t>::value,
	"alphabet restricted variant of digest_ra_unorm_t must be digest_ra_unorm_t itself.");
static_assert(is_same<digest_to_non_ra<digest_ra_unorm_t>, digest_unorm_t>::value,
	"non alphabet restricted variant of digest_ra_unorm_t must be digest_unorm_t.");

static_assert(is_same<digest_to_unorm<digest_ra_long_t>, digest_ra_long_unorm_t>::value,
	"unnormalized variant of digest_ra_long_t must be digest_ra_long_unorm_t.");
static_assert(is_same<digest_to_norm<digest_ra_long_t>, digest_ra_long_t>::value,
	"normalized variant of digest_ra_long_t must be digest_ra_long_t itself.");
static_assert(is_same<digest_to_short<digest_ra_long_t>, digest_ra_t>::value,
	"short variant of digest_ra_long_t must be digest_ra_t.");
static_assert(is_same<digest_to_long<digest_ra_long_t>, digest_ra_long_t>::value,
	"long variant of digest_ra_long_t must be digest_ra_long_t itself.");
static_assert(is_same<digest_to_ra<digest_ra_long_t>, digest_ra_long_t>::value,
	"alphabet restricted variant of digest_ra_long_t must be digest_ra_long_t itself.");
static_assert(is_same<digest_to_non_ra<digest_ra_long_t>, digest_long_t>::value,
	"non alphabet restricted variant of digest_ra_long_t must be digest_long_t.");

static_assert(is_same<digest_to_unorm<digest_ra_long_unorm_t>, digest_ra_long_unorm_t>::value,
	"unnormalized variant of digest_ra_long_unorm_t must be digest_ra_long_unorm_t itself.");
static_assert(is_same<digest_to_norm<digest_ra_long_unorm_t>, digest_ra_long_t>::value,
	"normalized variant of digest_ra_long_unorm_t must be digest_ra_long_t.");
static_assert(is_same<digest_to_short<digest_ra_long_unorm_t>, digest_ra_unorm_t>::value,
	"short variant of digest_ra_long_unorm_t must be digest_ra_unorm_t.");
static_assert(is_same<digest_to_long<digest_ra_long_unorm_t>, digest_ra_long_unorm_t>::value,
	"long variant of digest_ra_long_unorm_t must be digest_ra_long_unorm_t itself.");
static_assert(is_same<digest_to_ra<digest_ra_long_unorm_t>, digest_ra_long_unorm_t>::value,
	"alphabet restricted variant of digest_ra_long_unorm_t must be digest_ra_long_unorm_t itself.");
static_assert(is_same<digest_to_non_ra<digest_ra_long_unorm_t>, digest_long_unorm_t>::value,
	"non alphabet restricted variant of digest_ra_long_unorm_t must be digest_long_unorm_t.");


#endif
