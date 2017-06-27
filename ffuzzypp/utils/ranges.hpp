/*

	ffuzzy++ Helper Libraries

	ranges.hpp
	Value range library

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
#ifndef FFUZZYPP_UTILS_RANGES_HPP
#define FFUZZYPP_UTILS_RANGES_HPP

#include <limits>


namespace ffuzzy {
namespace ranges {

/*
	Inclusive range ([a, b] where a <= b)
*/
template <typename T>
class inclusive
{
	T v_begin;
	T v_end;
public:
	class iterator
	{
	private:
		T value;
		T value_end;
		bool is_end;
	private:
		constexpr iterator(
			T value,
			T value_end,
			bool is_end = false
		) noexcept
			: value(value)
			, value_end(value_end)
			, is_end(is_end)
		{ }
		friend class inclusive;
	public:
		constexpr iterator(const iterator&) noexcept = default;
		constexpr T operator*(void) const noexcept
		{
			return value;
		}
		/* can be constexpr in C++14 */
		#if __cpp_constexpr >= 201304L
		constexpr
		#endif
		iterator& operator++(void) noexcept
		{
			if (!is_end)
			{
				if (value == value_end)
					is_end = true;
				else
					++value;
			}
			return *this;
		}
	public:
		constexpr bool operator==(
			const iterator& other
		) const noexcept
		{
			return
				value == other.value &&
				value_end == other.value_end &&
				is_end == other.is_end;
		}
		constexpr bool operator!=(
			const iterator& other
		) const noexcept
		{
			return !(*this == other);
		}
	};
public:
	constexpr iterator begin(void) const noexcept
	{
		return iterator(v_begin, v_end, false);
	}
	constexpr iterator end(void) const noexcept
	{
		return iterator(v_end, v_end, true);
	}
public:
	constexpr inclusive(T vbegin, T vend) noexcept
		: v_begin(vbegin), v_end(vend) { }
	constexpr inclusive(void) noexcept
		: v_begin(std::numeric_limits<T>::min())
		, v_end  (std::numeric_limits<T>::max()) { }
};

}}

#endif
