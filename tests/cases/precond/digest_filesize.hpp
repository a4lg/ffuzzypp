/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/precond/digest_filesize.hpp
	File size (used in digest generator) precondition tests

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
#ifndef FFUZZYPP_TESTCASES_PRECOND_DIGEST_FILESIZE_HPP
#define FFUZZYPP_TESTCASES_PRECOND_DIGEST_FILESIZE_HPP

#include <limits>


static_assert(digest_filesize::max_theoretical_size == digest_filesize::max_size,
	"Basic sanity check on digest_filesize failed.");

static_assert(!digest_filesize::is_not_meaningful(digest_filesize::min_supported_size),
	"digest_filesize::is_not_meaningful must not return true on min_supported_size "
	"since this size is meaningful (inclusive).");
static_assert(!digest_filesize::is_not_meaningful(digest_filesize::max_portable_size),
	"digest_filesize::is_not_meaningful must not return true on max_portable_size "
	"since this size is meaningful (inclusive).");
static_assert(!digest_filesize::is_not_meaningful(digest_filesize::max_theoretical_size),
	"digest_filesize::is_not_meaningful must not return true on max_theoretical_size "
	"since this size is meaningful (inclusive).");
static_assert(digest_filesize::min_supported_size == 0 ||
	digest_filesize::is_not_meaningful(digest_filesize::min_supported_size - 1),
	"digest_filesize::is_not_meaningful must return true on min_supported_size - 1.");

static_assert(digest_filesize::is_supported_by_ssdeep_2_12(digest_filesize::min_supported_size),
	"digest_filesize::is_supported_by_ssdeep_2_12 must not return false on min_supported_size "
	"since this size is supported (inclusive).");
static_assert(digest_filesize::is_supported_by_ssdeep_2_12(digest_filesize::max_portable_size),
	"digest_filesize::is_supported_by_ssdeep_2_12 must not return false on max_portable_size "
	"since this size is supported (inclusive).");
static_assert(digest_filesize::min_supported_size == 0 ||
	!digest_filesize::is_supported_by_ssdeep_2_12(digest_filesize::min_supported_size - 1),
	"digest_filesize::is_supported_by_ssdeep_2_12 must return false on min_supported_size - 1.");
static_assert(digest_filesize::max_portable_size == numeric_limits<digest_filesize_t>::max() ||
	!digest_filesize::is_supported_by_ssdeep_2_12(digest_filesize::max_portable_size + 1),
	"digest_filesize::is_supported_by_ssdeep_2_12 must return false on max_portable_size + 1.");

static_assert(digest_filesize::is_supported_by_ffuzzy_3_0(digest_filesize::max_theoretical_size),
	"digest_filesize::is_supported_by_ffuzzy_3_0 must not return false on max_theoretical_size "
	"since this size is supported (inclusive).");
static_assert(digest_filesize::max_theoretical_size == numeric_limits<digest_filesize_t>::max() ||
	!digest_filesize::is_supported_by_ssdeep_2_12(digest_filesize::max_portable_size + 1),
	"digest_filesize::is_supported_by_ssdeep_2_12 must return false on max_portable_size + 1.");

#endif
