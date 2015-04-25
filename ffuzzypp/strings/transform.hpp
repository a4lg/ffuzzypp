/*

	ffuzzy++ : C++ implementation of fast fuzzy hashing

	strings/transform.hpp
	String transformation (char-to-char mapping) utility

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
#ifndef FFUZZYPP_STRINGS_TRANSFORM_HPP
#define FFUZZYPP_STRINGS_TRANSFORM_HPP

namespace ffuzzy {
namespace strings {

class default_char_transform
{
private:
	default_char_transform(void) = delete;
	default_char_transform(const default_char_transform&) = delete;
public:
	static constexpr char transform(char ch) noexcept
	{
		return ch;
	}
};

}}

#endif
