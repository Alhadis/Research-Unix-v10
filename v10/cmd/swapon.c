#include <stdio.h>
#include <fstab.h>

main(argc, argv)
	int argc;
	char *argv[];
{
	int stat = 0;

	--argc, argv++;
	if (argc == 0) {
		fprintf(stderr, "usage: swapon name...\n");
		exit(1);
	}
	if (argc == 1 && !strcmp(*argv, "-a")) {
		struct	fstab	*fsp;
		if (setfsent() == 0)
			perror(FSTAB), exit(1);
		while ((fsp = getfsent()) != 0){
			if (fsp->fs_ftype != FSSWAP)
				continue;
			printf("Adding %s as swap device\n",
			    fsp->fs_spec);
			fflush(stdout);
			if (swapon(fsp->fs_spec) < 0) {
				stat = 1;
				perror(fsp->fs_spec);
			}
		}
		endfsent();
		exit(stat);
	}
	do {
		if (swapon(*argv) < 0) {
			stat = 1;
			perror(*argv);
		}
		argc--;
		argv++;
	} while (argc > 0);
	exit(stat);
}
