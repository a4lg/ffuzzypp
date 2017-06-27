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

	Copyright (C) 2017 Tsukasa OI <floss_ssdeep@irq.a4lg.com>

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

// Wrapper class
template <bool IsAlphabetRestricted, bool IsShort, bool IsNormalized> class digest;

// Digest (alphabet restricted; short; normalized)
template <>
class digest<true, true, true>
	: public digest_base<true, true, true>
{
public:
	digest(void) noexcept = default; // initialize to undefined state
	digest(const digest& other) noexcept : digest_base<true, true, true>(other) {}
	explicit digest(const char* str) noexcept(false) : digest_base<true, true, true>(str) {}
	explicit digest(const std::string& str) : digest(str.c_str()) {}
	const digest& operator=(const digest& other) noexcept
	{
		digest_base<true, true, true>::operator=(other);
		return *this;
	}
};

// Digest (alphabet restricted; short; unnormalized)
template <>
class digest<true, true, false>
	: public digest_base<true, true, false>
{
public:
	digest(void) noexcept = default; // initialize to undefined state
	digest(const digest& other) noexcept : digest_base<true, true, false>(other) {}
	digest(const digest<true, true, true>& other) noexcept
	{
		digest_data<true, true>::operator=(other);
	}
	explicit digest(const char* str) noexcept(false) : digest_base<true, true, false>(str) {}
	explicit digest(const std::string& str) : digest(str.c_str()) {}
	const digest& operator=(const digest& other) noexcept
	{
		digest_base<true, true, false>::operator=(other);
		return *this;
	}
	const digest& operator=(const digest<true, true, true>& other) noexcept
	{
		digest_data<true, true>::operator=(other);
		return *this;
	}
};

// Digest (alphabet restricted; long; normalized)
template <>
class digest<true, false, true>
	: public digest_base<true, false, true>
{
public:
	digest(void) noexcept = default; // initialize to undefined state
	digest(const digest& other) noexcept : digest_base<true, false, true>(other) {}
	digest(const digest<true, true, true>& other) noexcept
	{
		internal::digest_copy::copy_to_long(*this, other);
	}
	explicit digest(const char* str) noexcept(false) : digest_base<true, false, true>(str) {}
	explicit digest(const std::string& str) : digest(str.c_str()) {}
	const digest& operator=(const digest& other) noexcept
	{
		digest_base<true, false, true>::operator=(other);
		return *this;
	}
	const digest& operator=(const digest<true, true, true>& other) noexcept
	{
		internal::digest_copy::copy_to_long(*this, other);
		return *this;
	}
};

// Digest (alphabet restricted; long; unnormalized)
template <>
class digest<true, false, false>
	: public digest_base<true, false, false>
{
public:
	digest(void) noexcept = default; // initialize to undefined state
	digest(const digest& other) noexcept : digest_base<true, false, false>(other) {}
	digest(const digest<true, false, true>& other) noexcept
	{
		digest_data<true, false>::operator=(other);
	}
	digest(const digest<true, true, false>& other) noexcept
	{
		internal::digest_copy::copy_to_long(*this, other);
	}
	digest(const digest<true, true, true>& other) noexcept
	{
		internal::digest_copy::copy_to_long(*this, other);
	}
	explicit digest(const char* str) noexcept(false) : digest_base<true, false, false>(str) {}
	explicit digest(const std::string& str) : digest(str.c_str()) {}
	const digest& operator=(const digest& other) noexcept
	{
		digest_base<true, false, false>::operator=(other);
		return *this;
	}
	const digest& operator=(const digest<true, false, true>& other) noexcept
	{
		digest_data<true, false>::operator=(other);
		return *this;
	}
	const digest& operator=(const digest<true, true, false>& other) noexcept
	{
		internal::digest_copy::copy_to_long(*this, other);
		return *this;
	}
	const digest& operator=(const digest<true, true, true>& other) noexcept
	{
		internal::digest_copy::copy_to_long(*this, other);
		return *this;
	}
};

// Digest (alphabet not restricted; short; normalized)
template <>
class digest<false, true, true>
	: public digest_base<false, true, true>
{
public:
	digest(void) noexcept = default; // initialize to undefined state
	digest(const digest& other) noexcept : digest_base<false, true, true>(other) {}
	digest(const digest<true, true, true>& other) noexcept
	{
		internal::digest_copy::copy_to_non_ra(*this, other);
	}
	explicit digest(const char* str) noexcept(false) : digest_base<false, true, true>(str) {}
	explicit digest(const std::string& str) : digest(str.c_str()) {}
	const digest& operator=(const digest& other) noexcept
	{
		digest_base<false, true, true>::operator=(other);
		return *this;
	}
	const digest& operator=(const digest<true, true, true>& other) noexcept
	{
		internal::digest_copy::copy_to_non_ra(*this, other);
		return *this;
	}
};

