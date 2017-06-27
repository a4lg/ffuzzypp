/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/small/nosequences.hpp
	Tests for nosequences

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
#ifndef FFUZZYPP_TESTCASES_SMALL_NOSEQUENCES_HPP
#define FFUZZYPP_TESTCASES_SMALL_NOSEQUENCES_HPP

#include <cstring>

#include "sequences.hpp"


class NoSequenceTests : public SequenceTests
{
protected:
	typedef strings::nosequences<> test_type;
};

TEST_F(NoSequenceTests, HasSequencesSpec)
{
	for (auto& entry : KnownEntries)
	{
		EXPECT_FALSE(test_type::has_sequences(entry.original, strlen(entry.original)))
			<< "has_sequences test failed on <" << entry.description << ">.";
	}
}

TEST_F(NoSequenceTests, CopyElimSequencesSpec)
{
	char buf[32]; // enough size for all cases above
	for (auto& entry : KnownEntries)
	{
		size_t elimsz = test_type::copy_elim_sequences(buf, entry.original, strlen(entry.original));
		buf[elimsz] = '\0';
		EXPECT_STREQ(entry.original, buf)
			<< "copy_elim_sequences test failed on <" << entry.description << ">.";
	}
}

TEST_F(NoSequenceTests, StringCopyCopyElimSequencesSpec)
{
	char buf[16+1]; // limited size buffer
	char *p;
	const char *q;
	for (auto& entry : KnownEntries)
	{
		p = buf;
		q = entry.original;
		bool fits_in_16chars =
			strlen(entry.original) <= 16;
		memset(buf, 1, 16+1);
		ASSERT_EQ(fits_in_16chars,
			test_type::string_copy<>::copy_elim_sequences(p, 16, q))
			<< "string_copy<>::copy_elim_sequences test failed on <" << entry.description << ">.";
		if (fits_in_16chars)
		{
			ASSERT_EQ(*q, '\0')
				<< "string_copy<>::copy_elim_sequences test failed on "
				<< "<" << entry.description << ">.";
			ASSERT_EQ(strlen(entry.original), p - buf)
				<< "string_copy<>::copy_elim_sequences test failed on "
				<< "<" << entry.description << ">.";
			*p = '\0';
			ASSERT_STREQ(entry.original, buf)
				<< "string_copy<>::copy_elim_sequences test failed on "
				<< "<" << entry.description << ">.";
		}
	}
}

TEST_F(NoSequenceTests, CopyRawSpec)
{
	char buf1[6] = "XYZDE";
	char buf2[4] = "ABC";
	test_type::copy_raw(buf1, buf2, 3);
	EXPECT_STREQ("ABCDE", buf1);
}

#endif
