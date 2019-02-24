#include "dep.h"
#include <math.h>

/* memory-related definitions and functions */
#define	DEPHUNKSIZE	128

typedef struct _deparena {
	struct _deparena *next;
	deplist arena[DEPHUNKSIZE];
} deparena;

static deparena *toparena = NULL;

static deplist *newdeplist ()
{
	static int nextdep;
	deparena *temp;
	if (toparena == NULL) {
		toparena = (deparena *) calloc (1, sizeof(deparena));
		nextdep = DEPHUNKSIZE - 1;
	} else if (nextdep < 0) {
		temp = toparena;
		toparena = (deparena *) calloc (1, sizeof(deparena));
		toparena->next = temp;
		nextdep = DEPHUNKSIZE - 1;
	}
	return &(toparena->arena[nextdep--]);
}

deplist *depgen (c, x)
double c;
variable *x;
{
	register deplist *dummy;
	dummy = newdeplist ();
	COEFF(dummy) = c;
	VAR(dummy) = x;
	return dummy;
}

void depfree (p)
deplist *p;
{
}

void depclean ()
{
	deparena *temp;
	while (toparena) {
		temp = toparena->next;
		free (toparena);
		toparena = temp;
	}
}

/* functions for dependency list manipulation */

deplist *depadd(coeffone, dlistone, coefftwo, dlisttwo)
float coeffone;
deplist *dlistone;
float coefftwo;
deplist *dlisttwo;
{
	/* produce a dependency list = coeffone*dlistone + coefftwo*dlisttwo */
	register deplist *onewalk, *twowalk, *newhead, *newwalk, *prevnew;
	deplist nuhead;
	prevnew = &nuhead;
	prevnew->next = NULL;
	onewalk = dlistone;
	twowalk = dlisttwo;
	while ((onewalk != NULL) || (twowalk != NULL)) {
		if (onewalk != NULL)
			if (twowalk != NULL)
				if (VAR(onewalk) > VAR(twowalk )) {
					newwalk = depgen (
					    coeffone*COEFF(onewalk),
					    VAR(onewalk)
					    );
					onewalk = NEXT(onewalk);
				}
				else
					if (VAR(onewalk) == VAR(twowalk)) {
						newwalk = depgen (
						    coeffone*COEFF(onewalk)
						    + coefftwo*COEFF(twowalk),
						    VAR(onewalk)
						    );
						onewalk = NEXT(onewalk);
						twowalk = NEXT(twowalk);
					}
					else {
						newwalk = depgen (
						    coefftwo*COEFF(twowalk),
						    VAR(twowalk)
						    );
						twowalk = NEXT(twowalk);
					}
			else {
				newwalk = depgen (
				    coeffone*COEFF(onewalk),
				    VAR(onewalk)
				    );
				onewalk = NEXT(onewalk);
			}
		else {
			newwalk = depgen (
			    coefftwo*COEFF(twowalk),
			    VAR(twowalk)
			    );
			twowalk = NEXT(twowalk);
		}
		if ((fabs(COEFF(newwalk))) > EPSILON) {
			NEXT(prevnew) = newwalk;
			prevnew = newwalk;
		} else
			depfree (newwalk);
	}
	newhead = nuhead.next;
	if (newhead != NULL)
		return(newhead);
	else
		return (depgen (0.0, (variable *) NULL));
}

deplist *depsubst(depinto, depfrom, depwho)
/* substitutes depfrom for depwho in depinto
	/* WARNING:  if depinto actually contains depfrom,
	/* depinto is replaced */
deplist *depinto, *depfrom;
variable *depwho;
{
	deplist *intowalker, *intoparent, *temp;
	for (intowalker = depinto;
	    intowalker != NULL;
	    intowalker = NEXT(intowalker))
		if (VAR (intowalker) == depwho)
			break;
		else
			intoparent = intowalker;
	if (intowalker == NULL)
		return(depinto);
	if (intowalker == depinto)
		depinto = NEXT(depinto);
	else
		NEXT(intoparent) = NEXT(intowalker);
	temp = depadd(1.0, depinto, COEFF(intowalker), depfrom);
/*
	depfree (depinto);
	tryfree(intowalker);
*/
	depinto = temp;
	return (temp);
}

/* functions related to variable manipulation */

static variable *depvarlist;

void depinit ()
{
	depvarlist = NULL;
}

void removedeps (v)
variable *v;
{
	/* Remove all explicit dependencies on v.
	*/
	variable *p;
	v->nextdep = depvarlist;
	depvarlist = v;
	for (p = depvarlist; p; p = p->nextdep)
		p->nrep = depsubst (p->rep, v->nrep, v);
}

void resetreps ()
{
	/* Accept the new dependencies.
	*/
	variable *p;
	for (p = depvarlist; p; p = p->nextdep)
		p->rep = p->nrep;
}

