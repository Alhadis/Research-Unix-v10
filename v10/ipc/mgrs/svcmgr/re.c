#include <setjmp.h>
#include <stdio.h>
#include <ctype.h>
#include <libc.h>
#include "mgr.h"

/*
 *  Compressed regular expressions.  A single lists represents all regular
 *  expressions.  Whenever a new one is created, the list is searched for an
 *  equivalent re.  If one is found, it is returned and its reference count
 *  incremented.  If the regular expression is a simple string, it is not
 *  compiled and strcmp is used to check for equivalence.
 */

Cregexp *relist;	/* the list */

/*
 * trap errors from recomp and regexec
 */
jmp_buf rxerr;

regerror(s)
char *s;
{
	logevent("regex error: %s\n", s);
	longjmp(rxerr, 1);
}

/*
 *  compile a regular expression inserting a ^ at the beginning and
 *  a $ at the end.
 */
Cregexp *
newre(re)
	char *re;
{
	char fullre[ARB];
	register Cregexp *rp;
	int i;

	/*
	 *  look for a previous version of the same
	 */
	for(rp=relist; rp; rp=rp->next)
		if(strcmp(rp->re, re)==0){
			rp->ref++;
			return(rp);
		}

	/*
	 *  alloc a new one
	 */
	rp = (Cregexp *)malloc(sizeof(Cregexp));
	if(rp == NULL)
		return((Cregexp *)0);
	rp->re = strdup(re);
	rp->ref = 1;
	rp->prog = NULL;
	if(setjmp(rxerr)){
		freere(rp);
		return((Cregexp *)0);
	}

	/*
	 *  add the ^$
	 */
	if(*re != '^')
		strcpy(fullre, "^");
	else
		fullre[0] = 0;
	strcat(fullre, re);
	i = strlen(fullre);
	if(fullre[i-1] != '$')
		strcat(fullre, "$");

	/*
	 *  compile the re
	 */
	rp->prog = regcomp(fullre);
	if(rp->prog == NULL){
		freere(rp);
		return((Cregexp *)0);
	}

	/*
	 *  thread into list 
	 */
	rp->next = relist;
	relist = rp;
	return(rp);
}

/*
 *  free a regular expression
 */
freere(rp)
	Cregexp *rp;
{
/*
 *  This code is left if the need arises.  Normally, it is cheaper
 *  in space to not free re's since most are eventually reused.
	Cregexp *trp;

	if(--(rp->ref) > 0)
		return;
	free(rp->re);
	if(rp->prog != NULL)
		free((char *)(rp->prog));
	if(relist == rp)
		relist = rp->next;
	else {
		for(trp=relist; trp->next!=rp; trp=trp->next)
			;
		trp->next = rp->next;
	}
	free((char *)rp);
*/
}

/*
 *  execute a regular expression
 */
execre(rp, string, sub, nsub)
	Cregexp *rp;
	char *string;
	regsubexp *sub;	/* subexpression elements */
	int nsub;	/* number of elements pointed to by sub */
{
	int rv;

	if(rp->prog==NULL){
		rv = strcmp(string, rp->re)==0;
		if(rv && sub && nsub>1){
			sub[0].sp = string;
			sub[0].ep = string+strlen(string);
		}
		return(rv);
	} else
		return(regexec(rp->prog, string, sub, nsub));
}
