/*	/sccs/src/cmd/uucp/s.versys.c
	versys.c	1.1	8/30/84 17:38:20
*/
#include "uucp.h"
VERSION(@(#)versys.c	1.1);

/*
 * verify system name
 * if abbr is nonzero, allow the name to be truncated
 * for compatibility with old uucps
 * input:
 *	name	-> system name
 * returns:  
 *	0	-> success
 *	FAIL	-> failure
 */

#define	OLDLEN	7	/* max length of old uucp names */

versys(name, abbr)
char *name;
int abbr;
{
	register FILE *fp;
	register char *iptr;
	char line[300];
	FILE *sysopen();

	if (EQUALS(name, Myname))
		return(0);

#ifndef MANYSYS	
#define	SYSCLOSE	fclose
	fp = fopen(SYSFILE, "r");
#else
#define	SYSCLOSE	pclose
	fp = sysopen(name);
#endif
	if (fp == NULL)
		return(FAIL);
	
	while (fgets(line, sizeof(line) ,fp) != NULL) {
		if((line[0] == '#') || (line[0] == ' ') || (line[0] == '\t') || 
			(line[0] == '\n'))
			continue;

		if ((iptr=strpbrk(line, " \t")) == NULL)
		    continue;	/* why? */
		*iptr = '\0';
		if (EQUALS(name, line)) {
			(void) SYSCLOSE(fp);
			return (0);
		}
		if (abbr && strlen(name) == OLDLEN && strncmp(name, line, OLDLEN) == SAME) {
			(void) fclose(fp);
			return(0);
		}
	}
	SYSCLOSE(fp);
	return(FAIL);
}
