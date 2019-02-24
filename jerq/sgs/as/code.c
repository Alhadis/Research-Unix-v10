static char ID[] = "@(#) code.c: 1.8 5/3/84";
#include <stdio.h>
#include "systems.h"
#include "symbols.h"
#include "codeout.h"
#include "gendefs.h"

short bitpos = 0;
extern symbol symtab[];
extern short Oflag;	/* debugging flag */
extern long
	commsiz,	/* size of .comment section */
	newdot;		/* up-to-date value of "." */

extern symbol	*dot;
extern unsigned short line;
   
codebuf textbuf[TBUFSIZE],	/* text buffer used in generate */
	databuf[TBUFSIZE];	/* data buffer used in generate */

static short tbufrot=0,		/* counters for these buffers */
	dbufrot=0;

extern FILE *fdtext,	/* files associated with buffers above */
	*fddata,
	*fdcomment;

#if MULTSECT
codebuf	text1buf[TBUFSIZE],	/* additional buffers used in generate */
	text2buf[TBUFSIZE],
	text3buf[TBUFSIZE],
	data1buf[TBUFSIZE],
	data2buf[TBUFSIZE],
	data3buf[TBUFSIZE];

static short	t1bufrot = 0,	/* counters for above buffers */
	t2bufrot = 0,
	t3bufrot = 0,
	d1bufrot = 0,
	d2bufrot = 0,
	d3bufrot = 0;

extern FILE	*fdtxt1,	/* files associated with above buffers */
	*fdtxt2,
	*fdtxt3,
	*fddat1,
	*fddat2,
	*fddat3;

extern char	*filenames[];
#endif

static short anytxt = 0,	/* flag indicating whether any bits were
				   generated in text since last NEWSTMT */
	anydata = 0;		/* same flag for data section	*/
   
/*
 *	generate creates an intermediate file entry from the info
 *	supplied thru its args. The intermediate file format consists
 *	of fixed sized records of the codebuf structure (see codeout.h).
 *	These entries represent the encoded user program and
 *	will be used as input to the second pass of the assembler.
 *	Different files, with their associated buffers, are maintained
 *	for each section, namely text and data .
 */
generate (nbits,action,value,sym)
	BYTE nbits;
	unsigned short action;
	long value;
	symbol *sym;
{
	FILE  *fd;
	register codebuf *bufptr1,    /* points to next available slot */
		*bufptr2;	/* always points to beginning of buffer */
	register short *count;	/* number of elements in buffer */

	switch (dot->styp){
		case TXT:{
			/* Check if any code was generated since last NEWSTMT.
			   If not then no need to output this record	*/
			if (action != NEWSTMT)
				anytxt += nbits; /* indicate code generated */
			else {	if (anytxt)
					anytxt = 0;  /* reset flag */
				else	return;
			}
#if MULTSECT
			switch (dot->sectnum)
			{
			case 0:	fd = fdtext;
				bufptr2 = textbuf;
				bufptr1 = &textbuf[tbufrot++];
				count = &tbufrot;
				break;
			case 1:	fd = fdtxt1;
				bufptr2 = text1buf;
				bufptr1 = &text1buf[t1bufrot++];
				count = &t1bufrot;
				break;
			case 2:	fd = fdtxt2;
				bufptr2 = text2buf;
				bufptr1 = &text2buf[t2bufrot++];
				count = &t2bufrot;
				break;
			case 3:	fd = fdtxt3;
				bufptr2 = text3buf;
				bufptr1 = &text3buf[t3bufrot++];
				count = &t3bufrot;
				break;
			}
#else
			fd = fdtext;
			bufptr2 = textbuf;
			bufptr1 =  &textbuf[tbufrot++];
			count = &tbufrot;
#endif
			break;
		}
		case BSS: {
			if (nbits != 0) {
				yyerror("Attempt to initialize bss");
				return;
			}
			/* otherwise, can write on any file, so treat as DAT */
		}
		case DAT:{
			/* Check if any data was generated since last NEWSTMT.
			   If not then no need to output this record	*/
			if (action != NEWSTMT)
				anydata += nbits; /* indicate data generated */
			else {	if (anydata)
					anydata = 0;  /* reset flag */
				else	return;
			}
#if MULTSECT
			switch (dot->sectnum)
			{
			case 0:	fd = fddata;
				bufptr2 = databuf;
				bufptr1 = &databuf[dbufrot++];
				count = &dbufrot;
				break;
			case 1:	fd = fddat1;
				bufptr2 = data1buf;
				bufptr1 = &data1buf[d1bufrot++];
				count = &d1bufrot;
				break;
			case 2:	fd = fddat2;
				bufptr2 = data2buf;
				bufptr1 = &data2buf[d2bufrot++];
				count = &d2bufrot;
				break;
			case 3:	fd = fddat3;
				bufptr2 = data3buf;
				bufptr1 = &data3buf[d3bufrot++];
				count = &d3bufrot;
				break;
			}
#else
			fd=fddata;
			bufptr2 = databuf;
			bufptr1 = &databuf[dbufrot++];
			count = &dbufrot;
#endif
			break;
		}
	}
	bufptr1->caction = action;
	bufptr1->cnbits = nbits;
	bufptr1->cvalue = value;
#if VAX
	bufptr1->cindex = (int)sym;
#else
	bufptr1->cindex = sym==NULL ? 0 : 1+(sym-symtab);
#endif
	if (*count == TBUFSIZE) {	/* buffer full? */
		fwrite((char *)bufptr2, sizeof(*bufptr2), TBUFSIZE, fd);
		(*count)= 0;
	}
#if DEBUG
 		if (Oflag) {
 			if (bitpos == 0)
 				printf("(%d:%6lx)	",dot->styp,newdot);
 			else
 				printf("		");
 			printf("%hd	%d	%hd	%.13lx	%hd\n",
 				line,(short)nbits,action,value,bufptr1->cindex);
 		}
#endif
	bitpos += nbits;
	newdot += bitpos/BITSPBY;
	bitpos %= BITSPBY;
}
    
