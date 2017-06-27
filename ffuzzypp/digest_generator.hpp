/*

	ffuzzy++ : C++ implementation of fast fuzzy hashing

	digest_generator.hpp
	Fuzzy digest generator


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
#ifndef FFUZZYPP_DIGEST_GENERATOR_HPP
#define FFUZZYPP_DIGEST_GENERATOR_HPP

#include <cassert>
#include <cstdint>
#include <cstddef>

#include <algorithm>
#include <string>

#include "base64.hpp"
#include "context_hash.hpp"
#include "context_hash_fast.hpp"
#include "rolling_hash.hpp"
#include "rolling_hash_ssdeep.hpp"
#include "digest_blocksize.hpp"
#include "digest_data.hpp"
#include "digest_base.hpp"
#include "digest_filesize.hpp"
#include "utils/likely.hpp"
#include "utils/safe_int.hpp"
#include "strings/transform.hpp"
#include "strings/sequences.hpp"
#include "strings/nosequences.hpp"

namespace ffuzzy {

struct digest_generator_error {};

class digest_generator
{
	// Digest characters and transformation for them
private:
	/*
		This type contains digest characters as Base64 indices.
		To distinguish valid and invalid characters,
		we need a "nil" value.

		We transform Base64 indices to actual Base64 characters
		when we are copying final digest value.
	*/
	typedef base64::transform_to_b64 digest_transform_t;
	static constexpr const char digest_nil = 64;
	static_assert(digest_nil < 0 || digest_nil > 63,
		"digest_nil must be out of Base64 range.");

	// Heuristics to guess maximum file size from block size (or its index)
public:
	static constexpr digest_filesize_t guessed_filesize(digest_blocksize_t blocksize) noexcept
	{
		return digest_filesize_t(blocksize) * digest_params::max_blockhash_len;
	}
	static constexpr digest_filesize_t guessed_filesize_at(unsigned i) noexcept
	{
		return guessed_filesize(digest_blocksize::at(i));
	}

	// Heuristic to guess block size from file size
public:
	static unsigned blockhash_index_guessed_by_filesize(digest_filesize_t size, unsigned start = 0) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(start < digest_blocksize::number_of_blockhashes);
		assert(size <= digest_filesize::max_size);
		#endif
		unsigned bi = start;
		digest_filesize_t bgs = guessed_filesize_at(bi);
		while (bgs < size)
		{
			bi++;
			bgs *= 2;
		}
		return bi;
	}

	// Data Structure
private:
	typedef context_hash_fast context_hash_t;
	struct blockhash_context
	{
		context_hash_t  hfull;
		context_hash_t  hhalf;
		char digest[digest_params::max_blockhash_len];
		char digesth;
		blockhash_len_t dindex;
	};
	blockhash_context bh[digest_blocksize::number_of_blockhashes];
	digest_filesize_t totalsz;
	digest_filesize_t totalsz_constant;
	digest_filesize_t reduce_border;
	rolling_hash roll;
	unsigned bhstart;
	unsigned bhend;
	unsigned bhendlimit;
	uint_least32_t rollmask;
	context_hash_t hlast;
	unsigned flags;
	static constexpr const unsigned FLAG_LASTHASH   = 0x1;
	static constexpr const unsigned FLAG_SZCONSTANT = 0x2;

	// Simple data structure manipulation
public:
	bool is_total_size_clamped(void) const noexcept { return totalsz > digest_filesize::max_size; }
	bool is_file_size_constant(void) const noexcept { return flags & FLAG_SZCONSTANT; }
	unsigned blockhash_index_start(void) const noexcept { return bhstart; }
	unsigned blockhash_index_end(void)   const noexcept { return bhend; }
	unsigned blockhash_index_end_limit(void) const noexcept { return bhendlimit; }
	digest_filesize_t total_size(void) const noexcept { return totalsz; }
	// constant file size (for fast ssdeep hashing)
	digest_filesize_t get_file_size_constant(void) const noexcept { return totalsz_constant; }
	bool set_file_size_constant(digest_filesize_t size) noexcept
	{
		if (is_file_size_constant() && totalsz_constant != size)
			return false;
		if (size > digest_filesize::max_size)
			return false;
		totalsz_constant = size;
		bhendlimit = std::min(
			digest_blocksize::number_of_blockhashes - 1,
			blockhash_index_guessed_by_filesize(size) + 1
		);
		flags |= FLAG_SZCONSTANT;
		return true;
	}

