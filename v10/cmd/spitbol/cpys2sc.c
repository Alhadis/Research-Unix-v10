
/*
	cpys2sc( cp,scptr,maxlen ) copies a c style string to a spitbol
	scblk.
*/

#include "spitblks.h"

cpys2sc( cp,endchr,scptr,maxlen )

char	*cp;
char	endchr;
struct	scblk	*scptr;
int	maxlen;

{
	int	cnt = 0, j;
	char	chr;

	while (( cnt < maxlen ) && ( ( chr = cp[cnt] ) != endchr )) {
		scptr -> str[cnt++] = chr;
	}

	scptr -> len = cnt;
}
