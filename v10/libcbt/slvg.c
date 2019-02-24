#include "stdio.h"
/* change the output of btsalvage or btcat -R to sortable */
/* an 8 byte sortable sequence number is appended to each key
 * that slvg2 can keep only the last record with a given key */

char *buf;
int buflen;
long reccnt;
extern char *malloc(), *realloc();

main()
{	int n, i, j;
	for(;;) {
		if(read(0, &n, 2) == 0)
			exit(0);
		if(n > buflen)
			space(n);
		read(0, buf, n);
		reccnt++;
		out(buf, n, '\t');
		countout();
		read(0, &n, 2);
		if(n > buflen)
			space(n);
		read(0, buf, n);
		j = n;
		for(i = 0; i < 4; i++) {
			putchar('a' + (j & 0xf));
			j >>= 4;
		}
		out(buf, n, '\n');
	}
}

space(n)
{
	while(n > buflen) {
		if(buflen == 0)
			buf = malloc(buflen = 1024);
		else
			buf = realloc(buf, buflen += 1024);
		if(buf == 0) {
			fprintf(stderr, "buf[%d]failed\n", buflen);
			exit(1);
		}
		
	}
}

out(s, n, c)
char *s;
{
	while(n-- > 0) {
		putchar('3' + ((*s >> 6) & 3));
		putchar((*s & 077) + ' ');
		s++;
	}
	putchar(c);
}

countout()
{	int i, n;
	n = reccnt;
	for(i = 7; i >= 0; i--)
		putchar('0' + ((n >> (4*i)) & 077));
}
