#include <stdio.h>

/* For netlib's pipe f2c: check first line of input for flags:
 * If the line has the form
 *	c$f2c -flag -flag...
 * (with "c$f2c " starting in column 1) and no funny characters
 * in the flags (all of which start with -), then echo "-flag -flag..."
 * on stdout; otherwise echo nothing.
 */

main(argc,argv)
 char **argv;
{
	FILE *f;
	char buf[128];
	static char good[256];
	int i;
	register char *s, *s1;

	if (argc <= 1) {
		fprintf(stderr, "%s: expected one arg: a file name\n", *argv);
		return 1;
		}
	if (!(f = fopen(argv[1],"r"))) {
		fprintf(stderr, "%s: can't open %s\n", argv[0], argv[1]);
		return 1;
		}
	if (fgets(buf, sizeof(buf), f)) {
		if (strncmp(buf, "c$f2c ", 6))
			goto done;
		for(i = 'a'; i <= 'z'; i++)
			good[i] = good[i+'A'-'a'] = 1;
		for(i = '0'; i <= '9'; i++)
			good[i] = 1;
		for(s = "\t -+=,.!%_\n"; *s; s++)
			good[*s] = 1;
		for(s = buf+6; good[*(unsigned char *)s]; s++);
		if (*s)
			goto done;
		for(s = buf+5, s1 = 0;;) {
			while(*++s == ' ');
			if (*s != '-') {
				if (!*s)
					break;
				goto done;
				}
			if (!s1)
				s1 = s;
			while(*++s != ' ')
				if (!*s)
					goto for_done;
			}
 for_done:
		if (s1)
			printf("%s", s1);
		}
 done:
	fclose(f);
	return 0;
	}