public:
	// Reset minimum context required
	void reset(void) noexcept
	{
		bh[0].hfull.reset();
		bh[0].hhalf.reset();
		bh[0].digest[digest_params::max_blockhash_len - 1] = digest_nil;
		bh[0].digesth = digest_nil;
		bh[0].dindex = 0;
		totalsz = 0;
		reduce_border = guessed_filesize_at(0);
		roll.reset();
		bhstart = 0;
		bhend   = 1;
		bhendlimit = digest_blocksize::number_of_blockhashes - 1;
		rollmask = 0;
		flags = 0;
	}

	// Update functions (by buffer or by character)
public:
	void update(const unsigned char* buf, size_t len) noexcept
	{
		rolling_hash r = roll;
		if (FFUZZYPP_UNLIKELY(len > digest_filesize::max_size
			|| digest_filesize::max_size - digest_filesize_t(len) < totalsz))
		{
			totalsz = digest_filesize::max_size + 1;
		}
		else
		{
			totalsz += digest_filesize_t(len);
		}
		while (len--)
		{
			unsigned char c = *buf++;
			r.update(c);
			for (unsigned i = bhstart; i < bhend; i++)
			{
				bh[i].hfull.update(c);
				bh[i].hhalf.update(c);
			}
			if (flags & FLAG_LASTHASH)
				hlast.update(c);
			uint_least32_t horg = (r.sum() + 1) & uint_least32_t(0xfffffffful);
			uint_least32_t h = horg / uint_least32_t(digest_blocksize::min_blocksize);
			if (0xfffffffful % digest_blocksize::min_blocksize != digest_blocksize::min_blocksize - 1 && !horg)
				continue;
			if (FFUZZYPP_LIKELY(h & rollmask))
				continue;
			if (horg % uint_least32_t(digest_blocksize::min_blocksize))
				continue;
			h >>= bhstart;
			unsigned i = bhstart;
			do
			{
				if (FFUZZYPP_UNLIKELY(bh[i].dindex == 0))
				{
					// fork to prepare larger block sizes
					if (bhend > bhendlimit)
					{
						if (bhendlimit == digest_blocksize::number_of_blockhashes - 1
							&& !(flags & FLAG_LASTHASH))
						{
							hlast = bh[i].hfull;
							flags |= FLAG_LASTHASH;
						}
					}
					else
					{
						bh[i+1].hfull = bh[i].hfull;
						bh[i+1].hhalf = bh[i].hhalf;
						bh[i+1].digest[digest_params::max_blockhash_len - 1] = digest_nil;
						bh[i+1].digesth = digest_nil;
						bh[i+1].dindex = 0;
						bhend++;
					}
				}
				bh[i].digest[bh[i].dindex] = bh[i].hfull.sum_in_base64();
				bh[i].digesth = bh[i].hhalf.sum_in_base64();
				if (bh[i].dindex < digest_params::max_blockhash_len - 1)
				{
					bh[i].dindex++;
					bh[i].hfull.reset();
					if (bh[i].dindex < digest_params::max_blockhash_len / 2)
					{
						bh[i].digesth = digest_nil;
						bh[i].hhalf.reset();
					}
				}
				// eliminate block sizes which will not be chosen
				else if (FFUZZYPP_UNLIKELY(bhend - bhstart >= 2
					&& reduce_border < (is_file_size_constant() ? totalsz_constant : totalsz)
					&& bh[i+1].dindex >= digest_params::max_blockhash_len / 2))
				{
					bhstart++;
					rollmask = rollmask * 2 + 1;
					reduce_border *= 2;
				}
				if (h & 1)
					break;
				h >>= 1;
			} while (++i < bhend);
		}
		roll = r;
	}
	void update(unsigned char c) noexcept
	{
		unsigned char C(c);
		update(&C, 1);
	}

	// High-level update utilities
	// (by file pointer or by file name; w/ or w/o internal buffer)
