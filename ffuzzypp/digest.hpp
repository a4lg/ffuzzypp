/*

	ffuzzy++ : C++ implementation of fast fuzzy hashing

	digest.hpp
	Fuzzy digest (wrapper with converters)


	CREDITS OF ORIGINAL VERSION OF SSDEEP

	Copyright (C) 2002 Andrew Tridgell <tridge@samba.org>
	Copyright (C) 2006 ManTech International Corporation
	Copyright (C) 2013 Helmut Grohne <helmut@subdivi.de>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA


	CREDIT OF MODIFIED PORTIONS

	Copyright (C) 2015 Tsukasa OI <floss_ssdeep@irq.a4lg.com>

*/
#ifndef FFUZZYPP_DIGEST_HPP
#define FFUZZYPP_DIGEST_HPP

#include <algorithm>
#include <functional>
#include <string>
#include <type_traits>

#include "digest_data.hpp"
#include "digest_base.hpp"
#include "utils/type_modifier.hpp"

namespace ffuzzy {


/*
	Digest Types:

	digest_data<IsShort>
		Base class which holds the most basic information for fuzzy digest
	digest_base<IsShort, IsNormalized>
		Base class to provide common implementation for digest.
		This type is partially specialized by IsNormalized parameter.
	digest<IsShort, IsNormalized>
		Final class you want to use.
		This type is specialized by both parameters.
*/

// Wrapper class
template <bool IsShort, bool IsNormalized> class digest;

// Digest (short; normalized)
template <>
class digest<true, true>
	: public digest_base<true, true>
{
public:
	digest(void) noexcept = default;
	digest(const digest& other) noexcept : digest_base<true, true>(other) {}
	explicit digest(const char* str) noexcept(false) : digest_base<true, true>(str) {}
	explicit digest(const std::string& str) : digest(str.c_str()) {}
	const digest& operator=(const digest& other) noexcept
	{
		digest_base<true, true>::operator=(other);
		return *this;
	}
};

// Digest (short; unnormalized)
template <>
class digest<true, false>
	: public digest_base<true, false>
{
public:
	digest(void) noexcept = default;
	digest(const digest& other) noexcept : digest_base<true, false>(other) {}
	digest(const digest<true, true>& other) noexcept
	{
		digest_data<true>::operator=(other);
	}
	explicit digest(const char* str) noexcept(false) : digest_base<true, false>(str) {}
	explicit digest(const std::string& str) : digest(str.c_str()) {}
	const digest& operator=(const digest& other) noexcept
	{
		digest_base<true, false>::operator=(other);
		return *this;
	}
	const digest& operator=(const digest<true, true>& other) noexcept
	{
		digest_data<true>::operator=(other);
		return *this;
	}
};

// Digest (long; normalized)
template <>
class digest<false, true>
	: public digest_base<false, true>
{
public:
	digest(void) noexcept = default;
	digest(const digest& other) noexcept : digest_base<false, true>(other) {}
	digest(const digest<true, true>& other) noexcept
	{
		internal::digest_copy::copy(*this, other);
	}
	explicit digest(const char* str) noexcept(false) : digest_base<false, true>(str) {}
	explicit digest(const std::string& str) : digest(str.c_str()) {}
	const digest& operator=(const digest& other) noexcept
	{
		digest_base<false, true>::operator=(other);
		return *this;
	}
	const digest& operator=(const digest<true, true>& other) noexcept
	{
		internal::digest_copy::copy(*static_cast<digest_data<false>*>(this), other);
		return *this;
	}
};

// Digest (long; unnormalized)
template <>
class digest<false, false>
	: public digest_base<false, false>
{
public:
	digest(void) noexcept = default;
	digest(const digest& other) noexcept : digest_base<false, false>(other) {}
	digest(const digest<false, true>& other) noexcept
	{
		digest_data<false>::operator=(other);
	}
	digest(const digest<true, false>& other) noexcept
	{
		internal::digest_copy::copy(*this, other);
	}
	digest(const digest<true, true>& other) noexcept
	{
		internal::digest_copy::copy(*this, other);
	}
	explicit digest(const char* str) noexcept(false) : digest_base<false, false>(str) {}
	explicit digest(const std::string& str) : digest(str.c_str()) {}
	const digest& operator=(const digest& other) noexcept
	{
		digest_base<false, false>::operator=(other);
		return *this;
	}
	const digest& operator=(const digest<false, true>& other) noexcept
	{
		digest_data<false>::operator=(other);
		return *this;
	}
	const digest& operator=(const digest<true, false>& other) noexcept
	{
		internal::digest_copy::copy(*static_cast<digest_data<false>*>(this), other);
		return *this;
	}
	const digest& operator=(const digest<true, true>& other) noexcept
	{
		internal::digest_copy::copy(*static_cast<digest_data<false>*>(this), other);
		return *this;
	}
};

// Typedefs for digest specializations
typedef digest< true, true>   digest_t;
typedef digest< true, false>  digest_unorm_t;
typedef digest<false, true>   digest_long_t;
typedef digest<false, false>  digest_long_unorm_t;



/*
	Type modifiers
*/
namespace internal
{
	template <typename T>
	struct digest_traits
	{
		static constexpr const bool is_valid = false;
	};
	template <bool IsShort, bool IsNormalized>
	struct digest_traits<digest_base<IsShort, IsNormalized>>
	{
		static constexpr const bool is_valid = true;
		typedef digest_base<IsShort,  true> norm_type;
		typedef digest_base<IsShort, false> unorm_type;
		typedef digest_base< true, IsNormalized> short_type;
		typedef digest_base<false, IsNormalized> long_type;
	};
	template <bool IsShort, bool IsNormalized>
	struct digest_traits<digest<IsShort, IsNormalized>>
	{
		static constexpr const bool is_valid = true;
		typedef digest<IsShort,  true> norm_type;
		typedef digest<IsShort, false> unorm_type;
		typedef digest< true, IsNormalized> short_type;
		typedef digest<false, IsNormalized> long_type;
	};
	template <typename T>
	struct digest_alt_type_selector
	{
	private:
		typedef digest_traits<typename std::remove_cv<T>::type> traits_type;
		static_assert(traits_type::is_valid, "You must give correct type to retrieve alternative types.");
	public:
		typedef typename type_mod::cv_match<T, typename traits_type::norm_type>::type   norm_type;
		typedef typename type_mod::cv_match<T, typename traits_type::unorm_type>::type  unorm_type;
		typedef typename type_mod::cv_match<T, typename traits_type::short_type>::type  short_type;
		typedef typename type_mod::cv_match<T, typename traits_type::long_type>::type   long_type;
	};
}
template <typename T> using digest_to_unorm = typename internal::digest_alt_type_selector<T>::unorm_type;
template <typename T> using digest_to_norm  = typename internal::digest_alt_type_selector<T>::norm_type;
template <typename T> using digest_to_short = typename internal::digest_alt_type_selector<T>::short_type;
template <typename T> using digest_to_long  = typename internal::digest_alt_type_selector<T>::long_type;


/*
	We expect following properties for each digest types:

	*	is a trivially default constructible type
	*	is a standard-layout type

	Digest types are:

	*	digest_data<IsShort>
	*	digest<IsShort, true>
	*	digest<IsShort, false>

	We also expect that these types are nearly equivalent.
*/
#ifdef FFUZZYPP_LOCAL_CHK
#error do not define FFUZZYPP_LOCAL_CHK
#endif
#ifdef FFUZZYPP_LOCAL_CHK1
#error do not define FFUZZYPP_LOCAL_CHK1
#endif
#define FFUZZYPP_LOCAL_CHK1(s, t) \
	static_assert(std::is_trivially_default_constructible<digest<s, t>>::value, \
		"digest<" #s ", " #t "> must be a trivially default constructible type."); \
	static_assert(std::is_standard_layout<digest<s, t>>::value, \
		"digest<" #s ", " #t "> must be a standard-layout type."); \
	static_assert(std::is_base_of<digest_data<s>, digest<s, t>>::value, \
		"digest_data<" #s ">, digest<" #s ", true> and digest<" #s ", false> must be nearly equivalent."); \
	static_assert(sizeof(digest_data<s>) == sizeof(digest<s, t>), \
		"digest_data<" #s ">, digest<" #s ", true> and digest<" #s ", false> must be nearly equivalent.")
#define FFUZZYPP_LOCAL_CHK(s) \
	static_assert(std::is_trivially_default_constructible<digest_data<s>>::value, \
		"digest_data<" #s "> must be a trivially default constructible type."); \
	static_assert(std::is_standard_layout<digest_data<s>>::value, \
		"digest_data<" #s "> must be a standard-layout type."); \
	FFUZZYPP_LOCAL_CHK1(s, true); \
	FFUZZYPP_LOCAL_CHK1(s, false)
FFUZZYPP_LOCAL_CHK(true);
FFUZZYPP_LOCAL_CHK(false);
#undef FFUZZYPP_LOCAL_CHK
#undef FFUZZYPP_LOCAL_CHK1

}


// Specialization of standard hash and swap
namespace std
{
	template <bool IsShort, bool IsNormalized>
	struct hash<ffuzzy::digest<IsShort, IsNormalized>>
	{
		size_t operator()(const ffuzzy::digest<IsShort, IsNormalized>& value) const
		{
			return value.hash();
		}
	};
	template <bool IsShort, bool IsNormalized>
	inline void swap(
		ffuzzy::digest<IsShort, IsNormalized>& a,
		ffuzzy::digest<IsShort, IsNormalized>& b
	) noexcept
	{
		ffuzzy::digest<IsShort, IsNormalized>::swap(a, b);
	}
}

#endif
