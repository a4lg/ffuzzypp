/*

	ffuzzy++ : C++ implementation of fast fuzzy hashing

	strings/edit_dist.hpp
	Indel distance implementation


	Copyright (C) 2014 kikairoya <kikairoya@gmail.com>
	Copyright (C) 2017 Tsukasa OI <floss_ssdeep@irq.a4lg.com>


	This program can be used, redistributed or modified under any of
	Boost Software License 1.0, GPL v2 or GPL v3

*/
#ifndef FFUZZYPP_STRINGS_EDIT_DIST_HPP
#define FFUZZYPP_STRINGS_EDIT_DIST_HPP

#include <cassert>
#include <cstddef>

#include <algorithm>
#include <limits>
#include <utility>

#include "../utils/safe_int.hpp"
#include "position_array.hpp"

namespace ffuzzy {
namespace strings {

namespace internal
{
	template <typename Tcost>
	class edit_dist_dp_impl
	{
	private:
		edit_dist_dp_impl(void) = delete;
		edit_dist_dp_impl(const edit_dist_dp_impl&) = delete;
	public:
		typedef Tcost cost_type;
		static_assert(
			safe_int::contracts::is_unsigned_integral_type<cost_type>(),
			"cost_type must be an unsigned integral type.");
	private:
		static void update_cost_inner(
			const char* s1,
			const char* s2, size_t s2len,
			size_t i,
			cost_type* &row1, cost_type* &row2
		) noexcept
		{
			row2[0] = static_cast<cost_type>(i) + 1;
			for (size_t j = 0; j < s2len; j++)
			{
				cost_type cost_a = row1[j+1] + 1;
				cost_type cost_d = row2[j]   + 1;
				cost_type cost_r = row1[j]   + (s1[i] == s2[j] ? 0 : 2);
				row2[j+1] = std::min(std::min(cost_a, cost_d), cost_r);
			}
			std::swap(row1, row2);
		}
	public:
		static cost_type cost(
			const char* s1, size_t s1len,
			const char* s2, size_t s2len,
			cost_type* row1, cost_type* row2
		) noexcept
		{
			for (size_t j = 0; j <= s2len; j++)
				row1[j] = static_cast<cost_type>(j);
			for (size_t i = 0; i < s1len; i++)
				update_cost_inner(s1, s2, s2len, i, row1, row2);
			return row1[s2len];
		}
		static cost_type cost_nonempty(
			const char* s1, size_t s1len,
			const char* s2, size_t s2len,
			cost_type* row1, cost_type* row2
		) noexcept
		{
			#ifdef FFUZZYPP_DEBUG
			assert(s1len > 0);
			assert(s2len > 0);
			#endif
			row1[0] = 1;
			for (size_t j = 0; j < s2len; j++)
			{
				cost_type cost_d = row1[j] + 1;
				cost_type cost_r = static_cast<cost_type>(j) + (s1[0] == s2[j] ? 0 : 2);
				row1[j+1] = std::min(cost_d, cost_r);
			}
			for (size_t i = 1; i < s1len; i++)
				update_cost_inner(s1, s2, s2len, i, row1, row2);
			return row1[s2len];
		}
	};

