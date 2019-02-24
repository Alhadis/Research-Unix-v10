#include <stdio.h>
#define MANUAL "spitv35.man"

main()
{
	register int c, linecnt;
	register FILE *f;

	f = fopen (MANUAL, "r");
	if (f == NULL) {
		perror (MANUAL);
		exit (1);
	}

	linecnt = 0;
	while ((c = getc (f)) != EOF) {
		putchar (c);
		if (c == '\n') {
			linecnt++;
			if (linecnt == 62) {
				linecnt = 0;
				putchar ('\f');
			}
		}
		if (c == '\r')
			getc (f);
	}
}
