/*

	ffuzzy++ : C++ implementation of fast fuzzy hashing

	digest_position_array_base.hpp
	Fuzzy digest (position array representation; base class)

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
#ifndef FFUZZYPP_DIGEST_POSITION_ARRAY_BASE_HPP
#define FFUZZYPP_DIGEST_POSITION_ARRAY_BASE_HPP

#include <cassert>
#include <cstddef>

#include <algorithm>
#include <limits>

#include "base64.hpp"
#include "digest_blocksize.hpp"
#include "digest_data.hpp"
#include "strings/position_array.hpp"

namespace ffuzzy {

template <bool IsAlphabetRestricted>
class digest_position_array_params
{
private:
	digest_position_array_params(void) = delete;
	digest_position_array_params(const digest_position_array_params&) = delete;
public:
	static constexpr const char char_min =
		IsAlphabetRestricted ? 0x00 : std::numeric_limits<char>::min();
	static constexpr const char char_max =
		IsAlphabetRestricted ? 0x3f : std::numeric_limits<char>::max();
	static constexpr const bool is_available =
		strings::is_auto_position_array_available<digest_params::max_blockhash_len, char, char_min, char_max>::value;
};


// Friend classes for digest_data class
enum struct comparison_version;
namespace internal
{
	template <comparison_version> class digest_comparison_base;
}
template <comparison_version> class digest_comparison;


template <bool IsAlphabetRestricted>
class digest_position_array_base
{
public:
	typedef strings::auto_position_array<
		digest_params::max_blockhash_len, char,
		digest_position_array_params<IsAlphabetRestricted>::char_min,
		digest_position_array_params<IsAlphabetRestricted>::char_max
	> traits_type;
	typedef typename traits_type::type     pa_type;
	typedef typename traits_type::int_type int_type;

	// Data structure
private:
	pa_type blkhash1;
	pa_type blkhash2;
	blockhash_len_t blkhash1_len;
	blockhash_len_t blkhash2_len;
	digest_blocksize_t blksize;
public:
	size_t blockhash1_len(void) const noexcept { return blkhash1_len; }
	size_t blockhash2_len(void) const noexcept { return blkhash2_len; }
	unsigned long blocksize(void) const noexcept { return blksize; }
	const pa_type& blockhash1_array(void) const noexcept { return blkhash1; }
	const pa_type& blockhash2_array(void) const noexcept { return blkhash2; }
	bool is_short(void) const noexcept { return blkhash2_len <= digest_params::max_blockhash_len / 2; }

	// "Initialization" and assignment
public:
	digest_position_array_base(void) noexcept = default; // initialize to undefined state
	digest_position_array_base(const digest_position_array_base& other) noexcept
		: blkhash1(other.blkhash1)
		, blkhash2(other.blkhash2)
		, blkhash1_len(other.blkhash1_len)
		, blkhash2_len(other.blkhash2_len)
		, blksize(other.blksize)
	{
		#ifdef FFUZZYPP_DEBUG
		assert(other.is_valid());
		#endif
	}
	const digest_position_array_base& operator=(const digest_position_array_base& other) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(other.is_valid());
		#endif
		blkhash1 = other.blkhash1;
		blkhash2 = other.blkhash2;
		blkhash1_len = other.blkhash1_len;
		blkhash2_len = other.blkhash2_len;
		blksize = other.blksize;
		return *this;
	}
	static void swap(digest_position_array_base& a, digest_position_array_base& b) noexcept
	{
		std::swap(a.blkhash1, b.blkhash1);
		std::swap(a.blkhash2, b.blkhash2);
		std::swap(a.blkhash1_len, b.blkhash1_len);
		std::swap(a.blkhash2_len, b.blkhash2_len);
		std::swap(a.blksize, b.blksize);
	}

	// Construction by digest
protected:
	template <bool IsShort>
	static void construct(
		digest_position_array_base& dest,
		const digest_data<IsAlphabetRestricted, IsShort>& src
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(src.is_valid());
		#endif
		dest.blkhash1.construct(src.digest, src.blkhash1_len);
		dest.blkhash2.construct(src.digest+src.blkhash1_len, src.blkhash2_len);
		dest.blkhash1_len = src.blkhash1_len;
		dest.blkhash2_len = src.blkhash2_len;
		dest.blksize = src.blksize;
	}
	template <bool IsShort>
	digest_position_array_base(const digest_data<IsAlphabetRestricted, IsShort>& src) noexcept
	{
		construct(src);
	}

	// Validators (for its validness and naturality)
private:
	bool is_valid_blockhash_position_array(const pa_type& parray, blockhash_len_t len) const noexcept
	{
		int_type mask, set_bits;
		const int_type* data = parray.bitmap_data();
		size_t array_size = parray.array_size;
		if (len)
		{
			mask = int_type(1u) << (len - 1);
			mask = mask ^ (-mask);
		}
		else
		{
			mask = -1;
		}
		set_bits = 0;
		for (size_t i = 0; i < array_size; i++)
		{
			int_type D = data[i];
			// check for invalid bits "above" length of the string
			if (D & mask)
				return false;
			// check for duplicate bits (e.g. n-th character of the string is BOTH 'A' and 'B')
			if (D & set_bits)
				return false;
			set_bits |= D;
			// check for sequences
			int_type tmp = D;
			for (blockhash_len_t j = 0; j < digest_params::max_blockhash_sequence; j++)
			{
				tmp <<= 1;
				tmp &= D;
			}
			if (tmp)
				return false;
		}
		// check if set_bits is now "all valid bits"
		if (len)
		{
			mask = int_type(1u) << (len - 1);
			mask = mask | (mask - 1);
		}
		else
		{
			mask = 0;
		}
		if (mask != set_bits)
			return false;
		return true;
	}
	bool is_natural_blockhash_position_array(const pa_type& parray, blockhash_len_t len) const noexcept
	{
		if (!IsAlphabetRestricted)
		{
			// check only if IsAlphabetRestricted is false
			// (represented by regular characters and non Base64 characters are possible)
			int_type mask, set_bits;
			if (len)
			{
				mask = int_type(1u) << (len - 1);
				mask = mask | (mask - 1);
			}
			else
			{
				set_bits = 0;
			}
			for (size_t i = 0; i < 64; i++)
				set_bits |= parray[base64::values[i]];
			// check if all valid bits are set by valid Base64 chareacters
			if (mask != set_bits)
				return false;
		}
		return true;
	}
public:
	bool is_valid(void) const noexcept
	{
		if (blkhash1_len > digest_params::max_blockhash_len)
			return false;
		if (blkhash2_len > digest_params::max_blockhash_len)
			return false;
		if (blksize > 0xfffffffful)
			return false;
		// Note: digest_position_array_base::is_valid takes time.
		if (!is_valid_blockhash_position_array(blkhash1, blkhash1_len))
			return false;
		if (!is_valid_blockhash_position_array(blkhash2, blkhash2_len))
			return false;
		return true;
	}
	bool is_natural(void) const noexcept
	{
		if (!is_valid())
			return false;
		if (blkhash1_len < blkhash2_len)
			return false;
		if (!digest_blocksize::is_natural(blksize))
			return false;
		if (!digest_blocksize::is_safe_to_double(blksize) && blkhash2_len >= 2)
			return false;
		// Note: digest_position_array_base::is_natural takes time.
		if (!is_natural_blockhash_position_array(blkhash1, blkhash1_len))
			return false;
		if (!is_natural_blockhash_position_array(blkhash2, blkhash2_len))
			return false;
		return true;
	}
	bool is_blocksize_natural(void) const noexcept
	{
		return digest_blocksize::is_natural(blksize);
	}

	// Comparison
public:
	static bool is_eq_except_blocksize(
		const digest_position_array_base& a,
		const digest_position_array_base& b
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(a.is_valid());
		assert(b.is_valid());
		#endif
		if (a.blkhash1_len != b.blkhash1_len)
			return false;
		if (a.blkhash2_len != b.blkhash2_len)
			return false;
		int_type data_a = a.bitmap_data();
		int_type data_b = b.bitmap_data();
		for (size_t i = 0; i < pa_type::array_size; i++)
		{
			if (data_a[i] != data_b[i])
				return false;
		}
		return true;
	}
	static bool is_eq(
		const digest_position_array_base& a,
		const digest_position_array_base& b
	) noexcept
	{
		if (a.blksize != b.blksize)
			return false;
		return is_eq_except_blocksize(a, b);
	}
	template <bool IsShort>
	static bool is_eq_except_blocksize(
		const digest_position_array_base& a,
		const digest_data<IsAlphabetRestricted, IsShort>& b
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(a.is_valid());
		assert(b.is_valid());
		#endif
		if (a.blkhash1_len != b.blkhash1_len)
			return false;
		if (a.blkhash2_len != b.blkhash2_len)
			return false;
		if (a.blksize != b.blksize)
			return false;
		const char* p = b.digest;
		for (blockhash_len_t i = 0; i < b.blkhash1_len; i++)
		{
			if (!(a.blkhash1[*p++] & (int_type(1u) << i)))
				return false;
		}
		for (blockhash_len_t i = 0; i < b.blkhash2_len; i++)
		{
			if (!(a.blkhash2[*p++] & (int_type(1u) << i)))
				return false;
		}
		return true;
	}
	template <bool IsShort>
	static bool is_eq(
		const digest_position_array_base& a,
		const digest_data<IsAlphabetRestricted, IsShort>& b
	) noexcept
	{
		if (a.blksize != b.blksize)
			return false;
		return is_eq_except_blocksize(a, b);
	}
public:
	friend bool operator==(const digest_position_array_base& a, const digest_position_array_base& b) noexcept { return  is_eq(a, b); }
	friend bool operator!=(const digest_position_array_base& a, const digest_position_array_base& b) noexcept { return !is_eq(a, b); }
public:
	template <bool IsShort>
	friend bool operator==(
		const digest_position_array_base& a,
		const digest_data<IsAlphabetRestricted, IsShort>& b
	) noexcept
	{
		return is_eq(a, b);
	}
	template <bool IsShort>
	friend bool operator!=(
		const digest_position_array_base& a,
		const digest_data<IsAlphabetRestricted, IsShort>& b
	) noexcept
	{
		return !is_eq(a, b);
	}
	template <bool IsShort>
	friend bool operator==(
		const digest_data<IsAlphabetRestricted, IsShort>& a,
		const digest_position_array_base& b
	) noexcept
	{
		return is_eq(b, a);
	}
	template <bool IsShort>
	friend bool operator!=(
		const digest_data<IsAlphabetRestricted, IsShort>& a,
		const digest_position_array_base& b
	) noexcept
	{
		return !is_eq(b, a);
	}

	// Friend classes
	template <comparison_version> friend class internal::digest_comparison_base;
	template <comparison_version> friend class digest_comparison;
};

}

#endif
