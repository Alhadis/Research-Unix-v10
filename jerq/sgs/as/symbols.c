/*
 * static char ID_smblsc[] = "@(#) symbols.c: 1.5 3/11/83";
 */

#include <stdio.h>
#include <string.h>
#include "syms.h"
#include "systems.h"
#include "symbols.h"
#include "symbols2.h"

/*
 *	"symbols.c" is a file containing functions for accessing the
 *	symbol table.  The following functions are povided:
 *
 *	newent(string)
 *		Creates a new symbol table entry for the symbol with name
 *		"string".  The type of the symbol is set to undefined.
 *		All other fields of the entry are set to zero.
 *
 *	lookup(string,install_flag,user_name_flag)
 *		Looks up the symbol whose name is "string" in the symbol
 *		table and returns a pointer to the entry for that symbol.
 *		Install_flag is INSTALL if symbol is to be installed,
 *		N_INSTALL if it is only to be looked up.  User_name_flag
 *		is USRNAME if the symbol is user-defined, MNEMON if it is
 *		an instruction mnemonic.
 *
 *
 *	traverse(func)
 *		Goes through the symbol table and calls the function "func"
 *		for each entry.
 *
 *	dmpstb()
 *		Dumps the symbol table out to the intermediate file
 *		"fdstab" after pass one. The file descriptor "fdstab"
 *		should be open for writing on the intermediate file that
 *		is to contain the symbol table before calling this
 *		function.  This procedure is only used in the multiple-
 *		process version of the assembler.
 *
 *	creasyms()
 *		Enters the instruction mnemonics found in instab[] into
 *		the symbol table.
 *
 *	addstr(string)
 *		Enters the "string" into the string table.  Called by
 *		newent().  Space for the string table is initially
 *		malloc()-ed in strtabinit().  If "string" would exceed
 *		the available space, then the string table is realloc()-ed
 *		with a larger size.  This procedure is only used in the
 *		flexnames version of the assembler.
 *
 *	strtabinit()
 *		Sets up the string table, with space malloc()-ed.  This
 *		procedure is only used in the flexnames version of the
 *		assembler.
 */

#if ONEPROC
extern short passnbr;
#endif

symbol
	symtab[NSYMS];
upsymins
	hashtab[NHASH];

#if FLEXNAMES
char	*strtab;
long	currindex;

char	*realloc(),
	*malloc();
#endif

short	symcnt = 0;


symbol *
newent(strptr)
	register char *strptr;
{
	register symbol *symptr;
	register char *ptr1;

	if (symcnt >= NSYMS) {
		aerror("Symbol table overflow");
		return(NULL);
	}
	symptr = &symtab[symcnt++];
#if FLEXNAMES
	if (strlen(strptr) > SYMNMLEN)
	{
		symptr->_name.tabentry.zeroes = 0L;
		symptr->_name.tabentry.offset = currindex;
		addstr(strptr);
	}
	else
#endif
		for (ptr1 = symptr->_name.name; *ptr1++ = *strptr++; )
			;
	symptr->styp = UNDEF;
	symptr->value = 0L;
	symptr->tag = 0;
	symptr->maxval = 0;
	return(symptr);
}

#if PASS1 || ONEPROC
#if DEBUG

unsigned numcalls,
	numids,
	numcoll;
#endif
#endif

upsymins *
lookup(sptr,install,usrname)
	char *sptr;
	BYTE install;
	BYTE usrname;
{
	register upsymins
		*hp;
	register char
		*ptr1,
		*ptr2;
	unsigned short register
		ihash = 0,
		hash;
	unsigned short probe;
	upsymins
		*ohp;

	ptr1 = sptr;
#if PASS1 || ONEPROC
#if DEBUG
	numcalls++;
#endif
#endif
	while (*ptr1) {
		ihash = ihash*4 + *ptr1++;
	}
	probe = 1;
	ihash += *--ptr1 * 32;
	hash = ihash % NHASH;
	hp = ohp = &hashtab[hash];
	do {
		if ((*hp).stp == NULL) {	/* free */
			if (install) {
#if PASS1 || ONEPROC
#if DEBUG
				numids++;
#endif
#endif
				(*hp).stp = newent(sptr);
			}
			return(hp);
		}
		else
		{
#if FLEXNAMES
		/* Compare the string given with the symbol string.	*/
		/* The symbol string can be in either the symbol entry	*/
		/* or the string table.					*/
 			if (strcmp(sptr,(hp->stp->_name.tabentry.zeroes != 0) ? (hp->stp->_name.name) : (&strtab[hp->stp->_name.tabentry.offset])) == 0)
#else
			for (ptr1=sptr,ptr2=((*hp).stp)->_name.name; *ptr2==*ptr1++; )
				if (*ptr2++ == '\0')
#endif
			{
#if PASS1 || ONEPROC
#if ONEPROC
			    if (passnbr == 1) {
#endif
					if (install && (*hp).itp->tag &&
						(*hp).itp->snext == NULL)
					{
						(*hp).itp->snext = newent(sptr);
					} /* if (install ...) */
					if (!usrname && (*hp).itp->tag) {
						return((upsymins *) &((*hp).itp->snext));
					} /* if (!usrname ...) */
#if ONEPROC
			        }
					if (!install && !usrname &&
					    (*hp).itp->tag && (passnbr==2))
				            return((upsymins *) &((*hp).itp->snext));
					else
#endif
#endif
				            return(hp); /* found it */
				} /* if (*ptr2++ == '\0') */

			hash = (hash + probe) /*% NHASH*/;
			hash -= (hash >= NHASH) ? NHASH : 0;
			probe += 2;
#if PASS1 || ONEPROC
#if DEBUG
			numcoll++;
#endif
#endif
		} /* else */
	} while ((hp = &hashtab[hash]) != ohp);
	aerror("Hash table overflow");
	return(NULL); /* can't reach here since `aerror' exits */
}

#if !PASS1
traverse(func)
	int (*func)();
{
	register short index;

	for (index=0; index < symcnt; ++index) {
		(*func)(&symtab[index]);
	}
}
#endif
#if PASS1 && !ONEPROC

FILE *fdstab;

dmpstb(){
	fwrite((char *)symtab,symcnt,SYMBOLL,fdstab);
}

#endif
#if PASS1 || ONEPROC

extern instr instab[];

creasyms()
{
	register instr *ip;
        register upsymins *hp;

	for (ip = instab; ip->name[0] != '\0'; ++ip) {
		hp = lookup(ip->name,N_INSTALL,MNEMON);
#if DEBUG
		/* Sanity check is "cpp-ed out" with the flexnames change. */
		if ((*hp).itp == NULL)
#endif
		(*hp).itp = ip;
#if DEBUG
		else
			aerror("Duplicate instruction table name");
#endif
	} /* for */
}
#endif


#if FLEXNAMES
long	size,
	basicsize = 4 * BUFSIZ;

addstr(strptr)
	register char	*strptr;
{
	register int	length;

	length = strlen(strptr);
	if (length + currindex >= size)
		if ((strtab = realloc(strtab,size += basicsize)) == NULL)
			aerror("cannot realloc string table");
	strcpy(&strtab[currindex],strptr);
	currindex += length + 1;
}	/* addstr(strptr) */


strtabinit()
{
	if ((strtab = malloc(size = basicsize)) == NULL)
		aerror("cannot malloc string table");
	currindex = 4;
}	/* strtabinit() */
#endif
