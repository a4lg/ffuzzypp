#
#
#	ffuzzy++ : C++ implementation of fast fuzzy hashing
#
#	tests/Makefile.am
#	Makefile template for tests
#
#	Copyright (C) 2014 Tsukasa OI.
#
#
#	Permission to use, copy, modify, and/or distribute this software for
#	any purpose with or without fee is hereby granted, provided that the
#	above copyright notice and this permission notice appear in all copies.
#
#	THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
#	WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
#	MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
#	ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
#	WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
#	ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
#	OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#
#
AM_CPPFLAGS = -I$(top_srcdir)
LIBS = -lgtest -lgtest_main

if ENABLE_TESTS
noinst_PROGRAMS = test-precond test-small
test_precond_SOURCES  = test_precond.cpp
test_small_SOURCES = test_small.cpp
if ENABLE_COMPATIBILITY_TESTS
noinst_PROGRAMS += test-compatibility-small test-compatibility-large
test_compatibility_small_SOURCES = test_compatibility_small.cpp
test_compatibility_large_SOURCES = test_compatibility_large.cpp
endif
endif

EXTRA_DIST = \
	.gitignore \
	cases/precond/digest.hpp \
	cases/precond/digest_filesize.hpp \
	cases/precond/digest_position_array_base.hpp \
	cases/precond/position_array.hpp \
	cases/precond/utils/minmax.hpp \
	cases/precond/utils/static_assert_query.hpp \
	cases/precond/utils/type_modifier.hpp \
	cases/compatibility/common/blockhash_comparison_min_matching.hpp \
	cases/compatibility/large/blockhash_comparison_min_matching.hpp \
	cases/compatibility/small/blockhash_comparison_max_matching.hpp \
	cases/compatibility/small/blockhash_comparison_min_matching.hpp \
	cases/compatibility/small/digest_comparison_identical.hpp \
	cases/compatibility/small/digest_comparison_identical_2_9_1.hpp \
	cases/compatibility/small/digest_comparison_identical_2_9_2.hpp \
	cases/compatibility/small/digest_data_naturality.hpp \
	cases/compatibility/small/digest_generator_forms.hpp \
	cases/compatibility/small/digest_generator_initial_guess.hpp \
	cases/compatibility/small/digest_generator_nil.hpp \
	cases/compatibility/small/digest_generator_rolling_hash_overflow.hpp \
	cases/compatibility/small/digest_normalization.hpp \
	cases/compatibility/small/digest_position_array_usage.hpp \
	cases/compatibility/small/digest_usage.hpp \
	cases/small/base64.hpp \
	cases/small/common_substr.hpp \
	cases/small/context_hash.hpp \
	cases/small/digest_blocksize.hpp \
	cases/small/digest_comparison_score_cap.hpp \
	cases/small/digest_generator.hpp \
	cases/small/edit_dist.hpp \
	cases/small/nosequences.hpp \
	cases/small/position_array.hpp \
	cases/small/rolling_hash.hpp \
	cases/small/sequences.hpp \
	cases/small/terminators.hpp \
	cases/small/transform.hpp \
	cases/small/utils/minmax.hpp \
	cases/small/utils/ranges.hpp
