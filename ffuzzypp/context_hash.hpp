/*

	ffuzzy++ : C++ implementation of fast fuzzy hashing

	context_hash.hpp
	Context computation (non-rolling) hash implementation


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
#ifndef FFUZZYPP_CONTEXT_HASH_HPP
#define FFUZZYPP_CONTEXT_HASH_HPP

#include <cstdint>

namespace ffuzzy {

class context_hash
{
private:
	// Don't use UINT32_C not to define __STDC_CONSTANT_MACROS in ffuzzy++
	static constexpr const uint_least32_t hash_init  = uint_least32_t(0x28021967ul);
	static constexpr const uint_least32_t hash_prime = uint_least32_t(0x01000193ul);
private:
	uint_least32_t h;
public:
	static constexpr uint_least32_t initial_state(void) noexcept
	{
		return hash_init;
	}
	void reset(void) noexcept
	{
		h = initial_state();
	}
	static constexpr uint_least32_t next_state(uint_least32_t h, unsigned char c) noexcept
	{
		return ((h * hash_prime) ^ uint_least32_t(c)) & uint_least32_t(0xfffffffful);
	}
	void update(unsigned char c) noexcept
	{
		h = next_state(h, c);
	}
	uint_least32_t sum(void) const noexcept
	{
		return h;
	}
public:
	char sum_in_base64(void) const noexcept
	{
		// sum for Base64 (returns Base64 index)
		return static_cast<char>(h & 0x3f);
	}
public:
	context_hash(void) noexcept = default; // initialize to undefined state
};

}

#endif
