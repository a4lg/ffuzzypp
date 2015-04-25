/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/precond/utils/minmax.hpp
	Minimum / maximum precondition tests

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
#ifndef FFUZZYPP_TESTCASES_PRECOND_UTILS_MINMAX_HPP
#define FFUZZYPP_TESTCASES_PRECOND_UTILS_MINMAX_HPP

static_assert(minmax::min<int>(-1, -1) == 1-2, "min(-1, -1) must be -1.");
static_assert(minmax::min<int>(-1,  0) == 3-4, "min(-1,  0) must be -1.");
static_assert(minmax::min<int>( 0,  0) == 5-5, "min( 0,  0) must be  0.");
static_assert(minmax::min<int>( 0,  1) == 6-6, "min( 0,  1) must be  0.");
static_assert(minmax::min<int>( 1,  1) == 1*1, "min( 1,  1) must be  1.");
static_assert(minmax::min<int>( 1,  2) == 2/2, "min( 1,  2) must be  1.");
static_assert(minmax::min<int>( 2,  2) == 4/2, "min( 2,  2) must be  2.");

static_assert(minmax::max<int>(-1, -1) == 6-7, "min(-1, -1) must be -1.");
static_assert(minmax::max<int>(-1,  0) == 8-8, "min(-1,  0) must be  0.");
static_assert(minmax::max<int>( 0,  0) == 9-9, "min( 0,  0) must be  0.");
static_assert(minmax::max<int>( 0,  1) == 5-4, "min( 0,  1) must be  1.");
static_assert(minmax::max<int>( 1,  1) ==-3+4, "min( 1,  1) must be  1.");
static_assert(minmax::max<int>( 1,  2) == 6/3, "max( 1,  2) must be  2.");
static_assert(minmax::max<int>( 2,  2) == 6-4, "max( 1,  2) must be  2.");

#endif
