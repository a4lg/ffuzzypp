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

	Copyright (C) 2015 Tsukasa OI <floss_ssdeep@irq.a4lg.com>

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
#include "digest_filesize.hpp"
#include "digest.hpp"
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
	rolling_hash roll;
	unsigned bhstart;
	unsigned bhend;
	unsigned bhendlimit;
	context_hash_t hlast;
	bool is_szclamped;
	bool is_szconstant;
	bool is_lasthash;

	// Simple data structure manipulation
public:
	bool is_total_size_clamped(void) const noexcept { return is_szclamped; }
	bool is_file_size_constant(void) const noexcept { return is_szconstant; }
	unsigned blockhash_index_start(void) const noexcept { return bhstart; }
	unsigned blockhash_index_end(void)   const noexcept { return bhend; }
	unsigned blockhash_index_end_limit(void) const noexcept { return bhendlimit; }
	digest_filesize_t total_size(void) const noexcept { return totalsz; }
	// constant file size (for fast ssdeep hashing)
	digest_filesize_t get_file_size_constant(void) const noexcept { return totalsz_constant; }
	bool set_file_size_constant(digest_filesize_t size) noexcept
	{
		if (is_szconstant && totalsz_constant != size)
			return false;
		if (size > digest_filesize::max_size)
			return false;
		totalsz_constant = size;
		bhendlimit = std::min(
			digest_blocksize::number_of_blockhashes - 1,
			blockhash_index_guessed_by_filesize(size) + 1
		);
		is_szconstant = true;
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
		totalsz   = 0;
		roll.reset();
		bhstart = 0;
		bhend   = 1;
		bhendlimit = digest_blocksize::number_of_blockhashes - 1;
		is_szclamped  = false;
		is_szconstant = false;
		is_lasthash  = false;
	}

