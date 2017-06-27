/*

	ffuzzy++ : C++ implementation of fast fuzzy hashing

	digest_position_array.hpp
	Fuzzy digest (position array representation)

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
#ifndef FFUZZYPP_DIGEST_POSITION_ARRAY_HPP
#define FFUZZYPP_DIGEST_POSITION_ARRAY_HPP

#include <algorithm>
#include <string>

#include "digest_base.hpp"
#include "digest_position_array_base.hpp"
#include "digest_comparison.hpp"

namespace ffuzzy {

template <bool IsAlphabetRestricted>
class digest_position_array
	: public digest_position_array_base<IsAlphabetRestricted>
{
public:
	static constexpr const bool is_alphabet_restricted = IsAlphabetRestricted;

public:
	digest_position_array(void) noexcept = default; // initialize to undefined state
	digest_position_array(const digest_position_array& other) noexcept
		: digest_position_array_base<IsAlphabetRestricted>(other) {}
	const digest_position_array& operator=(const digest_position_array& other) noexcept
	{
		digest_position_array_base<IsAlphabetRestricted>::operator=(other);
		return *this;
	}

	// Initialization by digest
public:
	template <bool IsShort>
	digest_position_array(const digest_base<IsAlphabetRestricted, IsShort, true>& src) noexcept
	{
		digest_position_array_base<IsAlphabetRestricted>::construct(*this, src);
	}
	template <bool IsShort>
	digest_position_array(const digest_base<IsAlphabetRestricted, IsShort, false>& src) noexcept
	{
		digest_position_array_base<IsAlphabetRestricted>::construct(*this, digest_data<IsAlphabetRestricted, IsShort>::normalize(src));
	}
public:
	template <bool IsShort>
	static void construct(
		digest_position_array& dest,
		const digest_base<IsAlphabetRestricted, IsShort, true>& src
	) noexcept
	{
		digest_position_array_base<IsAlphabetRestricted>::construct(dest, src);
	}
	template <bool IsShort>
	static void construct(
		digest_position_array& dest,
		const digest_base<IsAlphabetRestricted, IsShort, false>& src
	) noexcept
	{
		digest_position_array_base<IsAlphabetRestricted>::construct(dest, digest_data<IsAlphabetRestricted, IsShort>::normalize(src));
	}

	// Initialization by digest string
public:
	explicit digest_position_array(const char* str) noexcept(false)
	{
		digest_base<IsAlphabetRestricted, false, true> digest(str);
		construct(*this, digest);
	}
	explicit digest_position_array(const std::string& str)
		: digest_position_array(str.c_str()) {}

	// Comparison
public:
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare(
		const digest_position_array& a,
		const digest_base<IsAlphabetRestricted, false, true>& b
	) noexcept
	{
		return digest_comparison<Version>::compare(a, b);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare(
		const digest_position_array& a,
		const digest_base<IsAlphabetRestricted, true, true>& b
	) noexcept
	{
		return digest_comparison<Version>::compare(a, b);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare(
		const digest_base<IsAlphabetRestricted, false, true>& a,
		const digest_position_array& b
	) noexcept
	{
		return digest_comparison<Version>::compare(b, a);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare(
		const digest_base<IsAlphabetRestricted, true, true>& a,
		const digest_position_array& b
	) noexcept
	{
		return digest_comparison<Version>::compare(b, a);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_near(
		const digest_position_array& a,
		const digest_base<IsAlphabetRestricted, false, true>& b
	) noexcept
	{
		return digest_comparison<Version>::compare_near(a, b);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_near(
		const digest_position_array& a,
		const digest_base<IsAlphabetRestricted, true, true>& b
	) noexcept
	{
		return digest_comparison<Version>::compare_near(a, b);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_near(
		const digest_base<IsAlphabetRestricted, false, true>& a,
		const digest_position_array& b
	) noexcept
	{
		return digest_comparison<Version>::compare_near(b, a);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_near(
		const digest_base<IsAlphabetRestricted, true, true>& a,
		const digest_position_array& b
	) noexcept
	{
		return digest_comparison<Version>::compare_near(b, a);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_near_eq(
		const digest_position_array& a,
		const digest_base<IsAlphabetRestricted, false, true>& b
	) noexcept
	{
		return digest_comparison<Version>::compare_near_eq(a, b);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_near_eq(
		const digest_position_array& a,
		const digest_base<IsAlphabetRestricted, true, true>& b
	) noexcept
	{
		return digest_comparison<Version>::compare_near_eq(a, b);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_near_eq(
		const digest_base<IsAlphabetRestricted, false, true>& a,
		const digest_position_array& b
	) noexcept
	{
		return digest_comparison<Version>::compare_near_eq(b, a);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_near_eq(
		const digest_base<IsAlphabetRestricted, true, true>& a,
		const digest_position_array& b
	) noexcept
	{
		return digest_comparison<Version>::compare_near_eq(b, a);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_near_lt(
		const digest_position_array& a,
		const digest_base<IsAlphabetRestricted, false, true>& b
	) noexcept
	{
		return digest_comparison<Version>::compare_near_lt(a, b);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_near_lt(
		const digest_position_array& a,
		const digest_base<IsAlphabetRestricted, true, true>& b
	) noexcept
	{
		return digest_comparison<Version>::compare_near_lt(a, b);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_near_lt(
		const digest_base<IsAlphabetRestricted, false, true>& a,
		const digest_position_array& b
	) noexcept
	{
		return digest_comparison<Version>::compare_near_lt(a, b);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_near_lt(
		const digest_base<IsAlphabetRestricted, true, true>& a,
		const digest_position_array& b
	) noexcept
	{
		return digest_comparison<Version>::compare_near_lt(a, b);
	}
public:
	template <comparison_version Version = comparison_version::latest>
	digest_comparison_score_t compare(const digest_base<IsAlphabetRestricted, false, true>& other) const noexcept
	{
		return compare<Version>(*this, other);
	}
	template <comparison_version Version = comparison_version::latest>
	digest_comparison_score_t compare(const digest_base<IsAlphabetRestricted, true, true>& other) const noexcept
	{
		return compare<Version>(*this, other);
	}
	template <comparison_version Version = comparison_version::latest>
	digest_comparison_score_t compare_near(const digest_base<IsAlphabetRestricted, false, true>& other) const noexcept
	{
		return compare_near<Version>(*this, other);
	}
	template <comparison_version Version = comparison_version::latest>
	digest_comparison_score_t compare_near(const digest_base<IsAlphabetRestricted, true, true>& other) const noexcept
	{
		return compare_near<Version>(*this, other);
	}
	template <comparison_version Version = comparison_version::latest>
	digest_comparison_score_t compare_near_eq(const digest_base<IsAlphabetRestricted, false, true>& other) const noexcept
	{
		return compare_near_eq<Version>(*this, other);
	}
	template <comparison_version Version = comparison_version::latest>
	digest_comparison_score_t compare_near_eq(const digest_base<IsAlphabetRestricted, true, true>& other) const noexcept
	{
		return compare_near_eq<Version>(*this, other);
	}
	template <comparison_version Version = comparison_version::latest>
	digest_comparison_score_t compare_near_lt(const digest_base<IsAlphabetRestricted, false, true>& other) const noexcept
	{
		return compare_near_lt<Version>(*this, other);
	}
	template <comparison_version Version = comparison_version::latest>
	digest_comparison_score_t compare_near_lt(const digest_base<IsAlphabetRestricted, true, true>& other) const noexcept
	{
		return compare_near_lt<Version>(*this, other);
	}
	template <comparison_version Version = comparison_version::latest>
	digest_comparison_score_t compare_near_gt(const digest_base<IsAlphabetRestricted, false, true>& other) const noexcept
	{
		return compare_near_lt<Version>(other, *this);
	}
	template <comparison_version Version = comparison_version::latest>
	digest_comparison_score_t compare_near_gt(const digest_base<IsAlphabetRestricted, true, true>& other) const noexcept
	{
		return compare_near_lt<Version>(other, *this);
	}

	// Comparison (on different digests)
public:
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_diff(
		const digest_position_array& a,
		const digest_base<IsAlphabetRestricted, false, true>& b
	) noexcept
	{
		return digest_comparison<Version>::compare_diff(a, b);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_diff(
		const digest_position_array& a,
		const digest_base<IsAlphabetRestricted, true, true>& b
	) noexcept
	{
		return digest_comparison<Version>::compare_diff(a, b);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_diff(
		const digest_base<IsAlphabetRestricted, false, true>& a,
		const digest_position_array& b
	) noexcept
	{
		return digest_comparison<Version>::compare_diff(b, a);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_diff(
		const digest_base<IsAlphabetRestricted, true, true>& a,
		const digest_position_array& b
	) noexcept
	{
		return digest_comparison<Version>::compare_diff(b, a);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_near_diff(
		const digest_position_array& a,
		const digest_base<IsAlphabetRestricted, false, true>& b
	) noexcept
	{
		return digest_comparison<Version>::compare_near_diff(a, b);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_near_diff(
		const digest_position_array& a,
		const digest_base<IsAlphabetRestricted, true, true>& b
	) noexcept
	{
		return digest_comparison<Version>::compare_near_diff(a, b);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_near_diff(
		const digest_base<IsAlphabetRestricted, false, true>& a,
		const digest_position_array& b
	) noexcept
	{
		return digest_comparison<Version>::compare_near_diff(b, a);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_near_diff(
		const digest_base<IsAlphabetRestricted, true, true>& a,
		const digest_position_array& b
	) noexcept
	{
		return digest_comparison<Version>::compare_near_diff(b, a);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_near_eq_diff(
		const digest_position_array& a,
		const digest_base<IsAlphabetRestricted, false, true>& b
	) noexcept
	{
		return digest_comparison<Version>::compare_near_eq_diff(a, b);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_near_eq_diff(
		const digest_position_array& a,
		const digest_base<IsAlphabetRestricted, true, true>& b
	) noexcept
	{
		return digest_comparison<Version>::compare_near_eq_diff(a, b);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_near_eq_diff(
		const digest_base<IsAlphabetRestricted, false, true>& a,
		const digest_position_array& b
	) noexcept
	{
		return digest_comparison<Version>::compare_near_eq_diff(b, a);
	}
	template <comparison_version Version = comparison_version::latest>
	static digest_comparison_score_t compare_near_eq_diff(
		const digest_base<IsAlphabetRestricted, true, true>& a,
		const digest_position_array& b
	) noexcept
	{
		return digest_comparison<Version>::compare_near_eq_diff(b, a);
	}
public:
	template <comparison_version Version = comparison_version::latest>
	digest_comparison_score_t compare_diff(const digest_base<IsAlphabetRestricted, false, true>& other) const noexcept
	{
		return compare_diff<Version>(*this, other);
	}
	template <comparison_version Version = comparison_version::latest>
	digest_comparison_score_t compare_diff(const digest_base<IsAlphabetRestricted, true, true>& other) const noexcept
	{
		return compare_diff<Version>(*this, other);
	}
	template <comparison_version Version = comparison_version::latest>
	digest_comparison_score_t compare_near_diff(const digest_base<IsAlphabetRestricted, false, true>& other) const noexcept
	{
		return compare_near_diff<Version>(*this, other);
	}
	template <comparison_version Version = comparison_version::latest>
	digest_comparison_score_t compare_near_diff(const digest_base<IsAlphabetRestricted, true, true>& other) const noexcept
	{
		return compare_near_diff<Version>(*this, other);
	}
	template <comparison_version Version = comparison_version::latest>
	digest_comparison_score_t compare_near_eq_diff(const digest_base<IsAlphabetRestricted, false, true>& other) const noexcept
	{
		return compare_near_eq_diff<Version>(*this, other);
	}
	template <comparison_version Version = comparison_version::latest>
	digest_comparison_score_t compare_near_eq_diff(const digest_base<IsAlphabetRestricted, true, true>& other) const noexcept
	{
		return compare_near_eq_diff<Version>(*this, other);
	}
};

typedef digest_position_array< true> digest_position_array_t;
typedef digest_position_array<false> digest_position_array_non_ra_t;

}


// Specialization of standard swap
namespace std
{
	template <bool IsAlphabetRestricted>
	inline void swap(
		ffuzzy::digest_position_array<IsAlphabetRestricted>& a,
		ffuzzy::digest_position_array<IsAlphabetRestricted>& b
	) noexcept
	{
		ffuzzy::digest_position_array<IsAlphabetRestricted>::swap(a, b);
	}
}

#endif
