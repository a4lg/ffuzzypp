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

	Copyright (C) 2017 Tsukasa OI <floss_ssdeep@irq.a4lg.com>

*/
#ifndef FFUZZYPP_DIGEST_COMPARISON_HPP
#define FFUZZYPP_DIGEST_COMPARISON_HPP

#include <cassert>
#include <cstddef>

#include <algorithm>
#include <string>

#include "digest_blocksize.hpp"
#include "digest_data.hpp"
#include "digest_position_array_base.hpp"
#include "strings/common_substr.hpp"
#include "strings/edit_dist.hpp"
#include "strings/position_array.hpp"
#include "utils/minmax.hpp"
#include "utils/safe_int.hpp"

namespace ffuzzy {

enum struct comparison_version
{
	v2_9,
	v2_13,
	latest = v2_13,
};

class blockhash_comparison_params
{
private:
	blockhash_comparison_params(void) = delete;
	blockhash_comparison_params(const blockhash_comparison_params&) = delete;
	// Parameters
public:
	static constexpr const size_t min_match_len = 7;
};

typedef unsigned digest_comparison_score_t;



namespace internal
{
	class blockhash_comparison_base
	{
		static_assert(safe_int::contracts::is_unsigned_integral_type<digest_comparison_score_t>(),
			"digest_comparison_score_t must be an unsigned integral type.");
	private:
		blockhash_comparison_base(void) = delete;
		blockhash_comparison_base(const blockhash_comparison_base&) = delete;

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
			static_assert(blockhash_comparison_params::min_match_len != 0,
				"blockhash_comparison_params::min_match_len must not be zero (due to implementation restriction).");
			static_assert(safe_int::safe_add<
					safe_int::uvalue<digest_blocksize_t, 99>,
					safe_int::uvalue<digest_blocksize_t, blockhash_comparison_params::min_match_len>
				>::is_valid,
				"99 + blockhash_comparison_params::min_match_len must be in range of digest_blocksize_t.");
			return blocksize <
				digest_blocksize_t(99u + blockhash_comparison_params::min_match_len) /
				digest_blocksize_t(blockhash_comparison_params::min_match_len) *
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
			assert(s1len >= blockhash_comparison_params::min_match_len);
			assert(s2len >= blockhash_comparison_params::min_match_len);
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
			assert(s1len >= blockhash_comparison_params::min_match_len);
			assert(s2len >= blockhash_comparison_params::min_match_len);
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
			assert(s1len >= blockhash_comparison_params::min_match_len && s1len <= digest_params::max_blockhash_len);
			assert(s2len >= blockhash_comparison_params::min_match_len && s2len <= digest_params::max_blockhash_len);
			assert(edit_distance <= s1len + s2len);
			#endif
			return CONST_uncapped_score(edit_distance, s1len, s2len);
		}
		static digest_comparison_score_t uncapped_score(
			const char* s1, blockhash_len_t s1len,
			const char* s2, blockhash_len_t s2len
		) noexcept
		{
			if (!strings::common_substr_fast<digest_params::max_blockhash_len,
				blockhash_comparison_params::min_match_len>::match(s1, size_t(s1len), s2, size_t(s2len)))
			{
				return 0;
			}
			return uncapped_score(
				edit_dist_t::cost(s1, size_t(s1len), s2, size_t(s2len)),
				s1len, s2len
			);
		}
		template <typename TBitmap, char CMin, char CMax>
		static digest_comparison_score_t uncapped_score(
			const strings::position_array<TBitmap, char, CMin, CMax>& s1, blockhash_len_t s1len,
			const char* s2, blockhash_len_t s2len
		) noexcept
		{
			if (!strings::common_substr_bitparallel<TBitmap, CMin, CMax,
				digest_params::max_blockhash_len, blockhash_comparison_params::min_match_len>::match(s1, s2, size_t(s2len)))
			{
				return 0;
			}
			return uncapped_score(
				strings::edit_dist_bitparallel<digest_comparison_score_t, TBitmap,
					CMin, CMax, digest_params::max_blockhash_len>::cost(s1, size_t(s1len), s2, size_t(s2len)),
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
				s1len < blockhash_comparison_params::min_match_len || s2len < blockhash_comparison_params::min_match_len
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
			assert(s1len >= blockhash_comparison_params::min_match_len && s1len <= digest_params::max_blockhash_len);
			assert(s2len >= blockhash_comparison_params::min_match_len && s2len <= digest_params::max_blockhash_len);
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
			if (!strings::common_substr_fast<digest_params::max_blockhash_len,
				blockhash_comparison_params::min_match_len>::match(s1, size_t(s1len), s2, size_t(s2len)))
			{
				return 0;
			}
			return score(
				edit_dist_t::cost(s1, size_t(s1len), s2, size_t(s2len)),
				blocksize, s1len, s2len
			);
		}
		template <typename TBitmap, char CMin, char CMax>
		static digest_comparison_score_t score(
			const strings::position_array<TBitmap, char, CMin, CMax>& s1, blockhash_len_t s1len,
			const char* s2, blockhash_len_t s2len,
			digest_blocksize_t blocksize
		) noexcept
		{
			if (!strings::common_substr_bitparallel<TBitmap, CMin, CMax,
				digest_params::max_blockhash_len, blockhash_comparison_params::min_match_len>::match(s1, s2, size_t(s2len)))
			{
				return 0;
			}
			return score(
				strings::edit_dist_bitparallel<digest_comparison_score_t, TBitmap,
					CMin, CMax, digest_params::max_blockhash_len>::cost(s1, size_t(s1len), s2, size_t(s2len)),
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
					- digest_comparison_score_t(2 * blockhash_comparison_params::min_match_len),
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
			assert(s1len >= blockhash_comparison_params::min_match_len && s1len <= digest_params::max_blockhash_len);
			assert(s2len >= blockhash_comparison_params::min_match_len && s2len <= digest_params::max_blockhash_len);
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
				(s1len and s2len should be at least blockhash_comparison_params::min_match_len).
			*/
			#ifdef FFUZZYPP_DEBUG
			assert(s1len >= blockhash_comparison_params::min_match_len && s1len <= digest_params::max_blockhash_len);
			assert(s2len >= blockhash_comparison_params::min_match_len && s2len <= digest_params::max_blockhash_len);
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
			static_assert(digest_params::max_blockhash_len / 2 >= blockhash_comparison_params::min_match_len,
				"max_blockhash_len / 2 must be at least blockhash_comparison_params::min_match_len.");
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
			assert(s1len >= blockhash_comparison_params::min_match_len && s1len <= digest_params::max_blockhash_len);
			assert(s2len >= blockhash_comparison_params::min_match_len && s2len <= digest_params::max_blockhash_len);
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
			assert(s1len >= blockhash_comparison_params::min_match_len && s1len <= digest_params::max_blockhash_len);
			assert(s2len >= blockhash_comparison_params::min_match_len && s2len <= digest_params::max_blockhash_len);
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
			assert(s1len >= blockhash_comparison_params::min_match_len && s1len <= digest_params::max_blockhash_len);
			assert(s2len >= blockhash_comparison_params::min_match_len && s2len <= digest_params::max_blockhash_len);
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
			assert(s1len >= blockhash_comparison_params::min_match_len && s1len <= digest_params::max_blockhash_len);
			assert(s2len >= blockhash_comparison_params::min_match_len && s2len <= digest_params::max_blockhash_len);
			#endif
			return CONST_capped_score(
				uncapped_max_matching_score(s1len, s2len),
				blocksize, s1len, s2len
			);
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
			assert(s1len >= blockhash_comparison_params::min_match_len && s1len <= digest_params::max_blockhash_len);
			assert(s2len >= blockhash_comparison_params::min_match_len && s2len <= digest_params::max_blockhash_len);
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
			assert(s1len >= blockhash_comparison_params::min_match_len && s1len <= digest_params::max_blockhash_len);
			assert(s2len >= blockhash_comparison_params::min_match_len && s2len <= digest_params::max_blockhash_len);
			#endif
			if (s1len == s2len)
				return score_identical_2_9(s1len, blocksize);
			return capped_max_matching_score(blocksize, s1len, s2len);
		}
	};

	template <comparison_version Version>
	class blockhash_comparison_base_1;

	template <>
	class blockhash_comparison_base_1<comparison_version::v2_13>
		: public blockhash_comparison_base
	{
	private:
		blockhash_comparison_base_1(void) = delete;
		blockhash_comparison_base_1(const blockhash_comparison_base_1&) = delete;
	public:
		static constexpr digest_comparison_score_t score_identical(
			blockhash_len_t blockhash_len,
			digest_blocksize_t blocksize
		) noexcept
		{
			return 100;
		}
	};

	template <>
	class blockhash_comparison_base_1<comparison_version::v2_9>
		: public blockhash_comparison_base
	{
	private:
		blockhash_comparison_base_1(void) = delete;
		blockhash_comparison_base_1(const blockhash_comparison_base_1&) = delete;
	public:
		static constexpr digest_comparison_score_t score_identical(
			blockhash_len_t blockhash_len,
			digest_blocksize_t blocksize
		) noexcept
		{
			return blockhash_comparison_base::score_identical_2_9(blockhash_len, blocksize);
		}
	};
}

template <comparison_version Version = comparison_version::latest>
class blockhash_comparison
	: public internal::blockhash_comparison_base_1<Version>
{
private:
	blockhash_comparison(void) = delete;
	blockhash_comparison(const blockhash_comparison&) = delete;
private:
	typedef internal::blockhash_comparison_base_1<Version> base_type;
public:
	// Capped maximum score for given block hash lengths (s1len <= s2len) and block size
	static digest_comparison_score_t max_matching_score_le(
		digest_blocksize_t blocksize,
		blockhash_len_t s1len,
		blockhash_len_t s2len
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(s1len >= blockhash_comparison_params::min_match_len && s1len <= digest_params::max_blockhash_len);
		assert(s2len >= blockhash_comparison_params::min_match_len && s2len <= digest_params::max_blockhash_len);
		assert(s1len <= s2len);
		#endif
		if (s1len == s2len)
			return base_type::score_identical(s1len, blocksize);
		return base_type::capped_max_matching_score_le(blocksize, s1len, s2len);
	}
	// Capped maximum score for given block hash lengths and block size
	static digest_comparison_score_t max_matching_score(
		digest_blocksize_t blocksize,
		blockhash_len_t s1len,
		blockhash_len_t s2len
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(s1len >= blockhash_comparison_params::min_match_len && s1len <= digest_params::max_blockhash_len);
		assert(s2len >= blockhash_comparison_params::min_match_len && s2len <= digest_params::max_blockhash_len);
		#endif
		if (s1len == s2len)
			return base_type::score_identical(s1len, blocksize);
		return base_type::capped_max_matching_score(blocksize, s1len, s2len);
	}
};



namespace internal
{
	template <comparison_version Version>
	class digest_comparison_base
	{
	private:
		digest_comparison_base(void) = delete;
		digest_comparison_base(const digest_comparison_base&) = delete;

