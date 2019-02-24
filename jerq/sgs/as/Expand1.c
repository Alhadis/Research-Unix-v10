static char ID[] = "@(#) expand1.c: 1.7 5/11/84";

#include <stdio.h>
#include "systems.h"
#include "symbols.h"
#include "expand.h"
#include "gendefs.h"

extern rangetag range[];
extern char idelta[];
extern char pcincr[];


#ifndef	MAXSS
#define MAXSS	200	/* maximum number of Selection set entries */
#endif
#ifndef	MAXSDI
#define MAXSDI	4000	/* maximum number of SDI's we can handle */
#endif
#ifndef	MAXLAB
#define MAXLAB	2000	/* max. number of labels whose address depends on SDI's */
#endif

char islongsdi[MAXSDI];

static ssentry selset[MAXSS];
static symbol *labset[MAXLAB];

unsigned short sdicnt = 0;
static short ssentctr = -1,
		labctr = -1;

static unsigned short PCmax;

extern symbol *dot;
extern long newdot;

update(ssptr,sditype)
	ssentry *ssptr;
	short sditype;
{
	register ssentry *ptr;
	register short cntr;
	register symbol *lptr;
	register symbol **ptr2;
	register unsigned short delta,
		sdipos;
	long instaddr;

	delta = idelta[ssptr->itype];
	sdipos = ssptr->sdicnt;
	instaddr = ssptr->minaddr;
	PCmax -= delta;
	dot->maxval -= delta;

	if (sditype) {	/* nonzero if short */
		instaddr += ssptr->maxaddr;
		for (cntr = ssentctr, ptr = &selset[0]; cntr-- >= 0; ++ptr) {
			if (ptr->sdicnt > sdipos)
				ptr->maxaddr -= delta;
		}
		for (cntr = labctr, ptr2 = &labset[0]; cntr-- >= 0; ) {
			lptr = *ptr2;
			if (lptr->value + lptr->maxval > instaddr) {
				lptr->maxval -= delta;
				if (lptr->maxval == 0) {
					*ptr2 = labset[labctr--];
					continue;
				}
			}
			ptr2++;
		}
	}
	else {	/* long */
		dot->value += delta;
		newdot += delta;
		islongsdi[sdipos] = (char)delta;
		for (cntr = ssentctr, ptr = &selset[0]; cntr-- >= 0; ++ptr) {
			if (ptr->sdicnt > sdipos) {
				ptr->minaddr += delta;
				ptr->maxaddr -= delta;
			}
		}
		for (cntr = labctr, ptr2 = &labset[0]; cntr-- >= 0; ) {
			lptr = *ptr2;
			if (lptr->value > instaddr) {
				lptr->value += delta;
				lptr->maxval -= delta;
				if (lptr->maxval == 0) {
					*ptr2 = labset[labctr--];
					continue;
				}
			}
			ptr2++;
		}
	}
}

/*
 *	"notdone" is used to indicate when pass 1 has been completed.
 *	This helps to detect undefined externals.
 */

static short notdone = YES;

/*
 *	"overflow" is used to indicate when the maximum number of
 *	SDI's have been received (MAXSDI). When "overflow" becomes
 *	non-zero, then only "expand" is called to optimize the
 *	SDI's that have already been received.
 */

static short overflow = NO;

sdiclass(sdiptr)
	register ssentry *sdiptr;
{
	register symbol *lptr;
	register short itype;
	register short ltype;
	register long span;

	lptr = sdiptr->labptr;
	itype = sdiptr->itype;
	if ((ltype = lptr->styp & TYPE) != UNDEF) {
		if (ltype != TXT)
			return(L_SDI);
		span = lptr->value;
	}
	else {
		if (notdone == NO)
			return(L_SDI);
		span = (dot->value != sdiptr->minaddr) ? dot->value :
			sdiptr->minaddr + pcincr[itype];
	}
	span += sdiptr->constant - (sdiptr->minaddr + pcincr[itype]);
	if ((span < range[itype].lbound) ||
		(span > range[itype].ubound))	/* definitely long */
		return(L_SDI);
	else {
		if (ltype != UNDEF) {
			span += (int)(lptr->maxval) -
				((lptr->value > sdiptr->minaddr) ?
				(sdiptr->maxaddr + idelta[itype]) : sdiptr->maxaddr);
			if ((span >= range[itype].lbound) &&
				(span <= range[itype].ubound))	/* definitely short */
				return(S_SDI);
		}
	}
	return(U_SDI);
}

