/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	ffuzzy.hpp
	Fuzzy hashing implementation

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
#ifndef FFUZZYPP_ROOT_FFUZZY_HPP
#define FFUZZYPP_ROOT_FFUZZY_HPP

#include "ffuzzypp/utils/likely.hpp"
#include "ffuzzypp/utils/minmax.hpp"
#include "ffuzzypp/utils/safe_int.hpp"
#include "ffuzzypp/utils/static_assert_query.hpp"
#include "ffuzzypp/utils/numeric_digits.hpp"
#include "ffuzzypp/utils/type_modifier.hpp"
#include "ffuzzypp/utils/ranges.hpp"
#include "ffuzzypp/base64.hpp"
#include "ffuzzypp/context_hash.hpp"
#include "ffuzzypp/context_hash_fast.hpp"
#include "ffuzzypp/rolling_hash.hpp"
#include "ffuzzypp/rolling_hash_ssdeep.hpp"
#include "ffuzzypp/strings/position_array.hpp"
#include "ffuzzypp/strings/common_substr.hpp"
#include "ffuzzypp/strings/edit_dist.hpp"
#include "ffuzzypp/strings/terminators.hpp"
#include "ffuzzypp/strings/transform.hpp"
#include "ffuzzypp/strings/sequences.hpp"
#include "ffuzzypp/strings/nosequences.hpp"
#include "ffuzzypp/digest_blocksize.hpp"
#include "ffuzzypp/digest_data.hpp"
#include "ffuzzypp/digest_position_array_base.hpp"
#include "ffuzzypp/digest_comparison.hpp"
#include "ffuzzypp/digest_base.hpp"
#include "ffuzzypp/digest_position_array.hpp"
#include "ffuzzypp/digest.hpp"
#include "ffuzzypp/digest_filesize.hpp"
#include "ffuzzypp/digest_generator.hpp"

#ifdef FFUZZYPP_COMPATIBILITY_SSDEEP_2_9
#error Configuration by FFUZZYPP_COMPATIBILITY_SSDEEP_2_9 is now removed. Read README for alternative method.
#endif
#ifdef FFUZZYPP_COMPATIBILITY_SSDEEP
static_assert(ffuzzy::digest_params::max_blockhash_len == 64,
	"Given parameter (digest_params::max_blockhash_len) is not compatible with ssdeep.");
static_assert(ffuzzy::digest_params::max_blockhash_sequence == 3,
	"Given parameter (digest_params::max_blockhash_sequence) is not compatible with ssdeep.");
static_assert(ffuzzy::blockhash_comparison_params::min_match_len == 7,
	"Given parameter (blockhash_comparison_params::min_match_len) is not compatible with ssdeep.");
static_assert(ffuzzy::rolling_hash::window_size == 7,
	"Given parameter (rolling_hash::window_size) is not compatible with ssdeep.");
static_assert(ffuzzy::digest_blocksize::number_of_blockhashes == 31,
	"Given parameter (digest_blocksize::number_of_blockhashes) is not compatible with ssdeep.");
static_assert(ffuzzy::digest_blocksize::min_blocksize == 3,
	"Given parameter (digest_blocksize::min_blocksize) is not compatible with ssdeep.");
static_assert(ffuzzy::digest_filesize::min_supported_size == 4097,
	"Given parameter (digest_filesize::min_supported_size) is not compatible with ssdeep.");
#endif

#endif