// Digest (alphabet not restricted; short; unnormalized)
template <>
class digest<false, true, false>
	: public digest_base<false, true, false>
{
public:
	digest(void) noexcept = default; // initialize to undefined state
	digest(const digest& other) noexcept : digest_base<false, true, false>(other) {}
	digest(const digest<false, true, true>& other) noexcept
	{
		digest_data<false, true>::operator=(other);
	}
	digest(const digest<true, true, false>& other) noexcept
	{
		internal::digest_copy::copy_to_non_ra(*this, other);
	}
	digest(const digest<true, true, true>& other) noexcept
	{
		internal::digest_copy::copy_to_non_ra(*this, other);
	}
	explicit digest(const char* str) noexcept(false) : digest_base<false, true, false>(str) {}
	explicit digest(const std::string& str) : digest(str.c_str()) {}
	const digest& operator=(const digest& other) noexcept
	{
		digest_base<false, true, false>::operator=(other);
		return *this;
	}
	const digest& operator=(const digest<false, true, true>& other) noexcept
	{
		digest_data<false, true>::operator=(other);
		return *this;
	}
	const digest& operator=(const digest<true, true, false>& other) noexcept
	{
		internal::digest_copy::copy_to_non_ra(*this, other);
		return *this;
	}
	const digest& operator=(const digest<true, true, true>& other) noexcept
	{
		internal::digest_copy::copy_to_non_ra(*this, other);
		return *this;
	}
};

// Digest (alphabet not restricted; long; normalized)
template <>
class digest<false, false, true>
	: public digest_base<false, false, true>
{
public:
	digest(void) noexcept = default; // initialize to undefined state
	digest(const digest& other) noexcept : digest_base<false, false, true>(other) {}
	digest(const digest<false, true, true>& other) noexcept
	{
		internal::digest_copy::copy_to_long(*this, other);
	}
	digest(const digest<true, false, true>& other) noexcept
	{
		internal::digest_copy::copy_to_non_ra(*this, other);
	}
	digest(const digest<true, true, true>& other) noexcept
	{
		internal::digest_copy::copy_to_long_non_ra(*this, other);
	}
	explicit digest(const char* str) noexcept(false) : digest_base<false, false, true>(str) {}
	explicit digest(const std::string& str) : digest(str.c_str()) {}
	const digest& operator=(const digest& other) noexcept
	{
		digest_base<false, false, true>::operator=(other);
		return *this;
	}
	const digest& operator=(const digest<false, true, true>& other) noexcept
	{
		internal::digest_copy::copy_to_long(*this, other);
		return *this;
	}
	const digest& operator=(const digest<true, false, true>& other) noexcept
	{
		internal::digest_copy::copy_to_non_ra(*this, other);
		return *this;
	}
	const digest& operator=(const digest<true, true, true>& other) noexcept
	{
		internal::digest_copy::copy_to_long_non_ra(*this, other);
		return *this;
	}
};