private:
	// Update hashing state by single character
	void update_internal(unsigned char c) noexcept
	{
		roll.update(c);
		// digest_blocksize_t is an unsigned integral type with at least 32-bit
		digest_blocksize_t h(roll.sum());
		for (unsigned i = bhstart; i < bhend; i++)
		{
			bh[i].hfull.update(c);
			bh[i].hhalf.update(c);
		}
		if (is_lasthash)
			hlast.update(c);
		digest_blocksize_t bs = digest_blocksize::at(bhstart);
		for (unsigned i = bhstart; i < bhend; i++, bs *= 2)
		{
			if ((h % bs) != bs - 1)
				break;
			if (bh[i].dindex == 0)
			{
				// fork to prepare larger block sizes
				#ifdef FFUZZYPP_DEBUG
				assert(i == bhend - 1);
				#endif
				if (bhend > bhendlimit)
				{
					if (bhendlimit == digest_blocksize::number_of_blockhashes - 1
						&& !is_lasthash)
					{
						hlast = bh[i].hfull;
						is_lasthash = true;
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
			else if (i == bhstart && bhend - bhstart >= 2
				&& guessed_filesize(bs) < (is_szconstant ? totalsz_constant : totalsz)
				&& bh[i+1].dindex >= digest_params::max_blockhash_len / 2)
			{
				// unlike fork to prepare larger block sizes,
				// we check i is bhstart first (because this test takes some time)
				#if 0
				assert(i == bhstart);
				#endif
				bhstart++;
			}
		}
	}

	// Total size manipulation
private:
	void step_size_1(void) noexcept
	{
		if (is_szclamped)
			return;
		if (totalsz++ == digest_filesize::max_size)
			is_szclamped = true;
	}
	void step_size_n(size_t len) noexcept
	{
		if (is_szclamped)
			return;
		if (len > digest_filesize::max_size
			|| digest_filesize::max_size - digest_filesize_t(len) < totalsz)
		{
			totalsz = digest_filesize::max_size + 1;
			is_szclamped = true;
		}
		else
			totalsz += digest_filesize_t(len);
	}

	// Update utilities (by character or by buffer)
public:
	void update(unsigned char c) noexcept
	{
		step_size_1();
		update_internal(c);
	}
	void update(const unsigned char* buf, size_t len) noexcept
	{
		step_size_n(len);
		while (len--)
			update_internal(*buf++);
	}

	// High-level update utilities
	// (by file pointer or by file name; w/ or w/o internal buffer)
public:
	template <size_t buffer_size = 4096>
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
	template <size_t buffer_size = 4096>
	bool update_by_stream(FILE* fp) noexcept
	{
		static_assert(buffer_size != 0, "buffer_size must not be zero.");
		unsigned char buf[buffer_size];
		return update_by_stream<buffer_size>(fp, buf);
	}
	template <size_t buffer_size = 4096>
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
	template <size_t buffer_size = 4096>
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
	template <typename Tseq, bool IsShort, bool Shortened>
	bool copy_digest_internal(digest_data<IsShort>& digest) noexcept
	{
		/*
			This function is not exactly "const" but mostly constant.
			You can call this function multiple times as you need.
		*/
		static_assert(Shortened == true || IsShort == false,
			"copying long result to short digest_data structure is prohibited.");
		if (is_szclamped)
			return false;
		if (is_szconstant && totalsz != totalsz_constant)
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
	template <bool IsShort, bool Shortened>
	bool copy_digest_base(digest_data<IsShort>& digest) noexcept
	{
		return copy_digest_internal<
				strings::nosequences<digest_transform_t>,
			IsShort, Shortened>(digest);
	}
public:
	template <bool IsShort, bool Shortened = true>
	bool copy_digest_normalized(digest_data<IsShort>& digest) noexcept
	{
		return copy_digest_internal<
			strings::sequences<
				digest_params::max_blockhash_sequence,
				digest_transform_t
			>, IsShort, Shortened
		>(digest);
	}
	bool copy_digest(digest_base<true, true>& digest) noexcept
	{
		return copy_digest_normalized<true, true>(digest);
	}
	bool copy_digest(digest_base<true, false>& digest) noexcept
	{
		return copy_digest_base<true, true>(digest);
	}
	bool copy_digest(digest_base<false, true>& digest) noexcept
	{
		return copy_digest_normalized<false, true>(digest);
	}
	bool copy_digest(digest_base<false, false>& digest) noexcept
	{
		return copy_digest_base<false, true>(digest);
	}
public:
	bool copy_digest_long_normalized(digest_base<false, true>& digest) noexcept
	{
		return copy_digest_normalized<false, false>(digest);
	}
	bool copy_digest_long(digest_base<false, true>& digest) noexcept
	{
		return copy_digest_normalized<false, false>(digest);
	}
	bool copy_digest_long(digest_base<false, false>& digest) noexcept
	{
		return copy_digest_base<false, false>(digest);
	}
private:
	template <typename T, bool IsShort, bool IsNormalized, bool Shortened = true>
	T digest_in_type(void)
	{
		// This object to be erased by NRVO.
		T d;
		if (IsNormalized)
		{
			if (!copy_digest_normalized<IsShort, Shortened>(d))
				throw digest_generator_error();
		}
		else
		{
			if (!copy_digest_base<IsShort, Shortened>(d))
				throw digest_generator_error();
		}
		return d;
	}
public:
	digest_unorm_t digest(void)
	{
		// Default options as you need (unnormalized; short form)
		return digest_in_type<digest_unorm_t, true, false>();
	}
	std::string digest_str(void)
	{
		return digest().pretty();
	}
public:
	template <bool Shortened = false>
	digest_long_unorm_t digest_long(void)
	{
		return digest_in_type<digest_long_unorm_t, false, false, Shortened>();
	}
	digest_t digest_normalized(void)
	{
		return digest_in_type<digest_t, true, true>();
	}
	template <bool Shortened = false>
	digest_long_t digest_long_normalized(void)
	{
		return digest_in_type<digest_long_t, false, true, Shortened>();
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
