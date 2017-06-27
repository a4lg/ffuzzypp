/*

	ffuzzy++ : C++ implementation of fast fuzzy hashing

	digest_comparison.hpp
	Comparison of fuzzy digests / block hashes


	CREDITS OF ORIGINAL VERSION OF SSDEEP

	Copyright (C) 2002 Andrew Tridgell <tridge@samba.org>
	Copyright (C) 2006 ManTech International Corporation
	Copyright (C) 2013 Helmut Grohne <helmut@subdivi.de>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA


	CREDIT OF MODIFIED PORTIONS

	Copyright (C) 2015 Tsukasa OI <floss_ssdeep@irq.a4lg.com>

*/
#ifndef FFUZZYPP_DIGEST_COMPARISON_HPP
#define FFUZZYPP_DIGEST_COMPARISON_HPP

#include <cassert>
#include <cstddef>

#include <algorithm>
#include <string>

#include "digest_blocksize.hpp"
#include "digest_data.hpp"
#include "strings/common_substr.hpp"
#include "strings/edit_dist.hpp"
#include "utils/minmax.hpp"
#include "utils/safe_int.hpp"

namespace ffuzzy {

typedef unsigned digest_comparison_score_t;

/*
	There are two comparison classes used to compare digests and block hashes.
	*	blockhash_comparison (which takes two block hash strings and common block size)
	*	digest_comparison    (which takes two normalized digests)
*/

class blockhash_comparison
{
	static_assert(safe_int::contracts::is_unsigned_integral_type<digest_comparison_score_t>(),
		"digest_comparison_score_t must be an unsigned integral type.");
private:
	blockhash_comparison(void) = delete;
	blockhash_comparison(const blockhash_comparison&) = delete;

	// Parameters
public:
	static constexpr const size_t min_match_len = 7;

	/*
		Common substring and edit distance types
	*/
public:
	static_assert(safe_int::uvalue<size_t, digest_params::max_blockhash_len>::is_valid,
		"max_blockhash_len must be in range of size_t.");
	static_assert(safe_int::safe_mul<
			safe_int::uvalue<digest_comparison_score_t, digest_params::max_blockhash_len>,
			safe_int::uvalue<digest_comparison_score_t, 2>
		>::is_valid,
		"max_blockhash_len * 2 must be in range of digest_comparison_score_t.");
	typedef strings::common_substr_fast<
		digest_params::max_blockhash_len,
		min_match_len
	> substr_match_t;
	typedef strings::edit_dist_norm<
		strings::edit_dist_nonempty_fast<digest_comparison_score_t, digest_params::max_blockhash_len>
	> edit_dist_t;

