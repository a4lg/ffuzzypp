/*

	ffuzzy++ : C++ implementation of fast fuzzy hashing

	strings/common_substr.hpp
	Utility to find common substring in two strings


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

	Copyright (C) 2014 Tsukasa OI <floss_ssdeep@irq.a4lg.com>

*/
#ifndef FFUZZYPP_STRINGS_COMMON_SUBSTR_HPP
#define FFUZZYPP_STRINGS_COMMON_SUBSTR_HPP

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include "../rolling_hash.hpp"

namespace ffuzzy {
namespace strings {

namespace internal
{
	template <size_t SubstrSize>
	class common_substr_common
	{
	private:
		common_substr_common(void) = delete;
		common_substr_common(const common_substr_common&) = delete;
	public:
		static constexpr const size_t substr_size = SubstrSize;
		static_assert(0 < substr_size, "substr_size must be nonzero.");
		static_assert(substr_size >= rolling_hash::window_size,
			"substr_size must be equal or greater than window_size.");
	public:
		static bool match_long_buf(
			const char* s1, size_t s1len,
			const char* s2, size_t s2len,
			uint_least32_t* hashes_buf
		) noexcept
		{
			#ifdef FFUZZYPP_DEBUG
			assert(s1);
			assert(s2);
			assert(s1len >= substr_size);
			assert(s2len >= substr_size);
			#endif
			rolling_hash r;
			// compute rolling hashes for each index of s1
			for (size_t i = 0; i < substr_size - 1; i++)
				r.update(static_cast<unsigned char>(s1[i]));
			for (size_t i = substr_size - 1; i < s1len; i++)
			{
				r.update(static_cast<unsigned char>(s1[i]));
				hashes_buf[i - (substr_size - 1)] = r.sum();
			}
			s1len -= (substr_size - 1);
			// compute rolling hashes for each index of s2
			r.reset();
			for (size_t j = 0; j < substr_size - 1; j++)
				r.update(static_cast<unsigned char>(s2[j]));
			for (size_t j = 0; j < s2len - (substr_size - 1); j++)
			{
				r.update(static_cast<unsigned char>(s2[j + (substr_size - 1)]));
				uint_least32_t h = r.sum();
				for (size_t i = 0; i < s1len; i++)
				{
					// make sure we actually have common substring if hash matches
					if (hashes_buf[i] == h && !memcmp(s1 + i, s2 + j, substr_size))
						return true;
				}
			}
			return false;
		}
	};
}

template <size_t MaxSize, size_t SubstrSize>
class common_substr_fast
{
private:
	common_substr_fast(void) = delete;
	common_substr_fast(const common_substr_fast&) = delete;
public:
	static constexpr const size_t max_size = MaxSize;
	static constexpr const size_t substr_size = SubstrSize;
	static_assert(0 < substr_size, "substr_size must be nonzero.");
	static_assert(substr_size <= max_size, "substring size must not be greater than the maximum size.");
public:
	static bool match(
		const char* s1, size_t s1len,
		const char* s2, size_t s2len
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(s1);
		assert(s2);
		assert(s1len <= max_size);
		#endif
		if (max_size >= substr_size)
		{
			uint_least32_t hashes[max_size - (substr_size - 1)];
			if (s1len < substr_size)
				return false;
			if (s2len < substr_size)
				return false;
			return internal
				::common_substr_common<substr_size>
				::match_long_buf(s1, s1len, s2, s2len, hashes);
		}
		else
			return false;
	}
};

}}

#endif