	template <typename Tcost, size_t MaxSize>
	class edit_dist_bitparallel_impl
	{
	private:
		edit_dist_bitparallel_impl(void) = delete;
		edit_dist_bitparallel_impl(const edit_dist_bitparallel_impl&) = delete;
	public:
		typedef Tcost cost_type;
	public:
		template <typename TBitmap, char CMin, char CMax>
		static cost_type cost_nonempty(
			const position_array<TBitmap, char, CMin, CMax>& s1, size_t s1len,
			const char* s2, size_t s2len
		) noexcept
		{
			cost_type cur = s1len;
			TBitmap msb = TBitmap(1ull) << (s1len - 1);
			TBitmap pv = -1;
			TBitmap nv = 0;
			for (size_t i = 0; i < s2len; i++)
			{
				TBitmap mt = s1[s2[i]];
				TBitmap zd = (((mt & pv) + pv) ^ pv) | mt | nv;
				TBitmap nh = pv & zd;
				if (nh & msb)
					--cur;
				TBitmap x  = nv | ~(pv | zd) | (pv & ~mt & TBitmap(1ull));
				TBitmap y  = (pv - nh) >> 1;
				/*
					i-th bit of ph does not depend on i-th bit of y
					(only upper bits of ph are affected).
					So, ph does not depend on invalid bit in y.
				*/
				TBitmap ph = (x + y) ^ y;
				if (ph & msb)
					++cur;
				TBitmap t = (ph << 1) + TBitmap(1ull);
				nv = t & zd;
				pv = (nh << 1) | ~(t | zd) | (t & (pv - nh));
			}
			return cur;
		}
		template <typename TBitmap, char CMin, char CMax>
		static cost_type cost(
			const position_array<TBitmap, char, CMin, CMax>& s1, size_t s1len,
			const char* s2, size_t s2len
		) noexcept
		{
			return s1len == 0
				? s2len
				: cost_nonempty(s1, s1len, s2, s2len);
		}
	};
}

template <typename Tcost, size_t MaxSize>
class edit_dist_dp
{
private:
	edit_dist_dp(void) = delete;
	edit_dist_dp(const edit_dist_dp&) = delete;
public:
	static constexpr const size_t max_size = MaxSize;
	static_assert(max_size > 0, "max_size must not be zero.");
	static_assert(safe_int::safe_add<
			safe_int::uvalue<size_t, max_size>,
			safe_int::uvalue<size_t, 1>
		>::is_valid,
		"max_size + 1 must be in range of size_t.");
	typedef Tcost cost_type;
	static_assert(safe_int::safe_mul<
			safe_int::uvalue<cost_type, max_size>,
			safe_int::uvalue<cost_type, 2>
		>::is_valid,
		"max_size * 2 must be in range of cost_type.");
public:
	static cost_type cost(
		const char* s1, size_t s1len,
		const char* s2, size_t s2len
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(s1len <= max_size);
		assert(s2len <= max_size);
		#endif
		cost_type rows[2][max_size + 1];
		return internal::edit_dist_dp_impl<cost_type>::cost(s1, s1len, s2, s2len, rows[0], rows[1]);
	}
};

template <typename Tcost, size_t MaxSize>
class edit_dist_nonempty_dp
{
private:
	edit_dist_nonempty_dp(void) = delete;
	edit_dist_nonempty_dp(const edit_dist_nonempty_dp&) = delete;
public:
	static constexpr const size_t max_size = MaxSize;
	static_assert(max_size > 0, "max_size must not be zero.");
	static_assert(safe_int::safe_add<
			safe_int::uvalue<size_t, max_size>,
			safe_int::uvalue<size_t, 1>
		>::is_valid,
		"max_size + 1 must be in range of size_t.");
	typedef Tcost cost_type;
	static_assert(safe_int::safe_mul<
			safe_int::uvalue<cost_type, max_size>,
			safe_int::uvalue<cost_type, 2>
		>::is_valid,
		"max_size * 2 must be in range of cost_type.");
public:
	static cost_type cost(
		const char* s1, size_t s1len,
		const char* s2, size_t s2len
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(s1len <= max_size);
		assert(s2len <= max_size);
		#endif
		cost_type rows[2][max_size + 1];
		return internal::edit_dist_dp_impl<cost_type>::cost_nonempty(s1, s1len, s2, s2len, rows[0], rows[1]);
	}
};

template <typename Tcost, typename TBitmap, char CMin, char CMax, size_t MaxSize>
class edit_dist_bitparallel
{
private:
	edit_dist_bitparallel(void) = delete;
	edit_dist_bitparallel(const edit_dist_bitparallel&) = delete;
public:
	static constexpr const size_t max_size = MaxSize;
	static_assert(max_size > 0, "max_size must not be zero.");
	static_assert(safe_int::safe_add<
			safe_int::uvalue<size_t, max_size>,
			safe_int::uvalue<size_t, 1>
		>::is_valid,
		"max_size + 1 must be in range of size_t.");
	typedef Tcost cost_type;
	static_assert(safe_int::safe_mul<
			safe_int::uvalue<cost_type, max_size>,
			safe_int::uvalue<cost_type, 2>
		>::is_valid,
		"max_size * 2 must be in range of cost_type.");
public:
	static cost_type cost(
		const position_array<TBitmap, char, CMin, CMax>& s1, size_t s1len,
		const char* s2, size_t s2len
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(s1len <= max_size);
		assert(s2len <= max_size);
		#endif
		return internal::edit_dist_bitparallel_impl<Tcost, MaxSize>::cost(s1, s1len, s2, s2len);
	}
};

template <typename Tcost, typename TBitmap, char CMin, char CMax, size_t MaxSize>
class edit_dist_bitparallel_wrapper
{
private:
	edit_dist_bitparallel_wrapper(void) = delete;
	edit_dist_bitparallel_wrapper(const edit_dist_bitparallel_wrapper&) = delete;
public:
	static constexpr const size_t max_size = MaxSize;
	static_assert(max_size > 0, "max_size must not be zero.");
	static_assert(safe_int::safe_add<
			safe_int::uvalue<size_t, max_size>,
			safe_int::uvalue<size_t, 1>
		>::is_valid,
		"max_size + 1 must be in range of size_t.");
	typedef Tcost cost_type;
	static_assert(safe_int::safe_mul<
			safe_int::uvalue<cost_type, max_size>,
			safe_int::uvalue<cost_type, 2>
		>::is_valid,
		"max_size * 2 must be in range of cost_type.");
public:
	static cost_type cost(
		const char* s1, size_t s1len,
		const char* s2, size_t s2len
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(s1len <= max_size);
		assert(s2len <= max_size);
		#endif
		position_array<TBitmap, char, CMin, CMax> parray(s1, s1len);
		return internal::edit_dist_bitparallel_impl<Tcost, MaxSize>::cost(parray, s1len, s2, s2len);
	}
};

template <typename Tcost, typename TBitmap, char CMin, char CMax, size_t MaxSize>
class edit_dist_nonempty_bitparallel
{
private:
	edit_dist_nonempty_bitparallel(void) = delete;
	edit_dist_nonempty_bitparallel(const edit_dist_nonempty_bitparallel&) = delete;
public:
	static constexpr const size_t max_size = MaxSize;
	static_assert(max_size > 0, "max_size must not be zero.");
	static_assert(safe_int::safe_add<
			safe_int::uvalue<size_t, max_size>,
			safe_int::uvalue<size_t, 1>
		>::is_valid,
		"max_size + 1 must be in range of size_t.");
	typedef Tcost cost_type;
	static_assert(safe_int::safe_mul<
			safe_int::uvalue<cost_type, max_size>,
			safe_int::uvalue<cost_type, 2>
		>::is_valid,
		"max_size * 2 must be in range of cost_type.");
public:
	static cost_type cost(
		const position_array<TBitmap, char, CMin, CMax>& s1, size_t s1len,
		const char* s2, size_t s2len
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(s1len <= max_size);
		assert(s2len <= max_size);
		#endif
		return internal::edit_dist_bitparallel_impl<Tcost, MaxSize>::cost_nonempty(s1, s1len, s2, s2len);
	}
};

template <typename Tcost, typename TBitmap, char CMin, char CMax, size_t MaxSize>
class edit_dist_nonempty_bitparallel_wrapper
{
private:
	edit_dist_nonempty_bitparallel_wrapper(void) = delete;
	edit_dist_nonempty_bitparallel_wrapper(const edit_dist_nonempty_bitparallel_wrapper&) = delete;
public:
	static constexpr const size_t max_size = MaxSize;
	static_assert(max_size > 0, "max_size must not be zero.");
	static_assert(safe_int::safe_add<
			safe_int::uvalue<size_t, max_size>,
			safe_int::uvalue<size_t, 1>
		>::is_valid,
		"max_size + 1 must be in range of size_t.");
	typedef Tcost cost_type;
	static_assert(safe_int::safe_mul<
			safe_int::uvalue<cost_type, max_size>,
			safe_int::uvalue<cost_type, 2>
		>::is_valid,
		"max_size * 2 must be in range of cost_type.");
public:
	static cost_type cost(
		const char* s1, size_t s1len,
		const char* s2, size_t s2len
	) noexcept
	{
		#ifdef FFUZZYPP_DEBUG
		assert(s1len <= max_size);
		assert(s2len <= max_size);
		#endif
		position_array<TBitmap, char, CMin, CMax> parray(s1, s1len);
		return internal::edit_dist_bitparallel_impl<Tcost, MaxSize>::cost_nonempty(parray, s1len, s2, s2len);
	}
};



namespace internal
{
	template <typename Tcost, char CMin, char CMax, size_t MaxSize, bool IsPositionArrayAvailable>
	class edit_dist_impl_selector;