	/*
		Score capping to prevent exaggerations
	*/
public:
	// Utility to check arithmetic overflow when preventing exaggrations
	static constexpr bool is_safe_for_score_capping(digest_blocksize_t blocksize) noexcept
	{
		static_assert(safe_int::safe_mul<
				safe_int::uvalue<digest_blocksize_t, digest_blocksize::min_blocksize>,
				safe_int::uvalue<digest_blocksize_t, 100>
			>::is_valid,
			"min_blocksize * 100 must be in range of digest_blocksize_t.");
		static_assert(min_match_len != 0,
			"min_match_len must not be zero (due to implementation restriction).");
		static_assert(safe_int::safe_add<
				safe_int::uvalue<digest_blocksize_t, 99>,
				safe_int::uvalue<digest_blocksize_t, min_match_len>
			>::is_valid,
			"99 + min_match_len must be in range of digest_blocksize_t.");
		return blocksize <
			digest_blocksize_t(99u + min_match_len) /
			digest_blocksize_t(min_match_len) *
			digest_blocksize::min_blocksize;
	}
private:
	// Score cap without checking arithmetic overflow
	static constexpr digest_comparison_score_t CONST_score_cap_unsafe(
		digest_blocksize_t blocksize,
		blockhash_len_t s1len,
		blockhash_len_t s2len
	) noexcept
	{
		static_assert(safe_int::safe_mul<
				safe_int::uvalue<digest_comparison_score_t, digest_params::max_blockhash_len>,
				safe_int::uvalue<digest_comparison_score_t, 100>
			>::is_valid,
			"max_blockhash_len * 100 must be in range of digest_comparison_score_t.");
		static_assert(safe_int::uvalue<digest_comparison_score_t, digest_blocksize::min_blocksize>::is_valid,
			"min_blocksize must be in range of digest_comparison_score_t.");
		return digest_comparison_score_t(blocksize / digest_blocksize::min_blocksize)
			* digest_comparison_score_t(minmax::min(s1len, s2len));
	}
	// ... with checking arithmetic overflow
	static constexpr digest_comparison_score_t CONST_score_cap_safe(
		digest_blocksize_t blocksize,
		blockhash_len_t s1len,
		blockhash_len_t s2len
	) noexcept
	{
		return is_safe_for_score_capping(blocksize)
			? CONST_score_cap_unsafe(blocksize, s1len, s2len)
			: digest_comparison_score_t(100);
	}
public:
	// Score cap without checking arithmetic overflow
	static digest_comparison_score_t score_cap_unsafe(
		digest_blocksize_t blocksize,
		blockhash_len_t s1len,
		blockhash_len_t s2len
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(is_safe_for_score_capping(blocksize));
		assert(s1len <= digest_params::max_blockhash_len);
		assert(s2len <= digest_params::max_blockhash_len);
		assert(s1len >= min_match_len);
		assert(s2len >= min_match_len);
		#endif
		return CONST_score_cap_unsafe(blocksize, s1len, s2len);
	}
	// ... with checking arithmetic overflow
	static digest_comparison_score_t score_cap(
		digest_blocksize_t blocksize,
		blockhash_len_t s1len,
		blockhash_len_t s2len
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(s1len <= digest_params::max_blockhash_len);
		assert(s2len <= digest_params::max_blockhash_len);
		assert(s1len >= min_match_len);
		assert(s2len >= min_match_len);
		#endif
		return CONST_score_cap_safe(blocksize, s1len, s2len);
	}