expand()
{
	register short change = YES;
	register short cntr;
	register ssentry *current;
	register short sditype;

	while (change) {
		change = NO;
		for (cntr = ssentctr, current = &selset[0]; cntr-- >= 0; ) {
			if ((sditype = sdiclass(current)) != U_SDI) {

				change = YES;
				update(current,sditype);
				*current = selset[ssentctr--];
			}
			else
				current++;
		}
	}
}

punt()
{
	register short cntr;
	register ssentry *current;
	register unsigned firstsdi;
	register ssentry *outptr;

	firstsdi = sdicnt;
	for (cntr = ssentctr, current = &selset[0]; cntr-- >= 0; ++current) {
		if (current->sdicnt < firstsdi) {
			firstsdi = current->sdicnt;
			outptr = current;
		}
	}
	update(outptr,L_SDI);
	*outptr = selset[ssentctr--];
	expand();
}

newlab(sym)
	register symbol *sym;
{
	static short labwarn = YES;

	if (++labctr == MAXLAB) {
		if (labwarn == YES) {
			werror("Table overflow: some optimizations lost (Labels)");
			labwarn = NO; /* don't warn again */
		}
		labctr--;	/* gone too far, back up */
		while (labctr == MAXLAB - 1) {
			punt();
		}	/* continue to punt until we free a label */
		labctr++;	/* now point to a free area */
	}

	labset[labctr] = sym;
}

deflab(sym)
	register symbol *sym;
{
	sym->maxval = PCmax;
	if (ssentctr >= 0) {
		newlab(sym);
		expand();
	}
}

#if !ONEPROC
extern FILE *fdlong;
#endif

fixsyms()
{
	notdone = NO;
	expand();
#if !ONEPROC
	fwrite(islongsdi,sizeof(*islongsdi),(int)(++sdicnt),fdlong);
#endif
}

sdi(sym,const,itype)
	register symbol *sym;
	long const;
	register short itype;
{
	register long span;

	if (sym) {
		if ((sym->styp & TYPE) == UNDEF)
			return(U_SDI);
		if ((sym->styp & TYPE) != TXT)
			return(L_SDI);
	}
	else
		return(L_SDI);

	span = sym->value + const - (dot->value + pcincr[itype]);
	if ((span < range[itype].lbound) ||
		(span > range[itype].ubound))	/* definitely long */
		return(L_SDI);
	else {
		/*
		 *	Now determine if the instruction is definitely
		 *	short. This calculation is different from the
		 *	similar calculation in "sdiclass" since, as an
		 *	optimization, the long form of the instruction
		 *	is not included in the calculation. This prevents
		 *	determining that the size of an instruction
		 *	is uncertain based on its maximum length. Note
		 *	any previous uncertain instruction lengths
		 *	are reflected by "dot->maxval".
		 */
		span += (long)((int)(sym->maxval) - dot->maxval);
		if ((span >= range[itype].lbound) &&
			(span <= range[itype].ubound))	/* definitely short */
			return(S_SDI);
	}
	return(U_SDI);
}

newsdi(sym,const,itype)
	register symbol *sym;
	long const;
	short itype;
{
	register ssentry *nsdi;

	if (++sdicnt == MAXSDI) {
		overflow = YES;
		sdicnt--;
	}
	if (++ssentctr == MAXSS) {
		ssentctr--;	/* gone too far, back up */
		punt();
		ssentctr++;	/* one sdi was removed, now point to free area */
	}
	nsdi = &selset[ssentctr];
	nsdi->sdicnt = sdicnt;
	nsdi->itype = itype;
	nsdi->minaddr = dot->value;
	nsdi->maxaddr = PCmax;
	nsdi->constant = const;
	nsdi->labptr = sym;
}

shortsdi(sym,const,itype)
	register symbol *sym;
	long const;
	register short itype;
{
	register int sditype;

	if (!overflow) {
		if (sym && ((sym->styp & TYPE) == UNDEF)) {
/*			if (dot->styp != TXT)
/*				return(L_SDI);
/*			sditype = U_SDI;
/*			newsdi(sym,const,itype);
/*			PCmax += idelta[itype];
/*			dot->maxval = PCmax;
*/
			return(L_SDI);
		}
		else {
			if ((sditype = sdi(sym,const,itype)) == U_SDI) {
				if (dot->styp != TXT)
					return(L_SDI);
				if (dot == sym) {
					sditype = S_SDI;
					goto ret;
				}
				newsdi(sym,const,itype);
				PCmax += idelta[itype];
				dot->maxval = PCmax;
			}
		}
	}
ret:	if (ssentctr > 0)
		expand();
	return(sditype);
}