flushbuf()
{
	fwrite((char *)textbuf, sizeof(*textbuf), tbufrot, fdtext);
	fwrite((char *)databuf, sizeof(*databuf), dbufrot, fddata);
	tbufrot = 0;
	dbufrot = 0;

#if MULTSECT
	fwrite((char *)text1buf,sizeof(*text1buf),t1bufrot,fdtxt1);
	fwrite((char *)text2buf,sizeof(*text2buf),t2bufrot,fdtxt2);
	fwrite((char *)text3buf,sizeof(*text3buf),t3bufrot,fdtxt3);
	fwrite((char *)data1buf,sizeof(*data1buf),d1bufrot,fddat1);
	fwrite((char *)data2buf,sizeof(*data2buf),d2bufrot,fddat2);
	fwrite((char *)data3buf,sizeof(*data3buf),d3bufrot,fddat3);

	fflush(fdtxt1);
	if (ferror(fdtxt1))
		aerror("trouble writing (text 1); probably out of temp-file space");
	fclose(fdtxt1);
	fflush(fdtxt2);
	if (ferror(fdtxt2))
		aerror("trouble writing (text 2); probably out of temp-file space");
	fclose(fdtxt2);
	fflush(fdtxt3);
	if (ferror(fdtxt3))
		aerror("trouble writing (text 3); probably out of temp-file space");
	fclose(fdtxt3);
	fflush(fddat1);
	if (ferror(fddat1))
		aerror("trouble writing (data 1); probably out of temp-file space");
	fclose(fddat1);
	fflush(fddat2);
	if (ferror(fddat2))
		aerror("trouble writing (data 2); probably out of temp-file space");
	fclose(fddat2);
	fflush(fddat3);
	if (ferror(fddat3))
		aerror("trouble writing (data3); probably out of temp-file space");
	fclose(fddat3);

	if ((fdtxt1 = fopen(filenames[9],"r")) == NULL)
		aerror("Unable to open temporary (text 1) file");

	while ((t1bufrot = fread((char *)text1buf,sizeof(*text1buf),TBUFSIZE,fdtxt1)) == TBUFSIZE)
		fwrite((char *)text1buf,sizeof(*text1buf),TBUFSIZE,fdtext);
	fwrite((char *)text1buf,sizeof(*text1buf),t1bufrot,fdtext);
	t1bufrot = 0;

	if ((fdtxt2 = fopen(filenames[10],"r")) == NULL)
		aerror("Unable to open temporary (text 2) file");

	while ((t2bufrot = fread((char *)text2buf,sizeof(*text2buf),TBUFSIZE,fdtxt2)) == TBUFSIZE)
		fwrite((char *)text2buf,sizeof(*text2buf),TBUFSIZE,fdtext);
	fwrite((char *)text2buf,sizeof(*text2buf),t2bufrot,fdtext);
	t2bufrot = 0;

	if ((fdtxt3 = fopen(filenames[11],"r")) == NULL)
		aerror("Unable to open temporary (text 3) file");

	while ((t3bufrot = fread((char *)text3buf,sizeof(*text3buf),TBUFSIZE,fdtxt3)) == TBUFSIZE)
		fwrite((char *)text3buf,sizeof(*text3buf),TBUFSIZE,fdtext);
	fwrite((char *)text3buf,sizeof(*text3buf),t3bufrot,fdtext);
	t3bufrot = 0;

	if ((fddat1 = fopen(filenames[12],"r")) == NULL)
		aerror("Unable to open temporary (data 1) file");

	while ((d1bufrot = fread((char *)data1buf,sizeof(*data1buf),TBUFSIZE,fddat1)) == TBUFSIZE)
		fwrite((char *)data1buf,sizeof(*data1buf),TBUFSIZE,fddata);
	fwrite((char *)data1buf,sizeof(*data1buf),d1bufrot,fddata);
	d1bufrot = 0;

	if ((fddat2 = fopen(filenames[13],"r")) == NULL)
		aerror("Unable to open temporary (data 2) file");

	while ((d2bufrot = fread((char *)data2buf,sizeof(*data2buf),TBUFSIZE,fddat2)) == TBUFSIZE)
		fwrite((char *)data2buf,sizeof(*data2buf),TBUFSIZE,fddata);
	fwrite((char *)data2buf,sizeof(*data2buf),d2bufrot,fddata);
	d2bufrot = 0;

	if ((fddat3 = fopen(filenames[14],"r")) == NULL)
		aerror("Unable to open temporary (data 3) file");

	while ((d3bufrot = fread((char *)data3buf,sizeof(*data3buf),TBUFSIZE,fddat3)) == TBUFSIZE)
		fwrite((char *)data3buf,sizeof(*data3buf),TBUFSIZE,fddata);
	fwrite((char *)data3buf,sizeof(*data3buf),d3bufrot,fddata);
	d3bufrot = 0;
#endif
}