	/*
		Scoring
	*/
private:
	// Uncapped comparison score for given edit distance and block hash lengths
	static constexpr digest_comparison_score_t CONST_uncapped_score(
		digest_comparison_score_t edit_distance,
		blockhash_len_t s1len,
		blockhash_len_t s2len
	) noexcept
	{
		static_assert(safe_int::safe_mul<
				safe_int::safe_mul<
					safe_int::uvalue<digest_comparison_score_t, digest_params::max_blockhash_len>,
					safe_int::uvalue<digest_comparison_score_t, digest_params::max_blockhash_len>
				>,
				safe_int::uvalue<digest_comparison_score_t, 2>
			>::is_valid,
			"max_blockhash_len * max_blockhash_len * 2 must be in range of digest_comparison_score_t.");
		return 100 - 100 * (edit_distance
			* digest_comparison_score_t(digest_params::max_blockhash_len)
			/ digest_comparison_score_t(s1len + s2len)
		) / digest_comparison_score_t(digest_params::max_blockhash_len);
	}
public:
	static digest_comparison_score_t uncapped_score(
		digest_comparison_score_t edit_distance,
		blockhash_len_t s1len,
		blockhash_len_t s2len
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(s1len >= min_match_len && s1len <= digest_params::max_blockhash_len);
		assert(s2len >= min_match_len && s2len <= digest_params::max_blockhash_len);
		assert(edit_distance <= s1len + s2len);
		#endif
		return CONST_uncapped_score(edit_distance, s1len, s2len);
	}
	static digest_comparison_score_t uncapped_score(
		const char* s1, blockhash_len_t s1len,
		const char* s2, blockhash_len_t s2len
	) noexcept
	{
		if (!substr_match_t::match(s1, size_t(s1len), s2, size_t(s2len)))
			return 0;
		return uncapped_score(
			edit_dist_t::cost(s1, size_t(s1len), s2, size_t(s2len)),
			s1len, s2len
		);
	}
private:
	static constexpr digest_comparison_score_t CONST_capped_score(
		digest_comparison_score_t uncapped_score,
		digest_blocksize_t blocksize,
		blockhash_len_t s1len,
		blockhash_len_t s2len
	) noexcept
	{
		return
			s1len < min_match_len || s2len < min_match_len
			? 0
			: !is_safe_for_score_capping(blocksize)
				? uncapped_score
				: minmax::min(
					uncapped_score,
					CONST_score_cap_unsafe(blocksize, s1len, s2len)
				);
	}
	static constexpr digest_comparison_score_t CONST_score(
		digest_comparison_score_t edit_distance,
		digest_blocksize_t blocksize,
		blockhash_len_t s1len,
		blockhash_len_t s2len
	) noexcept
	{
		return CONST_capped_score(
			CONST_uncapped_score(edit_distance, s1len, s2len),
			blocksize, s1len, s2len
		);
	}
public:
	static digest_comparison_score_t score(
		digest_comparison_score_t edit_distance,
		digest_blocksize_t blocksize,
		blockhash_len_t s1len,
		blockhash_len_t s2len
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(s1len >= min_match_len && s1len <= digest_params::max_blockhash_len);
		assert(s2len >= min_match_len && s2len <= digest_params::max_blockhash_len);
		assert(edit_distance <= s1len + s2len);
		#endif
		return CONST_score(edit_distance, blocksize, s1len, s2len);
	}
	static digest_comparison_score_t score(
		const char* s1, blockhash_len_t s1len,
		const char* s2, blockhash_len_t s2len,
		digest_blocksize_t blocksize
	) noexcept
	{
		/*
			Note:
			This function does not handle cases that s1 and s2 are identical.
			For these cases, use score_identical function.
		*/
		if (!substr_match_t::match(s1, size_t(s1len), s2, size_t(s2len)))
			return 0;
		return score(
			edit_dist_t::cost(s1, size_t(s1len), s2, size_t(s2len)),
			blocksize, s1len, s2len
		);
	}

	/*
		Scoring (on identical strings [edit distance is always zero])
	*/
public:
	static constexpr digest_comparison_score_t score_identical_2_9(
		blockhash_len_t blockhash_len,
		digest_blocksize_t blocksize
	) noexcept
	{
		return CONST_score(0, blocksize, blockhash_len, blockhash_len);
	}
	static constexpr digest_comparison_score_t score_identical(
		blockhash_len_t blockhash_len,
		digest_blocksize_t blocksize
	) noexcept
	{
		#ifdef FFUZZYPP_COMPATIBILITY_SSDEEP_2_9
		return score_identical_2_9(blockhash_len, blocksize);
		#else
		return 100;
		#endif
	}

