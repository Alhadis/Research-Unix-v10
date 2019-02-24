/*
 *	Routines for Doing Alphanumeric Input/Output (the -a option)
 *	D.P.Mitchell  83/07/01.
 */

#include <stdio.h>
#define CHAR1	'#'

extern int permutation[];
char trans[66] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz./";
int untrans[] = {
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 62, 63, 
26, 27, 28, 29, 30, 31, 32, 33, 34, 35,  0,  0,  0,  0,  0,  0, 
 0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 
15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  0,  0,  0,  0,  0, 
 0, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 
51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,  0,  0,  0,  0,  0, 
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
};

out_alpha(c_buffer)
int c_buffer[128];
{
	int six[171];
	register c0, c1, c2;
	register src, dst;

	for (src = 0, dst = 0; src < 128;) {
		c0 = c_buffer[permutation[src++]];
		c1 = c_buffer[permutation[src++]];
		if (src < 128)
			c2 = c_buffer[permutation[src++]];
		else
			c2 = 0;
		six[dst++] = trans[(c0 >> 2) & 077];
		six[dst++] = trans[((c0 & 03) << 4) | ((c1 >> 4) & 017)];
		six[dst++] = trans[((c1 & 017) << 2) | ((c2 >> 6) & 03)];
		if (dst < 171)
			six[dst++] = trans[c2 & 077];
	}
	putchar(CHAR1);
	for (src = 0; src < 57; src++)
		putchar(six[src]);
	putchar('\n');
	putchar(CHAR1);
	for (; src < 114; src++)
		putchar(six[src]);
	putchar('\n');
	putchar(CHAR1);
	for (; src < 171; src++)
		putchar(six[src]);
	putchar('\n');
}

int
in_alpha(c_buffer)
int c_buffer[128];
{
	int six[171];
	register c0, c1, c2, c3;
	register src, dst;
	char line[64];
	extern char *fgets();

	for (dst = 0; dst < 171; ) {
		do {
			if (fgets(line, 64, stdin) == NULL)
				return 0;
		} while (line[0] != CHAR1);
		for (src = 1; src < 58; src++)
			six[dst++] = line[src];
	}
	for (src = 0, dst = 0; src < 171; ) {
		c0 = untrans[six[src++]];
		c1 = untrans[six[src++]];
		c2 = untrans[six[src++]];
		if (src < 171)
			c3 = untrans[six[src++]];
		else
			c3 = 0;
		c_buffer[permutation[dst++]] = (c0 << 2) | (c1 >> 4);
		c_buffer[permutation[dst++]] = ((c1 & 017) << 4) | (c2 >> 2);
		if (dst < 128)
			c_buffer[permutation[dst++]] = ((c2 & 03) << 6) | c3;
	}
	return 1;
}
