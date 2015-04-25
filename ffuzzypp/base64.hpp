/*

	ffuzzy++ : C++ implementation of fast fuzzy hashing

	base64.hpp
	Base64 utilities

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
#ifndef FFUZZYPP_BASE64_HPP
#define FFUZZYPP_BASE64_HPP

namespace ffuzzy {

class base64
{
private:
	base64(void) = delete;
	base64(const base64&) = delete;

	// Base64 table
public:
	static constexpr const char values[64] =
	{
		// This should be the same order of ordinal Base64
		// (as well as b64 variable in fuzzy.c)
		'A', 'B', 'C', 'D', 'E', 'F', 'G',
		'H', 'I', 'J', 'K', 'L', 'M', 'N',
		'O', 'P', 'Q', 'R', 'S', 'T', 'U',
		'V', 'W', 'X', 'Y', 'Z',
		'a', 'b', 'c', 'd', 'e', 'f', 'g',
		'h', 'i', 'j', 'k', 'l', 'm', 'n',
		'o', 'p', 'q', 'r', 's', 't', 'u',
		'v', 'w', 'x', 'y', 'z',
		'0', '1', '2', '3', '4',
		'5', '6', '7', '8', '9',
		'+', '/'
	};
	static_assert(
		sizeof(values) == 64 &&
		values[ 0] == 'A' &&
		values[26] == 'a' &&
		values[52] == '0' &&
		values[62] == '+' &&
		values[63] == '/',
		"minimum sanity check for Base64 failed.");

	// Base64 transformation (from index to actual character)
public:
	class transform_to_b64
	{
	private:
		transform_to_b64(void) = delete;
		transform_to_b64(const transform_to_b64&) = delete;
	public:
		static constexpr char transform(char ch) noexcept
		{
			return base64::values[static_cast<unsigned char>(ch)];
		}
	};

	// Utility to check whether given character is in Base64 charset
public:
	static bool isbase64(char c) noexcept
	{
		switch (c)
		{
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
		case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
		case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
		case 'V': case 'W': case 'X': case 'Y': case 'Z':
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
		case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
		case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
		case 'v': case 'w': case 'x': case 'y': case 'z':
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
		case '+': case '/':
			return true;
		default:
			return false;
		}
	}
};

#ifdef FFUZZYPP_DECLARATIONS
constexpr const char base64::values[64];
#endif

}

#endif