// Digest (alphabet not restricted; long; unnormalized)
template <>
class digest<false, false, false>
	: public digest_base<false, false, false>
{
public:
	digest(void) noexcept = default; // initialize to undefined state
	digest(const digest& other) noexcept : digest_base<false, false, false>(other) {}
	digest(const digest<false, false, true>& other) noexcept
	{
		digest_data<false, false>::operator=(other);
	}
	digest(const digest<false, true, false>& other) noexcept
	{
		internal::digest_copy::copy_to_long(*this, other);
	}
	digest(const digest<false, true, true>& other) noexcept
	{
		internal::digest_copy::copy_to_long(*this, other);
	}
	digest(const digest<true, false, false>& other) noexcept
	{
		internal::digest_copy::copy_to_non_ra(*this, other);
	}
	digest(const digest<true, false, true>& other) noexcept
	{
		internal::digest_copy::copy_to_non_ra(*this, other);
	}
	digest(const digest<true, true, false>& other) noexcept
	{
		internal::digest_copy::copy_to_long_non_ra(*this, other);
	}
	digest(const digest<true, true, true>& other) noexcept
	{
		internal::digest_copy::copy_to_long_non_ra(*this, other);
	}
	explicit digest(const char* str) noexcept(false) : digest_base<false, false, false>(str) {}
	explicit digest(const std::string& str) : digest(str.c_str()) {}
	const digest& operator=(const digest& other) noexcept
	{
		digest_base<false, false, false>::operator=(other);
		return *this;
	}
	const digest& operator=(const digest<false, false, true>& other) noexcept
	{
		digest_data<false, false>::operator=(other);
		return *this;
	}
	const digest& operator=(const digest<false, true, false>& other) noexcept
	{
		internal::digest_copy::copy_to_long(*this, other);
		return *this;
	}
	const digest& operator=(const digest<false, true, true>& other) noexcept
	{
		internal::digest_copy::copy_to_long(*this, other);
		return *this;
	}
	const digest& operator=(const digest<true, false, false>& other) noexcept
	{
		internal::digest_copy::copy_to_non_ra(*this, other);
		return *this;
	}
	const digest& operator=(const digest<true, false, true>& other) noexcept
	{
		internal::digest_copy::copy_to_non_ra(*this, other);
		return *this;
	}
	const digest& operator=(const digest<true, true, false>& other) noexcept
	{
		internal::digest_copy::copy_to_long_non_ra(*this, other);
		return *this;
	}
	const digest& operator=(const digest<true, true, true>& other) noexcept
	{
		internal::digest_copy::copy_to_long_non_ra(*this, other);
		return *this;
	}
};

// Typedefs for digest specializations
typedef digest<false,  true, true>   digest_t;
typedef digest<false,  true, false>  digest_unorm_t;
typedef digest<false, false, true>   digest_long_t;
typedef digest<false, false, false>  digest_long_unorm_t;
typedef digest< true,  true, true>   digest_ra_t;
typedef digest< true,  true, false>  digest_ra_unorm_t;
typedef digest< true, false, true>   digest_ra_long_t;
typedef digest< true, false, false>  digest_ra_long_unorm_t;



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
	template <bool IsAlphabetRestricted, bool IsShort, bool IsNormalized>
	struct digest_traits<digest_base<IsAlphabetRestricted, IsShort, IsNormalized>>
	{
		static constexpr const bool is_valid = true;
		typedef digest_base<IsAlphabetRestricted, IsShort,  true> norm_type;
		typedef digest_base<IsAlphabetRestricted, IsShort, false> unorm_type;
		typedef digest_base<IsAlphabetRestricted,  true, IsNormalized> short_type;
		typedef digest_base<IsAlphabetRestricted, false, IsNormalized> long_type;
		typedef digest_base< true, IsShort, IsNormalized> ra_type;
		typedef digest_base<false, IsShort, IsNormalized> non_ra_type;
	};
	template <bool IsAlphabetRestricted, bool IsShort, bool IsNormalized>
	struct digest_traits<digest<IsAlphabetRestricted, IsShort, IsNormalized>>
	{
		static constexpr const bool is_valid = true;
		typedef digest<IsAlphabetRestricted, IsShort,  true> norm_type;
		typedef digest<IsAlphabetRestricted, IsShort, false> unorm_type;
		typedef digest<IsAlphabetRestricted,  true, IsNormalized> short_type;
		typedef digest<IsAlphabetRestricted, false, IsNormalized> long_type;
		typedef digest< true, IsShort, IsNormalized> ra_type;
		typedef digest<false, IsShort, IsNormalized> non_ra_type;
	};
	template <typename T>
	struct digest_alt_type_selector
	{
	private:
		typedef digest_traits<typename std::remove_cv<T>::type> traits_type;
		static_assert(traits_type::is_valid, "You must give correct type to retrieve alternative types.");
	public:
		typedef typename type_mod::cv_match<T, typename traits_type::norm_type>::type    norm_type;
		typedef typename type_mod::cv_match<T, typename traits_type::unorm_type>::type   unorm_type;
		typedef typename type_mod::cv_match<T, typename traits_type::short_type>::type   short_type;
		typedef typename type_mod::cv_match<T, typename traits_type::long_type>::type    long_type;
		typedef typename type_mod::cv_match<T, typename traits_type::ra_type>::type      ra_type;
		typedef typename type_mod::cv_match<T, typename traits_type::non_ra_type>::type  non_ra_type;
	};
}
template <typename T> using digest_to_unorm  = typename internal::digest_alt_type_selector<T>::unorm_type;
template <typename T> using digest_to_norm   = typename internal::digest_alt_type_selector<T>::norm_type;
template <typename T> using digest_to_short  = typename internal::digest_alt_type_selector<T>::short_type;
template <typename T> using digest_to_long   = typename internal::digest_alt_type_selector<T>::long_type;
template <typename T> using digest_to_ra     = typename internal::digest_alt_type_selector<T>::ra_type;
template <typename T> using digest_to_non_ra = typename internal::digest_alt_type_selector<T>::non_ra_type;


