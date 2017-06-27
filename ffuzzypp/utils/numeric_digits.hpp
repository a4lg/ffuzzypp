/*

	ffuzzy++ Helper Libraries

	numeric_digits.hpp
	Construction of numeric digits

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
#ifndef FFUZZYPP_UTILS_NUMERIC_DIGITS_HPP
#define FFUZZYPP_UTILS_NUMERIC_DIGITS_HPP

#include <cstddef>
#include <cstdint>

#include <limits>

#include "safe_int.hpp"
#include "static_assert_query.hpp"

namespace ffuzzy {
namespace numeric_digits {

template <typename T, size_t Base>
static constexpr size_t in_base(T value) noexcept
{
	static_assert(
		safe_int::contracts::is_unsigned_integral_type<T>(),
		"T must be an unsigned integral type.");
	static_assert(Base >= 2, "Base must be at least two.");
	return (value < Base) ? 1 : 1 + in_base<T, Base>(value / Base);
}

template <typename T>
static constexpr size_t in_decimal(T value) noexcept
{
	return in_base<T, 10>(value);
}

// Sanity checks
#ifdef FFUZZYPP_STATIC_SANITY_CHECKS
namespace internal
{
	static constexpr uintmax_t expull(uintmax_t base, uintmax_t exponent) noexcept
	{
		return
			exponent == 0 ? 1 :
			base * expull(base, exponent-1);
	}
	static_assert(expull(10, 0) == 1ull, "sanity check for expull(10, 0) failed.");
	static_assert(expull(10, 1) == 10ull, "sanity check for expull(10, 1) failed.");
	static_assert(expull(10, 2) == 100ull, "sanity check for expull(10, 2) failed.");
	static_assert(expull(10, 3) == 1000ull, "sanity check for expull(10, 3) failed.");
	static_assert(expull(10, 4) == 10000ull, "sanity check for expull(10, 4) failed.");
	static_assert(expull(10, 19) == 10000000000000000000ull, "sanity check for expull(10, 19) failed.");

	template <typename T>
	struct digits_in_decimal_check
	{
	private:
		template <uintmax_t i>
		struct case_digit1
		{
			static constexpr const bool value = in_decimal<T>(i) == 1;
		};
		template <uintmax_t i>
		struct case_digit2
		{
			static constexpr const bool value = in_decimal<T>(i + 10u) == 2;
		};
		template <uintmax_t i>
		struct case_many_borders
		{
			static constexpr const bool value0 =
				std::numeric_limits<T>::max() < (expull(10, i+1) - 1)
					|| in_decimal<T>(T(expull(10, i+1) - 1)) == i + 1;
			static constexpr const bool value1 =
				std::numeric_limits<T>::max() < expull(10, i+1)
					|| in_decimal<T>(T(expull(10, i+1))) == i + 2;
			static constexpr const bool value = value0 && value1;
		};
	public:
		static constexpr const bool value =
			static_assert_query::is_all<case_digit1, 10>::value &&
			static_assert_query::is_all<case_digit2, 90>::value &&
			static_assert_query::is_all<case_many_borders, 19>::value;
	};
	#ifdef FFUZZYPP_LOCAL_CHK
	#error do not define FFUZZYPP_LOCAL_CHK
	#endif
	#define FFUZZYPP_LOCAL_CHK(typ) \
		static_assert(digits_in_decimal_check<typ>::value, \
			"sanity checks for digits_in_decimal failed for " #typ ".")
	FFUZZYPP_LOCAL_CHK(unsigned char);
	FFUZZYPP_LOCAL_CHK(unsigned short);
	FFUZZYPP_LOCAL_CHK(unsigned int);
	FFUZZYPP_LOCAL_CHK(unsigned long);
	FFUZZYPP_LOCAL_CHK(unsigned long long);
	FFUZZYPP_LOCAL_CHK(uint_least8_t);
	FFUZZYPP_LOCAL_CHK(uint_least16_t);
	FFUZZYPP_LOCAL_CHK(uint_least32_t);
	FFUZZYPP_LOCAL_CHK(uint_least64_t);
	FFUZZYPP_LOCAL_CHK(uint_fast8_t);
	FFUZZYPP_LOCAL_CHK(uint_fast16_t);
	FFUZZYPP_LOCAL_CHK(uint_fast32_t);
	FFUZZYPP_LOCAL_CHK(uint_fast64_t);
	FFUZZYPP_LOCAL_CHK(uintmax_t);
	FFUZZYPP_LOCAL_CHK(size_t);
	#undef  FFUZZYPP_LOCAL_CHK
}
#endif

}}

#endif
