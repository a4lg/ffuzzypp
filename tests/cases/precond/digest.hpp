/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/precond/digest.hpp
	Digest type precondition tests

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
#ifndef FFUZZYPP_TESTCASES_PRECOND_DIGEST_HPP
#define FFUZZYPP_TESTCASES_PRECOND_DIGEST_HPP

#include <type_traits>


/*
	Test Case : Generalized
*/

#if defined(digest_to_unorm) || defined(digest_to_norm) || defined(digest_to_short) || defined(digest_to_long)
#error Symbol conflict found and cannot continue testing.
#endif
#if defined(digest) || defined(digest_base)
#error Symbol conflict found and cannot continue testing.
#endif
#define FFUZZYPP_LOCAL_CHK2(Ttransform, Prefix, Tbase, IS_, IN_, IS_expected, IN_expected) \
	static_assert(is_same<Ttransform<Prefix Tbase<IS_, IN_>>, Prefix Tbase<IS_expected, IN_expected>>::value, \
		#Ttransform "<" #Prefix " " #Tbase "<" #IS_ ", " #IN_ ">> must be " #Prefix " " #Tbase "<" #IS_expected ", " #IN_expected ">.")
#define FFUZZYPP_LOCAL_CHK1(Ttransform, Tbase, IS_, IN_, IS_expected, IN_expected) \
	FFUZZYPP_LOCAL_CHK2(Ttransform, , Tbase, IS_, IN_, IS_expected, IN_expected); \
	FFUZZYPP_LOCAL_CHK2(Ttransform, const, Tbase, IS_, IN_, IS_expected, IN_expected); \
	FFUZZYPP_LOCAL_CHK2(Ttransform, volatile, Tbase, IS_, IN_, IS_expected, IN_expected); \
	FFUZZYPP_LOCAL_CHK2(Ttransform, const volatile, Tbase, IS_, IN_, IS_expected, IN_expected)
#define FFUZZYPP_LOCAL_CHK(Tbase, IsShort, IsNormalized) \
	FFUZZYPP_LOCAL_CHK1(digest_to_unorm, Tbase, IsShort, IsNormalized, IsShort, false); \
	FFUZZYPP_LOCAL_CHK1(digest_to_norm,  Tbase, IsShort, IsNormalized, IsShort, true); \
	FFUZZYPP_LOCAL_CHK1(digest_to_short, Tbase, IsShort, IsNormalized, true,  IsNormalized); \
	FFUZZYPP_LOCAL_CHK1(digest_to_long,  Tbase, IsShort, IsNormalized, false, IsNormalized)
FFUZZYPP_LOCAL_CHK(digest_base,  true,  true);
FFUZZYPP_LOCAL_CHK(digest_base,  true, false);
FFUZZYPP_LOCAL_CHK(digest_base, false,  true);
FFUZZYPP_LOCAL_CHK(digest_base, false, false);
FFUZZYPP_LOCAL_CHK(digest,  true,  true);
FFUZZYPP_LOCAL_CHK(digest,  true, false);
FFUZZYPP_LOCAL_CHK(digest, false,  true);
FFUZZYPP_LOCAL_CHK(digest, false, false);
#undef FFUZZYPP_LOCAL_CHK2
#undef FFUZZYPP_LOCAL_CHK1
#undef FFUZZYPP_LOCAL_CHK


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

static_assert(is_same<digest_to_unorm<digest_unorm_t>, digest_unorm_t>::value,
	"unnormalized variant of digest_unorm_t must be digest_unorm_t itself.");
static_assert(is_same<digest_to_norm<digest_unorm_t>, digest_t>::value,
	"normalized variant of digest_unorm_t must be digest_t.");
static_assert(is_same<digest_to_short<digest_unorm_t>, digest_unorm_t>::value,
	"short variant of digest_unorm_t must be digest_unorm_t itself.");
static_assert(is_same<digest_to_long<digest_unorm_t>, digest_long_unorm_t>::value,
	"long variant of digest_unorm_t must be digest_long_unorm_t.");

static_assert(is_same<digest_to_unorm<digest_long_t>, digest_long_unorm_t>::value,
	"unnormalized variant of digest_long_t must be digest_long_unorm_t.");
static_assert(is_same<digest_to_norm<digest_long_t>, digest_long_t>::value,
	"normalized variant of digest_long_t must be digest_long_t itself.");
static_assert(is_same<digest_to_short<digest_long_t>, digest_t>::value,
	"short variant of digest_long_t must be digest_t.");
static_assert(is_same<digest_to_long<digest_long_t>, digest_long_t>::value,
	"long variant of digest_long_t must be digest_long_t itself.");

static_assert(is_same<digest_to_unorm<digest_long_unorm_t>, digest_long_unorm_t>::value,
	"unnormalized variant of digest_long_unorm_t must be digest_long_unorm_t itself.");
static_assert(is_same<digest_to_norm<digest_long_unorm_t>, digest_long_t>::value,
	"normalized variant of digest_long_unorm_t must be digest_long_t.");
static_assert(is_same<digest_to_short<digest_long_unorm_t>, digest_unorm_t>::value,
	"short variant of digest_long_unorm_t must be digest_unorm_t.");
static_assert(is_same<digest_to_long<digest_long_unorm_t>, digest_long_unorm_t>::value,
	"long variant of digest_long_unorm_t must be digest_long_unorm_t itself.");


#endif
