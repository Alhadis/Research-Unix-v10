 /* call format:
 * FILE *pathopen(),*fp;
 * 	fp = pathopen(filestring);
 *
 * Search paths specified in bpath for file. If found,
 * open the filename pointed to by filestring and return filepointer fp.
 * If search fails return 0.
 * Same convention as fopen().
 */
/*	"@(#) pathopen.c: V 2.8 6/3/83"	*/
#include <stdio.h>
#include <bas.h>

char bpath[100] = ".:";

extern FILE	*fopen();



FILE *
pathopen(name)
char	name[];
{
	FILE	*fp;
	char	*getpath();
	char	filpath[LINMAX+1];
	register char	*p;

	p = bpath;			/* start with first entry in paths */

	while ((p = getpath(filpath,p)) != NULL)
	{
		strcat(filpath,"/");
		strcat(filpath,name);
		if ((fp = fopen(filpath,"r")) != NULL)	/* if open succeeds */
			break;
	}
	if (fp == NULL)				/* if open did not succeed */
	{
		error(inst.thing.linno,inst.thing.stmtno,4);
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
	if (*p == ':')
		p++;				/* bump past delim */
	while (*p != '\0' && *p != ':')
	{
		field[i++] = *p++;
	}
	field[i] = '\0';			/* null terminate */
	if (p != s)
		return(p);
	return(0);
}



postb(name)
char	name[];
{
	register int	i;

	i = 0;
	for (i = 0; name[i] != '\0' && name[i] != '\n'; i++) ;
	name[i] = '\0';		/* insure terminating character is a null */
	if (!(name[i - 2] == '.' && name[i - 1] == 'b') &&
		!(name[i - 2] == '.' && name[i - 1] == 'B'))
		strcat(name,".b");
	return(0);
}
