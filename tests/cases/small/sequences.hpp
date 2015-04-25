/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/small/sequences.hpp
	Tests for sequences

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
#ifndef FFUZZYPP_TESTCASES_SMALL_SEQUENCES_HPP
#define FFUZZYPP_TESTCASES_SMALL_SEQUENCES_HPP

#include <cstring>


class SequenceTests : public ::testing::Test
{
public:
	static const size_t MaxSequenceSizeToTest = 3;
	struct known_entry
	{
		const char* description;
		const char* original;
		bool has_sequences;
		const char* eliminated;
		bool fits_in_16chars; // if sequences are eliminated
	};
	static const known_entry KnownEntries[];

	typedef strings::sequences<MaxSequenceSizeToTest> test_type;
};
const SequenceTests::known_entry SequenceTests::KnownEntries[] =
{
	{"empty", "", false, "", true},
	{"sequence len 1", "A", false, "A", true},
	{"sequence len 2", "AA", false, "AA", true},
	{"sequence len 3", "AAA", false, "AAA", true},
	{"sequence len 4", "AAAA", true, "AAA", true},
	{"sequence len 5", "AAAAA", true, "AAA", true},
	{"sequence len 6", "AAAAAA", true, "AAA", true},
	{"sequence len 7", "AAAAAAA", true, "AAA", true},
	{"sequence len 8", "AAAAAAAA", true, "AAA", true},
	{"sequence len 9", "AAAAAAAAA", true, "AAA", true},
	{"sequence len 10", "AAAAAAAAAA", true, "AAA", true},
	{"sequence len 11", "AAAAAAAAAAA", true, "AAA", true},
	{"sequence len 12", "AAAAAAAAAAAA", true, "AAA", true},
	{"sequence len 13", "AAAAAAAAAAAAA", true, "AAA", true},
	{"sequence len 14", "AAAAAAAAAAAAAA", true, "AAA", true},
	{"sequence len 15", "AAAAAAAAAAAAAAA", true, "AAA", true},
	{"sequence len 16", "AAAAAAAAAAAAAAAA", true, "AAA", true},
	{"sequence len 17", "AAAAAAAAAAAAAAAAA", true, "AAA", true},
	{"sequence len 18", "AAAAAAAAAAAAAAAAAA", true, "AAA", true},
	{"sequence len 19", "AAAAAAAAAAAAAAAAAAA", true, "AAA", true},
	{"sequence len 20", "AAAAAAAAAAAAAAAAAAAA", true, "AAA", true},
	{"multisequence test 1", "AAAAABBBBB", true, "AAABBB", true},
	{"multisequence test 2", "AAAAA.BBBBB", true, "AAA.BBB", true},
	{"multisequence test 3", "AABBBCCCCDDDDD", true, "AABBBCCCDDD", true},
	{"multisequence test 4", "DDDDDAABBBCCCC", true, "DDDAABBBCCC", true},
	{"noseq len 2", "AB", false, "AB", true},
	{"noseq len 3", "ABC", false, "ABC", true},
	{"noseq len 4", "ABCD", false, "ABCD", true},
	{"noseq len 5", "ABCDE", false, "ABCDE", true},
	{"noseq len 6", "ABCDEF", false, "ABCDEF", true},
	{"noseq len 7", "ABCDEFG", false, "ABCDEFG", true},
	{"noseq len 8", "ABCDEFGH", false, "ABCDEFGH", true},
	{"noseq len 9", "ABCDEFGHa", false, "ABCDEFGHa", true},
	{"noseq len 10", "ABCDEFGHab", false, "ABCDEFGHab", true},
	{"noseq len 11", "ABCDEFGHabc", false, "ABCDEFGHabc", true},
	{"noseq len 12", "ABCDEFGHabcd", false, "ABCDEFGHabcd", true},
	{"noseq len 13", "ABCDEFGHabcde", false, "ABCDEFGHabcde", true},
	{"noseq len 14", "ABCDEFGHabcdef", false, "ABCDEFGHabcdef", true},
	{"noseq len 15", "ABCDEFGHabcdefg", false, "ABCDEFGHabcdefg", true},
	{"noseq len 16", "ABCDEFGHabcdefgh", false, "ABCDEFGHabcdefgh", true},
	{"noseq len 17", "ABCDEFGHabcdefgh0", false, "ABCDEFGHabcdefgh0", false},
	{"noseq len 18", "ABCDEFGHabcdefgh01", false, "ABCDEFGHabcdefgh01", false},
	{"noseq len 19", "ABCDEFGHabcdefgh012", false, "ABCDEFGHabcdefgh012", false},
	{"noseq len 20", "ABCDEFGHabcdefgh0123", false, "ABCDEFGHabcdefgh0123", false},
	{"seq+seq  1", "AAAABBBB,.,.,.,.,.", true, "AAABBB,.,.,.,.,.", true},
	{"seq+seq  2", "AAAA,BBBB.,.,.,.,.", true, "AAA,BBB.,.,.,.,.", true},
	{"seq+seq  3", "AAAA,.BBBB,.,.,.,.", true, "AAA,.BBB,.,.,.,.", true},
	{"seq+seq  4", ",.,.,.,.,.AAAABBBB", true, ",.,.,.,.,.AAABBB", true},
	{"seq+seq  5", ",.,.,.,.,AAAA.BBBB", true, ",.,.,.,.,AAA.BBB", true},
	{"seq+seq  6", ",.,.,.,.AAAA,.BBBB", true, ",.,.,.,.AAA,.BBB", true},
	{"seq+seq  7", ",.,.,.AAAABBBB,.,.", true, ",.,.,.AAABBB,.,.", true},
	{"seq+seq  8", ",.,.,AAAA.BBBB,.,.", true, ",.,.,AAA.BBB,.,.", true},
	{"seq+seq  9", ",.,.AAAA,.BBBB,.,.", true, ",.,.AAA,.BBB,.,.", true},
	{"seq+seq 10", ".AAAABBBB,.,.,.,.,", true, ".AAABBB,.,.,.,.,", true},
	{"seq+seq 11", ".AAAA,BBBB.,.,.,.,", true, ".AAA,BBB.,.,.,.,", true},
	{"seq+seq 12", ".AAAA,.BBBB,.,.,.,", true, ".AAA,.BBB,.,.,.,", true},
	{"seq+seq 13", ".,.,.,.,.AAAABBBB,", true, ".,.,.,.,.AAABBB,", true},
	{"seq+seq 14", ".,.,.,.,AAAA.BBBB,", true, ".,.,.,.,AAA.BBB,", true},
	{"seq+seq 15", ".,.,.,.AAAA,.BBBB,", true, ".,.,.,.AAA,.BBB,", true},
	{"seq+noseq  1", "AAAABBB,.,.,.,.,.", true, "AAABBB,.,.,.,.,.", true},
	{"seq+noseq  2", "AAAA,BBB.,.,.,.,.", true, "AAA,BBB.,.,.,.,.", true},
	{"seq+noseq  3", "AAAA,.BBB,.,.,.,.", true, "AAA,.BBB,.,.,.,.", true},
	{"seq+noseq  4", ",.,.,.,.,.AAAABBB", true, ",.,.,.,.,.AAABBB", true},
	{"seq+noseq  5", ",.,.,.,.,AAAA.BBB", true, ",.,.,.,.,AAA.BBB", true},
	{"seq+noseq  6", ",.,.,.,.AAAA,.BBB", true, ",.,.,.,.AAA,.BBB", true},
	{"seq+noseq  7", ",.,.,.AAAABBB,.,.", true, ",.,.,.AAABBB,.,.", true},
	{"seq+noseq  8", ",.,.,AAAA.BBB,.,.", true, ",.,.,AAA.BBB,.,.", true},
	{"seq+noseq  9", ",.,.AAAA,.BBB,.,.", true, ",.,.AAA,.BBB,.,.", true},
	{"seq+noseq 10", ".AAAABBB,.,.,.,.,", true, ".AAABBB,.,.,.,.,", true},
	{"seq+noseq 11", ".AAAA,BBB.,.,.,.,", true, ".AAA,BBB.,.,.,.,", true},
	{"seq+noseq 12", ".AAAA,.BBB,.,.,.,", true, ".AAA,.BBB,.,.,.,", true},
	{"seq+noseq 13", ".,.,.,.,.AAAABBB,", true, ".,.,.,.,.AAABBB,", true},
	{"seq+noseq 14", ".,.,.,.,AAAA.BBB,", true, ".,.,.,.,AAA.BBB,", true},
	{"seq+noseq 15", ".,.,.,.AAAA,.BBB,", true, ".,.,.,.AAA,.BBB,", true},
	{"noseq+seq  1", "AAABBBB,.,.,.,.,.", true, "AAABBB,.,.,.,.,.", true},
	{"noseq+seq  2", "AAA,BBBB.,.,.,.,.", true, "AAA,BBB.,.,.,.,.", true},
	{"noseq+seq  3", "AAA,.BBBB,.,.,.,.", true, "AAA,.BBB,.,.,.,.", true},
	{"noseq+seq  4", ",.,.,.,.,.AAABBBB", true, ",.,.,.,.,.AAABBB", true},
	{"noseq+seq  5", ",.,.,.,.,AAA.BBBB", true, ",.,.,.,.,AAA.BBB", true},
	{"noseq+seq  6", ",.,.,.,.AAA,.BBBB", true, ",.,.,.,.AAA,.BBB", true},
	{"noseq+seq  7", ",.,.,.AAABBBB,.,.", true, ",.,.,.AAABBB,.,.", true},
	{"noseq+seq  8", ",.,.,AAA.BBBB,.,.", true, ",.,.,AAA.BBB,.,.", true},
	{"noseq+seq  9", ",.,.AAA,.BBBB,.,.", true, ",.,.AAA,.BBB,.,.", true},
	{"noseq+seq 10", ".AAABBBB,.,.,.,.,", true, ".AAABBB,.,.,.,.,", true},
	{"noseq+seq 11", ".AAA,BBBB.,.,.,.,", true, ".AAA,BBB.,.,.,.,", true},
	{"noseq+seq 12", ".AAA,.BBBB,.,.,.,", true, ".AAA,.BBB,.,.,.,", true},
	{"noseq+seq 13", ".,.,.,.,.AAABBBB,", true, ".,.,.,.,.AAABBB,", true},
	{"noseq+seq 14", ".,.,.,.,AAA.BBBB,", true, ".,.,.,.,AAA.BBB,", true},
	{"noseq+seq 15", ".,.,.,.AAA,.BBBB,", true, ".,.,.,.AAA,.BBB,", true},
	{"seq4+seq5  1", "AAAABBBBB,.,.,.,.,.", true, "AAABBB,.,.,.,.,.", true},
	{"seq4+seq5  2", "AAAA,BBBBB.,.,.,.,.", true, "AAA,BBB.,.,.,.,.", true},
	{"seq4+seq5  3", "AAAA,.BBBBB,.,.,.,.", true, "AAA,.BBB,.,.,.,.", true},
	{"seq4+seq5  4", ",.,.,.,.,.AAAABBBBB", true, ",.,.,.,.,.AAABBB", true},
	{"seq4+seq5  5", ",.,.,.,.,AAAA.BBBBB", true, ",.,.,.,.,AAA.BBB", true},
	{"seq4+seq5  6", ",.,.,.,.AAAA,.BBBBB", true, ",.,.,.,.AAA,.BBB", true},
	{"seq4+seq5  7", ",.,.,.AAAABBBBB,.,.", true, ",.,.,.AAABBB,.,.", true},
	{"seq4+seq5  8", ",.,.,AAAA.BBBBB,.,.", true, ",.,.,AAA.BBB,.,.", true},
	{"seq4+seq5  9", ",.,.AAAA,.BBBBB,.,.", true, ",.,.AAA,.BBB,.,.", true},
	{"seq4+seq5 10", ".AAAABBBBB,.,.,.,.,", true, ".AAABBB,.,.,.,.,", true},
	{"seq4+seq5 11", ".AAAA,BBBBB.,.,.,.,", true, ".AAA,BBB.,.,.,.,", true},
	{"seq4+seq5 12", ".AAAA,.BBBBB,.,.,.,", true, ".AAA,.BBB,.,.,.,", true},
	{"seq4+seq5 13", ".,.,.,.,.AAAABBBBB,", true, ".,.,.,.,.AAABBB,", true},
	{"seq4+seq5 14", ".,.,.,.,AAAA.BBBBB,", true, ".,.,.,.,AAA.BBB,", true},
	{"seq4+seq5 15", ".,.,.,.AAAA,.BBBBB,", true, ".,.,.,.AAA,.BBB,", true},
	{"seq5+seq4  1", "AAAAABBBB,.,.,.,.,.", true, "AAABBB,.,.,.,.,.", true},
	{"seq5+seq4  2", "AAAAA,BBBB.,.,.,.,.", true, "AAA,BBB.,.,.,.,.", true},
	{"seq5+seq4  3", "AAAAA,.BBBB,.,.,.,.", true, "AAA,.BBB,.,.,.,.", true},
	{"seq5+seq4  4", ",.,.,.,.,.AAAAABBBB", true, ",.,.,.,.,.AAABBB", true},
	{"seq5+seq4  5", ",.,.,.,.,AAAAA.BBBB", true, ",.,.,.,.,AAA.BBB", true},
	{"seq5+seq4  6", ",.,.,.,.AAAAA,.BBBB", true, ",.,.,.,.AAA,.BBB", true},
	{"seq5+seq4  7", ",.,.,.AAAAABBBB,.,.", true, ",.,.,.AAABBB,.,.", true},
	{"seq5+seq4  8", ",.,.,AAAAA.BBBB,.,.", true, ",.,.,AAA.BBB,.,.", true},
	{"seq5+seq4  9", ",.,.AAAAA,.BBBB,.,.", true, ",.,.AAA,.BBB,.,.", true},
	{"seq5+seq4 10", ".AAAAABBBB,.,.,.,.,", true, ".AAABBB,.,.,.,.,", true},
	{"seq5+seq4 11", ".AAAAA,BBBB.,.,.,.,", true, ".AAA,BBB.,.,.,.,", true},
	{"seq5+seq4 12", ".AAAAA,.BBBB,.,.,.,", true, ".AAA,.BBB,.,.,.,", true},
	{"seq5+seq4 13", ".,.,.,.,.AAAAABBBB,", true, ".,.,.,.,.AAABBB,", true},
	{"seq5+seq4 14", ".,.,.,.,AAAAA.BBBB,", true, ".,.,.,.,AAA.BBB,", true},
	{"seq5+seq4 15", ".,.,.,.AAAAA,.BBBB,", true, ".,.,.,.AAA,.BBB,", true},
	{"long seq4 1", "AAAA.,.,.,.,.,.,.,", true, "AAA.,.,.,.,.,.,.,", false},
	{"long seq4 2", ",.,.,.,.,.,.,.AAAA", true, ",.,.,.,.,.,.,.AAA", false},
	{"long seq4 3", ",AAAA,.,.,.,.,.,.,", true, ",AAA,.,.,.,.,.,.,", false},
	{"long seq4 4", ",.,.,.,.,.,.,AAAA.", true, ",.,.,.,.,.,.,AAA.", false},
	{"long seq5 1", "AAAAA.,.,.,.,.,.,.,", true, "AAA.,.,.,.,.,.,.,", false},
	{"long seq5 2", ",.,.,.,.,.,.,.AAAAA", true, ",.,.,.,.,.,.,.AAA", false},
	{"long seq5 3", ",AAAAA,.,.,.,.,.,.,", true, ",AAA,.,.,.,.,.,.,", false},
	{"long seq5 4", ",.,.,.,.,.,.,AAAAA.", true, ",.,.,.,.,.,.,AAA.", false},
	{"long seq+seq  1", "AAAABBBB,.,.,.,.,.,", true, "AAABBB,.,.,.,.,.,", false},
	{"long seq+seq  2", "AAAA,BBBB.,.,.,.,.,", true, "AAA,BBB.,.,.,.,.,", false},
	{"long seq+seq  3", "AAAA,.BBBB,.,.,.,.,", true, "AAA,.BBB,.,.,.,.,", false},
	{"long seq+seq  4", ",.,.,.,.,.,AAAABBBB", true, ",.,.,.,.,.,AAABBB", false},
	{"long seq+seq  5", ",.,.,.,.,.AAAA,BBBB", true, ",.,.,.,.,.AAA,BBB", false},
	{"long seq+seq  6", ",.,.,.,.,AAAA.,BBBB", true, ",.,.,.,.,AAA.,BBB", false},
	{"long seq+seq  7", ",.,.,.AAAABBBB,.,.,", true, ",.,.,.AAABBB,.,.,", false},
	{"long seq+seq  8", ",.,.,AAAA.BBBB,.,.,", true, ",.,.,AAA.BBB,.,.,", false},
	{"long seq+seq  9", ",.,.AAAA,.BBBB,.,.,", true, ",.,.AAA,.BBB,.,.,", false},
	{"long seq+seq 10", ".AAAABBBB,.,.,.,.,.", true, ".AAABBB,.,.,.,.,.", false},
	{"long seq+seq 11", ".AAAA,BBBB.,.,.,.,.", true, ".AAA,BBB.,.,.,.,.", false},
	{"long seq+seq 12", ".AAAA,.BBBB,.,.,.,.", true, ".AAA,.BBB,.,.,.,.", false},
	{"long seq+seq 13", ",.,.,.,.,.AAAABBBB,", true, ",.,.,.,.,.AAABBB,", false},
	{"long seq+seq 14", ",.,.,.,.,AAAA.BBBB,", true, ",.,.,.,.,AAA.BBB,", false},
	{"long seq+seq 15", ",.,.,.,.AAAA,.BBBB,", true, ",.,.,.,.AAA,.BBB,", false},
	{"long seq+noseq  1", "AAAABBB,.,.,.,.,.,", true, "AAABBB,.,.,.,.,.,", false},
	{"long seq+noseq  2", "AAAA,BBB.,.,.,.,.,", true, "AAA,BBB.,.,.,.,.,", false},
	{"long seq+noseq  3", "AAAA,.BBB,.,.,.,.,", true, "AAA,.BBB,.,.,.,.,", false},
	{"long seq+noseq  4", ",.,.,.,.,.,AAAABBB", true, ",.,.,.,.,.,AAABBB", false},
	{"long seq+noseq  5", ",.,.,.,.,.AAAA,BBB", true, ",.,.,.,.,.AAA,BBB", false},
	{"long seq+noseq  6", ",.,.,.,.,AAAA.,BBB", true, ",.,.,.,.,AAA.,BBB", false},
	{"long seq+noseq  7", ",.,.,.AAAABBB,.,.,", true, ",.,.,.AAABBB,.,.,", false},
	{"long seq+noseq  8", ",.,.,AAAA.BBB,.,.,", true, ",.,.,AAA.BBB,.,.,", false},
	{"long seq+noseq  9", ",.,.AAAA,.BBB,.,.,", true, ",.,.AAA,.BBB,.,.,", false},
	{"long seq+noseq 10", ".AAAABBB,.,.,.,.,.", true, ".AAABBB,.,.,.,.,.", false},
	{"long seq+noseq 11", ".AAAA,BBB.,.,.,.,.", true, ".AAA,BBB.,.,.,.,.", false},
	{"long seq+noseq 12", ".AAAA,.BBB,.,.,.,.", true, ".AAA,.BBB,.,.,.,.", false},
	{"long seq+noseq 13", ",.,.,.,.,.AAAABBB,", true, ",.,.,.,.,.AAABBB,", false},
	{"long seq+noseq 14", ",.,.,.,.,AAAA.BBB,", true, ",.,.,.,.,AAA.BBB,", false},
	{"long seq+noseq 15", ",.,.,.,.AAAA,.BBB,", true, ",.,.,.,.AAA,.BBB,", false},
	{"long noseq+seq  1", "AAABBBB,.,.,.,.,.,", true, "AAABBB,.,.,.,.,.,", false},
	{"long noseq+seq  2", "AAA,BBBB.,.,.,.,.,", true, "AAA,BBB.,.,.,.,.,", false},
	{"long noseq+seq  3", "AAA,.BBBB,.,.,.,.,", true, "AAA,.BBB,.,.,.,.,", false},
	{"long noseq+seq  4", ",.,.,.,.,.,AAABBBB", true, ",.,.,.,.,.,AAABBB", false},
	{"long noseq+seq  5", ",.,.,.,.,.AAA,BBBB", true, ",.,.,.,.,.AAA,BBB", false},
	{"long noseq+seq  6", ",.,.,.,.,AAA.,BBBB", true, ",.,.,.,.,AAA.,BBB", false},
	{"long noseq+seq  7", ",.,.,.AAABBBB,.,.,", true, ",.,.,.AAABBB,.,.,", false},
	{"long noseq+seq  8", ",.,.,AAA.BBBB,.,.,", true, ",.,.,AAA.BBB,.,.,", false},
	{"long noseq+seq  9", ",.,.AAA,.BBBB,.,.,", true, ",.,.AAA,.BBB,.,.,", false},
	{"long noseq+seq 10", ".AAABBBB,.,.,.,.,.", true, ".AAABBB,.,.,.,.,.", false},
	{"long noseq+seq 11", ".AAA,BBBB.,.,.,.,.", true, ".AAA,BBB.,.,.,.,.", false},
	{"long noseq+seq 12", ".AAA,.BBBB,.,.,.,.", true, ".AAA,.BBB,.,.,.,.", false},
	{"long noseq+seq 13", ",.,.,.,.,.AAABBBB,", true, ",.,.,.,.,.AAABBB,", false},
	{"long noseq+seq 14", ",.,.,.,.,AAA.BBBB,", true, ",.,.,.,.,AAA.BBB,", false},
	{"long noseq+seq 15", ",.,.,.,.AAA,.BBBB,", true, ",.,.,.,.AAA,.BBB,", false},
	{"long seq4+seq5  1", "AAAABBBBB,.,.,.,.,.,", true, "AAABBB,.,.,.,.,.,", false},
	{"long seq4+seq5  2", "AAAA,BBBBB.,.,.,.,.,", true, "AAA,BBB.,.,.,.,.,", false},
	{"long seq4+seq5  3", "AAAA,.BBBBB,.,.,.,.,", true, "AAA,.BBB,.,.,.,.,", false},
	{"long seq4+seq5  4", ",.,.,.,.,.,AAAABBBBB", true, ",.,.,.,.,.,AAABBB", false},
	{"long seq4+seq5  5", ",.,.,.,.,.AAAA,BBBBB", true, ",.,.,.,.,.AAA,BBB", false},
	{"long seq4+seq5  6", ",.,.,.,.,AAAA.,BBBBB", true, ",.,.,.,.,AAA.,BBB", false},
	{"long seq4+seq5  7", ",.,.,.AAAABBBBB,.,.,", true, ",.,.,.AAABBB,.,.,", false},
	{"long seq4+seq5  8", ",.,.,AAAA.BBBBB,.,.,", true, ",.,.,AAA.BBB,.,.,", false},
	{"long seq4+seq5  9", ",.,.AAAA,.BBBBB,.,.,", true, ",.,.AAA,.BBB,.,.,", false},
	{"long seq4+seq5 10", ".AAAABBBBB,.,.,.,.,.", true, ".AAABBB,.,.,.,.,.", false},
	{"long seq4+seq5 11", ".AAAA,BBBBB.,.,.,.,.", true, ".AAA,BBB.,.,.,.,.", false},
	{"long seq4+seq5 12", ".AAAA,.BBBBB,.,.,.,.", true, ".AAA,.BBB,.,.,.,.", false},
	{"long seq4+seq5 13", ",.,.,.,.,.AAAABBBBB,", true, ",.,.,.,.,.AAABBB,", false},
	{"long seq4+seq5 14", ",.,.,.,.,AAAA.BBBBB,", true, ",.,.,.,.,AAA.BBB,", false},
	{"long seq4+seq5 15", ",.,.,.,.AAAA,.BBBBB,", true, ",.,.,.,.AAA,.BBB,", false},
	{"long seq5+seq4  1", "AAAAABBBB,.,.,.,.,.,", true, "AAABBB,.,.,.,.,.,", false},
	{"long seq5+seq4  2", "AAAAA,BBBB.,.,.,.,.,", true, "AAA,BBB.,.,.,.,.,", false},
	{"long seq5+seq4  3", "AAAAA,.BBBB,.,.,.,.,", true, "AAA,.BBB,.,.,.,.,", false},
	{"long seq5+seq4  4", ",.,.,.,.,.,AAAAABBBB", true, ",.,.,.,.,.,AAABBB", false},
	{"long seq5+seq4  5", ",.,.,.,.,.AAAAA,BBBB", true, ",.,.,.,.,.AAA,BBB", false},
	{"long seq5+seq4  6", ",.,.,.,.,AAAAA.,BBBB", true, ",.,.,.,.,AAA.,BBB", false},
	{"long seq5+seq4  7", ",.,.,.AAAAABBBB,.,.,", true, ",.,.,.AAABBB,.,.,", false},
	{"long seq5+seq4  8", ",.,.,AAAAA.BBBB,.,.,", true, ",.,.,AAA.BBB,.,.,", false},
	{"long seq5+seq4  9", ",.,.AAAAA,.BBBB,.,.,", true, ",.,.AAA,.BBB,.,.,", false},
	{"long seq5+seq4 10", ".AAAAABBBB,.,.,.,.,.", true, ".AAABBB,.,.,.,.,.", false},
	{"long seq5+seq4 11", ".AAAAA,BBBB.,.,.,.,.", true, ".AAA,BBB.,.,.,.,.", false},
	{"long seq5+seq4 12", ".AAAAA,.BBBB,.,.,.,.", true, ".AAA,.BBB,.,.,.,.", false},
	{"long seq5+seq4 13", ",.,.,.,.,.AAAAABBBB,", true, ",.,.,.,.,.AAABBB,", false},
	{"long seq5+seq4 14", ",.,.,.,.,AAAAA.BBBB,", true, ",.,.,.,.,AAA.BBB,", false},
	{"long seq5+seq4 15", ",.,.,.,.AAAAA,.BBBB,", true, ",.,.,.,.AAA,.BBB,", false},
};


