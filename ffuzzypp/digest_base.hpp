/*

	ffuzzy++ : C++ implementation of fast fuzzy hashing

	digest_base.hpp
	Fuzzy digest (wrapper with comparison methods)


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
#ifndef FFUZZYPP_DIGEST_BASE_HPP
#define FFUZZYPP_DIGEST_BASE_HPP

#include <string>

#include "digest_blocksize.hpp"
#include "digest_data.hpp"
#include "digest_comparison.hpp"
#include "utils/type_modifier.hpp"

namespace ffuzzy {

template <bool IsAlphabetRestricted, bool IsShort, bool IsNormalized> class digest_base;

// Normalized form of digest (with specialized/fast comparison)
template <bool IsAlphabetRestricted, bool IsShort>
class digest_base<IsAlphabetRestricted, IsShort, true>
	: public digest_data<IsAlphabetRestricted, IsShort>
{
public:
	digest_base(void) noexcept = default; // initialize to undefined state
	digest_base(const digest_base& other) noexcept : digest_data<IsAlphabetRestricted, IsShort>(other) {}
	const digest_base& operator=(const digest_base& other) noexcept
	{
		digest_data<IsAlphabetRestricted, IsShort>::operator=(other);
		return *this;
	}
public:
	explicit digest_base(const char* str) noexcept(false)
	{
		if (!digest_data<IsAlphabetRestricted, IsShort>::parse_normalized(*this, str))
			throw digest_parse_error();
	}
	explicit digest_base(const std::string& str)
		: digest_base(str.c_str()) {}
	static bool parse_normalized(digest_base& digest, const char* str) noexcept
	{
		return digest_data<IsAlphabetRestricted, IsShort>::parse_normalized(digest, str);
	}
	static bool parse(digest_base& digest, const char* str) noexcept
	{
		return digest_data<IsAlphabetRestricted, IsShort>::parse_normalized(digest, str);
	}
public:
	bool is_valid(void) const noexcept
	{
		return digest_data<IsAlphabetRestricted, IsShort>::is_valid() && this->template is_normalized();
	}

	// Comparison
public:
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare(
		const digest_base& a,
		const digest_base& b
	) noexcept
	{
		return digest_comparison<Version>::compare(a, b);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_identical(
		const digest_base& value
	) noexcept
	{
		return digest_comparison<Version>::compare_identical(value);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_near(
		const digest_base& a,
		const digest_base& b
	) noexcept
	{
		return digest_comparison<Version>::compare_near(a, b);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_near_eq(
		const digest_base& a,
		const digest_base& b
	) noexcept
	{
		return digest_comparison<Version>::compare_near_eq(a, b);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_near_lt(
		const digest_base& a,
		const digest_base& b
	) noexcept
	{
		return digest_comparison<Version>::compare_near_lt(a, b);
	}
public:
	template <comparison_version Version = comparison_version::latest>
	digest_comparison_score_t compare(const digest_base& other) const noexcept
	{
		return compare<Version>(*this, other);
	}
	template <comparison_version Version = comparison_version::latest>
	digest_comparison_score_t compare_identical(void) const noexcept
	{
		return compare_identical<Version>(*this);
	}
	template <comparison_version Version = comparison_version::latest>
	digest_comparison_score_t compare_near(const digest_base& other) const noexcept
	{
		return compare_near<Version>(*this, other);
	}
	template <comparison_version Version = comparison_version::latest>
	digest_comparison_score_t compare_near_eq(const digest_base& other) const noexcept
	{
		return compare_near_eq<Version>(*this, other);
	}
	template <comparison_version Version = comparison_version::latest>
	digest_comparison_score_t compare_near_lt(const digest_base& other) const noexcept
	{
		return compare_near_lt<Version>(*this, other);
	}

	// Comparison (on different digests)
public:
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_diff(
		const digest_base& a,
		const digest_base& b
	) noexcept
	{
		return digest_comparison<Version>::compare_diff(a, b);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_near_diff(
		const digest_base& a,
		const digest_base& b
	) noexcept
	{
		return digest_comparison<Version>::compare_near_diff(a, b);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_near_eq_diff(
		const digest_base& a,
		const digest_base& b
	) noexcept
	{
		return digest_comparison<Version>::compare_near_eq_diff(a, b);
	}
public:
	template <comparison_version Version = comparison_version::latest>
	digest_comparison_score_t compare_diff(const digest_base& other) const noexcept
	{
		return compare_diff<Version>(*this, other);
	}
	template <comparison_version Version = comparison_version::latest>
	digest_comparison_score_t compare_near_diff(const digest_base& other) const noexcept
	{
		return compare_near_diff<Version>(*this, other);
	}
	template <comparison_version Version = comparison_version::latest>
	digest_comparison_score_t compare_near_eq_diff(const digest_base& other) const noexcept
	{
		return compare_near_eq_diff<Version>(*this, other);
	}
};

// Unnormalized form of digest (with normalization ability and slow comparison)
template <bool IsAlphabetRestricted, bool IsShort>
class digest_base<IsAlphabetRestricted, IsShort, false>
	: public digest_data<IsAlphabetRestricted, IsShort>
{
public:
	digest_base(void) noexcept = default; // initialize to undefined state
	digest_base(const digest_base& other) noexcept : digest_data<IsAlphabetRestricted, IsShort>(other) {}
	digest_base(const digest_base<IsAlphabetRestricted, IsShort, true>& other) noexcept : digest_data<IsAlphabetRestricted, IsShort>(other) {}
	const digest_base& operator=(const digest_base& other) noexcept
	{
		digest_data<IsAlphabetRestricted, IsShort>::operator=(other);
		return *this;
	}
	const digest_base& operator=(const digest_base<IsAlphabetRestricted, IsShort, true>& other) noexcept
	{
		digest_data<IsAlphabetRestricted, IsShort>::operator=(other);
		return *this;
	}
public:
	explicit digest_base(const char* str) noexcept(false)
	{
		if (!digest_data<IsAlphabetRestricted, IsShort>::parse(*this, str))
			throw digest_parse_error();
	}
	explicit digest_base(const std::string& str)
		: digest_base(str.c_str()) {}
	static bool parse_normalized(digest_base& digest, const char* str) noexcept
	{
		return digest_data<IsAlphabetRestricted, IsShort>::parse_normalized(digest, str);
	}
	static bool parse(digest_base& digest, const char* str) noexcept
	{
		return digest_data<IsAlphabetRestricted, IsShort>::parse(digest, str);
	}
public:
	digest_base<IsAlphabetRestricted, IsShort, true> to_normalized(void) const noexcept
	{
		return digest_data<IsAlphabetRestricted, IsShort>::
			template normalize<digest_base<IsAlphabetRestricted, IsShort, true>>(*this);
	}
	explicit operator digest_base<IsAlphabetRestricted, IsShort, true>(void) const noexcept { return to_normalized(); }
public:
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare(
		const digest_base& a,
		const digest_base& b
	) noexcept
	{
		return digest_comparison<Version>::compare_unnormalized(a, b);
	}
public:
	template <comparison_version Version = comparison_version::latest>
	digest_comparison_score_t compare(const digest_base& other) const noexcept { return compare<Version>(*this, other); }
};

}

#endif
