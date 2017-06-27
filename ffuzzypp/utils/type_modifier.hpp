/*

	ffuzzy++ Helper Libraries

	type_modifier.hpp
	C++11 type modifier utilities

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
#ifndef FFUZZYPP_UTILS_TYPE_MODIFIER_HPP
#define FFUZZYPP_UTILS_TYPE_MODIFIER_HPP

#include <type_traits>

namespace ffuzzy {
namespace type_mod {

template <typename T, bool IsConst, bool IsVolatile>
struct cv_selector;
template <typename T>
struct cv_selector<T, false, false>
{
	typedef T type;
};
template <typename T>
struct cv_selector<T, true, false>
{
	typedef const T type;
};
template <typename T>
struct cv_selector<T, false, true>
{
	typedef volatile T type;
};
template <typename T>
struct cv_selector<T, true, true>
{
	typedef const volatile T type;
};
template <
	typename T,
	typename Tmatch,
	bool IsConst = std::is_const<T>::value,
	bool IsVolatile = std::is_volatile<T>::value
>
struct cv_match
{
	typedef typename cv_selector<Tmatch, IsConst, IsVolatile>::type type;
};

}}

#endif
