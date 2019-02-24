#include <sys/types.h>
#include <sys/stat.h>

/*
 *	newer x y
 *
 *	returns 0 if x exists and y does not, or if
 *	files x and y both exist and x was modified
 *	at least as recently as y, and nonzero otherwise.
 */

main (argc, argv)
	int argc;
	register int **argv;
{
	struct stat x, y;

	/* insist on exactly two args */
	if (argc != 3)
		return 1;
	
	/* does the first file exist? */
	if (stat (argv[1], &x) < 0)
		return 1;
	
	/* does the second file exist? */
	if (stat (argv[2], &y) < 0)
		return 0;
	
	/* fail if the first file is older than the second */
	if (x.st_mtime < y.st_mtime)
		return 1;
	
	/* otherwise, succeed */
	return 0;
}