#if MULTSECT
long	dottxt[4],
	dotdat[4],
#else
long dottxt = 0L,
	dotdat = 0L,
#endif
	dotbss = 0L;

/*
 *	cgsect changes the section into which the assembler is 
 *	generating info , e.g. text or data
 */
#if MULTSECT
cgsect(newtype,newsectnum)
	register short	newtype;
	register short	newsectnum;
#else
cgsect(newtype)
	register short newtype;
#endif
{
	/* determine what section we are currently in and save it */
	switch(dot->styp){
		case TXT:{
#if MULTSECT
			dottxt[dot->sectnum] = newdot;
#else
			dottxt = newdot;
#endif
			break;
		}
		case DAT:{
#if MULTSECT
			dotdat[dot->sectnum] = newdot;
#else
			dotdat = newdot;
#endif
			break;
		}
		case BSS:{
			dotbss = newdot;
			break;
		}
	}

	dot->styp = newtype;	/* change to new type */
#if MULTSECT
	dot->sectnum = newsectnum;
#endif
	/* change to requested section */
	switch(newtype){
		case TXT:{
#if MULTSECT
			newdot = dottxt[newsectnum];
#else
			newdot = dottxt;
#endif
			break;
		}
		case DAT:{
#if MULTSECT
			newdot = dotdat[newsectnum];
#else
			newdot = dotdat;
#endif
			break;
		}
		case BSS:{
			newdot = dotbss;
			break;
		}
	}
	dot->value = newdot;	/* synchronize */
}

void
comment(string)
char *string;
{
	int size;
	size = strlen(string) + 1;
	commsiz += size;
	fwrite(string,size,1,fdcomment);
}
