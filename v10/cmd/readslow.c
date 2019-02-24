#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main(argc, argv)
char **argv;
{	int i, f=0;
	int seekf = 0;
	char buf[256];
	long amtread, lseek();
	struct stat sbuf;

	if (argc>1 && strcmp(argv[1], "-e") == 0) {
		seekf = 1;
		argc--;
		argv++;
	}
	if (argc>1) {
		if ((f = open(argv[1], 0)) < 0) {
			fprintf(stderr, "readslow: can't open %s\n", argv[1]);
			exit(1);
		}
	}
	amtread = 0;
	if (seekf)
		amtread = lseek(f, 0L, 2);
	for(;;) {
		if((i=read(f,buf,256))>0) {
			write(1,buf,i);
			amtread += i;
		} else if (i==0) {
			sleep(10);
			fstat(f, &sbuf);
			if (sbuf.st_size < amtread) {
				printf("\n<<< file truncated -- restarting >>>\n");
				lseek(f, 0L, 0);
				amtread = 0;
				continue;
			}
		} else
			exit(1);
	}
}
