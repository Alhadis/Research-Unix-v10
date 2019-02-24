#include <stdio.h>

int nchin; char inbuf[256];

main(argc, argv)
char **argv;
{
	static char outbuf[BUFSIZ];
	setbuf(stdout, outbuf);
	thinksort(0);
	exit(0);
}

char *
getbuf()
{
	nchin = read(0, inbuf, sizeof inbuf);
	return (nchin > 0) ? inbuf : 0;
}

wrtc(c) { putchar(c); }
wrts(s) char *s; { fputs(s, stdout); }
wrtflush() { fflush(stdout); }
