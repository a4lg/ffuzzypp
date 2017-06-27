/*

	ffuzzy++ : C++ implementation of fast fuzzy hashing

	digest_filesize.hpp
	File size utilities

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
#ifndef FFUZZYPP_DIGEST_FILESIZE_HPP
#define FFUZZYPP_DIGEST_FILESIZE_HPP

#include <cstddef>
#include <cstdint>

#include "digest_blocksize.hpp"
#include "digest_data.hpp"
#include "utils/safe_int.hpp"

namespace ffuzzy {

typedef uint_least64_t digest_filesize_t;

// File size utilities
class digest_filesize
{
	static_assert(safe_int::contracts::is_unsigned_integral_type<digest_filesize_t>(),
		"digest_filesize_t must be an unsigned integral type.");
private:
	digest_filesize(void) = delete;
	digest_filesize(const digest_filesize&) = delete;

	// Minimum size (as supported by ssdeep)
private:
	static constexpr const unsigned long u_min_supported_size = 4096 + 1;
public:
	static_assert(u_min_supported_size <= 0xfffffffful,
		"u_min_supported_size must be in range of 32-bit unsigned integer.");
	static_assert(safe_int::uvalue<digest_filesize_t, u_min_supported_size>::is_valid,
		"u_min_supported_size must be in range of digest_filesize_t.");
	static constexpr const digest_filesize_t min_supported_size = u_min_supported_size;

	// Maximum size (theoretical limit)
public:
	static_assert(safe_int::safe_add<
			safe_int::safe_mul<
				safe_int::uvalue<digest_filesize_t, digest_blocksize::max_blocksize>,
				safe_int::uvalue<digest_filesize_t, digest_params::max_blockhash_len>
			>,
			safe_int::uvalue<digest_filesize_t, 1>
		>::is_valid,
		"max_blocksize * max_blockhash_len + 1 (== max_size + 1) must be "
		"in range of digest_filesize_t.");
	static constexpr const digest_filesize_t max_size =
		digest_filesize_t(digest_blocksize::max_blocksize) *
		digest_filesize_t(digest_params::max_blockhash_len);
	static constexpr const digest_filesize_t max_theoretical_size = max_size;

	// Maximum portable size (with ssdeep 2.6-2.12)
public:
	static_assert(safe_int::uvalue<digest_filesize_t, 0xfffffffful>::is_valid,
		"digest_filesize_t must be able to represent all 32-bit unsigned integer values.");
	static constexpr const digest_filesize_t max_portable_size =
		minmax::min(max_size, digest_filesize_t(0xfffffffful));
	static_assert(min_supported_size <= max_portable_size,
		"min_supported_size must not exceed max_portable_size.");

	// Predicates
public:
	static constexpr bool is_portable(digest_filesize_t total_size) noexcept
	{
		/*
			WARNING: Given size may not be "portable" on insane architectures.

			Portability:
			* ffuzzy++ (3.0)
			* ssdeep (including 2.6-2.12)

			Note:
			Mr.Kornblum (the original author of ssdeep) considers
			version 2.9 the "standard". Note that this release does not
			support files equal to or larger than 4GiB.
		*/
		return total_size >= min_supported_size && total_size <= max_portable_size;
	}
	static constexpr bool is_supported_by_ssdeep_2_12(digest_filesize_t total_size) noexcept
	{
		return total_size >= min_supported_size && total_size <= max_portable_size;
	}
	static constexpr bool is_supported_by_ffuzzy_3_0(digest_filesize_t total_size) noexcept
	{
		return total_size <= max_theoretical_size;
	}
	static constexpr bool is_supported(digest_filesize_t total_size) noexcept
	{
		// whether supported by "this" version of ffuzzy++
		return is_supported_by_ffuzzy_3_0(total_size);
	}

	// Predicate (equivalent to "not meaningful results" in ssdeep)
public:
	static constexpr bool is_not_meaningful(digest_filesize_t total_size) noexcept
	{
		return total_size < min_supported_size;
	}
};


}

#endif