		// Comparison (on identical digests)
	public:
		template <bool IsAlphabetRestricted, bool IsShort>
		static digest_comparison_score_t compare_identical_2_9(
			const digest_data<IsAlphabetRestricted, IsShort>& a
		) noexcept
		{
			if (digest_blocksize::is_safe_to_double(a.blksize))
			{
				return std::max(
					blockhash_comparison<Version>::score_identical(a.blkhash1_len, a.blksize),
					blockhash_comparison<Version>::score_identical(a.blkhash2_len, a.blksize * 2)
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
				return blockhash_comparison<Version>::score_identical(a.blkhash1_len, a.blksize);
			}
		}

		// Comparison (on different digests)
	public:
		template <bool IsAlphabetRestricted, bool IsShort>
		static digest_comparison_score_t compare_near_diff(
			const digest_data<IsAlphabetRestricted, IsShort>& a,
			const digest_data<IsAlphabetRestricted, IsShort>& b
		) noexcept
		{
			#ifdef FFUZZYPP_DEBUG
			assert(a.is_valid() && a.is_normalized());
			assert(b.is_valid() && b.is_normalized());
			assert(digest_blocksize::is_near(a.blksize, b.blksize));
			assert(a != b);
			#endif
			if (digest_blocksize::is_safe_to_double(a.blksize))
			{
				if (digest_blocksize::is_near_eq(a.blksize, b.blksize))
					return std::max(
						blockhash_comparison<Version>::score(
							a.digest, a.blkhash1_len,
							b.digest, b.blkhash1_len,
							a.blksize),
						blockhash_comparison<Version>::score(
							a.digest+a.blkhash1_len, a.blkhash2_len,
							b.digest+b.blkhash1_len, b.blkhash2_len,
							a.blksize * 2));
				else if (digest_blocksize::is_near_lt(a.blksize, b.blksize))
					return blockhash_comparison<Version>::score(
						a.digest+a.blkhash1_len, a.blkhash2_len,
						b.digest, b.blkhash1_len,
						b.blksize);
				else // digest_blocksize::is_near_gt
					return blockhash_comparison<Version>::score(
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
					static_assert(blockhash_comparison_params::min_match_len > 1,
						"if the block size is not safe to double, the second block hash should not match "
						"(due to implementation restrictions).");
					#if defined(FFUZZYPP_DEBUG) && 0
					assert(a.blkhash2_len <= 1);
					assert(b.blkhash2_len <= 1);
					#endif
					return blockhash_comparison<Version>::score(
						a.digest, a.blkhash1_len,
						b.digest, b.blkhash1_len,
						a.blksize);
				}
				else if (digest_blocksize::is_near_gt(a.blksize, b.blksize))
					return blockhash_comparison<Version>::score(
						a.digest, a.blkhash1_len,
						b.digest+b.blkhash1_len, b.blkhash2_len,
						a.blksize);
				else // overflow (no common substring)
					return 0;
			}
		}
		template <bool IsAlphabetRestricted, bool IsShort>
		static digest_comparison_score_t compare_near_diff(
			const digest_position_array_base<IsAlphabetRestricted>& a,
			const digest_data<IsAlphabetRestricted, IsShort>& b
		) noexcept
		{
			#ifdef FFUZZYPP_DEBUG
			assert(a.is_valid());
			assert(b.is_valid() && b.is_normalized());
			assert(digest_blocksize::is_near(a.blksize, b.blksize));
			assert(a != b);
			#endif
			if (digest_blocksize::is_safe_to_double(a.blksize))
			{
				if (digest_blocksize::is_near_eq(a.blksize, b.blksize))
					return std::max(
						blockhash_comparison<Version>::score(
							a.blkhash1, a.blkhash1_len,
							b.digest, b.blkhash1_len,
							a.blksize),
						blockhash_comparison<Version>::score(
							a.blkhash2, a.blkhash2_len,
							b.digest+b.blkhash1_len, b.blkhash2_len,
							a.blksize * 2));
				else if (digest_blocksize::is_near_lt(a.blksize, b.blksize))
					return blockhash_comparison<Version>::score(
						a.blkhash2, a.blkhash2_len,
						b.digest, b.blkhash1_len,
						b.blksize);
				else // digest_blocksize::is_near_gt
					return blockhash_comparison<Version>::score(
						a.blkhash1, a.blkhash1_len,
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
					static_assert(blockhash_comparison_params::min_match_len > 1,
						"if the block size is not safe to double, the second block hash should not match "
						"(due to implementation restrictions).");
					#if defined(FFUZZYPP_DEBUG) && 0
					assert(a.blkhash2_len <= 1);
					assert(b.blkhash2_len <= 1);
					#endif
					return blockhash_comparison<Version>::score(
						a.blkhash1, a.blkhash1_len,
						b.digest, b.blkhash1_len,
						a.blksize);
				}
				else if (digest_blocksize::is_near_gt(a.blksize, b.blksize))
					return blockhash_comparison<Version>::score(
						a.blkhash1, a.blkhash1_len,
						b.digest+b.blkhash1_len, b.blkhash2_len,
						a.blksize);
				else // overflow (no common substring)
					return 0;
			}
		}
		template <bool IsAlphabetRestricted, bool IsShort>
		static digest_comparison_score_t compare_diff(
			const digest_data<IsAlphabetRestricted, IsShort>& a,
			const digest_data<IsAlphabetRestricted, IsShort>& b
		) noexcept
		{
			if (!digest_blocksize::is_near(a.blksize, b.blksize))
				return 0;
			return compare_near_diff(a, b);
		}
		template <bool IsAlphabetRestricted, bool IsShort>
		static digest_comparison_score_t compare_diff(
			const digest_position_array_base<IsAlphabetRestricted>& a,
			const digest_data<IsAlphabetRestricted, IsShort>& b
		) noexcept
		{
			if (!digest_blocksize::is_near(a.blksize, b.blksize))
				return 0;
			return compare_near_diff(a, b);
		}

		// Comparison (on different digests; specialized versions)
	public:
		template <bool IsAlphabetRestricted, bool IsShort>
		static digest_comparison_score_t compare_near_eq_diff(
			const digest_data<IsAlphabetRestricted, IsShort>& a,
			const digest_data<IsAlphabetRestricted, IsShort>& b
		) noexcept
		{
			#ifdef FFUZZYPP_DEBUG
			assert(a.is_valid() && a.is_normalized());
			assert(b.is_valid() && b.is_normalized());
			assert(digest_blocksize::is_near_eq(a.blksize, b.blksize));
			assert(a != b);
			#endif
			if (digest_blocksize::is_safe_to_double(a.blksize))
				return std::max(
					blockhash_comparison<Version>::score(
						a.digest, a.blkhash1_len,
						b.digest, b.blkhash1_len,
						a.blksize),
					blockhash_comparison<Version>::score(
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
				static_assert(blockhash_comparison_params::min_match_len > 1,
					"if the block size is not safe to double, the second block hash should not match "
					"(due to implementation restrictions).");
				#if defined(FFUZZYPP_DEBUG) && 0
				assert(a.blkhash2_len <= 1);
				assert(b.blkhash2_len <= 1);
				#endif
				return blockhash_comparison<Version>::score(
					a.digest, a.blkhash1_len,
					b.digest, b.blkhash1_len,
					a.blksize);
			}
		}
		template <bool IsAlphabetRestricted, bool IsShort>
		static digest_comparison_score_t compare_near_eq_diff(
			const digest_position_array_base<IsAlphabetRestricted>& a,
			const digest_data<IsAlphabetRestricted, IsShort>& b
		) noexcept
		{
			#ifdef FFUZZYPP_DEBUG
			assert(a.is_valid());
			assert(b.is_valid() && b.is_normalized());
			assert(digest_blocksize::is_near_eq(a.blksize, b.blksize));
			assert(a != b);
			#endif
			if (digest_blocksize::is_safe_to_double(a.blksize))
				return std::max(
					blockhash_comparison<Version>::score(
						a.blkhash1, a.blkhash1_len,
						b.digest, b.blkhash1_len,
						a.blksize),
					blockhash_comparison<Version>::score(
						a.blkhash2, a.blkhash2_len,
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
				static_assert(blockhash_comparison_params::min_match_len > 1,
					"if the block size is not safe to double, the second block hash should not match "
					"(due to implementation restrictions).");
				#if defined(FFUZZYPP_DEBUG) && 0
				assert(a.blkhash2_len <= 1);
				assert(b.blkhash2_len <= 1);
				#endif
				return blockhash_comparison<Version>::score(
					a.blkhash1, a.blkhash1_len,
					b.digest, b.blkhash1_len,
					a.blksize);
			}
		}
		template <bool IsAlphabetRestricted, bool IsShort>
		static digest_comparison_score_t compare_near_lt(
			const digest_data<IsAlphabetRestricted, IsShort>& a,
			const digest_data<IsAlphabetRestricted, IsShort>& b
		) noexcept
		{
			#ifdef FFUZZYPP_DEBUG
			assert(a.is_valid() && a.is_normalized());
			assert(b.is_valid() && b.is_normalized());
			assert(digest_blocksize::is_near_lt(a.blksize, b.blksize));
			assert(a != b);
			#endif
			// implies digest_blocksize::is_safe_to_double(a.blksize)
			return blockhash_comparison<Version>::score(
				a.digest+a.blkhash1_len, a.blkhash2_len,
				b.digest, b.blkhash1_len,
				b.blksize);
		}
		template <bool IsAlphabetRestricted, bool IsShort>
		static digest_comparison_score_t compare_near_lt(
			const digest_position_array_base<IsAlphabetRestricted>& a,
			const digest_data<IsAlphabetRestricted, IsShort>& b
		) noexcept
		{
			#ifdef FFUZZYPP_DEBUG
			assert(a.is_valid());
			assert(b.is_valid() && b.is_normalized());
			assert(digest_blocksize::is_near_lt(a.blksize, b.blksize));
			assert(a != b);
			#endif
			// implies digest_blocksize::is_safe_to_double(a.blksize)
			return blockhash_comparison<Version>::score(
				a.blkhash2, a.blkhash2_len,
				b.digest, b.blkhash1_len,
				b.blksize);
		}
		template <bool IsAlphabetRestricted, bool IsShort>
		static digest_comparison_score_t compare_near_lt(
			const digest_data<IsAlphabetRestricted, IsShort>& a,
			const digest_position_array_base<IsAlphabetRestricted>& b
		) noexcept
		{
			#ifdef FFUZZYPP_DEBUG
			assert(a.is_valid() && a.is_normalized());
			assert(b.is_valid());
			assert(digest_blocksize::is_near_lt(a.blksize, b.blksize));
			assert(a != b);
			#endif
			// implies digest_blocksize::is_safe_to_double(a.blksize)
			return blockhash_comparison<Version>::score(
				b.blkhash1, b.blkhash1_len,
				a.digest+a.blkhash1_len, a.blkhash2_len,
				b.blksize);
		}
	};

	template <comparison_version Version>
	class digest_comparison_base_1;

	template <>
	class digest_comparison_base_1<comparison_version::v2_13>
		: public digest_comparison_base<comparison_version::v2_13>
	{
	private:
		digest_comparison_base_1(void) = delete;
		digest_comparison_base_1(const digest_comparison_base_1&) = delete;
	public:
		template <bool IsAlphabetRestricted, bool IsShort>
		static digest_comparison_score_t compare_identical(
			const digest_data<IsAlphabetRestricted, IsShort>& a
		) noexcept
		{
			return 100;
		}
	};

	template <>
	class digest_comparison_base_1<comparison_version::v2_9>
		: public digest_comparison_base<comparison_version::v2_9>
	{
	private:
		digest_comparison_base_1(void) = delete;
		digest_comparison_base_1(const digest_comparison_base_1&) = delete;
	public:
		template <bool IsAlphabetRestricted, bool IsShort>
		static digest_comparison_score_t compare_identical(
			const digest_data<IsAlphabetRestricted, IsShort>& a
		) noexcept
		{
			return digest_comparison_base<comparison_version::v2_9>::compare_identical_2_9(a);
		}
	};
}

template <comparison_version Version = comparison_version::latest>
class digest_comparison
	: public internal::digest_comparison_base_1<Version>
{
private:
	digest_comparison(void) = delete;
	digest_comparison(const digest_comparison&) = delete;
private:
	typedef internal::digest_comparison_base_1<Version> base_type;

	// Comparison (possibly equivalent)
public:
	template <bool IsAlphabetRestricted, bool IsShort>
	static digest_comparison_score_t compare_near(
		const digest_data<IsAlphabetRestricted, IsShort>& a,
		const digest_data<IsAlphabetRestricted, IsShort>& b
	) noexcept
	{
		if (a == b)
			return base_type::compare_identical(b);
		return base_type::compare_near_diff(a, b);
	}
	template <bool IsAlphabetRestricted, bool IsShort>
	static digest_comparison_score_t compare_near(
		const digest_position_array_base<IsAlphabetRestricted>& a,
		const digest_data<IsAlphabetRestricted, IsShort>& b
	) noexcept
	{
		if (a == b)
			return base_type::compare_identical(b);
		return base_type::compare_near_diff(a, b);
	}

	// Specialized comparison (possibly equivalent)
public:
	template <bool IsAlphabetRestricted, bool IsShort>
	static digest_comparison_score_t compare_near_eq(
		const digest_data<IsAlphabetRestricted, IsShort>& a,
		const digest_data<IsAlphabetRestricted, IsShort>& b
	) noexcept
	{
		if (digest_data<IsAlphabetRestricted, IsShort>::is_eq_except_blocksize(a, b))
			return base_type::compare_identical(b);
		return base_type::compare_near_eq_diff(a, b);
	}
	template <bool IsAlphabetRestricted, bool IsShort>
	static digest_comparison_score_t compare_near_eq(
		const digest_position_array_base<IsAlphabetRestricted>& a,
		const digest_data<IsAlphabetRestricted, IsShort>& b
	) noexcept
	{
		if (digest_position_array_base<IsAlphabetRestricted>::is_eq_except_blocksize(a, b))
			return base_type::compare_identical(b);
		return base_type::compare_near_eq_diff(a, b);
	}

	// Comparison (for normalized digests)
public:
	template <bool IsAlphabetRestricted, bool IsShort>
	static digest_comparison_score_t compare(
		const digest_data<IsAlphabetRestricted, IsShort>& a,
		const digest_data<IsAlphabetRestricted, IsShort>& b
	) noexcept
	{
		if (!digest_blocksize::is_near(a.blksize, b.blksize))
			return 0;
		return compare_near(a, b);
	}
	template <bool IsAlphabetRestricted, bool IsShort>
	static digest_comparison_score_t compare(
		const digest_position_array_base<IsAlphabetRestricted>& a,
		const digest_data<IsAlphabetRestricted, IsShort>& b
	) noexcept
	{
		if (!digest_blocksize::is_near(a.blksize, b.blksize))
			return 0;
		return compare_near(a, b);
	}

	// Comparison (for unnormalized form of digests)
public:
	template <bool IsAlphabetRestricted, bool IsShort>
	static digest_comparison_score_t compare_unnormalized(
		const digest_data<IsAlphabetRestricted, IsShort>& a,
		const digest_data<IsAlphabetRestricted, IsShort>& b
	) noexcept
	{
		return compare(
			digest_data<IsAlphabetRestricted, IsShort>::normalize(a),
			digest_data<IsAlphabetRestricted, IsShort>::normalize(b)
		);
	}

	// Comparison (for strings)
public:
	template <bool IsAlphabetRestricted = false, bool IsShort = false>
	static digest_comparison_score_t compare(
		const char* a,
		const char* b
	) noexcept(false)
	{
		digest_data<IsAlphabetRestricted, IsShort> da, db;
		if (!digest_data<IsAlphabetRestricted, IsShort>::parse_normalized(da, a))
			throw digest_parse_error();
		if (!digest_data<IsAlphabetRestricted, IsShort>::parse_normalized(db, b))
			throw digest_parse_error();
		return compare(da, db);
	}
	template <bool IsShort = false>
	static digest_comparison_score_t compare(
		const std::string& a,
		const std::string& b
	)
	{
		return compare<false, IsShort>(a.c_str(), b.c_str());
	}
};

}

#endif
