/*
 * compile with:
 *
 *      g++ -g -Wall embed.cpp `pkg-config vips-cpp --cflags --libs`
 *
 */

#define DEBUG

#include <vips/vips8.h>

using namespace vips;


#if defined(BUILD_MONOLITHIC)
#define main       vips_example_embed_main
#endif

int
main( int argc, const char **argv )
{
	if (vips_init(argv[0]))
		vips_error_exit(NULL);

	VImage in = VImage::new_from_file(argv[1],
		VImage::option()->set("access", "sequential"));

	VImage out = in.embed(10, 10, 1000, 1000,
		VImage::option()->set("extend", "copy"));

	out.write_to_file(argv[2]);

	vips_shutdown();

	return 0;
}