/*
	We expect following properties for each digest types:

	*	is a trivially default constructible type
	*	is a standard-layout type

	Digest types are:

	*	digest_data<IsAlphabetRestricted, IsShort>
	*	digest<IsAlphabetRestricted, IsShort, true>
	*	digest<IsAlphabetRestricted, IsShort, false>

	We also expect that these types are nearly equivalent.
*/
#ifdef FFUZZYPP_LOCAL_CHK
#error do not define FFUZZYPP_LOCAL_CHK
#endif
#ifdef FFUZZYPP_LOCAL_CHK1
#error do not define FFUZZYPP_LOCAL_CHK1
#endif
#define FFUZZYPP_LOCAL_CHK1(IsAlphabetRestricted, IsShort, IsNormalized) \
	static_assert(std::is_trivially_default_constructible<digest<IsAlphabetRestricted, IsShort, IsNormalized>>::value, \
		"digest<" #IsAlphabetRestricted ", " #IsShort ", " #IsNormalized "> must be a trivially default constructible type."); \
	static_assert(std::is_standard_layout<digest<IsAlphabetRestricted, IsShort, IsNormalized>>::value, \
		"digest<" #IsAlphabetRestricted ", " #IsShort ", " #IsNormalized "> must be a standard-layout type."); \
	static_assert(std::is_base_of<digest_data<IsAlphabetRestricted, IsShort>, digest<IsAlphabetRestricted, IsShort, IsNormalized>>::value, \
		"digest_data<" #IsAlphabetRestricted ", " #IsShort ">, digest<" #IsAlphabetRestricted ", " #IsShort ", true> and " \
		"digest<" #IsAlphabetRestricted ", " #IsShort ", false> must be nearly equivalent."); \
	static_assert(sizeof(digest_data<IsAlphabetRestricted, IsShort>) == sizeof(digest<IsAlphabetRestricted, IsShort, IsNormalized>), \
		"digest_data<" #IsAlphabetRestricted ", " #IsShort ">, digest<" #IsAlphabetRestricted ", " #IsShort ", true> and " \
		"digest<" #IsAlphabetRestricted ", " #IsShort ", false> must be nearly equivalent.")
#define FFUZZYPP_LOCAL_CHK(IsAlphabetRestricted, IsShort) \
	static_assert(std::is_trivially_default_constructible<digest_data<IsAlphabetRestricted, IsShort>>::value, \
		"digest_data<" #IsAlphabetRestricted ", " #IsShort "> must be a trivially default constructible type."); \
	static_assert(std::is_standard_layout<digest_data<IsAlphabetRestricted, IsShort>>::value, \
		"digest_data<" #IsAlphabetRestricted ", " #IsShort "> must be a standard-layout type."); \
	FFUZZYPP_LOCAL_CHK1(IsAlphabetRestricted, IsShort, true); \
	FFUZZYPP_LOCAL_CHK1(IsAlphabetRestricted, IsShort, false)
FFUZZYPP_LOCAL_CHK(true, true);
FFUZZYPP_LOCAL_CHK(true, false);
FFUZZYPP_LOCAL_CHK(false, true);
FFUZZYPP_LOCAL_CHK(false, false);
#undef FFUZZYPP_LOCAL_CHK
#undef FFUZZYPP_LOCAL_CHK1

}


// Specialization of standard hash and swap
namespace std
{
	template <bool IsAlphabetRestricted, bool IsShort, bool IsNormalized>
	struct hash<ffuzzy::digest<IsAlphabetRestricted, IsShort, IsNormalized>>
	{
		size_t operator()(const ffuzzy::digest<IsAlphabetRestricted, IsShort, IsNormalized>& value) const
		{
			return value.hash();
		}
	};
	template <bool IsAlphabetRestricted, bool IsShort, bool IsNormalized>
	inline void swap(
		ffuzzy::digest<IsAlphabetRestricted, IsShort, IsNormalized>& a,
		ffuzzy::digest<IsAlphabetRestricted, IsShort, IsNormalized>& b
	) noexcept
	{
		ffuzzy::digest<IsAlphabetRestricted, IsShort, IsNormalized>::swap(a, b);
	}
}

#endif
