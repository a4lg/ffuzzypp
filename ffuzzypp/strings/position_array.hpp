/*

	ffuzzy++ : C++ implementation of fast fuzzy hashing

	strings/position_array.hpp
	Position array (for bit-parallel algorithms)

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
#ifndef FFUZZYPP_STRINGS_POSITION_ARRAY_HPP
#define FFUZZYPP_STRINGS_POSITION_ARRAY_HPP

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include <limits>
#include <type_traits>

namespace ffuzzy {
namespace strings {

namespace internal
{
	template <typename T>
	class digits_on_size
	{
	private:
		digits_on_size(void) = delete;
		digits_on_size(const digits_on_size&) = delete;
	public:
		static constexpr const size_t size =
			std::numeric_limits<T>::digits <= std::numeric_limits<size_t>::max()
				? std::numeric_limits<T>::digits
				: std::numeric_limits<size_t>::max();
	};

	/*
		Template to return guaranteed bit length on unsigned integral types:
		General template works only for binary (radix==2) types and
		specialized templates work for radix!=2 binary types.
	*/
	template <typename T>
	class guaranteed_bitlen
	{
	private:
		guaranteed_bitlen(void) = delete;
		guaranteed_bitlen(const guaranteed_bitlen&) = delete;
	private:
		static_assert(std::is_integral<T>::value, "T must be an integral type.");
		static_assert(std::is_unsigned<T>::value, "T must be an unsigned integral type.");
		static_assert(std::numeric_limits<T>::radix == 2, "T must be a binary unsigned integral type.");
	public:
		static constexpr const size_t size = digits_on_size<T>::size;
	};

	template <>
	class guaranteed_bitlen<unsigned>
	{
	private:
		guaranteed_bitlen(void) = delete;
		guaranteed_bitlen(const guaranteed_bitlen&) = delete;
	private:
		typedef std::numeric_limits<unsigned> limits_type;
	public:
		static constexpr const size_t size =
			limits_type::radix == 2 && limits_type::digits < 16
				? digits_on_size<unsigned>::size : 16;
	};

	template <>
	class guaranteed_bitlen<unsigned long>
	{
	private:
		guaranteed_bitlen(void) = delete;
		guaranteed_bitlen(const guaranteed_bitlen&) = delete;
	private:
		typedef std::numeric_limits<unsigned long> limits_type;
	public:
		static constexpr const size_t size =
			limits_type::radix == 2 && limits_type::digits < 32
				? digits_on_size<unsigned long>::size : 32;
	};

	template <>
	class guaranteed_bitlen<unsigned long long>
	{
	private:
		guaranteed_bitlen(void) = delete;
		guaranteed_bitlen(const guaranteed_bitlen&) = delete;
	private:
		typedef std::numeric_limits<unsigned long long> limits_type;
	public:
		static constexpr const size_t size =
			limits_type::radix == 2 && limits_type::digits < 64
				? digits_on_size<unsigned long long>::size : 64;
	};
}

class position_array_params
{
private:
	position_array_params(void) = delete;
	position_array_params(const position_array_params&) = delete;
public:
	#ifdef FFUZZYPP_DISABLE_POSITION_ARRAY
	static constexpr const bool is_disabled_by_build = true;
	#else
	static constexpr const bool is_disabled_by_build = false;
	#endif
	static constexpr const size_t max_efficient_array_size = 1024;
};
static_assert(position_array_params::max_efficient_array_size >= 64,
	"position_array_params::max_efficient_array_size must not be less than 64.");

template <
	typename TBitmap,
	typename TChar,
	TChar CMin = std::numeric_limits<TChar>::min(),
	TChar CMax = std::numeric_limits<TChar>::max()
>
class position_array_safety
{
private:
	position_array_safety(void) = delete;
	position_array_safety(const position_array_safety&) = delete;
public:
	static constexpr const bool is_welldefined =
		std::is_integral<TBitmap>::value &&
		std::is_unsigned<TBitmap>::value &&
		std::is_same<TBitmap, decltype(TBitmap(0u) | TBitmap(0u))>::value &&
		std::is_integral<TChar>::value &&
		(std::numeric_limits<TBitmap>::radix == 2 ||
			std::is_same<TBitmap, unsigned>::value ||
			std::is_same<TBitmap, unsigned long>::value ||
			std::is_same<TBitmap, unsigned long long>::value
		) &&
		(CMin <= CMax) &&
		(std::is_unsigned<TChar>::value
			? CMax - CMin <= std::numeric_limits<size_t>::max()
			: (0 <= CMin
				? std::numeric_limits<intmax_t>::min() + intmax_t(CMin) <= intmax_t(CMax)
				: intmax_t(CMax) <= std::numeric_limits<intmax_t>::max() + intmax_t(CMin)
			) && intmax_t(CMax) - intmax_t(CMin) <= std::numeric_limits<size_t>::max()
		) &&
		(size_t(CMax) - size_t(CMin) < std::numeric_limits<size_t>::max()) &&
		(sizeof(TBitmap) <= std::numeric_limits<size_t>::max() / (size_t(CMax) - size_t(CMin) + 1u));
public:
	static constexpr const bool is_considered_efficient =
		!position_array_params::is_disabled_by_build && is_welldefined &&
			(size_t(CMax) - size_t(CMin) + 1u) <= position_array_params::max_efficient_array_size;
};