	/*
		Minimum scores (with substring matches)
	*/
private:
	// Uncapped minimum score if there is a common substring
	static constexpr digest_comparison_score_t CONST_uncapped_min_matching_score(
		blockhash_len_t s1len,
		blockhash_len_t s2len
	) noexcept
	{
		return CONST_uncapped_score(
			digest_comparison_score_t(s1len + s2len)
				- digest_comparison_score_t(2 * min_match_len),
			s1len, s2len
		);
	}
public:
	// Uncapped minimum score if there is a common substring
	static digest_comparison_score_t uncapped_min_matching_score(
		blockhash_len_t s1len,
		blockhash_len_t s2len
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(s1len >= min_match_len && s1len <= digest_params::max_blockhash_len);
		assert(s2len >= min_match_len && s2len <= digest_params::max_blockhash_len);
		#endif
		return CONST_uncapped_min_matching_score(s1len, s2len);
	}
	// Capped minimum score if there is a common substring
	static digest_comparison_score_t min_matching_score(
		digest_blocksize_t blocksize,
		blockhash_len_t s1len,
		blockhash_len_t s2len
	) noexcept
	{
		/*
			This function returns "minimum" matching score.
			There are no less scores but zero for given
			block hash lengths and the block size.
			We assume that we have common strings in block hash
			(s1len and s2len should be at least min_match_len).
		*/
		#ifdef FFUZZYPP_DEBUG
		assert(s1len >= min_match_len && s1len <= digest_params::max_blockhash_len);
		assert(s2len >= min_match_len && s2len <= digest_params::max_blockhash_len);
		#endif
		return CONST_capped_score(
			uncapped_min_matching_score(s1len, s2len),
			blocksize, s1len, s2len
		);
	}
	// Uncapped minimum scores for certain block hash lengths
public:
	static constexpr digest_comparison_score_t uncapped_min_matching_score_full(void) noexcept
	{
		return CONST_uncapped_min_matching_score(
			digest_params::max_blockhash_len,
			digest_params::max_blockhash_len
		);
	}
	static constexpr digest_comparison_score_t uncapped_min_matching_score_half(void) noexcept
	{
		static_assert(digest_params::max_blockhash_len / 2 >= min_match_len,
			"max_blockhash_len / 2 must be at least min_match_len.");
		return CONST_uncapped_min_matching_score(
			digest_params::max_blockhash_len / 2,
			digest_params::max_blockhash_len / 2
		);
	}
	static constexpr digest_comparison_score_t uncapped_min_matching_score_halfandfull(void) noexcept
	{
		return CONST_uncapped_min_matching_score(
			digest_params::max_blockhash_len / 2,
			digest_params::max_blockhash_len
		);
	}
	// Capped minimum scores for certain block hash lengths
	// Note:
	//   This function does not care about non-"is_safe_to_double" cases.
	//   On such cases, comparison score shall be always zero
	//   (because block hashes will not match).
public:
	static constexpr digest_comparison_score_t min_matching_score_full(
		digest_blocksize_t blocksize
	) noexcept
	{
		return CONST_capped_score(
			uncapped_min_matching_score_full(), blocksize,
			digest_params::max_blockhash_len,
			digest_params::max_blockhash_len
		);
	}
	static constexpr digest_comparison_score_t min_matching_score_half(
		digest_blocksize_t blocksize
	) noexcept
	{
		return CONST_capped_score(
			uncapped_min_matching_score_half(), blocksize,
			digest_params::max_blockhash_len / 2,
			digest_params::max_blockhash_len / 2
		);
	}
	static constexpr digest_comparison_score_t min_matching_score_halfandfull(
		digest_blocksize_t blocksize
	) noexcept
	{
		return CONST_capped_score(
			uncapped_min_matching_score_halfandfull(), blocksize,
			digest_params::max_blockhash_len / 2,
			digest_params::max_blockhash_len
		);
	}

