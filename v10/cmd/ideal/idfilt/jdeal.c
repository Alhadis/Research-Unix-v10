#include <stdio.h>

int	dbg;
int	jpflag;
	char foo;

main(argc, argv)
char *argv[];
{
	FILE *f, *popen();
	FILE *tojerq, *fromjerq;
	int c;
	char buf[100], fname[100];

	while (argc > 1 && argv[1][0] == '-') {
		switch (argv[1][1]) {
		case 'd':
			dbg = 1;
			break;
		case 'j':
			jpflag = 1;
			break;
		}
		argc--;
		argv++;
	}
	if (argc > 1)
		strcpy(fname, argv[1]);
	if (jpflag == 0)
		system("run /usr/cvw/ideal/idfilt/iderq");
/*
	if ((pfp = popen("/usr/cvw/ideal/ideal -n >/dev/tty5", "w")) == NULL) {
		fprintf(stderr, "jdeal: can't open ideal\n");
		exit(2);
	}
*/
	if ((tojerq = fopen ("/dev/tty5", "w")) == NULL) {
		fprintf (stderr, "jdeal: can't talk to jerq\n");
		exit(2);
	}
	if ((fromjerq = fopen ("/dev/tty5", "r")) == NULL) {
		fprintf (stderr, "jdeal: can't hear jerq\n");
		exit (3);
	}
	while (fgets(buf, sizeof buf, stdin) != NULL) {
		switch (buf[0]) {
		case 'q':
			exit(0);
			break;
		case '\n':
		case 'p':
			f = fopen(fname, "r");
fprintf (stderr, "got here\n");
			while (((foo = getc(fromjerq)) != EOF) && ((c = getc(f)) != EOF)) {
fprintf (stderr, "%c", c);
				putc(c, tojerq);
			}
			fflush(tojerq);
			fclose(f);
			break;
		case 'e':
			if (sscanf(buf, "%*s %s", fname) > 0)
				printf("editing file %s\n", fname);
			else
				sprintf(buf, "e %s", fname);
			system(buf);
			break;
		case 'f':
			sscanf(buf, "%*s %s", fname);
			printf("%s\n", fname);
			break;
		case '!':
			system(buf+1);
			printf("!\n");
			break;
		default:
			printf("eh?\n");
			break;
		}
	}
}
