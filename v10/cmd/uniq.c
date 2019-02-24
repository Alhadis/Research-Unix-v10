/*
 * Deal with duplicated lines in a file
 */
#include <stdio.h>
#include <ctype.h>
#define	SIZE	1000
int	fields	= 0;
int	letters	= 0;
int	linec	= 0;
char	mode;
int	uniq;
char	*skip();

main(argc, argv)
int argc;
char *argv[];
{
	static char b1[SIZE], b2[SIZE];

	while(argc > 1) {
		if(*argv[1] == '-') {
			if (isdigit(argv[1][1]))
				fields = atoi(&argv[1][1]);
			else mode = argv[1][1];
			argc--;
			argv++;
			continue;
		}
		if(*argv[1] == '+') {
			letters = atoi(&argv[1][1]);
			argc--;
			argv++;
			continue;
		}
		if (freopen(argv[1], "r", stdin) == NULL) {
			fprintf(stderr, "cannot open %s\n", argv[1]);
			exit(1);
		}
		break;
	}
	if(argc > 2) {
		fprintf(stderr, "unexpected argument %s\n", argv[2]);
		exit(1);
	}

	if(gline(b1))
		exit(0);
	for(;;) {
		linec++;
		if(gline(b2)) {
			pline(b1);
			exit(0);
		}
		if(!equal(b1, b2)) {
			pline(b1);
			linec = 0;
			do {
				linec++;
				if(gline(b1)) {
					pline(b2);
					exit(0);
				}
			} while(equal(b1, b2));
			pline(b2);
			linec = 0;
		}
	}
}

gline(buf)
register char buf[];
{
	register c, n;

	n = 0;
	while((c = getchar()) != '\n') {
		if(c == 0)
			continue;
		if(c == EOF)
			return(1);
		*buf++ = c;
		n++;
		if(n >= SIZE-1) {
			fprintf(stderr, "line too long\n");
			exit(1);
		}
	}
	*buf = 0;
	return(0);
}

pline(buf)
register char buf[];
{

	switch(mode) {

	case 'u':
		if(uniq) {
			uniq = 0;
			return;
		}
		break;

	case 'd':
		if(uniq) break;
		return;

	case 'c':
		printf("%4d ", linec);
	}
	uniq = 0;
	fputs(buf, stdout);
	putchar('\n');
}

equal(b1, b2)
register char b1[], b2[];
{
	register char c;

	b1 = skip(b1);
	b2 = skip(b2);
	while((c = *b1++) != 0)
		if(c != *b2++) return(0);
	if(*b2 != 0)
		return(0);
	uniq++;
	return(1);
}

char *
skip(s)
register char *s;
{
	register nf, nl;

	nf = nl = 0;
	while(nf++ < fields) {
		while(*s == ' ' || *s == '\t')
			s++;
		while( !(*s == ' ' || *s == '\t' || *s == 0) ) 
			s++;
	}
	while(nl++ < letters && *s != 0) 
			s++;
	return(s);
}