	/*
		Maximum scores (by difference of block hash lengths)
	*/
private:
	// Uncapped maximum score for given block hash lengths (s1len <= s2len)
	static constexpr digest_comparison_score_t CONST_uncapped_max_matching_score_le(
		blockhash_len_t s1len,
		blockhash_len_t s2len
	) noexcept
	{
		return CONST_uncapped_score(s2len - s1len, s1len, s2len);
	}
	// Uncapped maximum score for given block hash lengths
	static constexpr digest_comparison_score_t CONST_uncapped_max_matching_score(
		blockhash_len_t s1len,
		blockhash_len_t s2len
	) noexcept
	{
		return s1len <= s2len
			? CONST_uncapped_max_matching_score_le(s1len, s2len)
			: CONST_uncapped_max_matching_score_le(s2len, s1len);
	}
public:
	// Uncapped maximum score for given block hash lengths (s1len <= s2len)
	static digest_comparison_score_t uncapped_max_matching_score_le(
		blockhash_len_t s1len,
		blockhash_len_t s2len
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(s1len >= min_match_len && s1len <= digest_params::max_blockhash_len);
		assert(s2len >= min_match_len && s2len <= digest_params::max_blockhash_len);
		assert(s1len <= s2len);
		#endif
		return CONST_uncapped_max_matching_score_le(s1len, s2len);
	}
	// Uncapped maximum score for given block hash lengths
	static digest_comparison_score_t uncapped_max_matching_score(
		blockhash_len_t s1len,
		blockhash_len_t s2len
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(s1len >= min_match_len && s1len <= digest_params::max_blockhash_len);
		assert(s2len >= min_match_len && s2len <= digest_params::max_blockhash_len);
		#endif
		return s1len <= s2len
			? uncapped_max_matching_score_le(s1len, s2len)
			: uncapped_max_matching_score_le(s2len, s1len);
	}
	// Capped maximum score for given block hash lengths (s1len <= s2len) and block size
	// Note:
	// This function will not return actual maximum score (but capped one) if s1len == s2len.
	// Actual maximum score depends on compatibility mode and use
	// max_matching_score_le function for actual value.
	static digest_comparison_score_t capped_max_matching_score_le(
		digest_blocksize_t blocksize,
		blockhash_len_t s1len,
		blockhash_len_t s2len
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(s1len >= min_match_len && s1len <= digest_params::max_blockhash_len);
		assert(s2len >= min_match_len && s2len <= digest_params::max_blockhash_len);
		assert(s1len <= s2len);
		#endif
		return CONST_capped_score(
			uncapped_max_matching_score_le(s1len, s2len),
			blocksize, s1len, s2len
		);
	}
	// Capped maximum score for given block hash lengths and block size
	// Note:
	// This function will not return actual maximum score (but capped one) if s1len == s2len.
	// Actual maximum score depends on compatibility mode and use
	// max_matching_score function for actual value.
	static digest_comparison_score_t capped_max_matching_score(
		digest_blocksize_t blocksize,
		blockhash_len_t s1len,
		blockhash_len_t s2len
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(s1len >= min_match_len && s1len <= digest_params::max_blockhash_len);
		assert(s2len >= min_match_len && s2len <= digest_params::max_blockhash_len);
		#endif
		return CONST_capped_score(
			uncapped_max_matching_score(s1len, s2len),
			blocksize, s1len, s2len
		);
	}
	// Capped maximum score for given block hash lengths (s1len <= s2len) and block size
	static digest_comparison_score_t max_matching_score_le(
		digest_blocksize_t blocksize,
		blockhash_len_t s1len,
		blockhash_len_t s2len
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(s1len >= min_match_len && s1len <= digest_params::max_blockhash_len);
		assert(s2len >= min_match_len && s2len <= digest_params::max_blockhash_len);
		assert(s1len <= s2len);
		#endif
		if (s1len == s2len)
			return score_identical(s1len, blocksize);
		return capped_max_matching_score_le(blocksize, s1len, s2len);
	}
	// Capped maximum score for given block hash lengths and block size
	static digest_comparison_score_t max_matching_score(
		digest_blocksize_t blocksize,
		blockhash_len_t s1len,
		blockhash_len_t s2len
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(s1len >= min_match_len && s1len <= digest_params::max_blockhash_len);
		assert(s2len >= min_match_len && s2len <= digest_params::max_blockhash_len);
		#endif
		if (s1len == s2len)
			return score_identical(s1len, blocksize);
		return capped_max_matching_score(blocksize, s1len, s2len);
	}
	// Capped maximum score for given block hash lengths (s1len <= s2len) and block size
	// (compatible with version 2.9)
	static digest_comparison_score_t max_matching_score_le_2_9(
		digest_blocksize_t blocksize,
		blockhash_len_t s1len,
		blockhash_len_t s2len
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(s1len >= min_match_len && s1len <= digest_params::max_blockhash_len);
		assert(s2len >= min_match_len && s2len <= digest_params::max_blockhash_len);
		assert(s1len <= s2len);
		#endif
		if (s1len == s2len)
			return score_identical_2_9(s1len, blocksize);
		return capped_max_matching_score_le(blocksize, s1len, s2len);
	}
	// Capped maximum score for given block hash lengths and block size
	// (compatible with version 2.9)
	static digest_comparison_score_t max_matching_score_2_9(
		digest_blocksize_t blocksize,
		blockhash_len_t s1len,
		blockhash_len_t s2len
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(s1len >= min_match_len && s1len <= digest_params::max_blockhash_len);
		assert(s2len >= min_match_len && s2len <= digest_params::max_blockhash_len);
		#endif
		if (s1len == s2len)
			return score_identical_2_9(s1len, blocksize);
		return capped_max_matching_score(blocksize, s1len, s2len);
	}
};


template <bool IsShort>
class digest_comparison
{
private:
	digest_comparison(void) = delete;
	digest_comparison(const digest_comparison&) = delete;

