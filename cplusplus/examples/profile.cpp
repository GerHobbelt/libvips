/*
 * compile with:
 *
 *      g++ -g -Wall profile.cpp `pkg-config vips-cpp --cflags --libs`
 *
 */

#define DEBUG

#include <vips/vips8.h>

using namespace vips;


#if defined(BUILD_MONOLITHIC)
#define main       vips_example_profile_main
#endif

int
main( int argc, const char **argv )
{
	if (vips_init(argv[0]))
		vips_error_exit(NULL);

	VImage in = VImage::new_from_file(argv[1]);

	VImage rows;
	VImage cols = in.profile(&rows);

	rows.write_to_file(argv[2]);
	cols.write_to_file(argv[3]);

	vips_shutdown();

	return 0;
}