public:
	static constexpr const size_t default_buffer_size = 4096;
	template <size_t buffer_size = default_buffer_size>
	bool update_by_stream(FILE* fp, unsigned char* tmpbuf) noexcept
	{
		static_assert(buffer_size != 0, "buffer_size must not be zero.");
		if (!fp)
			return false;
		while (true)
		{
			size_t n = fread(tmpbuf, 1, buffer_size, fp);
			if (n == 0)
				break;
			update(tmpbuf, n);
		}
		if (feof(fp))
			return true;
		return false;
	}
	template <size_t buffer_size = default_buffer_size>
	bool update_by_stream(FILE* fp) noexcept
	{
		static_assert(buffer_size != 0, "buffer_size must not be zero.");
		unsigned char buf[buffer_size];
		return update_by_stream<buffer_size>(fp, buf);
	}
	template <size_t buffer_size = default_buffer_size>
	bool update_by_file(const char* filename) noexcept
	{
		static_assert(buffer_size != 0, "buffer_size must not be zero.");
		FILE* fp = fopen(filename, "rb");
		if (!fp)
			return false;
		bool ret = update_by_stream<buffer_size>(fp);
		fclose(fp);
		return ret;
	}
	template <size_t buffer_size = default_buffer_size>
	bool update_by_file(const char* filename, unsigned char* tmpbuf) noexcept
	{
		static_assert(buffer_size != 0, "buffer_size must not be zero.");
		FILE* fp = fopen(filename, "rb");
		if (!fp)
			return false;
		bool ret = update_by_stream<buffer_size>(fp, tmpbuf);
		fclose(fp);
		return ret;
	}

	// Digest finalization
private:
	// Heuristic to guess block hash index to start from the current state
	unsigned blockhash_index_guessed_to_start(void) const noexcept
	{
		unsigned bi = blockhash_index_guessed_by_filesize(totalsz, bhstart);
		#ifdef FFUZZYPP_DEBUG
		assert(bi < digest_blocksize::number_of_blockhashes);
		#endif
		bi = std::min(bi, bhend - 1);
		while (bi > bhstart && bh[bi].dindex < digest_params::max_blockhash_len / 2)
			bi--;
		#ifdef FFUZZYPP_DEBUG
		assert(bi >= bhstart && bi < bhend);
		assert(bi == 0 || bh[bi].dindex >= digest_params::max_blockhash_len / 2);
		#endif
		return bi;
	}
	// Copy the final (resulting) digest
	template <typename Tseq, bool IsAlphabetRestricted, bool IsShort, bool Shortened>
	bool copy_digest_internal(digest_data<IsAlphabetRestricted, IsShort>& digest) noexcept
	{
		/*
			This function is not exactly "const" but mostly constant.
			You can call this function multiple times as you need.
		*/
		static_assert(Shortened == true || IsShort == false,
			"copying long result to short digest_data structure is prohibited.");
		if (is_total_size_clamped())
			return false;
		if (is_file_size_constant() && totalsz != totalsz_constant)
			return false;
		unsigned bi = blockhash_index_guessed_to_start();
		digest.blksize = digest_blocksize::at(bi);

		uint_least32_t rh = roll.sum();
		// Copy first block hash (digest)
		{
			char chtmp = bh[bi].digest[digest_params::max_blockhash_len - 1];
			size_t sz = bh[bi].dindex;
			if (rh != 0)
				bh[bi].digest[sz++] = bh[bi].hfull.sum_in_base64();
			else if (chtmp != digest_nil)
				sz++;
			digest.blkhash1_len = Tseq::copy_elim_sequences(digest.digest, bh[bi].digest, sz);
			bh[bi].digest[digest_params::max_blockhash_len - 1] = chtmp;
		}
		// Copy second block hash if we need
		if (bi < bhend - 1)
		{
			size_t dindex = bh[bi+1].dindex;
			if (Shortened)
				dindex = std::min(dindex, size_t(digest_params::max_blockhash_len / 2 - 1));
			char chtmp = bh[bi+1].digest[dindex];
			size_t sz = dindex;
			if (rh != 0)
			{
				bh[bi+1].digest[sz++] = Shortened
					? bh[bi+1].hhalf.sum_in_base64()
					: bh[bi+1].hfull.sum_in_base64();
			}
			else
			{
				if (Shortened)
				{
					if (bh[bi+1].digesth != digest_nil)
						bh[bi+1].digest[sz++] = bh[bi+1].digesth;
				}
				else
				{
					if (dindex == digest_params::max_blockhash_len - 1 && chtmp != digest_nil)
						sz++;
				}
			}
			digest.blkhash2_len = Tseq::copy_elim_sequences(
				digest.digest + digest.blkhash1_len, bh[bi+1].digest, sz);
			bh[bi+1].digest[dindex] = chtmp;
		}
		else if (rh != 0)
		{
			#ifdef FFUZZYPP_DEBUG
			assert(bi == 0 || bi == digest_blocksize::number_of_blockhashes - 1);
			#endif
			if (bi == 0)
				digest.digest[digest.blkhash1_len] = bh[bi].hfull.sum_in_base64();
			else
				digest.digest[digest.blkhash1_len] = hlast.sum_in_base64();
			digest.blkhash2_len = Tseq::copy_elim_sequences(
				digest.digest + digest.blkhash1_len,
				digest.digest + digest.blkhash1_len,
				1);
		}
		else
		{
			digest.blkhash2_len = 0;
		}
		return true;
	}

	// Digest finalization (and copying) utilities
