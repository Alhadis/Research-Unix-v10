/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	***********************************
 *	***********************************
 *	***** LIST THE PROGRAM BUFFER *****
 *	*** R. B. Drake WH 8C-005 X4163 ***
 *	**** Fri Aug 24 17:41:50 1979 *****
 *	***********************************
 *	***********************************

*/
/*   "@(#) list.c:  V 1.1  12/21/80" */

#include "bas.h"
int start,finish; /* first and last line numbers */
int stpflg;
extern char linbuf[];
stopl()
{
	stpflg = 1;
	printf("\n%s %d\n","INTERRUPT LINE",inst.thing.linno);
	signal(2,stopl);
}
char *expand();
static int n;
extern int aut,autflg,inc;
#define retrn(x) {signal(2,quit);stpflg=0;return(x);}
list(opt)
int opt;
{
extern char *lbdptr,*curptr;
	int quit();
	/* if there is no expression then list the
	 * entire program */

	signal(2,stopl);
	n=0;
	if(expr[0] == 0)
	{
		if(opt != 1)
		{
		if(fetch(0,&lbdptr) < 0)
		{
			error(0,5);
			retrn(-1);
		}
		prin();
		}
		else
		lbdptr = curptr;
		while(fetch(-1,&lbdptr) == 0 && stpflg == 0)
		{
			prin();
			if(n==23 && opt == 1) break;
		}
		retrn(0);
	}
	/* there is an expression so parse it into
	 * start and finish */
	lnpars();
		if(fetch(start,&lbdptr) == -1)
		{
			error(0,5);
			retrn(-1);
		}
	prin(); /* print the first line */
	if(inst.thing.linno < finish)
	while(fetch(-1,&lbdptr) == 0 && stpflg == 0)
	{
		if(inst.thing.linno > finish)
		{
			lbdptr=curptr;
			retrn(0);
		}
		prin();
	}
	retrn(0);
}
/* print routine */

prin()
{
	char *op;
	int i;
	n++;
		expand();
		printf("%d",inst.thing.linno);
		if((inst.thing.opcode.hibyte&0377) > 0200)
		{
			i=(inst.thing.opcode.hibyte&0377) - 0200;
			while(i--)
				printf("\t");
		}
		else printf(" ");
		op = decode(inst.thing.opcode.lobyte);
		if(op[0] == '\010')
			printf("%s\n",linbuf);
		else
		printf("%s %s\n",op,linbuf);
	aut = inst.thing.linno;
	return(0);
}
char *expand()
{
	char c;
	char *pt,*strcat(),*carcat(),*lin;
	int i,quoflg;
	pt=lin=linbuf;
	quoflg = 1;
	for(i=0;i<LINMAX;linbuf[i++]=0);
	while((c = *expr++) != '\0')
	{
		if(c== '"') quoflg *= -1;
		if((c >= '\001') && ( c <= '\037') && !nocode(inst.thing.opcode.lobyte)
			&& quoflg != -1)
		{
			pt=carcat(pt,' ');
			pt=strcat(pt,keyword[c-1]);
			pt=carcat(pt,' ');
			if(c != '\15') /* if non an "=" */
			lin=pt; /* after the last keyword has been
				encountered, lin will point to the
				line number portion of the expression */
		}
		else
			pt=carcat(pt,c);
	}
	return(lin); /* return pointer to line number, used by resequence
			to find referenced line numbers in "if", "goto" etc */
}
