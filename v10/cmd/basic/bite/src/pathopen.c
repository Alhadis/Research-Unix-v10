/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	//// SEARCH PATHS AND OPEN FILE /////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Wed Sep 12 11:18:06 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////

 * call format:
 * FILE *pathopen(),*fp;
 * 	fp = pathopen(filestring);
 *
 * Search paths specified in bpath for file. If found,
 * open the filename pointed to by filestring and return filepointer fp.
 * If search fails return 0.
 * Same convention as fopen().
 */
/*   "@(#) pathopen.c:  V 1.2  3/14/81" */
#include "bas.h"
char bpath[100] = ":./:/usr/lib/bites:";

FILE *
pathopen(name)
char	name[];
{
	FILE *fopen(),*fp;
	char	*getpath();
	char filpath[80];
	register char *p;
	p = bpath;	/* start with first entry in paths */

	while((p = getpath(filpath,p)) != 0)
	{
		strcat(filpath,"/");
		strcat(filpath,name);
		if((fp = fopen(filpath,"r")) != 0) /* if open succeeds */
			break;
	}
	if(fp == 0)	/* if open did not succeed */
	{
		error(inst.thing.linno,4);
		return(0);
	}
	return(fp);
}

/*
 * SCAN PATH STRING
 */
char *
getpath(field,s)
char	*s;
char	field[];
{
	int	i;
	char	*p;

	i = 0;
	p = s;
	if(*p == ':') *p++;	/* bump past delim */
	while(*p != '\0' && *p != ':')
	{
		field[i++] = *p++;
	}
	field[i] = '\0'; /* null terminate */

	if(p != s)
		return(p);

	return(0);
}
