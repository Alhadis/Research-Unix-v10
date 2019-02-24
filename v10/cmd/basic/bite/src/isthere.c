/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	******************************************
 *	******************************************
 *	*** IS STRING IN THERE ? IF SO WHERE ? ***
 *	****** R. B. Drake WH 8C-005 X4163 *******
 *	******** Fri Aug 24 17:38:24 1979 ********
 *	******************************************
 *	******************************************

*/
/*   "@(#) isthere.c:  V 1.1  12/21/80" */

/* determine if a group of characters is in a string and if so
 * return a pointer to the start of the group otherwise return
 * -1 */
#include "bas.h"
static char *beyond; /*pointer to first non-blank beyond group*/
char *isthere(s,t,flg)
char *s,*t;
int flg; /* used to turn quote testing on and off */
{
	int i;
	char *quot();
	for(;*t != 0;t++)
	{
		if((*t == '"') && ( flg ==1))
		{
			t=quot(t);
			if(t < (char *)0)
				return(0);
			if(*t == '\0')
				return(0);
		}
		for(i=0; s[i] == 0 || s[i] == t[i];i++)
			if(s[i] == '\0')
			{
			for(i=i;t[i] == ' ';i++);
			beyond = &t[i];
			return(t);
			}
	}
	return(0);
}
/* take the string starting at ptr and */
shrink(ptr)
char *ptr;
{
	extern char linbuf[];
	while(*beyond != '\0')
		*ptr++ = *beyond++;
	*ptr = '\0';
}
/* routine to push the incoming pointer past anything enclosed
 * in double quotes. If we hit the end of string first, its an error */
char *quot(t)
char *t;
{
	t++; /* get past the opening quote */
	while(*t++ != '"')
		if(*t == '\0')
		{
			error(inst.thing.linno,11);
			return(0);
		}
	return(t);
}
