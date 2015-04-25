/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/compatibility/small/digest_generator_initial_guess.hpp
	Tests for digest generator (regarding initial guess of block size)

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
#ifndef FFUZZYPP_TESTCASES_COMPATIBILITY_SMALL_DIGEST_GENERATOR_INITIAL_GUESS_HPP
#define FFUZZYPP_TESTCASES_COMPATIBILITY_SMALL_DIGEST_GENERATOR_INITIAL_GUESS_HPP

#include <string>


class DigestGeneratorInitialGuessTests : public ::testing::Test
{
public:
	static void fill_seq(
		digest_generator& generator,
		unsigned initial_repetition,
		size_t size_to_fill
	)
	{
		static const unsigned char zerobuf[4096] = {0};
		static const unsigned char seq30[] = { 0xfc, 0x5d, 0x5d, 0x5d, 0x5d, 0xeb, 0xf0 };
		for (unsigned i = 0; i < initial_repetition; i++)
			generator.update(seq30, sizeof(seq30));
		if (size_to_fill < generator.total_size())
			FAIL() << "size_to_fill must be equal to or greater than initial fill size.";
		digest_filesize_t remaining = size_to_fill - generator.total_size();
		while (remaining >= sizeof(zerobuf))
		{
			generator.update(zerobuf, sizeof(zerobuf));
			remaining -= sizeof(zerobuf);
		}
		generator.update(zerobuf, remaining);
	}
};


TEST_F(DigestGeneratorInitialGuessTests, BlockHashIndexGuessedByFilesizeBelowBorder)
{
	for (unsigned i = 0; i < digest_blocksize::number_of_blockhashes; i++)
	{
		EXPECT_EQ(i, digest_generator::blockhash_index_guessed_by_filesize(
			digest_generator::guessed_filesize_at(i)
		)) << "guessed index for guessed file size at " << i << " should be same as " << i << ".";
	}
}

TEST_F(DigestGeneratorInitialGuessTests, BlockHashIndexGuessedByFilesizeAboveBorder)
{
	for (unsigned i = 0; i < digest_blocksize::number_of_blockhashes - 1; i++)
	{
		EXPECT_EQ(i + 1, digest_generator::blockhash_index_guessed_by_filesize(
			digest_generator::guessed_filesize_at(i) + 1
		)) << "guessed file size must be maximum file size for block size index " << i << ".";
	}
}

TEST_F(DigestGeneratorInitialGuessTests, BlockHashIndexGuessedByFilesizeBelowBorderWithStart)
{
	for (unsigned j = 0; j < digest_blocksize::number_of_blockhashes; j++)
	{
		for (unsigned i = 0; i < digest_blocksize::number_of_blockhashes; i++)
		{
			EXPECT_EQ(std::max(i, j), digest_generator::blockhash_index_guessed_by_filesize(
				digest_generator::guessed_filesize_at(i), j))
				<< "guessed index for guessed file size at " << i
				<< " and start index of" << j << " should be same as "
				<< std::max(i, j) << ".";
		}
	}
}

TEST_F(DigestGeneratorInitialGuessTests, BlockHashIndexGuessedByFilesizeAboveBorderWithStart)
{
	for (unsigned j = 0; j < digest_blocksize::number_of_blockhashes; j++)
	{
		for (unsigned i = 0; i < digest_blocksize::number_of_blockhashes - 1; i++)
		{
			EXPECT_EQ(std::max(i + 1, j), digest_generator::blockhash_index_guessed_by_filesize(
				digest_generator::guessed_filesize_at(i) + 1, j))
				<< "guessed file size must be maximum size for block size index " << i
				<< " and still guessed block size index must not be less than " << j << ".";
		}
	}
}

TEST_F(DigestGeneratorInitialGuessTests, InitialBlocksizeGuessFalseTest)
{
	digest_generator gen;
	for (unsigned index = 2; index < 18; index++)
	{
		digest_filesize_t sz;
		// maximum size for specific block size estimate
		sz = digest_generator::guessed_filesize_at(index);
		gen.reset();
		gen.set_file_size_constant(sz);
		fill_seq(gen, 31, sz);
		ASSERT_EQ(sz, gen.total_size());
		EXPECT_EQ(
			string("3:0000000000000000000000000000000:0000000000000000000000000000000"),
			gen.digest_str()
		);
		// ... plus one (next block size? no, for this test)
		sz++;
		gen.reset();
		gen.set_file_size_constant(sz);
		fill_seq(gen, 31, sz);
		ASSERT_EQ(sz, gen.total_size());
		EXPECT_EQ(
			string("3:0000000000000000000000000000000:0000000000000000000000000000000"),
			gen.digest_str()
		);
	}
}

TEST_F(DigestGeneratorInitialGuessTests, InitialBlocksizeGuessTrueTest)
{
	digest_generator gen;
	ASSERT_EQ(12, digest_blocksize::at(2));
	ASSERT_EQ(768, digest_generator::guessed_filesize_at(2));
	for (unsigned index = 2; index < 18; index++)
	{
		digest_filesize_t sz;
		// maximum size for specific block size estimate
		sz = digest_generator::guessed_filesize_at(index);
		gen.reset();
		gen.set_file_size_constant(sz);
		fill_seq(gen, 32, sz);
		ASSERT_EQ(sz, gen.total_size());
		EXPECT_EQ(
			to_string(static_cast<unsigned long>(digest_blocksize::at(index))) +
				":00000000000000000000000000000000:00000000000000000000000000000000",
			gen.digest_str()
		);
		gen.reset();
		gen.set_file_size_constant(sz);
		fill_seq(gen, 64, sz);
		ASSERT_EQ(sz, gen.total_size());
		EXPECT_EQ(
			to_string(static_cast<unsigned long>(digest_blocksize::at(index))) +
				":0000000000000000000000000000000000000000000000000000000000000000"
				":0000000000000000000000000000000U",
			gen.digest_str()
		);
		// ... plus one (next block size estimate!)
		sz++;
		gen.reset();
		gen.set_file_size_constant(sz);
		fill_seq(gen, 32, sz);
		ASSERT_EQ(sz, gen.total_size());
		EXPECT_EQ(
			to_string(static_cast<unsigned long>(digest_blocksize::at(index + 1))) +
				":00000000000000000000000000000000:00000000000000000000000000000000",
			gen.digest_str()
		);
		gen.reset();
		gen.set_file_size_constant(sz);
		fill_seq(gen, 64, sz);
		ASSERT_EQ(sz, gen.total_size());
		EXPECT_EQ(
			to_string(static_cast<unsigned long>(digest_blocksize::at(index + 1))) +
				":0000000000000000000000000000000000000000000000000000000000000000"
				":0000000000000000000000000000000U",
			gen.digest_str()
		);
	}
}

#endif
