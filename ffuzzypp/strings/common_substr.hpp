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


	CREDIT OF MODIFIED PORTIONS (HASHARRAY ALGORITHM)

	Copyright (C) 2014 Tsukasa OI <floss_ssdeep@irq.a4lg.com>

	CREDIT OF ADDED PORTIONS (BIT-PARALLEL ALGORITHM)

	Copyright (C) 2017 Tsukasa OI <floss_ssdeep@irq.a4lg.com>

*/
#ifndef FFUZZYPP_STRINGS_COMMON_SUBSTR_HPP
#define FFUZZYPP_STRINGS_COMMON_SUBSTR_HPP

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include <limits>
#include <type_traits>

#include "../rolling_hash.hpp"
#include "position_array.hpp"

namespace ffuzzy {
namespace strings {

namespace internal
{
	template <size_t SubstrSize>
	class common_substr_hasharray_impl
	{
	private:
		common_substr_hasharray_impl(void) = delete;
		common_substr_hasharray_impl(const common_substr_hasharray_impl&) = delete;
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

	template <
		size_t SubstrSize,
		size_t MaxSize = std::numeric_limits<size_t>::max()
	>
	class common_substr_bitparallel_impl
	{
	private:
		common_substr_bitparallel_impl(void) = delete;
		common_substr_bitparallel_impl(const common_substr_bitparallel_impl&) = delete;
	public:
		static constexpr const size_t substr_size = SubstrSize;
		static constexpr const size_t max_size = MaxSize;
		static_assert(0 < substr_size, "substr_size must be nonzero.");
		static_assert(0 < max_size, "max_size must be nonzero.");
	public:
		template <typename TBitmap, char CMin, char CMax>
		static bool match(
			const position_array<TBitmap, char, CMin, CMax>& s1,
			const char* s2, size_t s2len
		) noexcept
		{
			typedef position_array<TBitmap, char, CMin, CMax> pa_type;
			#ifdef FFUZZYPP_DEBUG
			assert(s2);
			assert(s2len >= substr_size);
			assert(s2len <= max_size);
			#endif
			// position array is too short to have such substrings
			if (pa_type::max_strlen < substr_size)
				return false;
			size_t r = substr_size - 1;
			while (true)
			{
				size_t l = r - (substr_size - 1);
				// we must reverse s2 because bitmap of s1 is reversed.
				const char* ch = &s2[s2len - 1 - r];
				TBitmap D = s1[*ch];
				while (D)
				{
					r--;
					D = (D << 1) & s1[*++ch];
					if (r == l && D)
						return true;
				}
				// Boyer-Moore-like skipping
				if (max_size - 1 <= std::numeric_limits<size_t>::max() - substr_size)
				{
					// (max_size - 1) + substr_size <= max(size_t)
					r += substr_size;
					if (r >= s2len)
						break;
				}
				else
				{
					// (max_size - 1) + substr_size > max(size_t)
					// which means we cannot safely perform r += substr_size first (very unlikely)
					if (r >= s2len - substr_size)
						break;
					r += substr_size;
				}
			}
			return false;
		}
	};
}

template <size_t MaxSize, size_t SubstrSize>
class common_substr_hasharray
{
private:
	common_substr_hasharray(void) = delete;
	common_substr_hasharray(const common_substr_hasharray&) = delete;
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
		uint_least32_t hashes[max_size - (substr_size - 1)];
		if (s1len < substr_size)
			return false;
		if (s2len < substr_size)
			return false;
		return internal
			::common_substr_hasharray_impl<substr_size>
			::match_long_buf(s1, s1len, s2, s2len, hashes);
	}
};

template <typename TBitmap, char CMin, char CMax, size_t MaxSize, size_t SubstrSize>
class common_substr_bitparallel
{
private:
	common_substr_bitparallel(void) = delete;
	common_substr_bitparallel(const common_substr_bitparallel&) = delete;
public:
	static constexpr const size_t max_size = MaxSize;
	static constexpr const size_t substr_size = SubstrSize;
	static_assert(0 < substr_size, "substr_size must be nonzero.");
	static_assert(substr_size <= max_size, "substring size must not be greater than the maximum size.");
	static_assert(max_size <= position_array<TBitmap, char, CMin, CMax>::max_strlen,
		"max_size must not be greater than max_strlen of corresponding position_matrix for efficiency.");
public:
	static bool match(
		const position_array<TBitmap, char, CMin, CMax>& s1,
		const char* s2, size_t s2len
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(s2);
		assert(s2len <= max_size);
		#endif
		if (s2len < substr_size)
			return false;
		return internal
			::common_substr_bitparallel_impl<substr_size, max_size>
			::match(s1, s2, s2len);
	}
};

template <typename TBitmap, char CMin, char CMax, size_t MaxSize, size_t SubstrSize>
class common_substr_bitparallel_wrapper
{
private:
	common_substr_bitparallel_wrapper(void) = delete;
	common_substr_bitparallel_wrapper(const common_substr_bitparallel_wrapper&) = delete;
public:
	static constexpr const size_t max_size = MaxSize;
	static constexpr const size_t substr_size = SubstrSize;
	static_assert(0 < substr_size, "substr_size must be nonzero.");
	static_assert(substr_size <= max_size, "substring size must not be greater than the maximum size.");
	static_assert(max_size <= position_array<TBitmap, char, CMin, CMax>::max_strlen,
		"max_size must not be greater than max_strlen of corresponding position_matrix for efficiency.");
public:
	static bool match(
		const char* s1, size_t s1len,
		const char* s2, size_t s2len
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(s2);
		assert(s2len <= max_size);
		#endif
		if (s2len < substr_size)
			return false;
		position_array<TBitmap, char, CMin, CMax> parray(s1, s1len);
		return internal
			::common_substr_bitparallel_impl<substr_size, max_size>
			::match(parray, s2, s2len);
	}
};



namespace internal
{
	template <char CMin, char CMax, size_t MaxSize, size_t SubstrSize, bool IsPositionArrayAvailable>
	class common_substr_impl_selector;

