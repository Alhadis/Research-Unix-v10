/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	***********************************
 *	***********************************
 *	***** LINE NUMBER RESEQUENCER *****
 *	*** R. B. Drake WH 8C-005 X4163 ***
 *	**** Sun Aug 26 11:23:45 1979 *****
 *	***********************************
 *	***********************************

*/
/*   "@(#) reseq.c:  V 1.1  12/21/80" */

/* resequence the program buffer */
#include "bas.h"
extern int no;
extern char temp[];
extern char *endob;
extern int _fi,_togo,subog;
extern int tsil,tospgo;
extern int start,finish;
extern char _string[];
	static struct REFP
		{
			int whoref;
			int refwhat;
			int newref;
		} *refptr,*savit;
	static int newnum;
breseq(begin,end)
int begin,end;
{
	FILE *fopen(),*fp;
	char *lin,*from,*to,*lp,*ptr,*prncpy(),*hold;
#ifdef LSX
	static char nam[] = {"/tmp/tmp.b"};
#endif
#ifndef LSX
	static char nam[] = {"tmp.b"};
#endif
	int i;
	lnpars(); /* get start and finish. finish will be increment */
	if(start==0) start=10;
	if(finish==0 || finish == 32767) finish = 10;
	expunge();
	refptr = (struct REFP *)endob;
	newnum=0;
	if(fetch(0,&lbdptr) < 0) /* get first line */
	{
		error(0,5);
		return(-1);
	}
	if(inst.thing.linno < begin || inst.thing.linno > end)
	--newnum;
	check(begin,end); /* if it references a line#, record it */
	while(fetch(-1,&lbdptr) == 0) /* search the remaining lines */
	{
		if(inst.thing.linno >= begin && inst.thing.linno <= end)
		newnum++;
		check(begin,end);
	}
	fetch(begin,&lbdptr);
		replace();
	clear();
	while(fetch(-1,&lbdptr) != -1)
	{
		if(inst.thing.linno > end) break;
		replace();
	}
	update();
	if(begin != 0)
	{
		fp=fopen(nam,"w");
		fetch(0,&lbdptr);
			outit(fp);
		while(fetch(-1,&lbdptr) == 0)
			outit(fp);
		fclose(fp);
		onew();
		fp=fopen(nam,"r");
		while(bed(fp) == 0);
		fclose(fp);
#ifndef LSX
		unlink(nam);
#endif
	}
	while(refptr != (struct REFP *)endob)
	{
	fetch(refptr->whoref,&lbdptr);
	lin = expand();
	if(refptr->refwhat < 0)
	{
	strcpy(temp,lin); /* put line refrences into temp */
	ptr=_string;
	lp=temp;
	i= -1*refptr->refwhat;
	while(1)
	{
		hold=prncpy(ptr,lp);
		if(atoi(ptr) == i) break;
		if(*hold != ',') break;
		lp= ++hold;
	}
	sprintf(ptr,"%d",refptr->newref);
	from= temp;
	to=lin;
	while(*from != '\0')
	{
		if(from == lp)
		{
			while(*from != ',' && *from != '\0')
				from++;
			while(*ptr != '\0')
				*to++ = *ptr++;
		}
		else
			*to++ = *from++;
	}
	*to = '\0';
	sprintf(_string,"%d %s %s\n",inst.thing.linno,decode(inst.thing.opcode.lobyte),linbuf);
	bed((FILE *)0);
	}
	else
		error(refptr->whoref,0);
	refptr++;
	}
	return(0);
}
check(begin,end)
int begin,end;
{
	char *lin;
	int oldlin;
	char buffer[80];
	if(cklist(inst.thing.opcode.lobyte))
	{
		lin = expand();
		if(!num(*lin)) return(0);
		while(*lin != '\0')
		{
			lin = prncpy(buffer,lin);
			oldlin=atoi(buffer);
			if(oldlin < begin || oldlin > end) return(0);
			--refptr;
			if(inst.thing.linno >= begin && inst.thing.linno <= end)
			refptr->whoref = start + newnum * finish;
			else refptr->whoref = inst.thing.linno;
			refptr->refwhat = oldlin;
			if(*lin == '\0') break;
			lin++;
		}
	}
	return(0);
}
cklist(code)
int code;
{
	int i;
	static int *list[] =
		{ &_fi,
		&_togo,
		&subog,
		&tsil,
		&no,
		&tospgo,
		0 };
	for(i=0;list[i] != 0;i++)
	{
		if(code == *list[i])
		return(1);
	}
	return(0);
}
replace()
{
	local.thing.linno=start;
	start += finish;
	*curptr++ = local.byte[0];
	*curptr++ = local.byte[1];
	savit = refptr;
	while(refptr != (struct REFP *)endob)
	{
		if(inst.thing.linno == refptr->refwhat)
		{
			refptr->refwhat *= -1;
			refptr->newref = local.thing.linno;
		}
		refptr++;
	}
	refptr=savit;
	return(0);
}
reseq()
{
	breseq(0,32767);
}
mov()
{
	extern char *expr;
	char *prncpy(),ary[6];
	int begin,end;
	expr=prncpy(ary,expr);
	if(*expr++ != ',')
	{
		error(0,42);
		return(0);
	}
	begin=atoi(ary);
	expr=prncpy(ary,expr);
	if(*expr++ != ',')
	{
		error(0,42);
		return(0);
	}
	end=atoi(ary);
	breseq(begin,end);
	return 0;
}