TEST_F(SequenceTests, HasSequencesSpec)
{
	for (auto& entry : KnownEntries)
	{
		EXPECT_EQ(entry.has_sequences, test_type::has_sequences(entry.original, strlen(entry.original)))
			<< "has_sequences test failed on <" << entry.description << ">.";
	}
}

TEST_F(SequenceTests, CopyElimSequencesSpec)
{
	char buf[32]; // enough size for all cases above
	for (auto& entry : KnownEntries)
	{
		size_t elimsz = test_type::copy_elim_sequences(buf, entry.original, strlen(entry.original));
		buf[elimsz] = '\0';
		EXPECT_STREQ(entry.eliminated, buf)
			<< "copy_elim_sequences test failed on <" << entry.description << ">.";
	}
}

TEST_F(SequenceTests, CopyElimSequencesSpecOnOverwrappingBuffers)
{
	char buf[32]; // enough size for all cases above
	for (auto& entry : KnownEntries)
	{
		strcpy(buf, entry.original);
		size_t elimsz = test_type::copy_elim_sequences(buf, buf, strlen(entry.original));
		buf[elimsz] = '\0';
		EXPECT_STREQ(entry.eliminated, buf)
			<< "copy_elim_sequences test (on overwrapping buffer) failed on <" << entry.description << ">.";
	}
}

