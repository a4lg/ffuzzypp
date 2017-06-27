/*

	ffuzzy++ Helper Libraries

	safe_int.hpp
	Safe integer handling utilities

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
#ifndef FFUZZYPP_UTILS_SAFE_INT_HPP
#define FFUZZYPP_UTILS_SAFE_INT_HPP

#include <cstdint>

#include <limits>
#include <type_traits>

namespace ffuzzy {
namespace safe_int {


namespace contracts
{
	template <typename T>
	static inline constexpr bool is_integral_type(void) noexcept
	{
		return std::is_integral<T>::value;
	}
	template <typename T>
	static inline constexpr bool is_signed_integral_type(void) noexcept
	{
		return is_integral_type<T>() &&
			std::is_unsigned<T>::value == false &&
			std::numeric_limits<T>::max() > 0 &&
			std::numeric_limits<T>::min() < 0;
	}
	template <typename T>
	static inline constexpr bool is_unsigned_integral_type(void) noexcept
	{
		return is_integral_type<T>() &&
			std::is_unsigned<T>::value == true &&
			std::numeric_limits<T>::min() == 0;
	}
	template <typename T>
	static inline constexpr bool is_binary_integral_type(void) noexcept
	{
		return
			std::numeric_limits<T>::radix == 2 &&
			std::numeric_limits<T>::digits >= 0;
	}

	// Same implementation but copying for better diagnostics
	template <typename T>
	struct integral_type
	{
		static_assert(std::is_integral<T>::value,
			"T must be an integral type.");
	};
	template <typename T>
	struct signed_integral_type
		: integral_type<T>
	{
		static_assert(std::is_unsigned<T>::value == false,
			"T must be a signed integral type.");
		static_assert(std::numeric_limits<T>::max() > 0,
			"numeric_limits<T>::max() must be positive for signed integral type.");
		static_assert(std::numeric_limits<T>::min() < 0,
			"numeric_limits<T>::min() must be negative for signed integral type.");
	};
	template <typename T>
	struct unsigned_integral_type
		: integral_type<T>
	{
		static_assert(std::is_unsigned<T>::value == true,
			"T must be an unsigned integral type.");
		static_assert(std::numeric_limits<T>::min() == 0,
			"numeric_limits<T>::min() must be zero for unsigned integral type.");
	};
	template <typename T>
	struct binary_integral_type
		: integral_type<T>
	{
		static_assert(std::numeric_limits<T>::radix == 2,
			"numeric_limits<T>::radix must be 2 for binary integral type.");
		static_assert(std::numeric_limits<T>::digits >= 0,
			"numeric_limits<T>::digits must not be negative for binary integral type.");
	};
}


namespace internal
{
	template <typename T1, typename T2>
	struct common_unsigned_integral_type
		: contracts::unsigned_integral_type<T1>
		, contracts::unsigned_integral_type<T2>
	{
		typedef typename std::conditional<
			std::numeric_limits<T1>::max() < std::numeric_limits<T2>::max(),
			T2, T1
		>::type type;
	};
	template <typename T>
	struct common_unsigned_integral_type<T, T>
	{
		typedef T type;
	};

	template <typename Tcommon, typename Tvalue, Tcommon Value, bool IsValid = true>
	struct value_base
	{
		typedef Tcommon common_type;
		typedef Tvalue  value_type;
		static constexpr const bool is_valid = IsValid
			&& std::numeric_limits<Tvalue>::min() <= Value
			&& std::numeric_limits<Tvalue>::max() >= Value;
		static constexpr const Tcommon value_in_common = Value;
		static constexpr const Tvalue  value =
			is_valid ? static_cast<Tvalue>(Value) : 0;
	};
}


template <typename T, uintmax_t Value, bool IsValid = true>
struct uvalue
	: contracts::unsigned_integral_type<T>
	, public internal::value_base<uintmax_t, T, Value, IsValid>
{};

template <typename T, intmax_t Value, bool IsValid = true>
struct svalue
	: contracts::signed_integral_type<T>
	, public internal::value_base<intmax_t, T, Value, IsValid>
{};




namespace operations
{

	template <typename TV1, typename TV2> struct safe_add;
	template <typename T1, uintmax_t V1, bool B1, typename T2, uintmax_t V2, bool B2>
	struct safe_add<uvalue<T1, V1, B1>, uvalue<T2, V2, B2>>
	{
	private:
		typedef typename internal::common_unsigned_integral_type<T1, T2>::type common_type;
	public:
		typedef uvalue<common_type, V1 + V2, B1 && B2 && (
			std::numeric_limits<common_type>::max() - V1 >= V2
		)> type;
	};


	template <typename TV1, typename TV2> struct safe_sub;
	template <typename T1, uintmax_t V1, bool B1, typename T2, uintmax_t V2, bool B2>
	struct safe_sub<uvalue<T1, V1, B1>, uvalue<T2, V2, B2>>
	{
	private:
		typedef typename internal::common_unsigned_integral_type<T1, T2>::type common_type;
	public:
		typedef uvalue<common_type, V1 - V2, B1 && B2 && V1 >= V2> type;
	};


	template <typename TV1, typename TV2> struct safe_mul;
	template <typename T1, uintmax_t V1, bool B1, typename T2, uintmax_t V2, bool B2>
	struct safe_mul<uvalue<T1, V1, B1>, uvalue<T2, V2, B2>>
	{
	private:
		typedef typename internal::common_unsigned_integral_type<T1, T2>::type common_type;
	public:
		typedef uvalue<common_type, V1 * V2, B1 && B2 && (
			V1 == 0 || V2 == 0 ||
			std::numeric_limits<common_type>::max() / V2 > V1
		)> type;
	};


	template <typename TV1, typename TV2> struct safe_div;
	template <typename T1, uintmax_t V1, bool B1, typename T2, uintmax_t V2, bool B2>
	struct safe_div<uvalue<T1, V1, B1>, uvalue<T2, V2, B2>>
	{
	private:
		typedef typename internal::common_unsigned_integral_type<T1, T2>::type common_type;
	public:
		typedef uvalue<common_type, V2 ? V1 / V2 : 0, B1 && B2 && V2 != 0> type;
	};


	template <typename TVbase, typename TVshift> struct safe_lshift;
	template <typename TB, uintmax_t VB, bool BB, typename TS, uintmax_t VS, bool BS>
	struct safe_lshift<uvalue<TB, VB, BB>, uvalue<TS, VS, BS>>
		: contracts::binary_integral_type<TB>
	{
	private:
		// Refer C++11 specification [3.9.1 p3] for reason that
		// conversion to "unsigned" for non-negative "int" numbers is safe.
		static constexpr const bool is_valid = BB && BS
			&& VS < static_cast<unsigned>(std::numeric_limits<TB>::digits)
			&& (std::numeric_limits<TB>::max() >> VS) >= VB;
	public:
		typedef uvalue<TB, is_valid ? (VB << VS) : 0, is_valid> type;
	};
	template <typename TB, uintmax_t VB, bool BB, typename TS, intmax_t VS, bool BS>
	struct safe_lshift<uvalue<TB, VB, BB>, svalue<TS, VS, BS>>
		: contracts::binary_integral_type<TB>
	{
	private:
		static constexpr const bool is_valid = BB && BS
			&& VS >= 0
			&& VS < std::numeric_limits<TB>::digits
			&& (std::numeric_limits<TB>::max() >> VS) >= VB;
	public:
		typedef uvalue<TB, is_valid ? (VB << VS) : 0, is_valid> type;
	};
	template <typename TB, intmax_t VB, bool BB, typename TS, uintmax_t VS, bool BS>
	struct safe_lshift<svalue<TB, VB, BB>, uvalue<TS, VS, BS>>
		: contracts::binary_integral_type<TB>
	{
	private:
		static constexpr const bool is_valid = BB && BS
			&& VS < static_cast<unsigned>(std::numeric_limits<TB>::digits)
			&& (std::numeric_limits<TB>::max() >> VS) >= VB;
	public:
		typedef svalue<TB, is_valid ? (VB << VS) : 0, is_valid> type;
	};
	template <typename TB, intmax_t VB, bool BB, typename TS, intmax_t VS, bool BS>
	struct safe_lshift<svalue<TB, VB, BB>, svalue<TS, VS, BS>>
		: contracts::binary_integral_type<TB>
	{
	private:
		static constexpr const bool is_valid = BB && BS
			&& VS >= 0
			&& VS < std::numeric_limits<TB>::digits
			&& (std::numeric_limits<TB>::max() >> VS) >= VB;
	public:
		typedef svalue<TB, is_valid ? (VB << VS) : 0, is_valid> type;
	};


	template <typename TVbase, typename TVshift> struct safe_rshift;
	template <typename TB, uintmax_t VB, bool BB, typename TS, uintmax_t VS, bool BS>
	struct safe_rshift<uvalue<TB, VB, BB>, uvalue<TS, VS, BS>>
		: contracts::binary_integral_type<TB>
	{
	private:
		static constexpr const bool is_valid = BB && BS
			&& VS < static_cast<unsigned>(std::numeric_limits<TB>::digits);
	public:
		typedef uvalue<TB, is_valid ? (VB >> VS) : 0, is_valid> type;
	};
	template <typename TB, uintmax_t VB, bool BB, typename TS, intmax_t VS, bool BS>
	struct safe_rshift<uvalue<TB, VB, BB>, svalue<TS, VS, BS>>
		: contracts::binary_integral_type<TB>
	{
	private:
		static constexpr const bool is_valid = BB && BS
			&& VS >= 0
			&& VS < std::numeric_limits<TB>::digits;
	public:
		typedef uvalue<TB, is_valid ? (VB >> VS) : 0, is_valid> type;
	};
	template <typename TB, intmax_t VB, bool BB, typename TS, uintmax_t VS, bool BS>
	struct safe_rshift<svalue<TB, VB, BB>, uvalue<TS, VS, BS>>
		: contracts::binary_integral_type<TB>
	{
	private:
		static constexpr const bool is_valid = BB && BS
			&& VS < static_cast<unsigned>(std::numeric_limits<TB>::digits);
	public:
		typedef svalue<TB, is_valid ? (VB >> VS) : 0, is_valid> type;
	};
	template <typename TB, intmax_t VB, bool BB, typename TS, intmax_t VS, bool BS>
	struct safe_rshift<svalue<TB, VB, BB>, svalue<TS, VS, BS>>
		: contracts::binary_integral_type<TB>
	{
	private:
		static constexpr const bool is_valid = BB && BS
			&& VS >= 0
			&& VS < std::numeric_limits<TB>::digits;
	public:
		typedef svalue<TB, is_valid ? (VB >> VS) : 0, is_valid> type;
	};


	namespace internal
	{
		// internal::value_base performs some required tests
		template <typename Tnew, typename TV> struct safe_cast_to_u;
		template <typename Tnew, typename Told, uintmax_t V, bool B>
		struct safe_cast_to_u<Tnew, uvalue<Told, V, B>>
		{
			typedef uvalue<Tnew, V, B> type;
		};
		template <typename Tnew, typename Told, intmax_t V, bool B>
		struct safe_cast_to_u<Tnew, svalue<Told, V, B>>
		{
		private:
			static constexpr const bool is_valid = B && V >= 0;
		public:
			typedef uvalue<Tnew,
				is_valid ? static_cast<uintmax_t>(V) : 0,
				is_valid> type;
		};
		template <typename Tnew, typename TV> struct safe_cast_to_s;
		template <typename Tnew, typename Told, intmax_t V, bool B>
		struct safe_cast_to_s<Tnew, svalue<Told, V, B>>
		{
			typedef svalue<Tnew, V, B> type;
		};
		template <typename Tnew, typename Told, uintmax_t V, bool B>
		struct safe_cast_to_s<Tnew, uvalue<Told, V, B>>
		{
		private:
			static constexpr const bool is_valid = B
				&& V <= static_cast<uintmax_t>(std::numeric_limits<intmax_t>::max());
		public:
			typedef svalue<Tnew,
				is_valid ? static_cast<intmax_t>(V) : 0,
				is_valid> type;
		};
	}
	template <typename Tnew, typename TV>
	struct safe_cast
		: public std::conditional<
			contracts::is_unsigned_integral_type<Tnew>(),
			internal::safe_cast_to_u<Tnew, TV>,
			typename std::conditional<
				contracts::is_signed_integral_type<Tnew>(),
				internal::safe_cast_to_s<Tnew, TV>, void
			>::type
		>::type {};

}




template <typename TV1, typename TV2>
using safe_add = typename operations::safe_add<TV1, TV2>::type;
template <typename TV1, typename TV2>
using safe_sub = typename operations::safe_sub<TV1, TV2>::type;
template <typename TV1, typename TV2>
using safe_mul = typename operations::safe_mul<TV1, TV2>::type;
template <typename TV1, typename TV2>
using safe_div = typename operations::safe_div<TV1, TV2>::type;
template <typename TVbase, typename TVshift>
using safe_lshift = typename operations::safe_lshift<TVbase, TVshift>::type;
template <typename TVbase, typename TVshift>
using safe_rshift = typename operations::safe_rshift<TVbase, TVshift>::type;
template <typename Tnew, typename TV>
using safe_cast = typename operations::safe_cast<Tnew, TV>::type;

}}

#endif
