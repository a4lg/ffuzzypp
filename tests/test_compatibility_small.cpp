/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/test_compatibility_small.cpp
	Relatively small compatibility tests

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
#ifndef FFUZZYPP_COMPATIBILITY_SSDEEP
#define FFUZZYPP_COMPATIBILITY_SSDEEP
#endif
#ifndef FFUZZYPP_DECLARATIONS
#define FFUZZYPP_DECLARATIONS
#endif
#include "ffuzzy.hpp"
using namespace ffuzzy;
using namespace std;

#include "cases/compatibility/small/blockhash_comparison_max_matching.hpp"
#include "cases/compatibility/small/blockhash_comparison_min_matching.hpp"
#include "cases/compatibility/small/digest_comparison_identical.hpp"
#include "cases/compatibility/small/digest_comparison_identical_2_9_1.hpp"
#include "cases/compatibility/small/digest_comparison_identical_2_9_2.hpp"
#include "cases/compatibility/small/digest_data_naturality.hpp"
#include "cases/compatibility/small/digest_generator_initial_guess.hpp"
#include "cases/compatibility/small/digest_generator_nil.hpp"
#include "cases/compatibility/small/digest_generator_rolling_hash_overflow.hpp"
#include "cases/compatibility/small/digest_generator_forms.hpp"
#include "cases/compatibility/small/digest_normalization.hpp"
#include "cases/compatibility/small/digest_position_array_usage.hpp"
#include "cases/compatibility/small/digest_usage.hpp"
