/*	"@(#) save.c: V 2.17 11/10/83"	*/

/* This routine is used to save BASIC programs in standard ASCII format
   on the UNIX file system. Files saved in this way can be edited by
   the standard unix editor if desired. However if that is done no
   syntax checking can be done until the file is "loaded" */

#include <stdio.h>
#include "bas.h"


extern char	filnam[],
		errbuf[];


save()
{
	FILE	*fopen(),
		*fp;
	char	*ptr;
	char	tfnam[15];
	LINNO	reallinno;
	UCHAR	realstmtno;

	skip00(&expr);
	strcpy(tfnam, filnam);
	if (expr[0] == '\0' && filnam[0] == '\0')
	{
		error(inst.thing.linno,inst.thing.stmtno,21);
		return(-1);
	}
	if (expr[0] != '\0')
	{
		if (strlen(expr) > LINMAX)
		{
			error(inst.thing.linno,inst.thing.stmtno,4);
			return(-1);
		}
		if (eval(expr) < 0)
			return(-1);
		if (exprtype != STRING)
		{
			error(inst.thing.linno,inst.thing.stmtno, 51);	/* numb. in str. exp */
			return(-1);
		}
		strcpy(filnam,exprval.sval);
		postb(filnam);
	}
	reallinno = inst.thing.linno;
	realstmtno = inst.thing.stmtno;
	if ((fp = fopen(filnam,"w")) == 0)
	{
		sprintf(errbuf, " %s\n",filnam);
		error2(reallinno,realstmtno,56,errbuf);
		strcpy(filnam, tfnam);
		return(-1);
	}
	if (fetch(FIRST,LINE,&ptr) < 0)
	{
		fclose(fp);
		preprint();
		printf("File saved on %s\n",filnam);
		return(0);
	}
	outit(fp);
	while (fetch(NEXT,LINE,&ptr) == 0)
		outit(fp);
	fclose(fp);
	preprint();
	printf("File saved on %s\n",filnam);
	return(0);
}



outit(fp)
FILE	*fp;
{
	char		buf[LINMAX+1];

	line_decode(curptr,buf);
	fprintf(fp,"%s",buf);
}



/* print the current default filename */
f()
{
	preprint();
	if (filnam[0] == '\0')
		printf("No current filename exists\n");
	else
		printf("Current filename is %s\n",filnam);
	return(0);
}
