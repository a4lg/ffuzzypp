/*

	ffuzzy++ Helper Libraries

	minmax.hpp
	C++14-compatible minimum / maximum library

	Copyright (C) 2015 Tsukasa OI <floss_ssdeep@irq.a4lg.com>


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
#ifndef FFUZZYPP_UTILS_MINMAX_HPP
#define FFUZZYPP_UTILS_MINMAX_HPP

/*
	std::min and std::max in C++11 are not marked constexpr.
	On the other hand, C++14's std::min and std::max are marked constexpr.

	This library implements constexpr min and max in C++11
	(but compatible with C++14).

	However, we won't implement initializer_list variant of
	min and max since we aren't using it.
*/

namespace ffuzzy {
namespace minmax {

template <class T>
inline constexpr const T& min(const T& a, const T& b)
{
	return b < a ? b : a;
}

template <class T, class Compare>
inline constexpr const T& min(const T& a, const T& b, Compare comp)
{
	return comp(b, a) ? b : a;
}

template <class T>
inline constexpr const T& max(const T& a, const T& b)
{
	return a < b ? b : a;
}

template <class T, class Compare>
inline constexpr const T& max(const T& a, const T& b, Compare comp)
{
	return comp(a, b) ? b : a;
}

}}

#endif
