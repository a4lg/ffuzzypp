/*

	ffuzzy++ Helper Libraries

	static_assert_query.hpp
	Basic queries for static assertions

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
#ifndef FFUZZYPP_UTILS_STATIC_ASSERT_QUERY_HPP
#define FFUZZYPP_UTILS_STATIC_ASSERT_QUERY_HPP

#include <cstdint>

namespace ffuzzy {
namespace static_assert_query {

namespace internal
{
	template <template <uintmax_t> class T, uintmax_t a, uintmax_t b>
	struct is_all_bounds
	{
		static_assert(a <= b, "bounds must be valid (a must not exceed b).");
		/*
			For unsigned types,
				a/2 + b/2 + ((a%2)+(b%2))/2
			is a remplacement of
				(a+b)/2
			while preventing arithmetic overflow of (a+b).
		*/
		static constexpr const bool value =
			a > b ? false :
			is_all_bounds<T, a, a / 2 + b / 2 + ((a % 2) + (b % 2)) / 2>::value &&
			is_all_bounds<T, a / 2 + b / 2 + ((a % 2) + (b % 2)) / 2 + 1, b>::value;
	};
	template <template <uintmax_t> class T, uintmax_t a>
	struct is_all_bounds<T, a, a>
	{
		static constexpr const bool value = T<a>::value;
	};

	template <template <uintmax_t> class T, uintmax_t a, uintmax_t b>
	struct is_any_bounds
	{
		static_assert(a <= b, "bounds must be valid (a must not exceed b).");
		static constexpr const bool value =
			a > b ? false :
			is_any_bounds<T, a, a / 2 + b / 2 + ((a % 2) + (b % 2)) / 2>::value ||
			is_any_bounds<T, a / 2 + b / 2 + ((a % 2) + (b % 2)) / 2 + 1, b>::value;
	};
	template <template <uintmax_t> class T, uintmax_t a>
	struct is_any_bounds<T, a, a>
	{
		static constexpr const bool value = T<a>::value;
	};
}


template <template <uintmax_t> class T, uintmax_t n>
struct is_all
{
	static constexpr const bool value = internal::is_all_bounds<T, 0, n-1>::value;
};
template <template <uintmax_t> class T>
struct is_all<T, 0>
{
	static constexpr const bool value = true;
};

template <template <uintmax_t> class T, uintmax_t n>
struct is_any
{
	static constexpr const bool value = internal::is_any_bounds<T, 0, n-1>::value;
};
template <template <uintmax_t> class T>
struct is_any<T, 0>
{
	static constexpr const bool value = false;
};

}}

#endif
