/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	***********************************
 *	***********************************
 *	*** CALL (OVERLAY) A SUBROUTINE ***
 *	*** R. B. Drake WH 8C-005 X4163 ***
 *	**** Wed Aug 29 16:40:29 1979 *****
 *	***********************************
 *	***********************************
 * Calling sequence:
 *  
 * 	call name,line#
 *  
 * "call" lokks at line "line#" and if that line contains "rem name"
 * it does a gosub to that line. If that line does not contain "rem name"
 * it searches the current directory for a file "name.b" and loads or
 * overlays it according to the line numbers that exist in the file.
 * then it executes a gosub line#.

*/
/*   "@(#) call.c:  V 1.1  12/21/80" */

#include "bas.h"
static int li;
static char file[15],Line[15];
call()
{
	int cur;
	FILE *pathopen(),*fp;
	char *prncpy(),*pt;
	char name[15];
	extern char *expr;
	cur=inst.thing.linno;
	pt=prncpy(file,expr);
	if(*pt++ != ',')
	{
		error(cur,8);
		return(-1);
	}
	pt=prncpy(Line,pt);
	li=atoi(Line);
	if(ch()) return(0);
	strcpy(name,file);
	postb(name);
	if((fp=pathopen(name)) == 0)
	{
		error(cur,4);
		return(-1);
	}
	while(bed(fp) == 0);
	fclose(fp);
	if(ch()) return(0);
	error(cur,43);
	return(-1);
}
ch()
{
	char *ptr;
	fetch(li,&ptr);
	if(!strcmp(file,expr))
	{
		expr=Line;
		if(gosub() == -1) return(0);
		return(1);
	}
	return(0);
}
