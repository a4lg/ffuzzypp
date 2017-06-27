/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/compatibility/common/blockhash_comparison_min_matching.hpp
	Tests for minimum matching scores (common parts)

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
#ifndef FFUZZYPP_TESTCASES_COMPATIBILITY_COMMON_BLOCKHASH_COMPARISON_MIN_MATCHING_HPP
#define FFUZZYPP_TESTCASES_COMPATIBILITY_COMMON_BLOCKHASH_COMPARISON_MIN_MATCHING_HPP

#include <cassert>
#include <string>


class BlockhashComparisonMinMatchingTests : public ::testing::Test
{
protected:
	const string s1template;
	const string s2template;
	const string filler;
private:
	static string construct_template(
		string::size_type len,
		const char c1,
		const char c2
	)
	{
		string str;
		for (string::size_type i = 0; i < len; i++)
			str.append(1, (i & 1) ? c2 : c1);
		return str;
	}
public:
	BlockhashComparisonMinMatchingTests(void)
		: ::testing::Test()
		, s1template(construct_template(digest_params::max_blockhash_len, '0', '1'))
		, s2template(construct_template(digest_params::max_blockhash_len, '2', '3'))
		, filler("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz")
	{
	}
private:
	static string construct_from_template(
		const string& template_str,
		const string& filler_str,
		const string::size_type total_length,
		const string::size_type filler_length,
		const string::size_type filler_index
	)
	{
		assert(template_str.size() != 0);
		assert(template_str.size() >= total_length);
		assert(filler_str.size() != 0);
		assert(filler_length + filler_index <= total_length);
		string str = template_str.substr(0, total_length);
		for (string::size_type i = 0; i < filler_length; i++)
			str[filler_index + i] = filler_str[i % filler_str.size()];
		return str;
	}
protected:
	string MinMatchingTemplate1(
		blockhash_len_t len,
		blockhash_len_t index
	)
	{
		return construct_from_template(
			s1template, filler,
			len, blockhash_comparison_params::min_match_len, index
		);
	}
	string MinMatchingTemplate2(
		blockhash_len_t len,
		blockhash_len_t index
	)
	{
		return construct_from_template(
			s2template, filler,
			len, blockhash_comparison_params::min_match_len, index
		);
	}
};

#endif
