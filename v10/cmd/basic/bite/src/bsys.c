/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	////////////// bsys.c ///////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Fri Aug 24 17:26:01 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////

 * Call system command from basic
 *
 */
/*   "@(#) bsys.c:  V 1.5  6/17/81" */
#include	"bas.h"

char	shpath[] = "/bin/sh";
char	shname[] = "sh";
extern char _string[];
bsys()
{
#ifdef	RESTRICT
	printf("RESTRICTED SHELL '!' NOT FUNCTIONAL\n");
	return(-1);
#else
	callsys(expr);
	return(0);
#endif
}
/*
 *
 * SYSTEM CALL ROUTINE
 */
callsys(string)
char	string[];
{
	int t,status;

	if((t = fork()) == 0)
		if(execl(shpath,shname,"-c",string, 0) < 0)
		{
			error(inst.thing.linno, 31);
			printf("%s NOT FOUND\n",string);
			exit(0);
		}
	if(t == -1) abt("t == -1 from fork");
	while(t != wait(&status))  ;
	return 0;
}

/*
 *
 * error abort
 */
abt(s)
char *s;
{
	printf("\nFailed to fork. %s\nReturn to interpreter\n", s);
}
