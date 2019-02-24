/*	/sccs/src/cmd/uucp/s.strpbrk.c
	strpbrk.c	1.1	8/30/84 17:37:42
*/
#include "uucp.h"
VERSION(@(#)strpbrk.c	1.1);

/*
 * Return ptr to first occurance of any character from `brkset'
 * in the character string `string'; NULL if none exists.
 */

char *
strpbrk(string, brkset)
register char *string, *brkset;
{
	register char *p;

	if(!string || !brkset)
		return(0);
	do {
		for(p=brkset; *p != '\0' && *p != *string; ++p)
			;
		if(*p != '\0')
			return(string);
	}
	while(*string++);
	return(0);
}