private:
	template <bool IsAlphabetRestricted, bool IsShort, bool Shortened>
	bool copy_digest_base(digest_data<IsAlphabetRestricted, IsShort>& digest) noexcept
	{
		return copy_digest_internal<
				strings::nosequences<
					typename digest_data_transformation<!IsAlphabetRestricted>::output_type
				>,
			IsAlphabetRestricted, IsShort, Shortened>(digest);
	}
public:
	template <bool IsAlphabetRestricted, bool IsShort, bool Shortened = true>
	bool copy_digest_normalized(digest_data<IsAlphabetRestricted, IsShort>& digest) noexcept
	{
		return copy_digest_internal<
			strings::sequences<
				digest_params::max_blockhash_sequence,
				typename digest_data_transformation<!IsAlphabetRestricted>::output_type
			>, IsAlphabetRestricted, IsShort, Shortened
		>(digest);
	}
	template <bool IsAlphabetRestricted, bool IsShort>
	bool copy_digest(digest_base<IsAlphabetRestricted, IsShort, true>& digest) noexcept
	{
		return copy_digest_normalized<IsAlphabetRestricted, IsShort, true>(digest);
	}
	template <bool IsAlphabetRestricted, bool IsShort>
	bool copy_digest(digest_base<IsAlphabetRestricted, IsShort, false>& digest) noexcept
	{
		return copy_digest_base<IsAlphabetRestricted, IsShort, true>(digest);
	}
public:
	template <bool IsAlphabetRestricted>
	bool copy_digest_long_normalized(digest_base<IsAlphabetRestricted, false, true>& digest) noexcept
	{
		return copy_digest_normalized<IsAlphabetRestricted, false, false>(digest);
	}
	template <bool IsAlphabetRestricted>
	bool copy_digest_long(digest_base<IsAlphabetRestricted, false, true>& digest) noexcept
	{
		return copy_digest_normalized<IsAlphabetRestricted, false, false>(digest);
	}
	template <bool IsAlphabetRestricted>
	bool copy_digest_long(digest_base<IsAlphabetRestricted, false, false>& digest) noexcept
	{
		return copy_digest_base<IsAlphabetRestricted, false, false>(digest);
	}
private:
	template <typename T, bool IsAlphabetRestricted, bool IsShort, bool IsNormalized, bool Shortened = true>
	T digest_in_type(void)
	{
		// This object to be erased by NRVO.
		T d;
		if (IsNormalized)
		{
			if (!copy_digest_normalized<IsAlphabetRestricted, IsShort, Shortened>(d))
				throw digest_generator_error();
		}
		else
		{
			if (!copy_digest_base<IsAlphabetRestricted, IsShort, Shortened>(d))
				throw digest_generator_error();
		}
		return d;
	}
public:
	digest_unorm_t digest(void)
	{
		// Default options as you need (unnormalized; short form)
		return digest_in_type<digest_unorm_t, false, true, false>();
	}
	std::string digest_str(void)
	{
		return digest().pretty();
	}
public:
	template <bool Shortened = false>
	digest_long_unorm_t digest_long(void)
	{
		return digest_in_type<digest_long_unorm_t, false, false, false, Shortened>();
	}
	digest_t digest_normalized(void)
	{
		return digest_in_type<digest_t, false, true, true>();
	}
	template <bool Shortened = false>
	digest_long_t digest_long_normalized(void)
	{
		return digest_in_type<digest_long_t, false, false, true, Shortened>();
	}
public:
	digest_ra_unorm_t digest_ra(void)
	{
		return digest_in_type<digest_ra_unorm_t, true, true, false>();
	}
	template <bool Shortened = false>
	digest_ra_long_unorm_t digest_ra_long(void)
	{
		return digest_in_type<digest_ra_long_unorm_t, true, false, false, Shortened>();
	}
	digest_ra_t digest_ra_normalized(void)
	{
		return digest_in_type<digest_ra_t, true, true, true>();
	}
	template <bool Shortened = false>
	digest_ra_long_t digest_ra_long_normalized(void)
	{
		return digest_in_type<digest_ra_long_t, true, false, true, Shortened>();
	}

	// Constructors
public:
	digest_generator(void) noexcept
	{
		reset();
	}
	digest_generator(const digest_generator&) noexcept = default;
};

}

#endif
