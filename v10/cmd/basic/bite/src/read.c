/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	***********************************************
 *	***********************************************
 *	*** READ INFORMATION FROM 'data' STATEMENTS ***
 *	********* R. B. Drake WH 8C-005 X4163 *********
 *	********** Sun Aug 26 11:19:29 1979 ***********
 *	***********************************************
 *	***********************************************
 * "read" locates the next item of information in a 'data' sttement
 * and places it in variables defined by a list following the read
 * command.

*/
/*   "@(#) read.c:  V 1.4  3/4/81" */

#include "bas.h"
static char *daptr;
static char *itmptr;
extern int atad;
double evalx();
char *prncpy();
int arflg;
getnext()
{
	while(fetch(-1, &daptr) == 0)
	{
		if(inst.thing.opcode.lobyte == atad)
		{
			itmptr = expr;
			return(0);
		}
	}
	error(inst.thing.linno,33);
	return(-1);
}
__read()
{
	char *ptr;
	int flg;
#ifdef	STRINGS
	char	*savptr;
	char	field[40];
	char	result[80];
#endif
	char ascop[80],ascno[80];
	ptr = expr;
	if(daptr == 0)
	{
		if(fetch(0, &daptr) != 0)
		{
			error(inst.thing.linno,33);
			return(0);
		}
		if(inst.thing.opcode.lobyte == atad)
			itmptr = expr;
		else
		if(getnext() < 0)
			return(0);
	}
	while(1)
	{
		flg = 0;
		ptr = prncpy(ascop,ptr);
		if(arflg == 1)
			flg = 1;
		switch(*itmptr)
		{
			case '\0':
				if(getnext() < 0)
					return(0);
				break;
			case ',':
				itmptr++;
				if(*itmptr == '\0')
					if(getnext() < 0)
						return(0);
		}
		itmptr = prncpy(ascno,itmptr);
#ifdef	STRINGS
		savptr = ascno;
		if(class(&savptr,field) < STCLASS)
		{
#endif
			if(flg==1)
				aputvar(ascop,evalx(ascno));
			else
				putvar(ascop,evalx(ascno));
#ifdef	STRINGS
		}
		else
		{
			evals(ascno,result);
			savptr=ascop;
			if(class(&savptr,field) == SACLASS)
				saputvar(field,result);
			else
				sputvar(field,result);
		}
#endif
		if(*ptr++ == '\0')
			return(0);
	}
}
restore()
{
	daptr = 0;
	return(0);
}
char *prncpy(to,from)
char register *to,*from;
{
	int prncnt,i;
	prncnt = 0;
	arflg = 0;
	for(i=0;*from != '\0';i++)
	{
		if(*from == ',' && prncnt == 0) break;
		to[i] = *from++;
		switch(to[i])
		{
			case '(':
				prncnt += 1;
				arflg = 1;
				break;
			case ')':
				prncnt -= 1;
		}
	}
	to[i] = '\0';
	return(from);
}