	template <typename Tcost, char CMin, char CMax, size_t MaxSize>
	class edit_dist_impl_selector<Tcost, CMin, CMax, MaxSize, true>
	{
	private:
		edit_dist_impl_selector(void) = delete;
		edit_dist_impl_selector(const edit_dist_impl_selector&) = delete;
	public:
		static Tcost cost(
			const char* s1, size_t s1len,
			const char* s2, size_t s2len
		) noexcept
		{
			typename auto_position_array<MaxSize, char, CMin, CMax>::type parray(s1, s1len);
			return edit_dist_bitparallel<Tcost,
				typename decltype(parray)::bitmap_type, CMin, CMax, MaxSize>::cost(parray, s1len, s2, s2len);
		}
		static Tcost cost_nonempty(
			const char* s1, size_t s1len,
			const char* s2, size_t s2len
		) noexcept
		{
			typename auto_position_array<MaxSize, char, CMin, CMax>::type parray(s1, s1len);
			return edit_dist_nonempty_bitparallel<Tcost,
				typename decltype(parray)::bitmap_type, CMin, CMax, MaxSize>::cost(parray, s1len, s2, s2len);
		}
	};

	template <typename Tcost, char CMin, char CMax, size_t MaxSize>
	class edit_dist_impl_selector<Tcost, CMin, CMax, MaxSize, false>
	{
	private:
		edit_dist_impl_selector(void) = delete;
		edit_dist_impl_selector(const edit_dist_impl_selector&) = delete;
	public:
		static Tcost cost(
			const char* s1, size_t s1len,
			const char* s2, size_t s2len
		) noexcept
		{
			return edit_dist_dp<Tcost, MaxSize>::cost(s1, s1len, s2, s2len);
		}
		static Tcost cost_nonempty(
			const char* s1, size_t s1len,
			const char* s2, size_t s2len
		) noexcept
		{
			return edit_dist_nonempty_dp<Tcost, MaxSize>::cost(s1, s1len, s2, s2len);
		}
	};
}



template <typename Tcost, size_t MaxSize>
class edit_dist_fast
{
private:
	edit_dist_fast(void) = delete;
	edit_dist_fast(const edit_dist_fast&) = delete;
public:
	static constexpr const size_t max_size = MaxSize;
	typedef Tcost cost_type;
public:
	template <
		char CMin = std::numeric_limits<char>::min(),
		char CMax = std::numeric_limits<char>::max()
	>
	static Tcost cost(
		const char* s1, size_t s1len,
		const char* s2, size_t s2len
	) noexcept
	{
		return internal::edit_dist_impl_selector<
			cost_type, CMin, CMax, max_size,
			is_auto_position_array_available<MaxSize, char, CMin, CMax>::value
		>::cost(s1, s1len, s2, s2len);
	}
};

template <typename Tcost, size_t MaxSize>
class edit_dist_nonempty_fast
{
private:
	edit_dist_nonempty_fast(void) = delete;
	edit_dist_nonempty_fast(const edit_dist_nonempty_fast&) = delete;
public:
	static constexpr const size_t max_size = MaxSize;
	typedef Tcost cost_type;
public:
	template <
		char CMin = std::numeric_limits<char>::min(),
		char CMax = std::numeric_limits<char>::max()
	>
	static Tcost cost(
		const char* s1, size_t s1len,
		const char* s2, size_t s2len
	) noexcept
	{
		return internal::edit_dist_impl_selector<
			cost_type, CMin, CMax, max_size,
			is_auto_position_array_available<MaxSize, char, CMin, CMax>::value
		>::cost_nonempty(s1, s1len, s2, s2len);
	}
};



template <typename Tedit_dist>
class edit_dist_norm
{
private:
	edit_dist_norm(void) = delete;
	edit_dist_norm(const edit_dist_norm&) = delete;
public:
	typedef typename Tedit_dist::cost_type cost_type;
	static cost_type cost(
		const char* s1, size_t s1len,
		const char* s2, size_t s2len
	) noexcept
	{
		// Normalization: make s1 short for better effficiency
		if (s1len <= s2len)
			return Tedit_dist::cost(s1, s1len, s2, s2len);
		else
			return Tedit_dist::cost(s2, s2len, s1, s1len);
	}
};

template <typename Tedit_dist>
class edit_dist_norm_rev
{
private:
	edit_dist_norm_rev(void) = delete;
	edit_dist_norm_rev(const edit_dist_norm_rev&) = delete;
public:
	typedef typename Tedit_dist::cost_type cost_type;
	static cost_type cost(
		const char* s1, size_t s1len,
		const char* s2, size_t s2len
	) noexcept
	{
		// Reverse normalization: make s2 short for (possibly) worst effficiency
		if (s1len <= s2len)
			return Tedit_dist::cost(s2, s2len, s1, s1len);
		else
			return Tedit_dist::cost(s1, s1len, s2, s2len);
	}
};

}}

#endif