	// Internal string interfaces
public:
	typedef blockhash_comparison::substr_match_t substr_match_t;
	typedef blockhash_comparison::edit_dist_t    edit_dist_t;

	// Comparison (on identical digests)
public:
	static digest_comparison_score_t compare_identical_2_9(
		const digest_data<IsShort>& a
	) noexcept
	{
		if (digest_blocksize::is_safe_to_double(a.blksize))
		{
			return std::max(
				blockhash_comparison::score_identical(a.blkhash1_len, a.blksize),
				blockhash_comparison::score_identical(a.blkhash2_len, a.blksize * 2)
			);
		}
		else
		{
			/*
				Assumption:
				If the block size is large enough, the second
				block hash is empty or 1-character.
				This makes second block hash comparison score zero.
			*/
			#if defined(FFUZZYPP_DEBUG) && 0
			assert(a.blkhash2_len <= 1);
			#endif
			return blockhash_comparison::score_identical(a.blkhash1_len, a.blksize);
		}
	}
	static digest_comparison_score_t compare_identical(
		const digest_data<IsShort>& a
	) noexcept
	{
		#ifdef FFUZZYPP_COMPATIBILITY_SSDEEP_2_9
		return compare_identical_2_9(a);
		#else
		return 100;
		#endif
	}

	// Comparison (on possibly different digests)
public:
	static digest_comparison_score_t compare_near(
		const digest_data<IsShort>& a,
		const digest_data<IsShort>& b
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(a.is_valid() && a.is_normalized());
		assert(b.is_valid() && b.is_normalized());
		assert(digest_blocksize::is_near(a.blksize, b.blksize));
		#endif
		if (a == b)
			return compare_identical(a);
		if (digest_blocksize::is_safe_to_double(a.blksize))
		{
			if (digest_blocksize::is_near_eq(a.blksize, b.blksize))
				return std::max(
					blockhash_comparison::score(
						a.digest, a.blkhash1_len,
						b.digest, b.blkhash1_len,
						a.blksize),
					blockhash_comparison::score(
						a.digest+a.blkhash1_len, a.blkhash2_len,
						b.digest+b.blkhash1_len, b.blkhash2_len,
						a.blksize * 2));
			else if (digest_blocksize::is_near_lt(a.blksize, b.blksize))
				return blockhash_comparison::score(
					a.digest+a.blkhash1_len, a.blkhash2_len,
					b.digest, b.blkhash1_len,
					b.blksize);
			else // digest_blocksize::is_near_gt
				return blockhash_comparison::score(
					a.digest, a.blkhash1_len,
					b.digest+b.blkhash1_len, b.blkhash2_len,
					a.blksize);
		}
		else
		{
			if (digest_blocksize::is_near_eq(a.blksize, b.blksize))
			{
				/*
					Assumption:
					If the block size is large enough, the second
					block hash is empty or 1-character.
					This makes second block hash comparison score zero.
				*/
				static_assert(blockhash_comparison::min_match_len > 1,
					"block hash comparison must not match too substring with length 0 or 1 "
					"(due to implementation restrictions).");
				#if defined(FFUZZYPP_DEBUG) && 0
				assert(a.blkhash2_len <= 1);
				assert(b.blkhash2_len <= 1);
				#endif
				return blockhash_comparison::score(
					a.digest, a.blkhash1_len,
					b.digest, b.blkhash1_len,
					a.blksize);
			}
			else if (digest_blocksize::is_near_gt(a.blksize, b.blksize))
				return blockhash_comparison::score(
					a.digest, a.blkhash1_len,
					b.digest+b.blkhash1_len, b.blkhash2_len,
					a.blksize);
			else // overflow (no common substring)
				return 0;
		}
	}
	static digest_comparison_score_t compare(
		const digest_data<IsShort>& a,
		const digest_data<IsShort>& b
	) noexcept
	{
		if (!digest_blocksize::is_near(a.blksize, b.blksize))
			return 0;
		return compare_near(a, b);
	}

