static char ID[] = "@(#) regions.c: 1.6 8/15/83";

#include <stdio.h>
#include "system.h"
#include "structs.h"
#include "extrns.h"
#include "list.h"
#include "params.h"
#include "sgsmacros.h"

void
bldreglist()
{
	register REGION  *rp;
	register ANODE *ap;
	ANODE	*newap,
		*splitnode();
/*
 * Now collate the regions into the available space list
 */

	for( rp = (REGION *) reglist.head; rp != NULL; rp = rp->rgnext ) {
		/*
		 * Map start of region to an avail space node
		 */
		ap = findnode(rp->rgorig, 1);

		if( (int) ap ==  -1 ) {
			lderror(1,0,NULL, "region %.8s outside of configured memory",
				rp->rgname);
			continue;
			}

		if( ap->adpaddr == rp->rgorig ) {
			/*
			 * Region starts on node boundry
			 */
			rp->rgaddrhd = ap;
			}
		else if( ap->adpaddr < rp->rgorig ) {
			/*
			 * Region starts within avail node
			 */
			rp->rgaddrhd = splitnode(ap,rp->rgorig);
			}
		else {
			/*
			 * Region starts between or before a node
			 */
			if( rp->rgorig+rp->rglength <= ap->adpaddr ) {
				lderror(1,0,NULL, "region %.8s is in unconfigured memory",
					rp->rgname);
				continue;
				}
			else
				rp->rgaddrhd = ap;
			}

		/*
		 * Now map the end of the region to an avail space node
		 */
		ap = findnode(rp->rgorig+rp->rglength - 1, 0);
		if( (int) ap  ==  -1 )
			/*
			 * If not found
			 */
			ap = (ANODE *) avlist.tail;

		if( rp->rgorig+rp->rglength < ap->adpaddr+ap->adsize ) {
			/*
			 * Region ends within the node
			 */
			splitnode(ap, rp->rgorig+rp->rglength);
			rp->rgaddrtl = ap;
			}
		else if( rp->rgorig+rp->rglength==ap->adpaddr+ap->adsize ) {
			/*
			 * Region ends at node boundry
			 */
			rp->rgaddrtl = ap;
			}
		else {
			/*
			 * Region ends after ap
			 */
			rp->rgaddrtl = ap;
			}

		/*
		 * Set region ptrs within address space subsequence to rp
		 */
		for( newap = rp->rgaddrhd; newap != rp->rgaddrtl->adnext; newap = newap->adnext )
			newap->adregp = rp;

		}
 
}
/*eject*/
bld_regions()
{

/*
 * Builds regions for the output sections of a tv load.
 *
 * This function does nothing if the user specified REGIONS
 * directives via an ifile.
 *
 * Rule 1: All output sections defined as members of a GROUP
 *		will go into the same region.
 * Rule 2: The output sections .data and .bss will go into the same
 *		region.
 * Rule 3: All other output sections will go into a separate region
 *		by itself.
 */

	REGION	*rp,		/* working pointer		*/
		*drp,		/* ptr to data/bss region	*/
		*trp,		/* ptr to tv region		*/
		*newregion();
	OUTSECT *osp,		/* working pointer		*/
		*dosp,		/* ptr to .data output section	*/
		*bosp,		/* ptr to .bss output section	*/
		*tosp;		/* ptr to .tv output section	*/
	long	paddr,		/* phys addr for regions	*/
		vaddr,		/* virtual address		*/
		length;

	dosp = bosp = tosp = NULL;

/*
 * For each output section, build or amend the appropriate region
 *
 * The outsclst contains, at this point, entries which correspond to
 * output GROUPs and independent output sections
 */

	for( osp = ((OUTSECT *) outsclst.head); osp != NULL; osp = osp->osnext ) {
		if(equal(osp->oshdr.s_name,_DATA,8))
			dosp = osp;
		else if(equal(osp->oshdr.s_name,_BSS,8))
			bosp = osp;
		else if(equal(osp->oshdr.s_name,_TV,8))
			tosp = osp;
		else {
			/*
			 * Since every output section is going to be a
			 * region, we have to align their virtual zeroes
			 * to a 16-byte boundry.
			 * 
			 * That is done by changing the virtual address.
			 *
			 * If the resultant section is too big, it is an
			 * error.
			 */
			paddr = osp->oshdr.s_paddr;
			vaddr = paddr & 0xfL;
			length = osp->oshdr.s_size;

			if( length + vaddr <= REGSIZE ) {	/* size is ok */
				rp = newregion(osp->oshdr.s_name);
				rp->rgorig = paddr;
				rp->rgvaddr = vaddr;
				rp->rglength = length;
				rp->rgaddrhd = rp->rgaddrtl = findsanode(osp);
				if (rp->rgaddrhd)
					rp->rgaddrhd->adregp = rp;
				listadd(l_REG, &reglist, rp);
				}
			else
				lderror(1,0,NULL, "default allocation failed: %.8s is too large",
					osp->oshdr.s_name);
			}
		}

/*
 * Assign physical address to .data/.bss and .tv regions
 */

	if(  dosp  ||   bosp ) {
		/*
		 * We don't know which of the two exist, so we
		 * set both equal if one is NULL.  Then we arrange
		 * it to look like .data has the lower address for
		 * simplicity.  We do NOT change the actual addresses.
		 */
		if( ! dosp  )
			dosp = bosp;
		else if( ! bosp )
			bosp = dosp;

		paddr = dosp->oshdr.s_paddr;

		if( bosp->oshdr.s_paddr < paddr ) {
			paddr = bosp->oshdr.s_paddr;
			osp = bosp;
			bosp = dosp;
			dosp = osp;
			}

		length = bosp->oshdr.s_size + (bosp->oshdr.s_paddr - paddr);

		vaddr = paddr & 0xfL;
		if( (length + vaddr) <= REGSIZE ) {
			drp = newregion(".rdata");
			drp->rgorig = paddr;
			drp->rgvaddr = vaddr;
			drp->rglength = length;
			drp->rgaddrhd = findsanode(dosp);
			drp->rgaddrtl = findsanode(bosp);
			if (drp->rgaddrhd)
				drp->rgaddrhd->adregp = drp;
			if (drp->rgaddrtl)
				drp->rgaddrtl->adregp = drp;
			listadd(l_REG, &reglist, drp);
			}
		else {
			if( bosp != dosp )
				lderror(1,0,NULL,
					"default allocation didn't put %.8s and %.8s into same region",
					_DATA, _BSS);
			else
				lderror(1,0,NULL, "default allocation failed for %.8s",
					dosp->oshdr.s_name);
			}
		}

	if( tosp != NULL ) {
		paddr = tosp->oshdr.s_paddr;
		if( (paddr & 0xfL) != 0L ) {
			lderror(2,0,NULL, "internal error: %.8s not aligned",
				_TV);
			return;
			}

		trp = newregion(".rtv");
		trp->rgorig = paddr;
		trp->rgvaddr = 0L;
		trp->rglength = tosp->oshdr.s_size;
		trp->rgaddrhd = trp->rgaddrtl = findsanode(tosp);
		if (trp->rgaddrhd)
			trp->rgaddrhd->adregp = trp;
		listadd(l_REG, &reglist, trp);
		}
	else if( aflag )
		lderror(1,0,NULL, "internal error: %.8s not built",
			_TV);
}
/*eject*/
REGION *
newregion(name)
char *name;
{
	register REGION *rp;

	rp = (REGION *) myalloc(sizeof(REGION));

	copy(rp->rgname, name, 8);

	return(rp);
}

