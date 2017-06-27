/*

	ffuzzy++ : C++ implementation of fast fuzzy hashing

	base64.hpp
	Base64 utilities

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
	static constexpr const char invalid_index = 64;
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
	static_assert(
		invalid_index < 0 || invalid_index >= 64,
		"invalid_index is not out of range.");

public:
	// Utility to check whether given character is in Base64 charset
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
	// Utility to convert character to Base64 index
	static char toindex(char c) noexcept
	{
		switch (c)
		{
		case 'A': return  0;
		case 'B': return  1;
		case 'C': return  2;
		case 'D': return  3;
		case 'E': return  4;
		case 'F': return  5;
		case 'G': return  6;
		case 'H': return  7;
		case 'I': return  8;
		case 'J': return  9;
		case 'K': return 10;
		case 'L': return 11;
		case 'M': return 12;
		case 'N': return 13;
		case 'O': return 14;
		case 'P': return 15;
		case 'Q': return 16;
		case 'R': return 17;
		case 'S': return 18;
		case 'T': return 19;
		case 'U': return 20;
		case 'V': return 21;
		case 'W': return 22;
		case 'X': return 23;
		case 'Y': return 24;
		case 'Z': return 25;
		case 'a': return 26;
		case 'b': return 27;
		case 'c': return 28;
		case 'd': return 29;
		case 'e': return 30;
		case 'f': return 31;
		case 'g': return 32;
		case 'h': return 33;
		case 'i': return 34;
		case 'j': return 35;
		case 'k': return 36;
		case 'l': return 37;
		case 'm': return 38;
		case 'n': return 39;
		case 'o': return 40;
		case 'p': return 41;
		case 'q': return 42;
		case 'r': return 43;
		case 's': return 44;
		case 't': return 45;
		case 'u': return 46;
		case 'v': return 47;
		case 'w': return 48;
		case 'x': return 49;
		case 'y': return 50;
		case 'z': return 51;
		case '0': return 52;
		case '1': return 53;
		case '2': return 54;
		case '3': return 55;
		case '4': return 56;
		case '5': return 57;
		case '6': return 58;
		case '7': return 59;
		case '8': return 60;
		case '9': return 61;
		case '+': return 62;
		case '/': return 63;
		default:  return invalid_index;
		}
	}

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

	// Base64 reverse-transformation (from Base64 character to index)
public:
	class transform_from_b64
	{
	private:
		transform_from_b64(void) = delete;
		transform_from_b64(const transform_from_b64&) = delete;
	public:
		static char transform(char ch) noexcept
		{
			return toindex(ch);
		}
	};
};

#ifdef FFUZZYPP_DECLARATIONS
constexpr const char base64::values[64];
#endif

}

#endif