	// Comparison (on possibly different digests; specialized versions)
public:
	static digest_comparison_score_t compare_near_eq(
		const digest_data<IsShort>& a,
		const digest_data<IsShort>& b
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(a.is_valid() && a.is_normalized());
		assert(b.is_valid() && b.is_normalized());
		assert(digest_blocksize::is_near_eq(a.blksize, b.blksize));
		#endif
		if (digest_data<IsShort>::is_eq_except_blocksize(a, b))
			return compare_identical(a);
		if (digest_blocksize::is_safe_to_double(a.blksize))
			return std::max(
				blockhash_comparison::score(
					a.digest, a.blkhash1_len,
					b.digest, b.blkhash1_len,
					a.blksize),
				blockhash_comparison::score(
					a.digest+a.blkhash1_len, a.blkhash2_len,
					b.digest+b.blkhash1_len, b.blkhash2_len,
					a.blksize * 2));
		else
		{
			/*
				Assumption:
				If the block size is large enough, the second
				block hash is empty or 1-character.
				This makes second block hash comparison score zero.
			*/
			#if defined(FFUZZYPP_DEBUG) && 0
			assert(a.blkhash2_len <= 1);
			assert(b.blkhash2_len <= 1);
			#endif
			static_assert(blockhash_comparison::min_match_len > 1,
				"block hash comparison must not match too substring with length 0 or 1 "
				"(due to implementation restrictions).");
			return blockhash_comparison::score(
				a.digest, a.blkhash1_len,
				b.digest, b.blkhash1_len,
				a.blksize);
		}
	}
	static digest_comparison_score_t compare_near_lt(
		const digest_data<IsShort>& a,
		const digest_data<IsShort>& b
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(a.is_valid() && a.is_normalized());
		assert(b.is_valid() && b.is_normalized());
		assert(digest_blocksize::is_near_lt(a.blksize, b.blksize));
		assert(a != b);
		#endif
		// implies digest_blocksize::is_safe_to_double(a.blksize)
		return blockhash_comparison::score(
			a.digest+a.blkhash1_len, a.blkhash2_len,
			b.digest, b.blkhash1_len,
			b.blksize);
	}

	// Comparison (for unnormalized form of digests)
public:
	static digest_comparison_score_t compare_unnormalized(
		const digest_data<IsShort>& a,
		const digest_data<IsShort>& b
	) noexcept
	{
		return compare(
			digest_data<IsShort>::normalize(a),
			digest_data<IsShort>::normalize(b)
		);
	}

	// Comparison (for strings)
public:
	static digest_comparison_score_t compare(
		const char* a,
		const char* b
	) noexcept(false)
	{
		digest_data<IsShort> da, db;
		if (!digest_data<IsShort>::parse_normalized(da, a))
			throw digest_parse_error();
		if (!digest_data<IsShort>::parse_normalized(db, b))
			throw digest_parse_error();
		return compare(da, db);
	}
	static digest_comparison_score_t compare(
		const std::string& a,
		const std::string& b
	)
	{
		return compare(a.c_str(), b.c_str());
	}
};

typedef digest_comparison< true>  digest_comparison_t;
typedef digest_comparison<false>  digest_long_comparison_t;

}

#endif