add_owner(osp,rp)
OUTSECT *osp;
REGION *rp;
{
	register ACTITEM *aip;

	aip = (ACTITEM *) myalloc(sizeof(ACTITEM));

	aip->dfownr.aitype = AIDFOWNR;
	aip->dfownr.aioutsec = osp;
	copy(aip->dfownr.ainamown,rp->rgname,8);

	listadd(l_AI,&ownlist,aip);
}

/*
 *	The following function [ alc_iand() ] is
 *	executed only if IANDD && USEREGIONS
 */

#if IANDD && USEREGIONS

alc_iandd()
{

/*
 * Perform the function specified by the "-i" flag:
 *	generate two regions, one for text (I space) and one for
 *	data (D space)
 */

	REGION	*trp, *drp;
	ANODE	*ap1, *ap2;
	MEMTYPE	*mp;
	OUTSECT	*sp;
	short	reg_count, squishable;
	long	newsize;

/*
 * Make sure that there are only two regions
 */

	reg_count = 0;
	for( trp = (REGION *) reglist.head; trp != NULL; trp = trp->rgnext )
		reg_count++;
	if(reg_count != 2) {
		lderror(0,0,NULL, "i flag ignored: %d regions specified",
			reg_count);
		return;
		}

/*
 * Make sure that no memory attribute of T was specified
 */

	for( mp = (MEMTYPE *) memlist.head; mp != NULL; mp = mp->mtnext ) {
		if(mp->mtattr == 4) {
			lderror(0,0,NULL, "i flag ignored: text memory specified");
			return;
			}
		}

/*
 * Allocate all input .text sections into the first region.
 */

	trp = (REGION *) reglist.head;
	for( sp = (OUTSECT *) outsclst.head; sp != NULL; sp = sp->osnext ) {
		if( equal(sp->oshdr.s_name,_TEXT,8)  &&
		    (sp->oshdr.s_paddr == -1L) ) {
			if(can_alloc(trp->rgaddrhd,trp->rgaddrtl,sp,
					&ap1,&ap2))
				do_alloc(ap1,ap2,sp);
			else
				lderror(1,0,NULL, ".text section won't fit in text region");
			}
		}

/*
 * If necessary, see about moving the data regions down to the 
 * even 4-byte boundry immediately following the end of the
 * allocated text
 */

	drp = trp->rgnext;
	squishable = 1;
	if( (drp->rgaddrhd != drp->rgaddrtl)  ||
	     (drp->rgaddrhd->adtype != ADAVAIL) )
		squishable = 0;

	if(squishable) {
		ap2 = drp->rgaddrhd;
		newsize = 0L;
		while( (ap2->adprev->adtype == ADAVAIL)  &&
		      (ap2->adprev->adpaddr + ap2->adprev->adsize == ap2->adpaddr)  &&
		      (ap2->adprev->adsize != 0) ) {
			ap2 = ap2->adprev;
			newsize += ap2->adsize;
			ap2->adregp = NULL;
			}
		if(newsize > 0L) {
			trp->rgaddrtl = ap2->adprev;
			trp->rglength -= newsize;
			drp->rgaddrhd->adpaddr = ap2->adpaddr;
			ap2->adprev->adnext = drp->rgaddrhd;
			drp->rgaddrhd->adprev = ap2->adprev;
			ap2 = drp->rgaddrhd;
			/*
			 * If the first anode is aligned on a
			 * 16-byte boundary, the region will
			 * have a vaddr of zero.  We can't let
			 * this happen; zero is NULL in C.
			 *
			 * Force virtual address to non-zero.
			 */
			if( (ap2->adpaddr & 0xf) == 0 ) {
				ap2->adpaddr += 2;
				ap2->adsize -= 2;
				drp->rglength -= 2;
				}
			else
				if( (ap2->adpaddr & 1) != 0 ) {
					++ap2->adpaddr;
					--ap2->adsize;
					--drp->rglength;
					}

			drp->rgorig = ap2->adpaddr;
			drp->rgvaddr = ap2->adpaddr & 0xf;
			if( drp->rgvaddr + drp->rglength > REGSIZE )
				drp->rglength = REGSIZE - drp->rgvaddr;
			}
		}

/*
 * Now allocate .data and .bss into RDATA.
 */

	for( sp = (OUTSECT *) outsclst.head; sp != NULL; sp = sp->osnext ) {
		if( (equal(sp->oshdr.s_name,_DATA,8) || equal(sp->oshdr.s_name,_BSS,8))
			&& (sp->oshdr.s_paddr == -1L) )
			if( can_alloc(drp->rgaddrhd, drp->rgaddrtl, sp, &ap1, &ap2) )
				do_alloc(ap1, ap2, sp);
			else
				lderror(1,0,NULL, "%.8s section won't fit in data region",
					sp->oshdr.s_name);
		}
}
#endif
