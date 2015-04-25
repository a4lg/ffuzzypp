/*

	ffuzzy++ : C++ implementation of fast fuzzy hashing

	strings/sequences.hpp
	Utilities to find/eliminate sequences of same characters

	Copyright (C) 2014 Tsukasa OI <floss_ssdeep@irq.a4lg.com


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
#ifndef FFUZZYPP_STRINGS_SEQUENCES_HPP
#define FFUZZYPP_STRINGS_SEQUENCES_HPP

#include <cstddef>

#include "terminators.hpp"
#include "transform.hpp"
#include "../utils/safe_int.hpp"

namespace ffuzzy {
namespace strings {

template <size_t MaxSequenceSize, typename Ttransform = default_char_transform>
class sequences
{
	static_assert(safe_int::safe_add<
			safe_int::uvalue<size_t, MaxSequenceSize>,
			safe_int::uvalue<size_t, 1>
		>::value,
		"MaxSequenceSize + 1 must be in range of size_t.");
private:
	sequences(void) = delete;
	sequences(const sequences&) = delete;
public:
	static constexpr const size_t max_sequence_size = MaxSequenceSize;
	static_assert(max_sequence_size != 0, "max_sequence_size must not be zero.");
public:
	static bool has_sequences(const char* buf, size_t size) noexcept
	{
		if (size <= max_sequence_size)
			return false;
		size_t seq = 0;
		char  prev = *buf++;
		while (--size)
		{
			char curr = *buf++;
			if (curr == prev)
			{
				if (++seq == max_sequence_size)
					return true;
			}
			else
			{
				seq = 0;
				prev = curr;
			}
		}
		return false;
	}
	static size_t copy_elim_sequences(char* out, const char* in, size_t size) noexcept
	{
		if (size <= max_sequence_size)
		{
			for (size_t i = 0; i < size; i++)
				*out++ = Ttransform::transform(*in++);
			return size;
		}
		size_t csz = 1;
		size_t seq = 0;
		char  prev = *in++;
		*out++ = Ttransform::transform(prev);
		while (--size)
		{
			char curr = *in++;
			if (curr == prev)
			{
				if (++seq >= max_sequence_size)
				{
					seq = max_sequence_size;
					continue;
				}
				*out++ = Ttransform::transform(curr); csz++;
			}
			else
			{
				*out++ = Ttransform::transform(curr); csz++;
				seq = 0; prev = curr;
			}
		}
		return csz;
	}
	template <char... terms>
	class string_copy
	{
	private:
		string_copy(void) = delete;
		string_copy(const string_copy&) = delete;
	public:
		static bool copy_elim_sequences(char*& out, size_t outsize, const char*& in) noexcept
		{
			size_t seq = 0;
			char  prev = *in;
			if (terminators<terms...>::isterm(prev))
				return true;
			if (outsize == 0)
				return false;
			*out++ = Ttransform::transform(prev); in++; outsize--;
			while (true)
			{
				char curr = *in;
				if (terminators<terms...>::isterm(curr))
					return true;
				in++;
				if (curr == prev)
				{
					if (++seq >= max_sequence_size)
					{
						seq = max_sequence_size;
						continue;
					}
					if (outsize == 0)
						return false;
					*out++ = Ttransform::transform(curr); outsize--;
				}
				else
				{
					if (outsize == 0)
						return false;
					*out++ = Ttransform::transform(curr); outsize--;
					seq = 0; prev = curr;
				}
			}
			// unreachable
			return false;
		}
	};
};

}}

#endif
