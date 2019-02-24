/*
 * tee-- pipe fitting
 */

#include	<signal.h>
int	aflag;
int	openf[100];

char in[512];


main(argc,argv)
char *argv[];
{
	register i, j;
	int r, n;

	while(argc > 1 && argv[1][0] == '-') {
		switch(argv[1][1]) {
		case 'a':
			aflag++;
			break;

		case 'i':
			signal(SIGINT, SIG_IGN);
			break;

		}
		argv++;
		argc--;
	}

	n = 0;
	while(argc-- > 1) {
		if(aflag) {
			openf[n] = open(argv[1], 1);
			if(openf[n] < 0)
				openf[n] = creat(argv[1], 0666);
			lseek(openf[n], 0L, 2);
		} else
			openf[n] = creat(argv[1], 0666);
		if(openf[n] < 0) {
			puts("tee: cannot open ");
			puts(argv[1]);
			puts("\n");
		} else
			n++;
		argv++;
	}
	openf[n++] = 1;

	for(;;) {
		r = read(0, in, 512);
		if(r <= 0)
			exit(0);
		for(i=0; i<n; i++)
			write(openf[i], in, r);
	}
}

puts(s)
char *s;
{
	while(*s)
		write(2, s++, 1);
}
