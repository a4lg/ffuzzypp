ffuzzy++ : C++ implementation of ssdeep-compatible fast fuzzy hashing
======================================================================



What is ffuzzy++?
------------------

ffuzzy++ is a fuzzy hash implementation compatible with
ssdeep version 2.13 (<http://ssdeep.sourceforge.net/>).
Using this implementation, you can:

*	Generate fuzzy digests from files and buffers
*	Compare fuzzy digests generated from ffuzzy++ or
	other ssdeep-compatible implementations

This implementation is designed to be fast, stable and thread-safe.
Sometimes this is about 60% to 120% faster than ssdeep when you
cluster digests and is about 10% to 60% faster than ssdeep when
you generate digests.

However, this C++ implementation is primarily designed to be fast
by exposing internal structure of fuzzy hashing. So this
implementation may not be safe as you think (depends on function
you use). Instead, you can boost your clustering application
using efficient digest utilities.



Requirements
-------------

You will need C++ compiler with full C++11 support.

You will also need GNU Autoconf Archive when you build `configure`
script (<https://www.gnu.org/software/autoconf-archive/>).

If you wish to run the test, Google C++ Testing Framework
(<https://code.google.com/p/googletest/>) will be needed.



Installation
-------------

This program can be installed without proper "installation".
Just copy `ffuzzy.hpp` and `ffuzzypp` directory to anywhere you want.
In this case, you need to define `FFUZZYPP_DECLARATIONS` before 
`ffuzzy.hpp` in **one** of the source files to make the linker happy.

You can compile and install a static helper library `libffuzzypp.a`
by running `./configure && make && make install`.
If you don't have `configure` script on the top directory, you need
to run `./bootstrap.sh` to make build files.



Configuration Options
----------------------

*	--disable-static-lib  
	Disable building `libffuzzypp.a`
*	--disable-debug  
	Disable assertions on examples and tests.
	Don't disable assertions if you wish to test ffuzzy++.
*	--disable-position-array  
	Disable using bit-parallel algorithms on examples and tests.
*	--enable-examples  
	Enable building examples (see "Building Examples" section)
*	--enable-tests  
	Enable building test cases (see "Running Tests" section)
*	--disable-compatibility-tests  
	Ignore ssdeep-specific parameters during compilation of
	test cases. This option prevents compiling compatibility tests.



Building Examples
------------------

There are two examples:

*	examples/compare-hash  
	You can use this program to compare two fuzzy digests.
*	examples/compute-hash  
	You can use this program to compute fuzzy digests from
	given files.



Running Tests
--------------

By default, compatibility tests (which depends on ssdeep-specific
parameters) are enabled and you can disable these tests by adding
`--disable-compatibility-tests` configuration option.

*	tests/test-small  
	Relatively small tests (that can be done in a few minutes).
*	tests/test-compatibility-small  
	Compatibility tests (depends on ssdeep parameters)
*	tests/test-compatibility-large  
	Large compatibility tests (depends on ssdeep parameters)
*	tests/test-precond  
	Precondition tests (it always succeeds at runtime because
	all "tests" are performed while compiling this executable).



Configuration
--------------

Defining certain macros before including `ffuzzy.hpp` can change some
behavior of this library.

*	`FFUZZYPP_DEBUG`  
	This macro enables assertions for debugging.
*	`FFUZZYPP_DISABLE_POSITION_ARRAY`  
	This macro disables using bit-parallel algorithms.



Caution
--------

*	This package is intended for static-linking to your program.
*	Packaging this implementation **alone** for OS distribution
	is not recommended.  The interface may change without notice.
*	Digests generated from files equal to or larger than 4GiB
	are not portable enough. See following secions for details.



Differences between ssdeep
---------------------------

When comparing identical digests, ssdeep 2.9 and 2.13 return different
values. By default, ffuzzy++ emulates behavior of version 2.13.
If you give `comparison_version::v2_9` to `comparison`-related function
as a template parameter, ffuzzy++ will emulate behavior of version 2.9.

Result of comparison is not in a signed type but in an unsigned type
because error handling things are moved out of comparison.
So comparison functions cannot return -1 on error (like ssdeep/libfuzzy).
Instead, digest parser (which will be used before comparison) will raise
an exception when it fails to parse digest string.



Differences between ssdeep -2.12 (DETAILS)
-------------------------------------------

Because of current implementation restrictions in ssdeep 2.12 and before
(not in ffuzzy++), the digest and comparison result may different when:

*	Extremely large files are given (for example):
	*	For files equal to or larger than 4GiB,  
		ssdeep 2.10-2.12 on 32-bit platform and ssdeep -2.9
		causes arithmetic overflow while handling file sizes.
		This will result in unexpected behavior.
	*	For files larger than 4GiB,  
		ssdeep -2.12's rolling hash implementation is not exactly
		rolling. This will result in incorrect digest value.
	*	For files larger than 96GiB,  
		ssdeep 2.10-2.12 will crash under some circumstances.
		ssdeep 2.9 with 64-bit file size patch will use incorrect value
		(because of arithmetic overflow) when generating digests.  
		ffuzzy++'s implementation matches mathematical properties
		of ssdeep digest generation algorithm while preventing
		arithmetic overflow bug on ssdeep 2.9 with
		64-bit file size patch.
	*	For files equal to or larger than 2^64 bytes,  
		ssdeep 2.10-2.12 on 64-bit platform will cause arithmetic
		overflow on file sizes. Although we don't accept
		files larger than 192GiB (see "Limitations" section), ssdeep
		-2.12 doesn't have mechanism to prevent file size overflow.
*	Some digests generated from extremely large files are given:
	*	For digests generated from files larger than 12GiB,  
		ssdeep -2.12 may cause arithmetic overflow while comparing
		digests. This may make incorrect comparison score of zero.

Overrall, ffuzzy++ is almost compatible with ssdeep -2.12. But on some
circumstances which ssdeep will fail or did not support (yes, actually
files equal to or larger than 4GiB are not officially supported by
ssdeep -2.12), ffuzzy++ returns mostly legit values
(based on mathematical properties of ssdeep).



Differences between ssdeep 2.13
--------------------------------

ssdeep 2.13 behaves very similar to ffuzzy++ due to bug fixes.
There are few implementation differences but behaves the same when:

*	Generating fuzzy digests from arbitrary input
*	Comparing digests generated by new implementations
*	Comparing digests generated from files up to 96GiB
	by previous ssdeep versions



Limitations
------------

Both implementations (ssdeep and ffuzzy++) have a limit of 192GiB in
digest generator. Based on mathematical properties of ssdeep,
we **could** accept much larger files. However, digests generated
from such files are nearly useless (because resulting digests will
not represent signature of whole file contents).

So I decided not to accept files larger than 192GiB on ffuzzy++
as well as ssdeep 2.10+.

Although ffuzzy++ implementation is completely platform-independent,
ssdeep digest for files equal to or larger than 4GiB are still not
portable enough (varies by ssdeep versions and platforms).

Make sure that you use new implementations (ssdeep 2.13 or ffuzzy++)
when you accept extremely large files. If you use older versions of
ssdeep, please consider rejecting such big files (equal to or larger
than 4GiB) when you generate digests.



License / Copying
------------------

See `COPYING.md` file for details.
