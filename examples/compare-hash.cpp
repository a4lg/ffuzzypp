/*

	ffuzzy++ examples

	compare-hash.cpp
	Fuzzy hash comparison program

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

#include <cstdio>

/*
	This enables compilation without libffuzzypp.a.
	Define this in **one** of the source files to make the linker happy.
*/
#define FFUZZYPP_DECLARATIONS

// the following line will activate assertions.
//#define FFUZZYPP_DEBUG

#include "ffuzzy.hpp"
using namespace ffuzzy;

/*
	Both "digest_unorm_t" and "digest_t" are standard layout types and
	contains exactly same sequences.
	This means you can contain these data in the single union
	to save memory. Note that some of operations are still invalid
	because "normalized" type requires digest buffer normalized.
*/
typedef union
{
	digest_unorm_t u;
	digest_t d;
} unified_digest_t;
typedef union
{
	digest_long_unorm_t u;
	digest_long_t d;
} unified_digest_long_t;

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "usage: %s HASH1 HASH2\n", argv[0]);
		return 1;
	}
	#if 1
	unified_digest_t h1, h2;
	#else
	unified_digest_long_t h1, h2;
	#endif
	char digestbuf[decltype(h1.u)::max_natural_chars];

	// Parse digests
	if (!decltype(h1.u)::parse(h1.u, argv[1]))
	{
		fprintf(stderr, "error: failed to parse HASH1.\n");
		return 1;
	}
	if (!decltype(h2.u)::parse(h2.u, argv[2]))
	{
		fprintf(stderr, "error: failed to parse HASH2.\n");
		return 1;
	}

	/*
		Restringize digests (just for demo)
		Notice that we're using h1.d instead of h1.u?
		This is not a good example but works perfectly.
	*/
	if (!h1.d.pretty_unsafe(digestbuf))
	{
		fprintf(stderr, "abort: failed to re-stringize HASH1.\n");
		return 1;
	}
	printf("HASH1 : %s\n", digestbuf);
	if (!h2.d.pretty_unsafe(digestbuf))
	{
		fprintf(stderr, "abort: failed to re-stringize HASH2.\n");
		return 1;
	}
	printf("HASH2 : %s\n", digestbuf);

	// Normalize digests and restringize them
	decltype(h1.d)::normalize(h1.d, h1.u);
	decltype(h2.d)::normalize(h2.d, h2.u);
	if (!h1.d.pretty_unsafe(digestbuf))
	{
		fprintf(stderr, "abort: failed to re-stringize HASH1.\n");
		return 1;
	}
	printf("NORM1 : %s\n", digestbuf);
	if (!h2.d.pretty_unsafe(digestbuf))
	{
		fprintf(stderr, "abort: failed to re-stringize HASH2.\n");
		return 1;
	}
	printf("NORM2 : %s\n", digestbuf);

	/*
		Compare them
		"Unnormalized" form has compare function but slow because of additional normalization)

		Note:
		Use `compare` or `compare<comparison_version::latest>` for latest
		version and `compare<comparison_version::v2_9>` for version 2.9 emulation.
	*/
	digest_comparison_score_t score =
		decltype(h1.d)::compare<comparison_version::v2_9>(h1.d, h2.d);
	printf("SCORE: %u\n", unsigned(score)); // safe to cast to unsigned (value is in [0,100])

	return 0;
}