TEST_F(SequenceTests, StringCopyCopyElimSequencesSpec)
{
	char buf[16+1]; // limited size buffer
	char *p;
	const char *q;
	for (auto& entry : KnownEntries)
	{
		p = buf;
		q = entry.original;
		memset(buf, 1, 16+1);
		ASSERT_EQ(entry.fits_in_16chars,
			test_type::string_copy<>::copy_elim_sequences(p, 16, q))
			<< "string_copy<>::copy_elim_sequences test failed on <" << entry.description << ">.";
		if (entry.fits_in_16chars)
		{
			ASSERT_EQ(*q, '\0')
				<< "string_copy<>::copy_elim_sequences test failed on "
				<< "<" << entry.description << ">.";
			ASSERT_EQ(strlen(entry.eliminated), p - buf)
				<< "string_copy<>::copy_elim_sequences test failed on "
				<< "<" << entry.description << ">.";
			*p = '\0';
			ASSERT_STREQ(entry.eliminated, buf)
				<< "string_copy<>::copy_elim_sequences test failed on "
				<< "<" << entry.description << ">.";
		}
	}
}

TEST_F(SequenceTests, StringCopyCopyElimSequencesSpecWithTerminator)
{
	static const char* const suffixes[] =
	{
		"",
		"A",
		"AA",
		"AAA",
		"AAAA",
		"AAAAA",
		"AAAAAA",
		"AAAAAAA",
		"AAAAAAAA",
		"B",
		"BB",
		"BBB",
		"BBBB",
		"BBBBB",
		"BBBBBB",
		"BBBBBBB",
		"BBBBBBBB",
	};
	char buf_in[32+1+8+1];
	char buf_out[16+1]; // limited size buffer
	char *p;
	const char *q;
	for (const auto& suffix : suffixes)
	{
		for (auto& entry : KnownEntries)
		{
			p = buf_out;
			q = buf_in;
			strcpy(buf_in, entry.original);
			strcat(buf_in, ":");
			strcat(buf_in, suffix);
			memset(buf_out, 1, 16+1);
			ASSERT_EQ(entry.fits_in_16chars,
				test_type::string_copy<':'>::copy_elim_sequences(p, 16, q))
				<< "string_copy<':'>::copy_elim_sequences test failed on <"
				<< entry.description << "> with terminator \"" << suffix << "\".";
			if (entry.fits_in_16chars)
			{
				ASSERT_EQ(*q, ':')
					<< "string_copy<':'>::copy_elim_sequences test failed on "
					<< "<" << entry.description << "> with terminator \"" << suffix << "\".";
				ASSERT_EQ(strlen(entry.eliminated), p - buf_out)
					<< "string_copy<':'>::copy_elim_sequences test failed on "
					<< "<" << entry.description << "> with terminator \"" << suffix << "\".";
				*p = '\0';
				ASSERT_STREQ(entry.eliminated, buf_out)
					<< "string_copy<':'>::copy_elim_sequences test failed on "
					<< "<" << entry.description << "> with terminator \"" << suffix << "\".";
			}
		}
	}
}

#endif
