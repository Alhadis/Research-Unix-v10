#include <sys/filio.h>
#include <sys/dkio.h>
#include <ctype.h>

char *netfiles = "/dev/dk/dk";

main(argc, argv)
	int argc;
	char *argv[];
{
	register fd;
	register r;
	int unit = 'a';

	while (argc > 1) {
		if (strcmp(argv[1], "-t") == 0)
			;
		else if (isdigit(*argv[1]))
			unit = *argv[1];
		else if (strcmp(argv[1], "-N") == 0) {
			netfiles = argv[2];
			argv++;
			argc--;
		}
		argc--;
		argv++;
	}
	fd = dkctlchan(2, unit);
	if (fd < 0) {
		printf("Can't open ctlchan to kmc\n");
		exit(1);
	}
	r = ioctl(fd, KIOCSHUT, (char *)0);
	if (r < 0)
		printf("Couldn't shut down kdi\n");
	return(0);
}
