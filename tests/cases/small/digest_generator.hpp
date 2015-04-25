/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/small/digest_generator.hpp
	Minimum tests for digest_generator

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
#ifndef FFUZZYPP_TESTCASES_SMALL_DIGEST_GENERATOR_HPP
#define FFUZZYPP_TESTCASES_SMALL_DIGEST_GENERATOR_HPP

#include <limits>


TEST(DigestGeneratorTests, SetFileSizeConstantTrueSpec)
{
	static const digest_filesize_t max_size = digest_filesize::max_size;
	static_assert(max_size >= 4, "max_size must be at least 4 for this test.");
	static const digest_filesize_t true_sizes[] =
	{
		0, 1, 2, 3,
		max_size - 3,
		max_size - 2,
		max_size - 1,
		max_size - 0,
	};
	digest_generator gen;
	for (auto& s : true_sizes)
	{
		EXPECT_TRUE(gen.set_file_size_constant(s))
			<< "set_file_size_constant and reset test (to true) failed at " << s << ".";
		gen.reset();
	}
}

TEST(DigestGeneratorTests, SetFileSizeConstantFalseSpec)
{
	static const digest_filesize_t max_size = digest_filesize::max_size;
	static const digest_filesize_t true_max_size = numeric_limits<digest_filesize_t>::max();
	if (true_max_size == max_size)
		return;
	digest_generator gen;
	for (digest_filesize_t i = max_size + 1; i != true_max_size && i - max_size < 100; i++)
	{
		EXPECT_FALSE(gen.set_file_size_constant(i))
			<< "set_file_size_constant and reset test (to false) failed at " << i << ".";
		gen.reset();
	}
	EXPECT_FALSE(gen.set_file_size_constant(true_max_size))
		<< "set_file_size_constant and reset test (to false) failed at "
		<< true_max_size << ".";
}

#endif
