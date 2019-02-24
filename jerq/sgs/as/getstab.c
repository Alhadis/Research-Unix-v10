static char ID[] = "@(#) getstab.c: 1.4 5/24/83";

/*
 *
 *	"getstab.c"  is  a  file  that contains routines for post-pass1
 *	processing of the symbol table.   The  following  routines  are
 *	provided:
 *
 *	getstab(file)	This  function  reads  the  symbol  table  from
 *			a file whose name  is  given  as  a  parameter.
 *			This is needed by the second and third passes.
 *
 *	fixstab(addr,incr,type)	This  function  "fixes" elements of the
 *			symbol table by adding  "incr"  to  all  values
 *			of symbols of type "type" that are greater than
 *			"addr".   This  is necessary when the length of
 *			of an instruction is increased.
 *
 */

#include <stdio.h>
#include "systems.h"
#include "gendefs.h"
#include "symbols.h"

#if !ONEPROC
/*
 *
 *	"getstab" is a routine that gets the symbol table from an
 *	intermediate file at the beginning of pass 2.  It
 *	opens the file whose name is passed as an argument, and reads
 *	lines of the form:
 *
 *	'action'	'symbol table index'	'nbits'	'value'
 *
 *	It calls "lookup" for each symbol, and stores the "type" and
 *	"value" into the entry returned by "lookup".
 *
 */

extern FILE *fopen();

extern upsymins *lookup();

getstab(file)
	char *file;
{
	FILE	*fd;
	symbol	symb;
	register symbol
		*ptr;

	if((fd = fopen(file,"r"))==NULL)
		aerror("Cannot Open Temporary (symbol table) File");
	while(fread((char *)&symb,SYMBOLL,1,fd) == 1) {
		ptr = (*lookup(symb._name.name,INSTALL,USRNAME)).stp;
		ptr->styp = symb.styp;
		ptr->value = symb.value;
	}
	fclose(fd);
}
#endif

/*
 *
 *	"fixstab" is a function that "fixes" elements of the symbol
 *	table by adding "incr" to the value of all symbols of type
 *	"type" with a value greater than "addr".  This function stores
 *	its parameters into the global variables "fxaddr", "fxincr",
 *	and "fxtype".  It then calls "traverse" causing it to call
 *	"fix" to fix all elements in the table of ordinary symbols,
 *	and calls "ltraverse" causing it to call "lfix" to fix all
 *	elements in the table of compiler generated symbols.
 *
 */

static long fxaddr;
static long fxincr;
static short fxtype;

fix(ptr)
	symbol *ptr;
{
	if (((ptr->styp & TYPE) == fxtype) &&
		(ptr->value >= fxaddr))
		ptr->value += fxincr;
}

fixstab(addr,incr,type)
	long addr;
	long incr;
	short type;
{
	fxaddr = addr;
	fxincr = incr;
	fxtype = type;
	traverse(fix);
}
