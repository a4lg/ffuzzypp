/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/test_small.cpp
	Relatively small tests (which can be completed in a few minutes)

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
#include <cstddef>
#include <gtest/gtest.h>

#ifdef FFUZZYPP_BLOCKDIGEST2_TERMS
#undef FFUZZYPP_BLOCKDIGEST2_TERMS
#endif
#ifndef FFUZZYPP_DECLARATIONS
#define FFUZZYPP_DECLARATIONS
#endif
#include "ffuzzy.hpp"
using namespace ffuzzy;
using namespace std;

#include "cases/small/base64.hpp"
#include "cases/small/context_hash.hpp"
#include "cases/small/digest_blocksize.hpp"
#include "cases/small/digest_comparison_score_cap.hpp"
#include "cases/small/digest_generator.hpp"
#include "cases/small/common_substr.hpp"
#include "cases/small/edit_dist.hpp"
#include "cases/small/nosequences.hpp"
#include "cases/small/position_array.hpp"
#include "cases/small/rolling_hash.hpp"
#include "cases/small/sequences.hpp"
#include "cases/small/terminators.hpp"
#include "cases/small/transform.hpp"
#include "cases/small/utils/minmax.hpp"
#include "cases/small/utils/ranges.hpp"
