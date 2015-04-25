/*

	ffuzzy++ examples

	compute-hash.cpp
	Fuzzy hash computation program

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

#include <cstdio>
#include <limits>

/*
	This enables compilation without libffuzzypp.a.
	Define this in **one** of the source files to make the linker happy.
*/
#define FFUZZYPP_DECLARATIONS

// the following line will activate assertions.
//#define FFUZZYPP_DEBUG

#include "ffuzzy.hpp"
using namespace ffuzzy;

// the following line will enable constant file size optimization.
#define OPTIMIZE_CONSTANT_FILE_SIZE


int main(int argc, char** argv)
{
	// digest_generator is reset by default.
	digest_generator gen;
	// Use "unnormalized" form not to remove
	// sequences with 4 or more identical characters.
	// You can use "digest_t" and you will get digests without such sequences.
	digest_unorm_t d;
	// Generated digest is always natural.
	// You don't have to give too large sizes.
	char digestbuf[digest_unorm_t::max_natural_chars];
	// File buffer
	static const size_t BUFFER_SIZE = 4096;
	unsigned char filebuf[BUFFER_SIZE];

	// The buffer which contains the format string.
	// Converting to "unsigned" is completely safe for
	// ssdeep-compatible configuration
	// (I mean, unless you haven't modified many parameters).
	static_assert(digest_unorm_t::max_natural_width <= std::numeric_limits<unsigned>::max(),
		"digest_unorm_t::max_natural_width must be small enough.");
	char digestformatbuf[digest_unorm_t::max_natural_width_digits + 9];
	sprintf(digestformatbuf, "%%-%us  %%s\n",
		static_cast<unsigned>(digest_unorm_t::max_natural_width));
	#if 0
	fprintf(stderr, "FORMAT STRING: %s\n", digestformatbuf);
	#endif

	// iterate over all files given
	for (int i = 1; i < argc; i++)
	{
		char* filename = argv[i];
		FILE* fp = fopen(filename, "rb");

		// error when failed to open file
		if (!fp)
		{
			perror(filename);
			return 1;
		}

		#ifdef OPTIMIZE_CONSTANT_FILE_SIZE
		/*
			Retrieve file size (but the file is not seekable, do nothing).
			Note that using off_t is not safe for 32-bit platform.
			Use something much more robust to retrieve file sizes.
		*/
		bool seekable = false;
		off_t filesize;
		if (fseek(fp, 0, SEEK_END) == 0)
		{
			/*
				If seekable, set file size constant for
				digest generator optimization. The generator will work without it
				but using it makes the program significantly faster.
			*/
			seekable = true;
			filesize = ftello(fp);
			if (fseek(fp, 0, SEEK_SET) != 0)
			{
				fprintf(stderr, "%s: could not seek to the beginning.\n", filename);
				fclose(fp);
				return 1;
			}
			/*
				set_file_size_constant fails if:
				*	set_file_size_constant is called multiple times (not happens here)
				*	given file size is too large to optimize
			*/
			if (!gen.set_file_size_constant(filesize))
			{
				fprintf(stderr, "%s: cannot optimize performance for this file.\n", filename);
				fclose(fp);
				return 1;
			}
		}
		else
		{
			#if 0
			fprintf(stderr, "%s: seek operation is not available.\n", filename);
			#endif
		}
		#endif

		// update generator by given file stream and buffer
		if (!gen.update_by_stream<BUFFER_SIZE>(fp, filebuf))
		{
			fprintf(stderr, "%s: failed to update fuzzy hashes.\n", filename);
			fclose(fp);
			return 1;
		}
		fclose(fp);

		/*
			copy_digest will fail if:
			*	The file was too big to process
			*	The file size did not match with file size constant
				(set by set_file_size_constant)
		*/
		if (!gen.copy_digest(d))
		{
			if (gen.is_total_size_clamped())
				fprintf(stderr, "%s: too big to process.\n", filename);
			#ifdef OPTIMIZE_CONSTANT_FILE_SIZE
			else if (seekable && (digest_filesize_t(filesize) != gen.total_size()))
				fprintf(stderr, "%s: file size changed while reading (or arithmetic overflow?)\n", filename);
			#endif
			else
				fprintf(stderr, "%s: failed to copy digest with unknown error.\n", filename);
			return 1;
		}

		/*
			If size of the digest buffer is equal or greater than max_natural_chars,
			copying the string digest with pretty_unsafe function is completely safe.

			However, this function still may fail due to failure of sprintf.
		*/
		if (!d.pretty_unsafe(digestbuf))
		{
			fprintf(stderr, "%s: failed to stringize the digest.\n", filename);
			return 1;
		}
		printf(digestformatbuf, digestbuf, filename);

		// reset the generator if we haven't reached the end
		if (i + 1 != argc)
			gen.reset();
	}
	return 0;
}
