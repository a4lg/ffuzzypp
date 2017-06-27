/*

	ffuzzy++ : C++ implementation of fast fuzzy hashing

	digest_data.hpp
	Fuzzy digest (data and basic portions)


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
#ifndef FFUZZYPP_DIGEST_DATA_HPP
#define FFUZZYPP_DIGEST_DATA_HPP

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring>

#include <functional>
#include <limits>
#include <string>
#include <type_traits>
#include <utility>

#include <errno.h>

#include "base64.hpp"
#include "digest_blocksize.hpp"
#include "strings/sequences.hpp"
#include "strings/nosequences.hpp"
#include "utils/safe_int.hpp"
#include "utils/numeric_digits.hpp"

namespace ffuzzy {

typedef size_t blockhash_len_t;
static_assert(
	safe_int::contracts::is_unsigned_integral_type<blockhash_len_t>(),
	"blockhash_len_t must be an unsigned integral type.");

struct digest_parse_error {};


class digest_params
{
private:
	digest_params(void) = delete;
	digest_params(const digest_params&) = delete;

	// Common digest parameters (for both short and long forms)
public:
	static constexpr const blockhash_len_t max_blockhash_len = 64;
	static constexpr const blockhash_len_t max_blockhash_sequence = 3;
	static_assert(max_blockhash_len >= 2, "max_blockhash_len must be 2 or greater.");
	static_assert(max_blockhash_sequence != 0, "max_blockhash_sequence must not be zero.");
	static_assert(safe_int::uvalue<size_t, max_blockhash_len>::is_valid,
		"max_blockhash_len must be in range of size_t.");
	static_assert(safe_int::uvalue<size_t, max_blockhash_sequence>::is_valid,
		"max_blockhash_sequence must be in range of size_t.");
};


template <bool IsAlphabetRestricted> class digest_data_transformation;

template<>
class digest_data_transformation<true>
{
private:
	digest_data_transformation(void) = delete;
	digest_data_transformation(const digest_data_transformation&) = delete;
public:
	typedef base64::transform_from_b64 input_type;
	typedef base64::transform_to_b64   output_type;
};

template<>
class digest_data_transformation<false>
{
private:
	digest_data_transformation(void) = delete;
	digest_data_transformation(const digest_data_transformation&) = delete;
public:
	typedef strings::default_char_transform input_type;
	typedef strings::default_char_transform output_type;
};


// Friend classes for digest_data class
enum struct comparison_version;
namespace internal
{
	template <comparison_version> class digest_comparison_base;
	class digest_copy;
}
template <comparison_version> class digest_comparison;
template <bool> class digest_position_array_base;


// Data structure for fuzzy digest (as base class)
template <bool IsAlphabetRestricted, bool IsShort>
class digest_data
{
	static_assert(digest_params::max_blockhash_len >= 4,
		"max_blockhash_len must be at least 4 due to restrictions in this implementation.");

	// Maximum lengths for each block hashes
public:
	static constexpr const blockhash_len_t max_blockhash1_len = digest_params::max_blockhash_len;
	static constexpr const blockhash_len_t max_blockhash2_len = IsShort
		? digest_params::max_blockhash_len / 2
		: digest_params::max_blockhash_len;

	// Maximum characters required to pretty-print "natural" digests
public:
	static_assert(safe_int::safe_mul<
			safe_int::uvalue<blockhash_len_t, digest_params::max_blockhash_len>,
			safe_int::uvalue<blockhash_len_t, 2>
		>::is_valid,
		"max_blockhash_len * 2 must be in range of blockhash_len_t.");
	static_assert(
		safe_int::safe_add<
			safe_int::safe_add<
				safe_int::uvalue<size_t, 3>,
				safe_int::uvalue<size_t, digest_blocksize::max_natural_digits>
			>,
			safe_int::safe_mul<
				safe_int::uvalue<size_t, digest_params::max_blockhash_len>,
				safe_int::uvalue<size_t, 2>
			>
		>::is_valid,
		"max_blockhash_len * 2 + max_natural_digits + 3 must be in range of size_t.");
	static constexpr const size_t max_natural_chars =
		max_blockhash1_len + max_blockhash2_len + // two block hashes
		digest_blocksize::max_natural_digits + // block size
		3; // two colons and '\0' as a terminator
	// width == chars exclusing '\0' (== chars - 1)
	static constexpr const size_t max_natural_width = max_natural_chars - 1;
	static constexpr const size_t max_natural_width_digits =
		numeric_digits::in_decimal<size_t>(max_natural_width);
	static_assert(max_natural_width != 0, "sanity check for max_natural_width failed.");

	// Data structure
private:
	char digest[max_blockhash1_len + max_blockhash2_len];
	blockhash_len_t blkhash1_len;
	blockhash_len_t blkhash2_len;
	digest_blocksize_t blksize;
public:
	size_t blockhash1_len(void) const noexcept { return blkhash1_len; }
	size_t blockhash2_len(void) const noexcept { return blkhash2_len; }
	unsigned long blocksize(void) const noexcept { return blksize; }
	size_t digest_size_used(void) const noexcept { return blkhash1_len + blkhash2_len; }
	const char* digest_buffer(void) const noexcept { return digest; }
	void copy_digest_buffer(char* buf) const noexcept { memcpy(buf, digest, digest_size_used()); }

	// "Initialization" and assignment
public:
	digest_data(void) noexcept = default; // initialize to undefined state
	digest_data(const digest_data& other) noexcept
		: blkhash1_len(other.blkhash1_len)
		, blkhash2_len(other.blkhash2_len)
		, blksize(other.blksize)
	{
		#ifdef FFUZZYPP_DEBUG
		assert(other.is_valid());
		#endif
		if (digest != other.digest)
			memcpy(digest, other.digest, blkhash1_len + blkhash2_len);
	}
	const digest_data& operator=(const digest_data& other) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(other.is_valid());
		#endif
		blkhash1_len = other.blkhash1_len;
		blkhash2_len = other.blkhash2_len;
		blksize = other.blksize;
		if (digest != other.digest)
			memcpy(digest, other.digest, blkhash1_len + blkhash2_len);
		return *this;
	}
	static void swap(digest_data& a, digest_data& b) noexcept
	{
		std::swap(a.blksize, b.blksize);
		std::swap(a.blkhash1_len, b.blkhash1_len);
		std::swap(a.blkhash2_len, b.blkhash2_len);
		std::swap(a.digest, b.digest); // C++11 version of swap
	}

	// Validators (for its validness and naturality)
public:
	bool is_valid(void) const noexcept
	{
		if (blkhash1_len > max_blockhash1_len)
			return false;
		if (blkhash2_len > max_blockhash2_len)
			return false;
		if (blksize > 0xfffffffful)
			return false;
		if (IsAlphabetRestricted)
		{
			for (blockhash_len_t i = 0, l = blkhash1_len + blkhash2_len; i < l; i++)
				if (digest[i] < char(0) || 64 <= digest[i])
					return false;
		}
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
		if (!IsAlphabetRestricted)
		{
			for (blockhash_len_t i = 0, l = blkhash1_len + blkhash2_len; i < l; i++)
				if (!base64::isbase64(digest[i]))
					return false;
		}
		return true;
	}
	bool is_blocksize_natural(void) const noexcept
	{
		return digest_blocksize::is_natural(blksize);
	}
	bool is_normalized(void) const noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(is_valid());
		#endif
		return
			!strings::sequences<digest_params::max_blockhash_sequence>
				::has_sequences(digest, blkhash1_len) &&
			!strings::sequences<digest_params::max_blockhash_sequence>
				::has_sequences(digest+blkhash1_len, blkhash2_len);
	}
	bool has_valid_base64_data(void) const noexcept
	{
		if (blkhash1_len > max_blockhash1_len)
			return false;
		if (blkhash2_len > max_blockhash2_len)
			return false;
		if (IsAlphabetRestricted)
		{
			for (blockhash_len_t i = 0, l = blkhash1_len + blkhash2_len; i < l; i++)
				if (digest[i] < char(0) || 64 <= digest[i])
					return false;
		}
		else
		{
			for (blockhash_len_t i = 0, l = blkhash1_len + blkhash2_len; i < l; i++)
				if (!base64::isbase64(digest[i]))
					return false;
		}
		return true;
	}

	// Equality
private:
	static bool is_eq_except_blocksize(const digest_data& a, const digest_data& b) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(a.is_valid());
		assert(b.is_valid());
		#endif
		return
			a.blkhash1_len == b.blkhash1_len &&
			a.blkhash2_len == b.blkhash2_len &&
			memcmp(a.digest, b.digest, a.digest_size_used()) == 0;
	}
	static bool is_eq(const digest_data& a, const digest_data& b) noexcept
	{
		return
			digest_blocksize::is_near_eq(a.blksize, b.blksize) &&
			is_eq_except_blocksize(a, b);
	}
public:
	friend bool operator==(const digest_data& a, const digest_data& b) noexcept { return  is_eq(a, b); }
	friend bool operator!=(const digest_data& a, const digest_data& b) noexcept { return !is_eq(a, b); }

	// Default comparison for sorting (in "dictionary" order or whatever)
	// Note that sort order differs depending on IsAlphabetRestricted.
public:
	friend bool operator<(const digest_data& a, const digest_data& b) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(a.is_valid());
		assert(b.is_valid());
		#endif
		if (a.blksize < b.blksize)
			return true;
		if (a.blksize > b.blksize)
			return false;
		if (a.blkhash1_len < b.blkhash1_len)
			return true;
		if (a.blkhash1_len > b.blkhash1_len)
			return false;
		if (a.blkhash2_len < b.blkhash2_len)
			return true;
		if (a.blkhash2_len > b.blkhash2_len)
			return false;
		if (memcmp(a.digest, b.digest, a.digest_size_used()) < 0)
			return true;
		return false;
	}
	friend bool operator>(const digest_data& a, const digest_data& b) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(a.is_valid());
		assert(b.is_valid());
		#endif
		if (a.blksize > b.blksize)
			return true;
		if (a.blksize < b.blksize)
			return false;
		if (a.blkhash1_len > b.blkhash1_len)
			return true;
		if (a.blkhash1_len < b.blkhash1_len)
			return false;
		if (a.blkhash2_len > b.blkhash2_len)
			return true;
		if (a.blkhash2_len < b.blkhash2_len)
			return false;
		if (memcmp(a.digest, b.digest, a.digest_size_used()) > 0)
			return true;
		return false;
	}
	friend bool operator<=(const digest_data& a, const digest_data& b) noexcept { return !(a > b); }
	friend bool operator>=(const digest_data& a, const digest_data& b) noexcept { return !(a < b); }

	// Predicates (including non-standard comparison predicates for fast sorting)
public:
	struct pred_equal_to
	{
		bool operator()(const digest_data& a, const digest_data& b) const noexcept { return a == b; }
	};
	struct pred_not_equal_to
	{
		bool operator()(const digest_data& a, const digest_data& b) const noexcept { return a != b; }
	};
	struct pred_less
	{
		bool operator()(const digest_data& a, const digest_data& b) const noexcept { return a < b; }
	};
	struct pred_less_equal
	{
		bool operator()(const digest_data& a, const digest_data& b) const noexcept { return a <= b; }
	};
	struct pred_greater
	{
		bool operator()(const digest_data& a, const digest_data& b) const noexcept { return a > b; }
	};
	struct pred_greater_equal
	{
		bool operator()(const digest_data& a, const digest_data& b) const noexcept { return a >= b; }
	};
	struct pred_less_blocksize
	{
		bool operator()(const digest_data& a, const digest_data& b) const noexcept
		{
			return a.blksize < b.blksize;
		}
	};
	struct pred_less_blocksize_natural
	{
		bool operator()(const digest_data& a, const digest_data& b) const noexcept
		{
			bool aN = digest_blocksize::is_natural(a.blksize);
			bool bN = digest_blocksize::is_natural(b.blksize);
			if (aN && !bN)
				return false;
			if (!aN && bN)
				return true;
			if (a.blksize < b.blksize)
				return true;
			return false;
		}
	};

	// Hash (for sets and dictionaries)
public:
	size_t hash(void) const noexcept
	{
		typedef typename std::conditional<
			(std::numeric_limits<size_t>::max() >= 0xfffffffful),
			size_t, uint_least32_t
		>::type hash_t;
		static constexpr const hash_t fnv_init  = 2166136261ul;
		static constexpr const hash_t fnv_prime = 16777619ul;
		hash_t h = fnv_init;
		h ^= hash_t(blksize);      h *= fnv_prime;
		h ^= hash_t(blkhash1_len); h *= fnv_prime;
		h ^= hash_t(blkhash2_len); h *= fnv_prime;
		for (blockhash_len_t i = 0, l = blkhash1_len + blkhash2_len; i < l; i++)
		{
			h ^= hash_t(static_cast<unsigned char>(digest[i]));
			h *= fnv_prime;
		}
		if (std::numeric_limits<size_t>::max() < 0xfffffffful)
			h ^= (h >> 16);
		return size_t(h);
	}

	// Normalization
public:
	static void normalize(digest_data& dest, const digest_data& source) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(source.is_valid());
		#endif
		dest.blksize = source.blksize;
		blockhash_len_t orig_blkhash1_len = source.blkhash1_len;
		dest.blkhash1_len = strings::sequences<digest_params::max_blockhash_sequence>
			::copy_elim_sequences(dest.digest, source.digest, source.blkhash1_len);
		dest.blkhash2_len = strings::sequences<digest_params::max_blockhash_sequence>
			::copy_elim_sequences(dest.digest + dest.blkhash1_len, source.digest + orig_blkhash1_len, source.blkhash2_len);
		#ifdef FFUZZYPP_DEBUG
		assert(dest.is_valid());
		assert(dest.is_normalized());
		#endif
	}
	template <typename Treturn = digest_data>
	static Treturn normalize(const digest_data& source) noexcept
	{
		Treturn dest;
		digest_data::normalize(dest, source);
		return dest;
	}
	void normalize(void) noexcept
	{
		normalize(*this, *this);
	}

	// Utility to parse digests
private:
	template <template <char... terms> class Tstring_copy>
	static bool parse_internal(digest_data& digest, const char* str) noexcept
	{
		const char* rem = str;
		char* out = digest.digest;
		errno = 0;
		// size of unsigned long is at least 32-bit
		unsigned long blksize = strtoul(str, const_cast<char**>(&rem), 10);
		if (rem == str)
			return false;
		if (errno == ERANGE && blksize == std::numeric_limits<unsigned long>::max())
			return false;
		if (blksize > 0xfffffffful)
			return false;
		digest.blksize = digest_blocksize_t(blksize);
		if (*rem++ != ':')
			return false;
		if (!Tstring_copy<':'>::copy_elim_sequences(out, max_blockhash1_len, rem))
			return false;
		digest.blkhash1_len = out - digest.digest;
		char* outtmp = out;
		if (*rem++ != ':')
			return false;
		#ifndef FFUZZYPP_BLOCKDIGEST2_TERMS
		// default from ffuzzy++ 4.0
		#define FFUZZYPP_BLOCKDIGEST2_TERMS ','
		#endif
		if (!Tstring_copy<FFUZZYPP_BLOCKDIGEST2_TERMS>::
			copy_elim_sequences(out, max_blockhash2_len, rem))
			return false;
		digest.blkhash2_len = out - outtmp;
		if (IsAlphabetRestricted)
		{
			for (blockhash_len_t i = 0; i < digest.blkhash1_len + digest.blkhash2_len; i++)
			{
				if (digest.digest[i] == base64::invalid_index)
					return false;
			}
		}
		return true;
	}
protected:
	static bool parse(digest_data& digest, const char* str) noexcept
	{
		return parse_internal<
			strings::nosequences<typename digest_data_transformation<IsAlphabetRestricted>::input_type>::template string_copy
		>(digest, str);
	}
	static bool parse(digest_data& digest, const std::string& str)
	{
		return parse(digest, str.c_str());
	}
	static bool parse_normalized(digest_data& digest, const char* str) noexcept
	{
		return parse_internal<
			strings::sequences<digest_params::max_blockhash_sequence,
				typename digest_data_transformation<IsAlphabetRestricted>::input_type>::template string_copy
		>(digest, str);
	}
	static bool parse_normalized(digest_data& digest, const std::string& str)
	{
		return parse_normalized(digest, str.c_str());
	}

	// Pretty printing
public:
	bool pretty_unsafe(char* out) const noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(is_valid());
		#endif
		int n = sprintf(out, "%lu:", static_cast<unsigned long>(blksize));
		if (n < 0)
			return false;
		out += n;
		strings::nosequences<typename digest_data_transformation<IsAlphabetRestricted>::output_type>::copy_raw(out, digest, blkhash1_len);
		out += blkhash1_len;
		*out++ = ':';
		strings::nosequences<typename digest_data_transformation<IsAlphabetRestricted>::output_type>::copy_raw(out, digest+blkhash1_len, blkhash2_len);
		out[blkhash2_len] = '\0';
		return true;
	}
	bool pretty(char* out, size_t outsize) const noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(is_valid());
		#endif
		int n = snprintf(out, outsize, "%lu:", static_cast<unsigned long>(blksize));
		if (n < 0)
			return false;
		if (size_t(n) == outsize)
			return false;
		outsize -= size_t(n);
		out += n;
		if (outsize < size_t(blkhash1_len) + size_t(blkhash2_len) + 2)
			return false;
		strings::nosequences<typename digest_data_transformation<IsAlphabetRestricted>::output_type>::copy_raw(out, digest, blkhash1_len);
		out += blkhash1_len;
		*out++ = ':';
		strings::nosequences<typename digest_data_transformation<IsAlphabetRestricted>::output_type>::copy_raw(out, digest+blkhash1_len, blkhash2_len);
		out[blkhash2_len] = '\0';
		return true;
	}
	std::string pretty(void) const
	{
		#ifdef FFUZZYPP_DEBUG
		assert(is_valid());
		#endif
		unsigned long bs(blksize);
		std::string str(std::to_string(bs));
		str.push_back(':');
		for (blockhash_len_t i = 0; i < blkhash1_len; i++)
			str.push_back(digest_data_transformation<IsAlphabetRestricted>::output_type::transform(digest[i]));
		str.push_back(':');
		for (blockhash_len_t i = 0; i < blkhash2_len; i++)
			str.push_back(digest_data_transformation<IsAlphabetRestricted>::output_type::transform(digest[blkhash1_len + i]));
		return str;
	}

	// Friend classes
	template <comparison_version> friend class internal::digest_comparison_base;
	template <comparison_version> friend class digest_comparison;
	template <bool> friend class digest_position_array_base;
	friend class digest_generator;
	friend class internal::digest_copy;
};


namespace internal
{
	// Utility to copy constrained digest data to non-constrained digest object
	class digest_copy
	{
	private:
		digest_copy(void) = delete;
		digest_copy(const digest_copy&) = delete;
	public:
		template <bool IsAlphabetRestricted>
		static void copy_to_long(
			digest_data<IsAlphabetRestricted, false>& dest,
			const digest_data<IsAlphabetRestricted, true>& src
		) noexcept
		{
			#ifdef FFUZZYPP_DEBUG
			assert(src.is_valid());
			#endif
			dest.blkhash1_len = src.blkhash1_len;
			dest.blkhash2_len = src.blkhash2_len;
			dest.blksize = src.blksize;
			memcpy(dest.digest, src.digest, src.blkhash1_len + src.blkhash2_len);
		}
		template <bool IsShort>
		static void copy_to_non_ra(
			digest_data<false, IsShort>& dest,
			const digest_data<true, IsShort>& src
		) noexcept
		{
			#ifdef FFUZZYPP_DEBUG
			assert(src.is_valid());
			#endif
			dest.blkhash1_len = src.blkhash1_len;
			dest.blkhash2_len = src.blkhash2_len;
			dest.blksize = src.blksize;
			strings::nosequences<base64::transform_to_b64>::
				copy_raw(dest.digest, src.digest, src.blkhash1_len + src.blkhash2_len);
		}
		static void copy_to_long_non_ra(
			digest_data<false, false>& dest,
			const digest_data<true, true>& src
		) noexcept
		{
			#ifdef FFUZZYPP_DEBUG
			assert(src.is_valid());
			#endif
			dest.blkhash1_len = src.blkhash1_len;
			dest.blkhash2_len = src.blkhash2_len;
			dest.blksize = src.blksize;
			strings::nosequences<base64::transform_to_b64>::
				copy_raw(dest.digest, src.digest, src.blkhash1_len + src.blkhash2_len);
		}
	};
}

}

#endif
