/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/precond/digest_position_array_base.hpp
	Digest type (position array) base class precondition tests

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
#ifndef FFUZZYPP_TESTCASES_PRECOND_DIGEST_POSITION_ARRAY_BASE_HPP
#define FFUZZYPP_TESTCASES_PRECOND_DIGEST_POSITION_ARRAY_BASE_HPP

#ifndef FFUZZYPP_DISABLE_POSITION_ARRAY
static_assert(digest_params::max_blockhash_len > 64 || digest_position_array_params<true>::is_available,
	"if max_blockhash_len is no greater than 64, position array version of the digest must be available.");
#endif

#endif
