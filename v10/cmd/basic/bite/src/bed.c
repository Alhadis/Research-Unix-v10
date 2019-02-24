/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	***********************************
 *	***********************************
 *	***** BITE INPUT TEXT EDITOR ******
 *	*** R. B. Drake WH 8C-005 X4163 ***
 *	**** Fri Aug 24 15:21:32 1979 *****
 *	***********************************
 *	***********************************
 * This routine handles all text input to bite. Text may originate from
 * a tty or from a UNIX file or data placed in a string array. Certain
 * syntax error messages are issued and all housekeeping having to do
 * with line numbers and the page index is handled here.

*/
/*   "@(#) bed.c:  V 1.3  11/12/81" */

#include "bas.h"
char *linptr; /* ALWAYS points to the next available 
		byte in the program txtbuf */
char *endob = txtbuf + (NMPAG * PGSIZ);
char _string[LINMAX];
union bascmd local;
union bascmd inst;
char linbuf[LINMAX];
char ascno[NOMAX],ascop[OPMAX];
struct pages index[NMPAG+1];
int pg;
static int size;
char *curptr; /* fetch sets this to point to the beginning of the
		current line. Other routines may change it */
char *lbdptr; /* fetch sets this to point to the beginning
		of the next sequential line in the txtbuf */
extern int singflg;
extern int tv;
/* This table is used to encode and decode certain key-worde for
   storage compression and for ease of recognition by the execution
   software */
