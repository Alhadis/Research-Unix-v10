#include <stdio.h>

int	dbg;
int	jpflag;

main(argc, argv)
char *argv[];
{
	FILE *pfp, *f, *popen();
	int c;
	char buf[100], fname[100], *whichjerq, cmd[100];
	char *getenv();

	whichjerq = getenv("JERQ");
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
		system("run /usr/bwk/pic/jp");
	sprintf(cmd, "/usr/bwk/pic/pic >/dev/%s", whichjerq);
	if ((pfp = popen(cmd, "w")) == NULL) {
		fprintf(stderr, "epic: can't open pic\n");
		exit(2);
	}
	while (fgets(buf, sizeof buf, stdin) != NULL) {
		switch (buf[0]) {
		case 'q':
			exit(0);
			break;
		case '\n':
		case 'p':
			f = fopen(fname, "r");
			while ((c = getc(f)) != EOF)
				putc(c, pfp);
			fflush(pfp);
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
