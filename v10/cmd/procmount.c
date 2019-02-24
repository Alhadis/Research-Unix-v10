#include <stdio.h>

int mntfstyp = 2;
char mntdir[] = "/proc";

main(argc, argv)
char **argv;
{
	register flag, n;

	if (argc > 1) {
		if (funmount(mntdir) < 0) {
			perror(mntdir);
			exit(1);
		}
	} else {
		if (fmount(mntfstyp, 0, mntdir, 0) < 0) {
			perror(mntdir);
			exit(1);
		}
	}
	printf("%s %s\n", mntdir, argc > 1 ? "unmounted" : "mounted");
	exit(0);
}
