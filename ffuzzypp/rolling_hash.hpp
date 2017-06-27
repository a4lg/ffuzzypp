/*

	ffuzzy++ : C++ implementation of fast fuzzy hashing

	rolling_hash.hpp
	Rolling hash implementation (without known bugs)


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

	Copyright (C) 2017 Tsukasa OI <floss_ssdeep@irq.a4lg.com>

*/
#ifndef FFUZZYPP_ROLLING_HASH_HPP
#define FFUZZYPP_ROLLING_HASH_HPP

#include <cstddef>
#include <cstdint>

namespace ffuzzy {

class rolling_hash
{
public:
	static constexpr const uint_least32_t window_size = 7;
	static_assert(window_size != 0, "window_size must not be zero.");
protected:
	uint_least32_t h1, h2, h3;
	uint_least32_t n;
	unsigned char window[window_size];
public:
	void reset(void) noexcept
	{
		h1 = h2 = h3 = n = 0;
		for (size_t i = 0; i < window_size; i++)
			window[i] = 0;
	}
	void update(unsigned char c) noexcept
	{
		uint_least32_t C(c);
		h2 = h2 - h1 + window_size * C;
		h1 = h1 + C - window[n];
		h3 = (h3 << 5) ^ C;
		window[n] = c;
		n++;
		if (n == window_size)
			n = 0;
	}
	uint_least32_t sum(void) const noexcept
	{
		return (h1 + h2 + h3) & uint_least32_t(0xfffffffful);
	}
public:
	rolling_hash(void) noexcept
	{
		reset();
	}
};

}

#endif
