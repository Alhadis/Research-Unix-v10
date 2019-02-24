static char ID[] = "@(#) strings.c: 1.1 1/7/82";

/*
 *
 *	"strings.c" is a file containing various functions for doing
 *	string handling and storage in the assembler.  The following
 *	functions are provided:					
 *								
 *	compare(str1,str2)	Compares one string to the other and
 *				returns one of the manifest constants
 *				LESS, EQUAL, or GREATER from "gendefs.h"
 *				to indicate how they are related in a
 *				lexicographic ordering.
 *
 */

#include "gendefs.h"

copystr(str1,str2)
	register char *str1, *str2;
{
	register int n = 0;

	while (*str2++ = *str1++) ++n;
	return(n);
}

/*
 *
 *	"compare" is a function that compares one string to another and
 *	returns one of the constants LESS, EQUAL, or GREATER depending
 *	on how they are related in a lexicographic ordering.  Two
 *	pointers are advanced down the two strings as long as they are
 *	equal.  If the end of the string is reached in this manner,
 *	EQUAL is returned.  Otherwise, the decision to return LESS or
 *	GREATER is made depending on how the two characters that differ
 *	compare with each other.
 *
 */

compare(str1,str2)
	register char *str1,*str2;
{
	while (*str1 == *str2++) 
		if (*str1++ == '\0')
			return(EQUAL);
	return((*str1 > *--str2) ? GREATER : LESS);
}
