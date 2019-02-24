/* idiff:  interactive diff */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

#define	HUGE	10000	/* large number of lines */

char	*progname;
char	diffout[100];
char	tempfile[100];
char	*outfile = "idiff.out";

FILE	*efopen(char *, char *);
char	*basename(char *);
void idiff(FILE *f1, FILE *f2, FILE *fin, FILE *fout);
void parse(char *s, int *pfrom1, int *pto1, int *pcmd, int *pfrom2, int *pto2);
void nskip(FILE *fin, int n);
void ncopy(FILE *fin, int n, FILE *fout);
void cleanup(int);

/* these have to be undone in a non-posix world: */

/* int fileno(FILE *); */
#define	S_ISDIR(n)	((n) & S_IFDIR)	/* vaxes */
#define	remove unlink

main(int argc, char *argv[])
{
	FILE *fin, *fout, *f1, *f2;
	char buf[BUFSIZ], diffopts[100], *p;
	struct stat stbuf;

	progname = argv[0];
	buf[0] = 0;
	while (argc > 3 && argv[1][0] == '-') {	/* assume all flags go to diff for now */
		strcat(diffopts, argv[1]);
		strcat(diffopts, " ");
		argv++;
		argc--;
	}
	if (argc != 3) {
		fprintf(stderr, "Usage: idiff [-opts] file1 file2\n");
		exit(1);
	}
	f1 = efopen(argv[1], "r");
	f2 = efopen(argv[2], "r");
	fstat(fileno(f2), &stbuf);
	if (S_ISDIR(stbuf.st_mode)) {
		fclose(f2);
		sprintf(buf, "%s/%s", argv[2], basename(argv[1]));
		f2 = efopen(buf, "r");
	}
	if (signal(SIGINT, SIG_IGN) != SIG_IGN)
		signal(SIGINT, cleanup);
	fout = efopen(outfile, "w");
	tmpnam(diffout);
	sprintf(buf,"diff %s %s %s >%s", diffopts, argv[1], argv[2], diffout);
	system(buf);
	fin = efopen(diffout, "r");
	tmpnam(tempfile);
	idiff(f1, f2, fin, fout);
	remove(diffout);
	printf("%s output in file %s\n", progname, outfile);
	exit(0);
}

void cleanup(int i)
{
	remove(diffout);
	remove(tempfile);
	remove(outfile);
	exit(1);
}

char *basename(char *s)	/* find last component of filename */
{
	static char *p;

	for (p = s+strlen(s)-1; p >= s; p--)
		if (*p == '/')
			return p+1;
	return s;
}

void idiff(FILE *f1, FILE *f2, FILE *fin, FILE *fout)	/* process diffs */
{
	char buf[BUFSIZ], buf2[BUFSIZ];
	FILE *ft;
	int cmd, n, from1, to1, from2, to2, nf1, nf2, done;

	nf1 = nf2 = 0;
	done = 0;
	while (!done && fgets(buf, sizeof buf, fin) != NULL) {
		parse(buf, &from1, &to1, &cmd, &from2, &to2);
		n = to1-from1 + to2-from2 + 1; /* #lines from diff */
		if (cmd == 'c')
			n += 2;
		else if (cmd == 'a')
			from1++;
		else if (cmd == 'd')
			from2++;
		printf("%s", buf);
		while (n-- > 0) {
			fgets(buf, sizeof buf, fin);
			printf("%s", buf);
		}
		for(;;) {
			printf("? ");
			fflush(stdout);
			fgets(buf, sizeof buf, stdin);
			switch (buf[0]) {
			case '2':
				to1 = to2 = HUGE;
				done = 1;
			case '>':
				nskip(f1, to1-nf1);
				ncopy(f2, to2-nf2, fout);
				break;
			case '1':
				to1 = to2 = HUGE;
				done = 1;
			case '<':
				nskip(f2, to2-nf2);
				ncopy(f1, to1-nf1, fout);
				break;
			case 'e':
				ncopy(f1, from1-1-nf1, fout);
				nskip(f2, from2-1-nf2);
				ft = efopen(tempfile, "w");
				ncopy(f1, to1+1-from1, ft);
				fprintf(ft, "---\n");
				ncopy(f2, to2+1-from2, ft);
				fclose(ft);
				sprintf(buf2, "ed %s", tempfile);	
				system(buf2);
				ft = efopen(tempfile, "r");
				ncopy(ft, HUGE, fout);
				fclose(ft);
				break;
			case '!':
				system(buf+1);
				printf("!\n");
				break;
			case 'd':
				nskip(f1, to1-nf1);
				nskip(f2, to2-nf2);
				break;
			default:
				printf("< > d e 1 2 !\n");
				continue;
			}
			break;
		}
		nf1 = to1;
		nf2 = to2;
	}
	ncopy(f1, HUGE, fout);	/* can fail on very long files */
	remove(tempfile);
}

void parse(char *s, int *pfrom1, int *pto1, int *pcmd, int *pfrom2, int *pto2)
{
#define a2i(p) while (isdigit(*s)) p = 10*(p) + *s++ - '0'

	*pfrom1 = *pto1 = *pfrom2 = *pto2 = 0;
	a2i(*pfrom1);
	if (*s == ',') {
		s++;
		a2i(*pto1);
	} else
		*pto1 = *pfrom1;
	*pcmd = *s++;
	a2i(*pfrom2);
	if (*s == ',') {
		s++;
		a2i(*pto2);
	} else
		*pto2 = *pfrom2;
}

void nskip(FILE *fin, int n)	/* skip n lines of file fin */
{
	char buf[BUFSIZ];

	while (n-- > 0)
		fgets(buf, sizeof buf, fin);
}

void ncopy(FILE *fin, int n, FILE *fout)	/* copy n lines from fin to fout */
{
	char buf[BUFSIZ];

	while (n-- > 0) {
		if (fgets(buf, sizeof buf, fin) == NULL)
			return;
		fputs(buf, fout);
	}
}

FILE *efopen(char *file, char *mode)	/* fopen file, die if can't */
{
	FILE *fp;
	extern char *progname;

	if ((fp = fopen(file, mode)) != NULL)
		return fp;
	fprintf(stderr, "%s: can't open file %s mode %s\n",
		progname, file, mode);
	exit(1);
}
