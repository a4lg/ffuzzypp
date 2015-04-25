/*

	ffuzzy++ : C++ implementation of fast fuzzy hasing

	tests/cases/precond/utils/type_modifier.hpp
	Precondition tests for type modifier library

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
#ifndef FFUZZYPP_TESTCASES_PRECOND_UTILS_TYPE_MODIFIER_HPP
#define FFUZZYPP_TESTCASES_PRECOND_UTILS_TYPE_MODIFIER_HPP

#include <type_traits>


/*
	Test Case : cv_selector
*/

namespace type_modifier_tests
{

class UnqualifiedType;
typedef UnqualifiedType AliasOfUnqualifiedType;
class QualifiedStruct {};
enum QualifiedEnum { QualifiedEnumA, QualifiedEnumB, };
typedef const void *ConstVoidPointer;
typedef void *VoidPointer;
typedef void **PointerOfVoidPointer;
typedef void * const *PointerOfVoidConstPointer;
typedef void * volatile *PointerOfVoidVolatilePointer;
typedef void * volatile const *PointerOfVoidConstVolatilePointer;

#define FFUZZYPP_LOCAL_CHK(T) \
	static_assert(is_same<volatile const T, const volatile T>::value, \
		"Basic sanity check failed (volatile const and const volatile must be same)."); \
	static_assert(is_same<T, typename type_mod::cv_selector<T, false, false>::type>::value, \
		"typename type_mod::cv_selector<T, false, false> must be T."); \
	static_assert(is_same<const T, typename type_mod::cv_selector<T, true, false>::type>::value, \
		"typename type_mod::cv_selector<T, true, false> must be const T."); \
	static_assert(is_same<volatile T, typename type_mod::cv_selector<T, false, true>::type>::value, \
		"typename type_mod::cv_selector<T, false, true> must be volatile T."); \
	static_assert(is_same<volatile const T, typename type_mod::cv_selector<T, true, true>::type>::value, \
		"typename type_mod::cv_selector<T, true, true> must be const volatile T.")
// Unqualified types
FFUZZYPP_LOCAL_CHK(UnqualifiedType);
FFUZZYPP_LOCAL_CHK(AliasOfUnqualifiedType);
// Qualified easy types (with no pointer modifiers)
FFUZZYPP_LOCAL_CHK(QualifiedStruct);
FFUZZYPP_LOCAL_CHK(QualifiedEnum);
FFUZZYPP_LOCAL_CHK(int);
// Pointers
static_assert(is_same<void * const volatile, void * volatile const>::value,
	"Basic sanity check failed (void * const volatile and void * volatile const must be same).");
FFUZZYPP_LOCAL_CHK(ConstVoidPointer);
FFUZZYPP_LOCAL_CHK(VoidPointer);
FFUZZYPP_LOCAL_CHK(PointerOfVoidPointer);
FFUZZYPP_LOCAL_CHK(PointerOfVoidConstPointer);
FFUZZYPP_LOCAL_CHK(PointerOfVoidVolatilePointer);
FFUZZYPP_LOCAL_CHK(PointerOfVoidConstVolatilePointer);
#undef FFUZZYPP_LOCAL_CHK
#define FFUZZYPP_LOCAL_CHK(PtrType, IsConst, IsVolatile, Suffix) \
	static_assert(is_same<typename type_mod::cv_selector<PtrType, IsConst, IsVolatile>::type, PtrType Suffix>::value, \
		"cv_selector<" #PtrType ", " #IsConst ", " #IsVolatile "> must be " #PtrType " " #Suffix ".")
#define FFUZZYPP_LOCAL_CHK1(PtrType) \
	FFUZZYPP_LOCAL_CHK(PtrType, false, false, ); \
	FFUZZYPP_LOCAL_CHK(PtrType, true, false, const); \
	FFUZZYPP_LOCAL_CHK(PtrType, false, true, volatile); \
	FFUZZYPP_LOCAL_CHK(PtrType, true, true, const volatile)
FFUZZYPP_LOCAL_CHK1(void *);
FFUZZYPP_LOCAL_CHK1(const void *);
FFUZZYPP_LOCAL_CHK1(volatile void *);
FFUZZYPP_LOCAL_CHK1(const volatile void *);
FFUZZYPP_LOCAL_CHK1(void * *);
FFUZZYPP_LOCAL_CHK1(void * const *);
FFUZZYPP_LOCAL_CHK1(void * volatile *);
FFUZZYPP_LOCAL_CHK1(void * const volatile *);
FFUZZYPP_LOCAL_CHK1(const void * *);
FFUZZYPP_LOCAL_CHK1(const void * const *);
FFUZZYPP_LOCAL_CHK1(const void * volatile *);
FFUZZYPP_LOCAL_CHK1(const void * const volatile *);
FFUZZYPP_LOCAL_CHK1(volatile void * *);
FFUZZYPP_LOCAL_CHK1(volatile void * const *);
FFUZZYPP_LOCAL_CHK1(volatile void * volatile *);
FFUZZYPP_LOCAL_CHK1(volatile void * const volatile *);
FFUZZYPP_LOCAL_CHK1(const volatile void * *);
FFUZZYPP_LOCAL_CHK1(const volatile void * const *);
FFUZZYPP_LOCAL_CHK1(const volatile void * volatile *);
FFUZZYPP_LOCAL_CHK1(const volatile void * const volatile *);
#undef FFUZZYPP_LOCAL_CHK1
#undef FFUZZYPP_LOCAL_CHK


/*
	Test Case : cv_match
*/
#define FFUZZYPP_LOCAL_CHK(T, Tmatch, Texpected) \
	static_assert(is_same<typename type_mod::cv_match<T, Tmatch>::type, Texpected>::value, \
		#Tmatch " with cv-modifiers from " #T " must be " #Texpected ".")
#define FFUZZYPP_LOCAL_CHK1(T, Tmatch) \
	FFUZZYPP_LOCAL_CHK(T, Tmatch, Tmatch); \
	FFUZZYPP_LOCAL_CHK(const T, Tmatch, const Tmatch); \
	FFUZZYPP_LOCAL_CHK(volatile T, Tmatch, volatile Tmatch); \
	FFUZZYPP_LOCAL_CHK(volatile const T, Tmatch, const volatile Tmatch)
FFUZZYPP_LOCAL_CHK1(char, int);
FFUZZYPP_LOCAL_CHK1(char, UnqualifiedType);
FFUZZYPP_LOCAL_CHK1(char, AliasOfUnqualifiedType);
FFUZZYPP_LOCAL_CHK1(char, QualifiedStruct);
FFUZZYPP_LOCAL_CHK1(char, QualifiedEnum);
#undef FFUZZYPP_LOCAL_CHK1
FFUZZYPP_LOCAL_CHK(const void *, int, int);
FFUZZYPP_LOCAL_CHK(void * const, int, const int);
FFUZZYPP_LOCAL_CHK(volatile void *, int, int);
FFUZZYPP_LOCAL_CHK(void * volatile, int, volatile int);
FFUZZYPP_LOCAL_CHK(int, void *, void *);
FFUZZYPP_LOCAL_CHK(const int, void *, void * const);
FFUZZYPP_LOCAL_CHK(volatile int, void *, void * volatile);
FFUZZYPP_LOCAL_CHK(const volatile int, void *, void * const volatile);
#undef FFUZZYPP_LOCAL_CHK


}

#endif
