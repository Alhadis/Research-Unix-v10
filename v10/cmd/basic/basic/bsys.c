/*
 * Call system command from basic
 *
 */
/*	"@(#) bsys.c: V 2.11 6/3/83"	*/
#include	<bas.h>
#include	<stdio.h>

extern char	*getenv();

char	*shpath,
	*shname;

extern char	_string[];


bsys()
{
	callsys(expr);
	return(0);
}



/*
 *
 * SYSTEM CALL ROUTINE
 */

callsys(string)
char	string[];
{
	int	t,
		status;

	findsh(shpath = getenv("SHELL"));		/* get shell */
	if ((t = fork()) == 0)
		if (execl(shpath,shname,"-c",string, 0) < 0)
		{
			error(inst.thing.linno,inst.thing.stmtno, 31);
			exit(-1);
		}
	if (t == -1)
		return(-1);
	while (t != wait(&status))  ;
	return(0);
}


/*
 * break off shell name
 */


findsh(shptr)
char	*shptr;
{
	char	*nptr;

	if (shptr == NULL || *shptr == NULL)
	{
		shname = "sh";			/* assume normal shell */
		shpath = "/bin/sh";
	}
	else
	{
		nptr = shptr;
		while (*++shptr)				/* find name */
			if (*shptr == '/' && *shptr + 1 != NULL)
				nptr = ++shptr;
		shname = nptr;
	}
}