template <
	typename TBitmap = unsigned long long,
	typename TChar = char,
	TChar CMin = std::numeric_limits<TChar>::min(),
	TChar CMax = std::numeric_limits<TChar>::max()
>
class position_array
{
private:
	static_assert(std::is_integral<TBitmap>::value, "TBitmap must be an integral type.");
	static_assert(std::is_unsigned<TBitmap>::value, "TBitmap must be an unsigned integral type.");
	static_assert(std::is_same<TBitmap, decltype(TBitmap(0u) | TBitmap(0u))>::value,
		"TBitmap must preserve its type after integral promotion.");
	static_assert(std::is_integral<TChar>::value, "TChar must be an integral type.");
	static_assert(CMin <= CMax, "CMin must not be greater than CMax.");
	static_assert(
		/*
			Because signed to unsigned conversion is well-defined,
			this predicate also indicates whether size_t(CMax) - size_t(CMin) is valid.
			Note that this predicate does not cover all the case where
			char is signed and (CMin - CMax) is greater than the maximum value of intmax_t.
		*/
		std::is_unsigned<TChar>::value
			? CMax - CMin <= std::numeric_limits<size_t>::max()
			: (0 <= CMin
				? std::numeric_limits<intmax_t>::min() + intmax_t(CMin) <= intmax_t(CMax)
				: intmax_t(CMax) <= std::numeric_limits<intmax_t>::max() + intmax_t(CMin)
			) && intmax_t(CMax) - intmax_t(CMin) <= std::numeric_limits<size_t>::max(),
		"(CMax - CMin) must be in range of both size_t and the widest type with same signedness as TChar.");
	static_assert(size_t(CMax) - size_t(CMin) < std::numeric_limits<size_t>::max(),
		"(size_t(CMax) - size_t(CMin) + 1) must be in range of size_t.");
	static_assert(sizeof(TBitmap) <= std::numeric_limits<size_t>::max() / (size_t(CMax) - size_t(CMin) + 1u),
		"(array_size * sizeof(TBitmap)) must be in range of size_t.");
public:
	static constexpr const size_t array_size = size_t(CMax) - size_t(CMin) + 1u;
	static constexpr const size_t max_strlen = internal::guaranteed_bitlen<TBitmap>::size;
	typedef TBitmap bitmap_type;
	typedef TChar char_type;
	static constexpr const char_type char_min = CMin;
	static constexpr const char_type char_max = CMax;
private:
	bitmap_type bitmap[array_size];
public:
	void reset(void) noexcept
	{
		memset(bitmap, 0, sizeof(bitmap));
	}
	void construct_noinit(const char_type* str, size_t len) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(len <= max_strlen);
		#endif
		for (size_t i = 0; i < len; i++)
		{
			char_type ch = *str++;
			#ifdef FFUZZYPP_DEBUG
			assert(char_min <= ch && ch <= char_max);
			#endif
			bitmap[size_t(ch) - size_t(char_min)] |= bitmap_type(1u) << i;
		}
	}
	void construct_noinit_safe(const char_type* str, size_t len) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(len <= max_strlen);
		#endif
		for (size_t i = 0; i < len; i++)
		{
			char_type ch = *str++;
			if (ch < char_min || char_max < ch)
				continue;
			bitmap[size_t(ch) - size_t(char_min)] |= bitmap_type(1u) << i;
		}
	}
	void construct(const char_type* str, size_t len) noexcept
	{
		reset();
		construct_noinit(str, len);
	}
	void construct_safe(const char_type* str, size_t len) noexcept
	{
		reset();
		construct_noinit_safe(str, len);
	}
public:
	bitmap_type bitmap_for(char_type ch) const noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(char_min <= ch && ch <= char_max);
		#endif
		return bitmap[size_t(ch) - size_t(char_min)];
	}
	bitmap_type bitmap_for_safe(char_type ch) const noexcept
	{
		if (ch < char_min || char_max < ch)
			return bitmap_type(0u);
		return bitmap[size_t(ch) - size_t(char_min)];
	}
	bitmap_type operator[](char_type ch) const noexcept
	{
		return bitmap_for(ch);
	}
	const bitmap_type* bitmap_data(void) const noexcept
	{
		return bitmap;
	}
