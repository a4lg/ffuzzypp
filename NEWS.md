ffuzzy++ : C++ implementation of fast fuzzy hashing
====================================================


Version 3.1 - 2017-06-28
-------------------------

*	NEW: Bit-parallelism  
	(clustering can be over 5 times faster than version 3.0)
*	Digest generator is optimized  
	(can be twice as fast as version 3.0)
*	Utility methods are added

Changes in version 4.0 are large enough to break source compatibility.
This release also breaks it if you use internal string classes but
otherwise version 3.1 is (relatively) unchanged.


Version 3.0.1 - 2017-06-04
---------------------------

*	Uses of deprecates type trait are now replaced with decent ones.


Version 3.0 - 2015-04-25
-------------------------

*	The initial public release
