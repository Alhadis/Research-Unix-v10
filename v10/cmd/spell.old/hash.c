/*	@(#)hash.c	1.1	*/
#include "hash.h"

#define CHARWIDTH 6
#define SHIFT (2*CHARWIDTH)

/* The hash function is first a substitution cipher into CHARWIDTH-bit
 * "bytes".  Then the string of such "bytes" is interpreted
 * as a huge integer, and taken mod two primes small enough to
 * avoid overflows.  Finally the two results are concatenated.
 * Unhashable trash produces an otherwise impossible value; it
 * would be prudent not to have that value in the dictionary.
*/
#define LOGNC 5
#define NC (1<<LOGNC)
#define MINCHAR ' '
#define MAXCHAR 0177

long pow2[NC*2];

static char hashtab[] = {
-1,	-1,	-1,	-1,	-1,	-1,	0,	31,	/*  &' */
-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,
2,	25,	20,	35,	54,	61,	40,	39,	/* 0-7 */
42,	33,	-1,	-1,	-1,	-1,	-1,	-1,
-1,	60,	43,	30,	5,	16,	47,	18,	/* A-G */
41,	36,	51,	6,	13,	56,	55,	58,
49,	12,	59,	46,	21,	32,	63,	34,
57,	52,	3,	-1,	-1,	-1,	-1,	-1,
-1,	22,	29,	8,	7,	10,	1,	28,	/* a-g */
11,	62,	37,	48,	15,	50,	9,	4,
19,	38,	45,	24,	23,	26,	17,	44,
27,	14,	53,	-1,	-1,	-1,	-1,	-1
};

long hash(s)
register char *s;
{
	register c, d;
	register long *lp,*pow2_nc;
	long h = 0;
	long g = 0;

	pow2_nc = pow2+2*NC;
	for(lp=pow2; (c = *s++) && lp<pow2_nc; ) {
		if(c<MINCHAR || c>MAXCHAR ||
		  (c=hashtab[c-MINCHAR])==(char)-1)
			return (1L<<HASHWIDTH) - 1;	/*trash value*/
		if(d = *s&0177) {
			s++;
			if(d<MINCHAR || d>MAXCHAR ||
			  (d=hashtab[d-MINCHAR])==(char)-1)
				return (1L<<HASHWIDTH) - 1;
			c |= d<<CHARWIDTH;
		}
		h += c * *lp++;
		g += c * *lp++;
	}
	return h%PRIME1 | ((g%PRIME2)<<WIDTH1);
}

hashinit()
{
	register i;
	if(WIDTH1+SHIFT+LOGNC >= BYTE*sizeof(long) ||	/*hashing risks overflow*/
	   WIDTH2+SHIFT+LOGNC >= BYTE*sizeof(long) ||
	   sizeof(hashtab) != MAXCHAR-MINCHAR+1)		/*incorrect hashtab*/
		abort();
	pow2[0] = 1;
	pow2[1] = 8;	/* historical dreg, when rebuilding make pow2[1]=1*/
	for(i=0; i<2*NC-3; i+=2) {
		pow2[i+2] = (pow2[i]<<SHIFT) % PRIME1;
		pow2[i+3] = (pow2[i+1]<<SHIFT) % PRIME2;
	}
}