char *keyword[] =
{
"goto",		/*  1 */
"go to",	/*  2 */
"then",		/*  3 */
"to",		/*  4 */
"step",		/*  5 */
"<>",		/* 6 */
"<=",		/*  7 */
"=<",		/*  10 */
"<",		/* 11 */
">=",		/* 12 */
"=>",		/* 13 */
">",		/* 14 */
"=",		/* 15 */
"gosub",	/* 16 */
"more",		/* 17 */
0 };
extern int rem;
extern int _sub;
extern int tel;
extern int stpflg;
extern int autflg,aut,inc;
bed(fp)
FILE *fp;
{
	register char *to,*from;
	int i,j,k,nlflg,quoflg,parflg,bflg;
	int erflg;
	char c,*strptr;
	char *pt,*save;
	char *isthere();
	
	/* zero the working txtbufs */
	strptr=_string;
	for(i=0;i<NOMAX;ascno[i++]=0);
	for(i=0;i<OPMAX;ascop[i++]=0);
	for(i=0;i<LINMAX;linbuf[i++]=0);
	local.thing.opcode.lobyte=local.thing.opcode.hibyte='\0';
	local.thing.linno=nlflg=stpflg=0;
	expr = linbuf;
	if(autflg != 0) /* if auto line numbering in affect */
	{
		aut += inc;
		sprintf(ascno,"%d",aut);
	}
	/* print the prompt character */
	if(fp != 0)
	if(isatty(fileno(fp)))
		printf("*%s",ascno);
	if(fp != 0)
		if(fgets(_string,LINMAX,fp) == 0) return(-1); /* EOF */
/* get a line of input from the user and store it in compressed
 * source form unless the line number is zero in which case
 * execute the instruction immediately
*/
	/* get the line number if any */
	while(*strptr == ' ' || *strptr == '	')
		strptr++; /* ignore leading white space */
	for(i=0,k=0;num((c= *strptr++)) && i < (NOMAX-1);i++)
	{
		ascno[i]=c;
		aut = atoi(ascno);
	}
	if(ascno[0] != '\0')
		if((local.thing.linno = aut) == 0)
		{
			error(0,0);
			return(0);
		}
	if(c== '\n')
	{
		if(local.thing.linno == 0 || autflg)
		{
			if(fetch(-1,&lbdptr) != 0)
			{
				if(fetch(0,&lbdptr) != 0)
				{
					error(0,1);
					return(0);
				}
			}
			prin();
			if(autflg) aut -= inc;
			return(0);
		}
		/*a line number was entered
		 * but nothing followed which implies
		 *delete
		*/
		delete();
		return(0);
	}
	/* if no blank was used as a delimiter, then insure
	 * that we don't lose the first character of the opcode.lobyte
	*/
		if( c == '.') /* if auto line num to be tuned off */
		{
			autflg=0;
			aut -= inc;
			return(0);
		}
	if(c== '\t')
	{
		local.thing.opcode.hibyte = '\201';
		while(*strptr == '\t')
		{
			local.thing.opcode.hibyte = (local.thing.opcode.hibyte&0377) + 1;
			strptr++;
		}
	}
	if( c != ' ' && c != '\t')
	{
		ascop[0] = c;
		k=1;
	}
	/* get the operation statement */
	bflg=0;
	for(j=0,i=k;(c= *strptr);i++)
	{
		if((c== ' ') && (strcmp("go",ascop) != 0)) 
		{
			bflg=1;
			while((c= *strptr) == ' ' || c== '\014')
				strptr++;
		}
		/* if an "=" is present here then the
		 *opcode.lobyte is "let" by default */
		if(c== '=' || (c== '(' && bflg != 1))
		{
			ascop[i]=c;
			strcpy(linbuf,ascop);
			strcpy(ascop,"\010");
			j=i+1;
			strptr++;
			break;
		}
		/* If a new line is present here then
		 * we have an operation with
		 * no following expression
		*/
		if(c== '\n')
		{
			nlflg=1;
			ascop[i]='\0';
			break;
		}
		if( i< (OPMAX -1))
		{
		if( bflg == 1) break;
		ascop[i]=c;
		}
		strptr++;
	}
	if(ascop[0] !=0)
	{
		int xxx = encode(ascop);

		local.thing.opcode.lobyte = xxx;
		if(xxx == -1)
		{
			error(local.thing.linno,1);
			local.thing.opcode.lobyte=tel;
			pt = strcat(linbuf,ascop);
			pt = carcat(linbuf,' ');
			erflg=1;
			j = pt - linbuf;
		}
		else erflg=0;
	}
	if(!nlflg)
	{
		/* put any expression in "linbuf" */
		for(i=j;(c= *strptr++) != '\n';i++)
		{
			if(i < (LINMAX -1))
			linbuf[i]=c;
		}
	/* encode keywords in the expression field */
    while(1)
    {
	if(!nocode(local.thing.opcode.lobyte) && erflg == 0)
	{
		/* first strip any blanks and control characters
		 * out of the expression unless they are quoted */
		quoflg = 1;
		parflg= 0;
		for(i=0,j=0;linbuf[i] != '\0';i++)
			if(linbuf[i] > '\040' || quoflg== -1)
			{
				linbuf[j++]=linbuf[i];
				switch (linbuf[i])
				{
					case '"' : quoflg *= -1;
						break;
					case '(' : parflg += 1;
						break;
					case ')' : parflg -= 1;
						break;
				}
			}
		linbuf[j]='\0';
		if(quoflg == -1)
		{
			error(local.thing.linno,11);
		}
		if(parflg != 0)
		{
			error(local.thing.linno,16);
		}
		for(i=0;keyword[i] != 0;i++)
		while((pt=isthere(keyword[i],linbuf,1)) != 0)
		{
			*pt++='\001'+i;
			shrink(pt);
		}
	}
	break;
    }
	}
	/* If the line number is zero then we have
	 * a command for immediate execution
	*/
	if( k != 0)
	{
		inst.thing.linno = 0;
		expr=linbuf;
		if(local.thing.opcode.lobyte == tv)
			list(1);
		else
		{
			bascall(local.thing.opcode.lobyte);
			if(autflg)
				aut -= inc;
		}
		return(0);
	}
	/* if a ligitimate line number has been entered
	 *put it in the program txtbuf
	*/
	if(singflg != 0) /* don't permit txtbuf editing in single step */
	{
		error(0,12);
		return(0); /* not fatal */
	}
	if(fetch(local.thing.linno,&lbdptr) == -1) inst.thing.linno = 0;
	save=curptr;
	/* if local.thing.linno is greater than inst.thing.linno (returned by fetch)
	 * then fetch has returned the last line of the program
	 * and the action is to add the new line to the end of the
	 * program */
	if(local.thing.linno > inst.thing.linno)
	{
		pg=(linptr-txtbuf)/PGSIZ; /* calculate page number*/
		if(index[pg].maxlin == 0)
		{
			index[pg].maxlin = local.thing.linno; /*first line*/
			index[pg].begin=linptr;
		}
		if(local.thing.linno > index[pg].maxlin)
			index[pg].maxlin=local.thing.linno;
		for(i=0;i<4;i++)
			*linptr++=local.byte[i];
		expr = linptr;
		for(i=0;linbuf[i] != 0;i++)
			*linptr++=linbuf[i];
		*linptr++='\0'; /* end the expression string */
		linptr[0]='\0'; /* make it an eof mark */
		linptr[1]= '\0';
	}
	/* if local.thing.linno is less than inst.thing.linno, then the new
	 * line must be inserted into the program txtbuf. In order
	 * to do that, the current line and all lines above it
	 * must be slid up to make room for the new line */
	if(local.thing.linno < inst.thing.linno) /* insert case */
	{
		/* make room */
		clear(); /*clear page index from current page up */
		from = linptr;
		linptr += strlen(linbuf) + 5;
		to = linptr;
		*linptr = '\0'; /*insure an eof mark*/
		linptr[1] = '\0';
		linptr[2] = '\0'; /* eof requires one 0 to end an expression
				string and two to mark eof */
		while(from != curptr)
			*--to = *--from;
		put();
		update(); /* rebuild page index */
		lbdptr=curptr=save;
		return(0);
	}
	/* if we arrive at this place in the program, local.thing.linno is
	 * by default equal to inst.thing.linno and the action is to replace
	 * the current line with the new one. There are three possible
	 * vvariations in the replacement algorithm. 1) the new line
	 * and the old one are the same size in which case the action
	 * is a simple replacement. 2) the new line is shorter than the
	 * old one in which case the action is to replace the old line
	 * and slide the remaining txtbuf down to reclaim the left over
	 * space. 3) the new line is longer than the old one in which
	 * case the txtbuf above the current line must be slid up by
	 * an amount equal to the difference in length to make room
	 * for the new line */

	size = strlen(expr) - strlen(linbuf);
	if(size == 0) /*case 1 */
	{
		put();
		lbdptr=curptr=save;
		return(0);
	}
	if(size > 0) /* case 2 */
	{
		put();
		cover(lbdptr,curptr);
		lbdptr=curptr=save;
		return(0);

	}
	clear(); /* case 3 by default of not being either 1 or 2 */
	from = linptr;
	linptr -= size;
	to = linptr;
	while(from != curptr)
		*--to = *--from;
	put();
	update();
	lbdptr=curptr=save;
	return(0);
}
