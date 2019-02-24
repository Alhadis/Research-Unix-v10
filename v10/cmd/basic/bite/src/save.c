/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	***********************************
 *	***********************************
 *	******* SAVE A BITE PROGRAM *******
 *	*** R. B. Drake WH 8C-005 X4163 ***
 *	**** Sun Aug 26 11:27:17 1979 *****
 *	***********************************
 *	***********************************

*/
/*   "@(#) save.c:  V 1.1  12/21/80" */

/* This routine is used to save BASIC programs in standard ASCII format
   on the UNIX file system. Files saved in this way can be edited by
   the standard unix editor if desired. However if that is done no
   syntax checking can be done until the file is "loaded" */
#include "bas.h"
extern char filnam[];

save()
{
	FILE *fopen(),*fp;
	char *ptr;
	if(expr[0] == '\0' && filnam[0] == '\0')
		getfnam();
	if(expr[0] != '\0')
	{
		if(strlen(expr) > 40)
		{
			error(inst.thing.linno,4);
			return(-1);
		}
		strcpy(filnam,expr);
		postb(filnam);
	}
	if(fetch(0,&ptr) < 0)
	{
		error(inst.thing.linno,5);
		return(-1);
	}
	if((fp=fopen(filnam,"w")) == 0)
	{
		error(inst.thing.linno,4);
		return(-1);
	}
	outit(fp);
	while(fetch(-1,&ptr) ==0)
		outit(fp);
	fclose(fp);
	printf("File saved on %s\n",filnam);
	return 0;
}
outit(fp)
FILE *fp;
{
extern char linbuf[];
char *op;
int i;
	expand();
	fprintf(fp,"%d",inst.thing.linno);
	if((inst.thing.opcode.hibyte&0377) > 0200)
	{
		i=(inst.thing.opcode.hibyte&0377) - 0200;
		while(i--)
			fprintf(fp,"\t");
	}
	else
		fprintf(fp," ");
	op = decode(inst.thing.opcode.lobyte);
	if(op[0]=='\010')
	fprintf(fp,"%s\n",linbuf);
	else
	fprintf(fp,"%s %s\n",op,linbuf);
}
