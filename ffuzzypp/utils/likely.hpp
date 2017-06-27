/*

	ffuzzy++ Helper Libraries

	likely.hpp
	Branch prediction control macros

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
#ifndef FFUZZYPP_UTILS_LIKELY_HPP
#define FFUZZYPP_UTILS_LIKELY_HPP

#define FFUZZYPP_LIKELY_PORTABLE(x) (!!(x))

#ifndef FFUZZYPP_DISABLE_COMPILER_BUILTINS
#if   !defined(__GNUC__)
#define FFUZZYPP_DISABLE_COMPILER_BUILTINS 1
#elif __GNUC__ < 2
// DISABLE: GNU compiler version < 2.0
#define FFUZZYPP_DISABLE_COMPILER_BUILTINS 1
#elif __GNUC__ >= 3
// ENABLE:  GNU compiler version >= 3.0
#elif !defined(__GNUC_MINOR__)
#define FFUZZYPP_DISABLE_COMPILER_BUILTINS 1
#elif __GNUC_MINOR__ <= 95
// DISABLE: GNU compiler version <= 2.95
#define FFUZZYPP_DISABLE_COMPILER_BUILTINS 1
#endif
#endif

#ifdef  FFUZZYPP_LIKELY
#undef  FFUZZYPP_LIKELY
#endif
#ifdef  FFUZZYPP_UNLIKELY
#undef  FFUZZYPP_UNLIKELY
#endif

#ifdef  FFUZZYPP_DISABLE_COMPILER_BUILTINS
#define FFUZZYPP_LIKELY   FFUZZYPP_LIKELY_PORTABLE
#define FFUZZYPP_UNLIKELY FFUZZYPP_LIKELY_PORTABLE
#else
#define FFUZZYPP_LIKELY(x)   (__builtin_expect(!!(x), 1))
#define FFUZZYPP_UNLIKELY(x) (__builtin_expect(!!(x), 0))
#endif

#endif
