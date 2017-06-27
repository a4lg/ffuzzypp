/*

	ffuzzy++ : C++ implementation of fast fuzzy hashing

	strings/nosequences.hpp
	String utilities with same interface as sequence utilities

	Copyright (C) 2014 Tsukasa OI <floss_ssdeep@irq.a4lg.com>


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
#ifndef FFUZZYPP_STRINGS_NOSEQUENCES_HPP
#define FFUZZYPP_STRINGS_NOSEQUENCES_HPP

#include <cstddef>

#include "terminators.hpp"
#include "transform.hpp"

namespace ffuzzy {
namespace strings {

template <typename Ttransform = default_char_transform>
class nosequences
{
private:
	nosequences(void) = delete;
	nosequences(const nosequences&) = delete;
public:
	static constexpr bool has_sequences(const char* buf, size_t size) noexcept
	{
		return false;
	}
	static void copy_raw(char* out, const char* in, size_t size) noexcept
	{
		while (size--)
			*out++ = Ttransform::transform(*in++);
	}
	static size_t copy_elim_sequences(char* out, const char* in, size_t size) noexcept
	{
		copy_raw(out, in, size);
		return size;
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
			while (outsize--)
			{
				char ch;
				if (terminators<terms...>::isterm(ch = *in))
					return true;
				in++;
				*out++ = Ttransform::transform(ch);
			}
			if (terminators<terms...>::isterm(*in))
				return true;
			return false;
		}
	};
};

}}

#endif
