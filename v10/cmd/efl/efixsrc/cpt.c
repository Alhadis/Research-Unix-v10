/* Set time of second argument to that of first. */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
main(argc, argv)
int argc;
char **argv;
{
	struct stat buf;
	time_t timep[2];
	char *progname = argv[0], *s;
	int rc = 0;

	if (argc < 3) {
		fprintf(stderr,
	"usage: %s f1 f2 [f3 ...]\n\tto copy times from f1 to f2 [f3 ...]\n",
			progname);
		exit(1);
		}
	s = *++argv;
	if (stat(s, &buf) != 0) {
		fprintf(stderr, "%s: can't stat %s!\n", progname, s);
		exit(1);
		}
	timep[0] = buf.st_atime;
	timep[1] = buf.st_mtime;
	while(s = *++argv)
		if (utime(s, timep) != 0) {
			fprintf(stderr, "%s: can't set time of %s!\n",
				progname, s);
			++rc;
			}
	exit(rc);
	}