	template <char CMin, char CMax, size_t MaxSize, size_t SubstrSize>
	class common_substr_impl_selector<CMin, CMax, MaxSize, SubstrSize, true>
	{
	private:
		common_substr_impl_selector(void) = delete;
		common_substr_impl_selector(const common_substr_impl_selector&) = delete;
	public:
		static bool match(
			const char* s1, size_t s1len,
			const char* s2, size_t s2len
		) noexcept
		{
			typename auto_position_array<MaxSize, char, CMin, CMax>::type parray(s1, s1len);
			return common_substr_bitparallel<typename decltype(parray)::bitmap_type,
				CMin, CMax, MaxSize, SubstrSize>::match(parray, s2, s2len);
		}
	};

	template <char CMin, char CMax, size_t MaxSize, size_t SubstrSize>
	class common_substr_impl_selector<CMin, CMax, MaxSize, SubstrSize, false>
	{
	private:
		common_substr_impl_selector(void) = delete;
		common_substr_impl_selector(const common_substr_impl_selector&) = delete;
	public:
		static bool match(
			const char* s1, size_t s1len,
			const char* s2, size_t s2len
		) noexcept
		{
			return common_substr_hasharray<MaxSize, SubstrSize>::match(s1, s1len, s2, s2len);
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
public:
	template <
		char CMin = std::numeric_limits<char>::min(),
		char CMax = std::numeric_limits<char>::max()
	>
	static bool match(
		const char* s1, size_t s1len,
		const char* s2, size_t s2len
	) noexcept
	{
		return internal::common_substr_impl_selector<
			CMin, CMax, max_size, substr_size,
			is_auto_position_array_available<MaxSize, char, CMin, CMax>::value
		>::match(s1, s1len, s2, s2len);
	}
};

}}

#endif
