/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/precond/position_array.hpp
	Position array type precondition tests

	Copyright (C) 2017 Tsukasa OI <floss_ssdeep@irq.a4lg.com>


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
#ifndef FFUZZYPP_TESTCASES_PRECOND_POSITION_ARRAY_HPP
#define FFUZZYPP_TESTCASES_PRECOND_POSITION_ARRAY_HPP

#include <type_traits>


// is_auto_position_array_available with portable range
#ifndef FFUZZYPP_DISABLE_POSITION_ARRAY
static_assert(!strings::is_auto_position_array_available<0, char, '0', '9'>::value,
	"No auto_position_array with MaxSize == 0 should be available.");
static_assert(strings::is_auto_position_array_available<1, char, '0', '9'>::value,
	"auto_position_array with MaxSize == 1 should be available.");
static_assert(strings::is_auto_position_array_available<15, char, '0', '9'>::value,
	"auto_position_array with MaxSize == 15 should be available.");
static_assert(strings::is_auto_position_array_available<16, char, '0', '9'>::value,
	"auto_position_array with MaxSize == 16 should be available.");
static_assert(strings::is_auto_position_array_available<17, char, '0', '9'>::value,
	"auto_position_array with MaxSize == 17 should be available.");
static_assert(strings::is_auto_position_array_available<18, char, '0', '9'>::value,
	"auto_position_array with MaxSize == 18 should be available.");
static_assert(strings::is_auto_position_array_available<31, char, '0', '9'>::value,
	"auto_position_array with MaxSize == 31 should be available.");
static_assert(strings::is_auto_position_array_available<32, char, '0', '9'>::value,
	"auto_position_array with MaxSize == 32 should be available.");
static_assert(strings::is_auto_position_array_available<33, char, '0', '9'>::value,
	"auto_position_array with MaxSize == 33 should be available.");
static_assert(strings::is_auto_position_array_available<34, char, '0', '9'>::value,
	"auto_position_array with MaxSize == 34 should be available.");
static_assert(strings::is_auto_position_array_available<63, char, '0', '9'>::value,
	"auto_position_array with MaxSize == 63 should be available.");
static_assert(strings::is_auto_position_array_available<64, char, '0', '9'>::value,
	"auto_position_array with MaxSize == 64 should be available.");
#endif
static_assert(!strings::is_auto_position_array_available<65, char, '0', '9'>::value,
	"No auto_position_array with MaxSize == 65 should be available.");

// auto_position_array<...>::type
#ifndef FFUZZYPP_DISABLE_POSITION_ARRAY
static_assert(is_same<
		typename strings::auto_position_array<1, char, '0', '9'>::type,
		strings::position_array<unsigned, char, '0', '9'>
	>::value,
	"auto position_array with MaxSize == 1 should be represented by unsigned.");
static_assert(is_same<
		typename strings::auto_position_array<15, char, '0', '9'>::type,
		strings::position_array<unsigned, char, '0', '9'>
	>::value,
	"auto position_array with MaxSize == 15 should be represented by unsigned.");
static_assert(is_same<
		typename strings::auto_position_array<16, char, '0', '9'>::type,
		strings::position_array<unsigned, char, '0', '9'>
	>::value,
	"auto position_array with MaxSize == 16 should be represented by unsigned.");
static_assert(is_same<
		typename strings::auto_position_array<17, char, '0', '9'>::type,
		strings::position_array<unsigned long, char, '0', '9'>
	>::value,
	"auto position_array with MaxSize == 17 should be represented by unsigned long.");
static_assert(is_same<
		typename strings::auto_position_array<18, char, '0', '9'>::type,
		strings::position_array<unsigned long, char, '0', '9'>
	>::value,
	"auto position_array with MaxSize == 18 should be represented by unsigned long.");
static_assert(is_same<
		typename strings::auto_position_array<31, char, '0', '9'>::type,
		strings::position_array<unsigned long, char, '0', '9'>
	>::value,
	"auto position_array with MaxSize == 31 should be represented by unsigned long.");
static_assert(is_same<
		typename strings::auto_position_array<32, char, '0', '9'>::type,
		strings::position_array<unsigned long, char, '0', '9'>
	>::value,
	"auto position_array with MaxSize == 32 should be represented by unsigned long.");
static_assert(is_same<
		typename strings::auto_position_array<33, char, '0', '9'>::type,
		strings::position_array<unsigned long long, char, '0', '9'>
	>::value,
	"auto position_array with MaxSize == 33 should be represented by unsigned long long.");
static_assert(is_same<
		typename strings::auto_position_array<34, char, '0', '9'>::type,
		strings::position_array<unsigned long long, char, '0', '9'>
	>::value,
	"auto position_array with MaxSize == 34 should be represented by unsigned long long.");
static_assert(is_same<
		typename strings::auto_position_array<63, char, '0', '9'>::type,
		strings::position_array<unsigned long long, char, '0', '9'>
	>::value,
	"auto position_array with MaxSize == 63 should be represented by unsigned long long.");
static_assert(is_same<
		typename strings::auto_position_array<64, char, '0', '9'>::type,
		strings::position_array<unsigned long long, char, '0', '9'>
	>::value,
	"auto position_array with MaxSize == 64 should be represented by unsigned long long.");
#endif

// specialized values of guaranteed_bitlen
static_assert(strings::internal::guaranteed_bitlen<unsigned>::size >= 16,
	"guaranteed_bitlen<unsigned> must be at least 16.");
static_assert(strings::internal::guaranteed_bitlen<unsigned long>::size >= 32,
	"guaranteed_bitlen<unsigned long> must be at least 32.");
static_assert(strings::internal::guaranteed_bitlen<unsigned long long>::size >= 64,
	"guaranteed_bitlen<unsigned long long> must be at least 64.");

// guaranteed lengths of position array classes
static_assert(strings::position_array<unsigned, char, '0', '9'>::max_strlen >= 16,
	"position_array with unsigned type must be able to process string with up to 16 chars.");
static_assert(strings::position_array<unsigned long, char, '0', '9'>::max_strlen >= 32,
	"position_array with unsigned long type must be able to process string with up to 32 chars.");
static_assert(strings::position_array<unsigned long long, char, '0', '9'>::max_strlen >= 64,
	"position_array with unsigned long long type must be able to process string with up to 64 chars.");

// array sizes of position array classes
static_assert(strings::position_array<unsigned, char, 0x00, 0x7f>::array_size == 0x80,
	"position_array with range from 0x00 to 0x7f should have array_size of 0x80.");
static_assert(strings::position_array<unsigned long, char, 0x00, 0x7f>::array_size == 0x80,
	"position_array with range from 0x00 to 0x7f should have array_size of 0x80.");
static_assert(strings::position_array<unsigned long long, char, 0x00, 0x7f>::array_size == 0x80,
	"position_array with range from 0x00 to 0x7f should have array_size of 0x80.");
static_assert(strings::position_array<unsigned, char, '0', '9'>::array_size == 10,
	"position_array with range from '0' to '9' should have array_size of 10.");
static_assert(strings::position_array<unsigned long, char, '0', '9'>::array_size == 10,
	"position_array with range from '0' to '9' should have array_size of 10.");
static_assert(strings::position_array<unsigned long long, char, '0', '9'>::array_size == 10,
	"position_array with range from '0' to '9' should have array_size of 10.");

#endif
