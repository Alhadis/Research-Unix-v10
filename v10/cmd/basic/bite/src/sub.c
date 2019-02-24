/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	******************************************************
 *	******************************************************
 *	*** SUBSTITUTE COMMAND SIMILAR TO UNIX EDITORS 's' ***
 *	************ R. B. Drake WH 8C-005 X4163 *************
 *	************** Sun Aug 26 11:51:03 1979 **************
 *	******************************************************
 *	******************************************************

*/
/*   "@(#) sub.c:  V 1.1  12/21/80" */

/* routine to do character substitution for the editor */
#include "bas.h"
#define ertst if(ifnul(work)) return(-1)
char temp[80];
extern char _string[];
int line;
sub()
{
	char *strptr,c,*op;
	char *lp,*work,*from,*to,*isthere(),what[80],with[80],delim;
	lp=what;
	work= expr;
	while(num(*work))
		*lp++ = *work++; /* get the line number to work on */
	ertst;
	*lp='\0';
	line = atoi(what);
	ertst;
	while((c= *work) == ' ' || c == '\t')
	{
		ertst;
		work++;
	}
	delim= *work++; /* get the delimeter */
	from=what;
	to=with;
	while(*work != delim)
	    {
		ertst;
		*from++ = *work++;
	}
	*from='\0';
	work++; /* bump past delimeter */
	while (*work != delim && *work != '\0')
		*to++ = *work++;
	*to++ = '\0';
	fetch(line,&lbdptr); /* get the line to be edited */
	if(inst.thing.linno != line) /* if no such line , complain and exit */
	    {
		error(line,0);
		return(-1);
	}
	expand(); /* expand the expression part of program line */
	op = decode(inst.thing.opcode.lobyte);
	if(op[0] == '\010')
	sprintf(temp,"%d %s\n",inst.thing.linno,linbuf);
	else
	sprintf(temp,"%d %s %s\n",inst.thing.linno,op,linbuf);
	if((lp=isthere(what,temp,0)) == 0)
	    {
		error(0,1);
		return(-1); /* no such expression in that line */
	}
	strptr= _string;
	from=temp;
	to=with;
	while( *from != '\n')
	{
		if( from == lp)
		{
			from += strlen(what);
			while(*to)
				*strptr++ = *to++;
		}
		else
			*strptr++ = *from++;
	}
	*strptr++= '\n';
	*strptr = '\0';
	bed((FILE *)0);
	fetch(line,&lbdptr);
	prin();
	return(0);
}
ifnul(work)
char *work;
{
	if(*work == '\0')
	{
		error(0,20);
		return(1);
	}
	return(0);
}
/* undo the last substitute or single line delete */
undo()
{
	char *strptr,*from;
	strptr=_string;
	from=temp;
	while(*from != '\0')
		*strptr++ = *from++;
	*strptr='\0';
	bed((FILE *)0);
	fetch(line,&lbdptr);
	prin();
	return(0);
}
