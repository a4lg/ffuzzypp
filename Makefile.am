#
#
#	ffuzzy++ : C++ implementation of fast fuzzy hashing
#
#	Makefile.am
#	Makefile template for top directory
#
#	Copyright (C) 2015 Tsukasa OI.
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
if ENABLE_STATIC_LIB
lib_LIBRARIES = libffuzzypp.a
libffuzzypp_a_SOURCES = ffuzzy_declarations.cpp
endif
nobase_include_HEADERS = \
	ffuzzy.hpp \
	ffuzzypp/base64.hpp \
	ffuzzypp/context_hash.hpp \
	ffuzzypp/context_hash_fast.hpp \
	ffuzzypp/digest.hpp \
	ffuzzypp/digest_base.hpp \
	ffuzzypp/digest_blocksize.hpp \
	ffuzzypp/digest_comparison.hpp \
	ffuzzypp/digest_data.hpp \
	ffuzzypp/digest_filesize.hpp \
	ffuzzypp/digest_generator.hpp \
	ffuzzypp/digest_position_array.hpp \
	ffuzzypp/digest_position_array_base.hpp \
	ffuzzypp/rolling_hash.hpp \
	ffuzzypp/rolling_hash_ssdeep.hpp \
	ffuzzypp/strings/common_substr.hpp \
	ffuzzypp/strings/edit_dist.hpp \
	ffuzzypp/strings/nosequences.hpp \
	ffuzzypp/strings/position_array.hpp \
	ffuzzypp/strings/sequences.hpp \
	ffuzzypp/strings/terminators.hpp \
	ffuzzypp/strings/transform.hpp \
	ffuzzypp/utils/likely.hpp \
	ffuzzypp/utils/minmax.hpp \
	ffuzzypp/utils/numeric_digits.hpp \
	ffuzzypp/utils/ranges.hpp \
	ffuzzypp/utils/safe_int.hpp \
	ffuzzypp/utils/static_assert_query.hpp \
	ffuzzypp/utils/type_modifier.hpp
EXTRA_DIST = \
	bootstrap.sh \
	README.md NEWS.md \
	COPYING.md COPYING.GPLv2 COPYING.Boost \
	COPYING.GPLv3 COPYING.GPLv3Autoconf \
	.gitignore .gitattributes ext/.gitignore
SUBDIRS = examples tests
