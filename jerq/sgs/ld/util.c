static char ID[] = "@(#) util.c: 1.5 5/27/83";
#include "system.h"

#include <stdio.h>
#include <signal.h>
#include "structs.h"
#include "extrns.h"
#include "sgs.h"
#include "sgsmacros.h"

#define INT int
#define BUSY 1
#define testbusy(p) ((INT)(p)&BUSY)
/*eject*/
ACTITEM *
dfnscngrp(type, bondaddr, align, block)
int type;
ADDRESS *bondaddr, *align, *block;
{

/*
 * Build a AIDFNSCN/AIDFNGRP action item
 */

	ACTITEM *p;

	p = (ACTITEM *) myalloc(sizeof(ACTITEM));

	p->dfnscn.aiinflnm = curfilnm;
	p->dfnscn.aiinlnno = lineno;
	p->dfnscn.aitype = type;
	p->dfnscn.aibndadr = -1L;
	p->dfnscn.aialign  =  0L;
	p->dfnscn.aiblock  =  0L;

	if ( bondaddr != NULL ) {	/* there is a bond */
		p->dfnscn.aibndadr = *bondaddr;
		}

	if (align != NULL) {		/* there is alignment */
		p->dfnscn.aialign = *align;
		chkpower2(p->dfnscn.aialign);
		}

	if (block) {
		p->dfnscn.aiblock = *block;
		chkpower2(*block);
		}

	return(p);
}
/*eject*/
chkpower2(n)
long n;
{

/*
 * Determine if 'n' is a power of two
 */

	long c;

	for( c = n; c > 2; c >>= 1 )
		if( (c % 2) == 1 ) {
			lderror(0, lineno, curfilnm, "%.1lx is not a power of 2", n );
			break;
			}
}
/*eject*/
char *
zero(ptr, cnt)
char *ptr;
int cnt;
{

/*
 * Zero "cnt" bytes starting at the address "ptr".
 * Return "ptr".
 */

	register char *p, *w;

	if( cnt > 0 ) {
		if( ((short) (p = ptr)) & 1 ) {
			*p++ = '\0';
			--cnt;
			}

		w = p + (cnt & (~1));
		while (p < w) {
			*((short *) p) = 0;
			p += sizeof(short);
			}

		if (cnt & 1)
			*p = '\0';
		}
	
	return( ptr );
}
/*eject*/
zerofile(fildes,bptr,bsiz)
	FILE		*fildes;	/* write opened file */
	unsigned	*bptr;		/* zero filled buffer */
	int		bsiz;		/* size of buffer */
{
	long	file_end;
	OUTSECT	*osptr;
/*
 * Insure that fildes is zero filled up to the point
 * of symbol table origin, or up to the last section if the symbol
 * table is stripped. Under TS, we simply seek to the point
 * and write one unsigned. Under RT or DMERT we must write 0's
 * throughout the entire space.
 */
#if PAGING
	if (Fflag || !sflag)
#else
	if (!sflag)
#endif
		file_end = symtborg - 4;
	else
	{
		file_end = 0L;
		for (osptr = (OUTSECT *) outsclst.head; osptr; osptr = osptr->osnext)
			file_end = max( file_end, osptr->oshdr.s_scnptr );

	}

#if RT
	rewind(fildes);
	while ( file_end > ftell(fildes) ) 
		fwrite( (char*) bptr, bsiz, 1, fildes);
#else
	fseek(fildes, file_end, 0);
	fwrite( (char *) bptr, sizeof(long), 1, fildes);
#endif
	rewind(fildes);		/* leave everythiing as we found it */

	return;
}
/*eject*/
char *
sname(s)
char *s;
{

/*
 * Returns pointer to "simple" name of path name; that is,
 * pointer to first character after last '/'.  If no slashes,
 * returns pointer to first char of arg.
 *
 * If the string ends in a slash:
 *	1. replace the terminating slash with '\0'
 *	2. return a pointer to the first character after the last
 *		'/', or the first character in the string
 */

	register char *p;

	p = s;
	while( *p )
		if(*p++ == '/')
			if( *p == '\0' ) {
				*(p-1) = '\0';
				break;
				}
			else
				s = p;

	return(s);
}
/*eject*/
char *
myalloc(nbytes)
int nbytes;
{

/*
 * Allocate "nbytes" of memory, and exit from ld
 * upon failure.
 */

	register char *mem;
	extern char *calloc();

	if( (mem = calloc(1,nbytes)) == NULL ) {
		lderror(0,0,NULL, "memory allocation failure on %d-byte 'calloc' call",
			nbytes);
		lderror(2,0,NULL, "%s run is too large and complex", SGS );
		}

	allocspc += (long) nbytes;
	maxalloc = max(maxalloc,allocspc);

	return(mem);
}


stoi(p)
char *p;
{

/*
 * Given a string of digits, perhaps beginning with "0" or "0x",
 * convert to an integer
 *
 * Return 0x8000 on any type of error
 */

	register int base, num;
	register char limit;

	num = 0;
	
	if (*p == '0') {
		if (*++p == 'x' || *p == 'X') {		/* string is hex */
			for (p++; *p; p++) {
				num <<= 4;
				if (*p >= '0' && *p <= '9')
					num += *p - '0';
				else if (*p >= 'a' && *p <= 'f')
					num += *p - 'a' + 10;
				else if (*p >= 'A' && *p <= 'F')
					num += *p - 'A' + 10;
				else return(0x8000);
				}
			return (num);
			}
		else {
			base = 8;
			limit = '7';
			}
		}
	else {
		base = 10;
		limit = '9';
		}

	for ( ; *p; p++)
		if( (*p >= '0')  &&  (*p <= limit) )
			num = num * base + *p - '0';
		else
			return(0x8000);

	return(num);
}
/*eject*/
copybyts(src,dest,len)
char *src, *dest;
int len;
{

/*
 * Copy len bytes from src to dest
 */
	
	while( len-- > 0 )
		*dest++ = *src++;
}
/*eject*/
/*VARARGS*/
lderror(lvl, ln,fln,strng, a1,a2,a3,a4,a5,a6,a7,a8,a9,aa)
int lvl;		/* error severity level */
int ln;			/* line in ifile containing the error */
char *fln;		/* name of the ifile containing the error */
char *strng;		/* error message format */
int a1,a2,a3,a4,a5,	/* error message format arguments */
    a6,a7,a8,a9,aa;
{

/*
 *  Prepend a filename and a line number to an error message.
 *  used for printing all parsing-related errors.
 */

	errlev = (errlev < lvl ) ? lvl : errlev;

	if( Sflag  &&  (lvl != 2) )
		return;

	fprintf(stderr, "%sld", SGS);
	if (fln != NULL)
		fprintf(stderr, " %s", sname(fln));
	if ( ln > 0 )
		fprintf(stderr, " %d", ln) ;

	switch(lvl) {
	case 0:
		fprintf(stderr, " warning: ");
		break;
	case 2:
		fprintf(stderr, " fatal: ");
		break;
	default:
		fprintf(stderr, ": ");
		break;
	}

	fprintf(stderr, strng, a1, a2, a3, a4, a5, a6, a7, a8, a9, aa);
	fprintf(stderr, "\n");
	fflush(stderr);

	if(lvl > 1)
		ldexit();
}