public:
	position_array(void) noexcept = default; // initialize to undefined state
	position_array(const position_array& other) noexcept
	{
		if (bitmap != other.bitmap)
			memcpy(bitmap, other.bitmap, sizeof(bitmap));
	}
	const position_array& operator=(const position_array& other) noexcept
	{
		if (bitmap != other.bitmap)
			memcpy(bitmap, other.bitmap, sizeof(bitmap));
		return *this;
	}
	position_array(const char_type* str, size_t len) noexcept
	{
		construct(str, len);
	}
	position_array(const char_type* str, size_t len, bool safe) noexcept
	{
		if (safe)
			construct_safe(str, len);
		else
			construct(str, len);
	}
	static void swap(position_array& a, position_array& b) noexcept
	{
		std::swap(a.bitmap, b.bitmap); // C++11 version of swap
	}
};



namespace internal
{
	template <size_t MaxSize, typename TChar, TChar CMin, TChar CMax, typename TEnabler = void>
	class auto_position_array_internal;

	template <size_t MaxSize, typename TChar, TChar CMin, TChar CMax>
	class auto_position_array_internal<MaxSize, TChar, CMin, CMax, typename std::enable_if<(
		MaxSize > 0 && MaxSize <= 16 && position_array_safety<unsigned, TChar, CMin, CMax>::is_considered_efficient
	)>::type>
	{
	private:
		auto_position_array_internal(void);
		auto_position_array_internal(const auto_position_array_internal&) = delete;
	public:
		typedef unsigned int_type;
	};
	template <size_t MaxSize, typename TChar, TChar CMin, TChar CMax>
	class auto_position_array_internal<MaxSize, TChar, CMin, CMax, typename std::enable_if<(
		MaxSize > 16 && MaxSize <= 32 && position_array_safety<unsigned long, TChar, CMin, CMax>::is_considered_efficient
	)>::type>
	{
	private:
		auto_position_array_internal(void);
		auto_position_array_internal(const auto_position_array_internal&) = delete;
	public:
		typedef unsigned long int_type;
	};
	template <size_t MaxSize, typename TChar, TChar CMin, TChar CMax>
	class auto_position_array_internal<MaxSize, TChar, CMin, CMax, typename std::enable_if<(
		MaxSize > 32 && MaxSize <= 64 && position_array_safety<unsigned long long, TChar, CMin, CMax>::is_considered_efficient
	)>::type>
	{
	private:
		auto_position_array_internal(void);
		auto_position_array_internal(const auto_position_array_internal&) = delete;
	public:
		typedef unsigned long long int_type;
	};
}

// Predicate to test if automatically-chosen position array available
template <
	size_t MaxSize,
	typename TChar = char,
	TChar CMin = std::numeric_limits<TChar>::min(),
	TChar CMax = std::numeric_limits<TChar>::max()
>
struct is_auto_position_array_available
	: std::integral_constant<bool, (
		(MaxSize  >  0 && MaxSize <= 16 && position_array_safety<unsigned, TChar, CMin, CMax>::is_considered_efficient) ||
		(MaxSize  > 16 && MaxSize <= 32 && position_array_safety<unsigned long, TChar, CMin, CMax>::is_considered_efficient) ||
		(MaxSize  > 32 && MaxSize <= 64 && position_array_safety<unsigned long long, TChar, CMin, CMax>::is_considered_efficient)
	)>
{ };



namespace internal
{
	template <size_t MaxSize, typename TChar, TChar CMin, TChar CMax, bool>
	class auto_position_array_impl;

	template <size_t MaxSize, typename TChar, TChar CMin, TChar CMax>
	class auto_position_array_impl<MaxSize, TChar, CMin, CMax, true>
	{
	private:
		auto_position_array_impl(void) = delete;
		auto_position_array_impl(const auto_position_array_impl&) = delete;
	public:
		typedef typename auto_position_array_internal<MaxSize, TChar, CMin, CMax>::int_type int_type;
		typedef position_array<int_type, TChar, CMin, CMax> type;
	};

	template <size_t MaxSize, typename TChar, TChar CMin, TChar CMax>
	class auto_position_array_impl<MaxSize, TChar, CMin, CMax, false>
	{
	private:
		auto_position_array_impl(void) = delete;
		auto_position_array_impl(const auto_position_array_impl&) = delete;
	};
}

// Automatically-chosen position array
template <
	size_t MaxSize,
	typename TChar = char,
	TChar CMin = std::numeric_limits<TChar>::min(),
	TChar CMax = std::numeric_limits<TChar>::max()
>
class auto_position_array
	: public internal::auto_position_array_impl<
		MaxSize, TChar, CMin, CMax,
		is_auto_position_array_available<MaxSize, TChar, CMin, CMax>::value
	>
{
private:
	auto_position_array(void) = delete;
	auto_position_array(const auto_position_array&) = delete;
};

}}

#endif
