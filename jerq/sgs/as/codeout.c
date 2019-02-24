static char ID[] = "@(#) codeout.c: 1.6 5/3/84";
#include <stdio.h>
#include "systems.h"
#include "symbols.h"
#include "codeout.h"
#include "gendefs.h"

extern FILE	*fdsect;	/* file written to by codgen */

extern symbol *dot,
	symtab[];	/* internal symbol table */

long	newdot;		/* up-to-date value of dot */

FILE	*fdcode;	/* temp file containing intermediate language */

#if DCODGEN /* direct code generation of arbitrary number of bytes */

codgen(nbits, val)
	register short nbits;
	long val;
{
	if (nbits) {
		if (nbits % BITSPBY)
			aerror("illegal code generation");
		newdot += nbits / BITSPBY;
		val <<= (sizeof(long)-sizeof(int))*BITSPBY;
		fwrite((char *)&val,(nbits/BITSPBY),1,fdsect);
	};
}

#else

static short
	poscnt = 0,	/* currect bit position in outword buffer */
			/* spans from [0 - BITSPOW]	*/
	bitpos = 0;	/* bit position within a byte in outword buffer	*/
			/* spans from [0 - BITSPBY]	*/

static OUTWTYPE	outword;	/* output buffer - see gendefs.h */
static int mask[] = { 0, 1, 3, 7,
		017, 037, 077,
		0177, 0377, 0777,
		01777, 03777, 07777,
		017777, 037777, 077777,
		0177777 };
codgen(nbits, val)
	register short nbits;
	long val;
{
	register short size;	/* space remaining in outword buffer */
	register long value;	/* working buffer for val */

	if (nbits) {
	    start:	value = val;
		/* generate as much of nbits as possible */
		if ((size = BITSPOW - poscnt) > nbits)
			size = nbits;
		value >>= nbits - size;	/* align value to get correct bits */
		value &= mask[size];
		value <<= BITSPOW - poscnt - size;
		outword |= (OUTWTYPE)value;	/* store field in the buffer */
		poscnt += size;
		bitpos += size;
		newdot += bitpos / BITSPBY;
		bitpos %= BITSPBY;
		if (poscnt == BITSPOW) {	/* is buffer full? */
			OUT(outword,fdsect);	/* see gendefs.h */
			poscnt = 0;
			outword = 0;
			if (nbits > size) {	/* more bits? */
				nbits -= size;
				goto start;
			}
		}
	}
}
#endif	/* DCODGEN */

extern int (*(modes[]))();	/* array of action routine functions */

extern upsymins *lookup();

extern short sttop;

codout(file,start,type)
	char *file;
	long start;
	short type;
{
	codebuf code;
	register symbol *sym;

	dot = (*lookup(".",INSTALL,USRNAME)).stp;
	dot->value = newdot = start;
	dot->styp = type;
	if ((fdcode = fopen(file, "r")) == NULL)
		aerror("Unable to open temporary file");

#ifndef DCODGEN
	poscnt = 0;
#endif
	while (fread((char *)(&code),sizeof(code),1,fdcode) == 1) {
		if (code.caction != 0) {
			if (code.caction <= NACTION) {
#if VAX
			sym = (code.cindex < 0) ? (symbol*)(-code.cindex)
				: (symbol*)(code.cindex);
#else
			sym = code.cindex ? (symtab + (code.cindex-1))
				: (symbol *) NULL;
#endif
			/* call to appropriate action routine */
			(*(modes[code.caction]))(sym,&code);
			}
			else	aerror("Invalid action routine");
		}
		/* do NOACTION case or handle any end-of-action processing */
		codgen((short)(code.cnbits),code.cvalue);
	}

	if (sttop > 0)
		aerror("Unbalanced Symbol Table Entries-Missing Scope Endings");
	fclose(fdcode);
}
