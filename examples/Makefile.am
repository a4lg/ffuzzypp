#
#
#	ffuzzy++ : C++ implementation of fast fuzzy hashing
#
#	examples/Makefile.am
#	Makefile template for examples
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

if ENABLE_EXAMPLES
noinst_PROGRAMS = compute-hash compare-hash
compute_hash_SOURCES = compute-hash.cpp
compare_hash_SOURCES = compare-hash.cpp
endif
EXTRA_DIST = .gitignore
