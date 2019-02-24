static char ID[] = "@(#) alloc.c: 1.7 5/27/83";
#include "system.h"

#include <stdio.h>
#include "structs.h"
#include "extrns.h"
#include "list.h"
#include "params.h"
#include "sgsmacros.h"
#include "ldmacros.h"

#if TRVEC
#include "sdpsrc/hd/define2.h"
#include "tv.h"
#include "ldtv.h"
#endif

#define L15	0xfL
/*eject*/
alloc()
{

/*
 * Perform the allocation of the output sections into the configured
 * memory
 */

	bldmemlist();	/* build initial address space from memlist	*/

#if USEREGIONS
	bldreglist();	/* collate regions into available space		*/
#endif

	alc_bonds();	/* allocate bonded output sections		*/

#if DEBUG
	if( dflag > 1 ) {
		fprintf(stderr, "\n\nalc_bonds");
		dump_mem();
		}
#endif

	alc_owners();	/* allocate out sects assigned to owners	*/
	alc_attowns();	/* allocate out sects assigned to attributes	*/

#if DEBUG
	if( dflag > 1 ) {
		fprintf(stderr, "\n\nalc_own, alc_att");
		dump_mem();
		}
#endif

#if IANDD && USEREGIONS
	if( iflag  &&  ( ! tvflag ) )
		alc_iandd();	/* separate I and D */
#endif

	alc_the_rest();		/* allocate everything as yet unallocated	*/

#if DEBUG
	if( dflag > 1 ) {
		fprintf(stderr, "\n\nalc_iandd, alc_rest");
		dump_mem();
		}
#endif

#if USEREGIONS
	if (tvflag && (reglist.head == NULL))
		bld_regions();
#endif

	audit_groups();

	audit_regions();

#if DEBUG
	if( dflag > 1 ) {
		fprintf(stderr, "\nbld_reg, audit_grp, audit_reg");
		dump_mem();
		}
#endif

	if (aflag)
		set_spec_syms();   /* generate special symbols for absolute load */

}
/*eject*/
bldmemlist()
{

	register MEMTYPE *mp;
	register ANODE *ap;

/*
 * Build the initial address space from the memlist
 */

	for( mp = (MEMTYPE *) memlist.head; mp != NULL; mp = mp->mtnext ) {
		ap = newnode();
		ap->adpaddr = mp->mtorig;
		ap->adsize  = mp->mtlength;
		ap->admemp  = mp;
		mp->mtaddrhd=mp->mtaddrtl = ap;
		listadd(l_ADR, &avlist, ap);
		}

#if DEBUG
	if( dflag )
		dump_mem();
#endif

}
/*eject*/
alc_bonds()
{

/*
 * Process the bond.list, allocating all output sections which have
 * been bonded to specific addresses
 */

	ACTITEM *aip,		/* ptr to bonded section */
		*nextaip;	/* ptr to next item on bond.list */
	ANODE	*sap,		/* ptr to memory node containing bond address */
		*eap, *newap, *splitnode();
	OUTSECT *osp,		/* ptr to output section description for the bonded section */
		*bsp;
	ADDRESS lastaddr, have, need;

	for( aip = (ACTITEM *) bondlist.head; aip != NULL; aip = nextaip ) {
		nextaip = aip->bond.ainext;
		osp = aip->bond.aioutsec;
		/*
		 * DSECT sections are bonded at the requested address,
		 * but are not allocated any memory
		 */
		if( osp->oshdr.s_flags & STYP_DSECT) {
			osp->oshdr.s_paddr = osp->oshdr.s_vaddr = aip->bond.aiadrbnd;
			listadd(l_DS, &dsectlst, osp);
			free(aip);
			continue;
			}
		/*
		 * For the current bonded section, find out in which of
		 * the space nodes the bond address is located
		 */
		for( sap = (ANODE *) avlist.head; sap != NULL; sap = sap->adnext ) {
			if( sap->adpaddr+(ADDRESS)sap->adsize > aip->bond.aiadrbnd)
				break;
			}

		if(sap == NULL) {
			lderror(1, aip->bond.aiinlnno, aip->bond.aiinflnm,
				"bond address %.2lx for %.8s is outside all configured memory",
				aip->bond.aiadrbnd, (*(osp->oshdr.s_name) == '\0') ? "GROUP" : osp->oshdr.s_name);
			free(aip);
			continue;
			}
		/*
		 * If an space node exists for the bond address, sap
		 * points to it
		 */
		if( sap->adpaddr > aip->bond.aiadrbnd ) {
			lderror(1, aip->bond.aiinlnno, aip->bond.aiinflnm,
				"bond address %.2lx for %.8s is not in configured memory",
				aip->bond.aiadrbnd, (*(osp->oshdr.s_name) == '\0') ? "GROUP" : osp->oshdr.s_name);
			free(aip);
			continue;
			}
		/*
		 * Make sure the space node is of type ADAVAIL (i.e, that
		 * it is unallocated memory
		 */
		if(sap->adtype != ADAVAIL) {
			bsp=sap->adscnptr;
			lderror(1, aip->bond.aiinlnno, aip->bond.aiinflnm,
				"bond address %.2lx for %.8s overlays previously allocated section %.8s at %.2lx",
				aip->bond.aiadrbnd, (*(osp->oshdr.s_name) == '\0') ? "GROUP" : osp->oshdr.s_name,
				bsp->oshdr.s_name, sap->adpaddr);
			free(aip);
			continue;
			}
		/*
		 * If the bond address is not the start of an AVAIL
		 * node, split the AVAIL node into two nodes. The
		 * first will be AVAIL, the second SECT
		 */
		if( sap->adpaddr < aip->bond.aiadrbnd ) {
			newap = splitnode(sap,aip->bond.aiadrbnd);
			sap = newap;
			}
		/*
		 * Make sure the entire bonded section will fit into memory
		 *	1. The bonded section must fit into configured memory
		 *	2. Each space node into which the section falls
		 *		must be of type AVAIL
		 *	3. Each space node must be contiguous
		 */
		have = sap->adpaddr + sap->adsize;
		/*
		 * special processing for special people:
		 *	if one needs to allocate more memory
		 *	than is explicitly demanded, do
		 *	so here [ in special.c ]
		 */

		need = sap->adpaddr + osp->oshdr.s_size;
		adjneed(&need, osp, sap);
		eap = sap;
		lastaddr = sap->adpaddr;
		while ( have < need ) {
			lastaddr += eap->adsize;
			eap = eap->adnext;
			if(eap == NULL) {
				lderror(1, aip->bond.aiinlnno, aip->bond.aiinflnm,
					"%.8s, bonded at %.2lx, won't fit into configured memory",
					 (*(osp->oshdr.s_name) == '\0') ? "GROUP" : osp->oshdr.s_name, aip->bond.aiadrbnd);
				free(aip);
				break;
				}
			if(eap->adtype != ADAVAIL) {
				bsp = eap->adscnptr;
				lderror(1, aip->bond.aiinlnno, aip->bond.aiinflnm,
					"%.8s at %.2lx overlays previously allocated section %.8s at %.2lx",
					 (*(osp->oshdr.s_name) == '\0') ? "GROUP" : osp->oshdr.s_name, aip->bond.aiadrbnd,
					 bsp->oshdr.s_name, eap->adpaddr);
				eap = NULL;
				free(aip);
				break;
				}
			if(eap->adpaddr != lastaddr) {
				lderror(1, aip->bond.aiinlnno, aip->bond.aiinflnm,
					"%.8s enters unconfigured memory at %.2lx",
					 (*(osp->oshdr.s_name) == '\0') ? "GROUP" : osp->oshdr.s_name, lastaddr);
				eap = NULL;
				free(aip);
				break;
				}
			have += eap->adsize;
			}
		if( eap == NULL )
			continue;
		/*
		 * If the bonded section does not end at the end of the
		 * space node, split the space node up. The first will
		 * be SECT, the second AVAIL
		 */
		if ( have > need )
			newap = splitnode(eap, need);
		/*
		 * Reserve and set the output section ptrs
		 * in the address subspace
		 */
		do_alloc(sap,eap,osp);

		free(aip);
		}
}
/*eject*/
alc_owners()
{

/*
 * Allocate the output sections which have been given explicit
 * owners.  The owner of a section can be either a memory area, or a
 * region
 */

	ACTITEM *aip,		/* ptr to section having an owner */
		*nextaip;	/* ptr to next item on ownlist    */
	MEMTYPE *mp;		/* ptr to a memory area item      */
#if USEREGIONS
	REGION *rp;		/* ptr to a region item		  */
#endif
	ANODE *ap1, *ap2;
	short failure;

	for( aip = (ACTITEM *) ownlist.head; aip != NULL; aip = nextaip ) {
		nextaip = aip->dfownr.ainext;
		/*
		 * It is meaningless to have owners for a DSECT
		 * since they are not allocated
		 */
		if( aip->dfownr.aioutsec->oshdr.s_flags & STYP_DSECT) {
			lderror(1, aip->dfownr.aiinlnno, aip->dfownr.aiinflnm,
			    "DSECT %.8s can't be given an owner",
			    aip->dfownr.aioutsec->oshdr.s_name);
			free(aip);
			continue;
			}

		/*
		 * Look in memlist for the owner's name
		 */
		failure = 1;
		for( mp = (MEMTYPE *) memlist.head; mp != NULL; mp = mp->mtnext ) {
			if( equal(aip->dfownr.ainamown,mp->mtname,8)  &&
			    (can_alloc(mp->mtaddrhd,mp->mtaddrtl,aip->dfownr.aioutsec,
				 &ap1,&ap2)) ) {
				/*
				 * Found where to put the output section
				 */
				do_alloc(ap1,ap2,aip->dfownr.aioutsec);
				failure = 0;
				break;
				}
			}
#if USEREGIONS
		/*
		 * If the name was not found in the memlist, (failure still
		 * 1), then try the reglist
		 */
		if( ! failure ) {
			free(aip);
			continue;
			}

		for( rp = (REGION *) reglist.head; rp != NULL; rp = rp->rgnext ) {
			if( equal(aip->dfownr.ainamown,rp->rgname,8) &&
			    (can_alloc(rp->rgaddrhd,rp->rgaddrtl,aip->dfownr.aioutsec,
				 &ap1,&ap2)) ) {
				/*
				 * Output section goes in this region
				 *  between ap1 and ap2
				 */
				do_alloc(ap1,ap2,aip->dfownr.aioutsec);
				failure = 0;
				break;
				}
			}
#endif


		/*
		 * If failed in to find where to put the output section,
		 * issue an error message
		 */
		if(failure)
			lderror(1, aip->dfownr.aiinlnno, aip->dfownr.aiinflnm,
				"can't allocate section %.8s into owner %.8s",
				aip->dfownr.aioutsec->oshdr.s_name,aip->dfownr.ainamown);

		free(aip);
		}

}
/*eject*/
alc_attowns()
{

/*
 * Allocate the output sections assigned to any memory type
 * with a given attribute
 */

	ACTITEM *aip,		/* ptr to section tied to an attribute */
		*nextaip;	/* ptr to next item on atownlst  */
	MEMTYPE *mp;		/* ptr to an item on the memlist */
	ANODE   *ap1, *ap2;
	short   failure;

	for( aip = (ACTITEM *) atownlst.head; aip != NULL; aip = nextaip ) {
		nextaip = aip->ownatr.ainext;
		/*
		 * It is meaningless to associate a DSECT with an
		 * attribute, since they are not allocated any memory
		 */
		if( aip->ownatr.aioutsec->oshdr.s_flags & STYP_DSECT) {
			lderror(1, aip->ownatr.aiinlnno, aip->ownatr.aiinflnm,
			    "DSECT %.8s can't be linked to an attribute",
			    aip->ownatr.aioutsec->oshdr.s_name);
			free(aip);
			continue;
			}

		/*
		 * Look for a memory area with the specified attribute
		 */
		failure = 1;
		for( mp = (MEMTYPE *) memlist.head; mp != NULL; mp = mp->mtnext ) {
			if( (aip->ownatr.aiownatt == mp->mtattr) &&
			    (can_alloc(mp->mtaddrhd,mp->mtaddrtl,
				     aip->ownatr.aioutsec,&ap1,&ap2)) ) {
				do_alloc(ap1,ap2,aip->ownatr.aioutsec);
				failure = 0;
				break;
				}
			}

		if(failure)
			lderror(1,0,NULL, "can't allocate %.8s with attr %x",
			      aip->ownatr.aioutsec->oshdr.s_name,aip->ownatr.aiownatt);

		free(aip);
		}
}
/*eject*/
alc_the_rest()
{

/*
 * Allocate every output section that is not as yet allocated
 */

	ANODE **anlp;		/* ptr to (sorted) avail node list      */
	OUTSECT **usp;		/* ptr to (sorted) unalloc outsect list */

	ANODE *ap;		/* ptr to element from anlp	*/
	short numanods,		/* number of avail nodes 	*/
	      nanl;		/* working index into anlp 	*/
	OUTSECT *osp;		/* ptr to element from usp	*/
	short numuaos,		/* number of unalloc outsects   */
	      nuaos;		/* working index into usp	*/
	int cmp_uos(),		/* qsort sorting routine	*/
	    cmp_anl();		/* qsort sorting routine	*/

/*
 * Build a list of unallocated output sections
 */

	numuaos = 0;
	for( osp = (OUTSECT *) outsclst.head; osp != NULL; osp = osp->osnext )
		if( (osp->oshdr.s_paddr == -1L)  &&  (! (osp->oshdr.s_flags & STYP_DSECT)) )
			numuaos++;
	if(numuaos == 0)
		return;		/* nothing left to allocate */

	usp = (OUTSECT **) myalloc(sizeof(OUTSECT *) * (numuaos + 1));
	nuaos = 0;
	for( osp = (OUTSECT *) outsclst.head; osp != NULL; osp = osp->osnext )
		if(osp->oshdr.s_paddr == -1L) {
			/*
			 * Allocate DSECTs so as to start at zero
			 */
			if( osp->oshdr.s_flags & STYP_DSECT) {
				osp->oshdr.s_paddr = 0L;
				listadd(l_DS,&dsectlst,osp);
				}
			else
				usp[nuaos++] = osp;
			}

#if USEREGIONS
/*
 * Sort only if more than 3 output sections.  This exempts the
 *	  basic load, containing only .text, .data, and .bss sections.
 *	  They will usually be in the usual order.
 *
 * NOTE:  Sort only if REGIONS directives were supplied
 */

	if( (numuaos > 3)  &&  (reglist.head != NULL) )
		qsort(usp, numuaos, sizeof(osp), cmp_uos);
#endif
	usp[nuaos] = NULL;

/*
 * Build the list of avail space nodes
 *
 * The size of the list used to hold the AVAIL pointers must be big
 * enough to allow for possible fragmentation of an existing AVAIL
 * node during allocation:
 *
 *	1 AVAIL node -> 1 AVAIL node (due to ALIGNment)
 *			1 ADSECT node (assigned to the OUTSECT)
 *			1 AVAIL node (remainder of original AVAIL node)
 *	
 */

	numanods = 0;
	for( ap = (ANODE *) avlist.head; ap != NULL; ap = ap->adnext )
		if( (ap->adtype == ADAVAIL)  &&  ((reglist.head == NULL) || (ap->adregp != NULL)) )
			numanods++;

	anlp = (ANODE **) myalloc( sizeof(ANODE *) * (numanods + 1)  +
				   sizeof(OUTSECT *) * (numuaos) );
	
	nanl = 0;
	for( ap = (ANODE *) avlist.head; ap != NULL; ap = ap->adnext )
		if( (ap->adtype==ADAVAIL)  &&  ((reglist.head == NULL) || (ap->adregp != NULL)) )
			anlp[nanl++] = ap;

#if USEREGIONS
/*
 * Sort the avail space node list, on space size, from low to high
 */

	qsort(anlp,numanods,sizeof(ap),cmp_anl);
#endif
	anlp[nanl] = NULL;	/* terminating entry	*/

	alc_lists(usp, anlp, nanl);

	free(usp);
	free(anlp);
}
/*eject*/
audit_groups()
{
	OUTSECT *grpp, *osp, *prevp, *p;
	ANODE *ap;
	ADDRESS addr;

	prevp = NULL;
	grpp = (OUTSECT *) outsclst.head;
	while (grpp) {
		if( grpp->oshdr.s_flags & STYP_GROUP) {
			addr = grpp->oshdr.s_paddr;
			if( (ap=findsanode(grpp)) == NULL )
				lderror(2,0,NULL, "internal error: audit_groups, findsanode failure");
			((OUTSECT *) grpp->osinclhd)->osblock = grpp->osblock;
			for( osp = (OUTSECT *) grpp->osinclhd; osp != NULL; osp = osp->osnext ) 
				if (osp->oshdr.s_flags & STYP_DSECT) {
					osp->oshdr.s_paddr = addr;
					listadd( l_DS, &dsectlst, osp );
				} else {
					if ( !(ap->adscnptr->oshdr.s_flags & STYP_GROUP))
						ap = splitnode( ap, addr );
					ap->adscnptr = osp;
					if( ap->adpaddr != addr )
						lderror(2,0,NULL, "internal error: audit_groups, address mismatch");
					osp->oshdr.s_paddr = addr;
					addr += osp->oshdr.s_size;
				}
			if( prevp )
				prevp->osnext = (OUTSECT *) grpp->osinclhd;
			else
				outsclst.head = (char *) grpp->osinclhd;
			prevp = (OUTSECT *) grpp->osincltl;
			prevp->osnext = grpp->osnext;
			p = grpp;
			grpp = grpp->osnext;
			if( grpp == NULL )
				outsclst.tail = (char *) prevp;
			free(p);
			}
		else {
			prevp = grpp;
			grpp = grpp->osnext;
			}
		}

#if DEBUG
	if( dflag )
		dmp_outsects();
#endif
}
/*eject*/
audit_regions()
{

/*
 * 1. Compute the virtual addresses of each output section
 *	The logic used by ld results in physical addresses being computed
 *	prior to virtual addresses
 *
 * 2. Perform a consistency check, to make sure sufficient regions 
 *	have been defined, where regions are necessary
 */

	register ANODE *ap;
	register OUTSECT *osp;
#if USEREGIONS
	register REGION	*rp;

	if( reglist.head == NULL ) {
		lderror(1,0,NULL, "internal error: audit_regions detected no regions built");
		return;
		}
#endif

	for( ap = (ANODE *) avlist.head; ap != NULL; ap = ap->adnext ) {
		if( ap->adtype == ADSECT) {
#if USEREGIONS
			rp = ap->adregp;
			osp = ap->adscnptr;
			if( ap->adpaddr == osp->oshdr.s_paddr )
				if( rp != NULL)
					osp->oshdr.s_vaddr = rp->rgvaddr + ap->adpaddr - rp->rgorig;
				else
					osp->oshdr.s_vaddr = osp->oshdr.s_paddr;
			/*
		 	 * Make sure every output section goes into some
		 	 * region
		 	 */
			if( ! ap->adregp )
				lderror(1,0,NULL,"output section %.8s not allocated into a region",
					osp->oshdr.s_name);
#else
			osp = ap->adscnptr;
			if( ap->adpaddr == osp->oshdr.s_paddr )
				osp->oshdr.s_vaddr = osp->oshdr.s_paddr;
#endif
			}
		}

	return;

}
/*eject*/
ANODE *
findsanode(osp)
OUTSECT	*osp;
{
	
/*
 * Run down the avlist and look for a particular
 * section pointer.  Return the pointer to the
 * ANODE containing it, or return NULL.
 */

	register ANODE	*ap;

	for( ap = (ANODE *) avlist.head; ap != NULL; ap = ap->adnext )
		if( ap->adscnptr == osp )
			break;

	return ( ap );
}
/*eject*/
alc_lists(usl,anl,nanl)
OUTSECT *usl[];		/* sorted list of output sections */
ANODE *anl[];		/* sorted list of available space */
short nanl;		/* next free entry in the anl list*/
{
	OUTSECT *usp;
	ANODE	*ap1, *ap2, *ap3;
	short	ani,		/* index into sorted ANODEs	*/
		usi,		/* index into sorted OUTSECTs	*/
		new_node;	/* flag if ANODE was split up	*/
	int cmp_anl();		/* qsort sorting routine	*/

/*
 * For each unallocated output section:
 *	1. Find the first free AVAIL node which can contain it
 *	2. Allocate the OUTSECT node to the AVAIL node
 *
 * The list of unallocated output sections is sorted DECREASING,
 * by section size.
 *
 * The list of available space is sorted INCREASING, on the size of
 * the space
 */

	for( usi = 0; usl[usi]; usi++ ) {

		usp = usl[usi];

		for( ani = 0; anl[ani]; ani++ )
			if( anl[ani]->adsize >=
			    (alignment(usp->osalign, anl[ani]->adpaddr)
			    + usp->oshdr.s_size) )
				break;

		if(anl[ani] == NULL) {
			lderror(1,0,NULL, "can't allocate output section %.8s, of size %10.1lx",
				usp->oshdr.s_name, usp->oshdr.s_size);
			continue;
			}

		new_node = (anl[ani]->adsize != usp->oshdr.s_size);
		ap3 = anl[ani]->adnext;
		if( ! can_alloc(anl[ani], anl[ani], usp, &ap1, &ap2)) {
				lderror(2,0,NULL, "internal error: in allocate lists, list confusion (%d %d)",
					usi, ani);
				dump_mem();
				}
		do_alloc(ap1, ap2, usp);

		if(new_node) {
			/*
			 * If the node at anl[ani] was fragmented, then
			 * update the anl list.  The current
			 * AVAIL node could have generated either one 
			 * or two smaller AVAIL nodes
			 */
			if( ap1 == anl[ani] )
				anl[ani] = ap1->adnext;
			else
#if USEREGIONS
				if( ap2->adnext != ap3 ) {
					anl[nanl++] = ap2->adnext;
					anl[nanl] = NULL;
					}
			qsort(anl, nanl, sizeof(ap1), cmp_anl);
#else
				if( ap2->adnext != ap3 ) {
					for( new_node = nanl; new_node > ani; new_node-- )
						anl[new_node] = anl[new_node-1];
					anl[ani+1] = ap2->adnext;
					anl[++nanl] = NULL;
					}
#endif
			}
		else {
			/*
			 * The node at anl[ani] was competely used,
			 * so remove it from the list and move all
			 * the other entries up one space
			 */
			for( nanl--; anl[ani]; ani++ )
				anl[ani] = anl[ani+1];
			}
		}

	return(nanl);	/* new length of available space list	*/
}
/*eject*/
can_alloc(sap,eap,scp,pap1,pap2)
ANODE *sap, *eap;
OUTSECT *scp;
ANODE **pap1, **pap2;
{

/*
 * Determine if the output section pointed to by scp can fit
 * somewhere between sap and eap.
 *
 * If it can, pap1 and pap2 are given the subrange start and end,
 * and 1 is returned. Otherwise 0 is returned
 */

	ADDRESS	disp,		/* alignment */
		have,		/* last address allocated */
		need;		/* final needed allocation */
	register ANODE *ap1, *ap2;	/* working pointers	*/
	ANODE *splitnode(),
		*newnode();
	short	success;

	success = 0;
	ap2=sap;
	while(!success) {
		ap1 = ap2;
		while(! (ap1->adtype == ADAVAIL && (ap1->adregp != NULL || reglist.head == NULL)) )
			if( ap1 == eap )
				return(0);
			else
				ap1=ap1->adnext;
		disp=alignment(scp->osalign,ap1->adpaddr);
		have = ap1->adsize + ap1->adpaddr;
		need = ap1->adpaddr + disp + scp->oshdr.s_size;
		ap2 = ap1;
		while ( need > have ) {
			if(ap2 == eap)
				return(0);
			ap2 = ap2->adnext;
			
			if(ap2->adtype != ADAVAIL)
				break;

			if(ap2->adprev->adpaddr+ap2->adprev->adsize != ap2->adpaddr)
				break;

			if(ap2->adregp==NULL && reglist.head != NULL )
				break;
			have += ap2->adsize;
			}
		if ( have >= need )
			success = 1;
		}

/*
 * To reach this point, success must have been achieved
 */

	if ( disp != 0L ) {
		/*
		 * Split the node because of alignment
		 */
		ANODE *newap;
		newap = splitnode(ap1,ap1->adpaddr+disp);
		if(ap1 == ap2)
			ap2 = newap;
		ap1 = newap;
		}
	if ( have > need ) {
		/*
		 * If ending in the middle of a node, split the node
		 */
		ANODE *newap;
		newap = splitnode(ap2,need);
		}

	*pap1 = ap1;	/* output parameters */
	*pap2 = ap2;

	return(1);
}
/*eject*/
ANODE *
splitnode(ap,addr)
ANODE *ap;
ADDRESS addr;
{
	register ANODE *newap;
	ANODE *newnode();

	newap = newnode();

	copynode(newap,ap);

	if( ap->adnext != NULL )
		ap->adnext->adprev = newap;
	newap->adprev = ap;
	ap->adnext = newap;

	ap->adsize = addr - ap->adpaddr;
	newap->adsize -= ap->adsize;
	newap->adpaddr = addr;

	if( (ANODE *) avlist.tail == ap )
		avlist.tail = (char *) newap;

	if( ap->admemp->mtaddrtl == ap )
		ap->admemp->mtaddrtl = newap;
	if( (ap->adregp != NULL)  &&  (ap->adregp->rgaddrtl == ap) )
		ap->adregp->rgaddrtl = newap;

	return(newap);
}
/*eject*/
do_alloc(ap1,ap2,scp)
ANODE *ap1, *ap2;
OUTSECT *scp;
{
	register ANODE *ap;

	for( ap = ap1; ap != ap2->adnext; ap = ap->adnext ) {
		ap->adtype = ADSECT;
		ap->adscnptr = scp;
		}

	scp->oshdr.s_paddr = ap1->adpaddr;
}
/*eject*/
ANODE *
findnode(adr,flg)
ADDRESS adr;
int flg;
{

/*
 * "flg" is used if the adr falls between two nodes.
 * 	flg == 1: return next node,
 * 	flg == 0: return prev node.
 */

	register ANODE *ap;

	for( ap = (ANODE *) avlist.head; ap != NULL; ap = ap->adnext ) {

		if(ap->adpaddr > adr)
			/*
			 * adr is between nodes
			 */
			if( flg )
				return(ap);
			else
				return(ap->adprev);

		if( ap->adpaddr+ap->adsize > adr )
			/*
			 * adr is within a node
			 */
			return(ap);

		}

	/*
	 * adr is outside all configured memory
	 */

	return((ANODE *) -1);
}
/*eject*/
cmp_anl(p1,p2)
ANODE **p1, **p2;
{

/*
 * Compare two ANODEs, returning:
 *
 *	-1	when p1 <  p2
 *	 0	when p1 == p2
 *	+1	when p1 >  p2
 *
 * The adsize field is used for the comparison
 */

	if( (*p1)->adsize <  (*p2)->adsize ) return(-1);

	if( (*p1)->adsize == (*p2)->adsize ) return(0);

	return(1);
}





cmp_uos(p1,p2)
OUTSECT **p1, **p2;
{

/*
 * Compare two OUTSECT nodes, returning:
 *
 *	+1	when p1 <  p2
 *	 0	when p1 == p2
 *	-1	when p1 >  p2
 *
 * The oshdr.s_size field is used for the comparison
 */

	if( (*p1)->oshdr.s_size < (*p2)->oshdr.s_size ) return(1);

	if( (*p1)->oshdr.s_size == (*p2)->oshdr.s_size ) return(0);

	return(-1);
}
/*eject*/
ANODE *
newnode()
{

/*
 * Return a blanked out node
 */

	register ANODE *ap;

	ap = (ANODE *) myalloc(sizeof(ANODE));

	ap->adtype = ADAVAIL;

	return(ap);
}





copynode(new,old)
ANODE *new, *old;
{
	new->adnext = old->adnext;
	new->adprev = old->adprev;
	new->adpaddr = old->adpaddr;
	new->adsize = old->adsize;
	new->admemp = old->admemp;
	new->adregp = old->adregp;
	new->adscnptr = old->adscnptr;
	new->adtype = old->adtype;
}
/*eject*/
long
alignment(align,paddr)
ADDRESS align ;	/* this will be a power or two */
ADDRESS paddr;
{
	register ADDRESS l;

	if ( align <= 1 )
		return(0);

	l = (paddr + (align - 1)) & ~(align - 1);

	return(l - paddr);
}
/*eject*/
creatsym(name,value)
char *name;
long value;
{

/*
 * Generate a special ld-define symbol. The name of the symbol is
 * pointed to by 'name', and its value is given by 'value'
 *
 * Such a symbol is defined to have a basic symbol type of T_NULL
 */

	register SYMTAB *p;
	SYMENT q;

/*
 * Special symbols are generated only under the "-a" flag
 */

	if( ! aflag )
		return;

	if( (p = (SYMTAB *) findsym(name)) == NULL ) {
		/*
		 * Case 1: This is the first time the special symbol
		 *		has been encountered
		 */
		zero(&q, SYMESZ);
#if FLEXNAMES
		if (strlen(name) > 8) {
			q.n_zeroes = 0L;
			q.n_nptr = name;
			}
		else
#endif
			copy(q.n_name, name, 8);
		q.n_sclass = C_EXT;
		q.n_scnum = -1;		/* for absolute symbol */
		q.n_value = value;
		q.n_type = T_NULL;
		p = makesym(&q, NULL);
		}
	else if( p->sment.n_scnum == 0 ) {
		/*
		 * Case 2: The special symbol has been previously
		 *		referenced but not defined
		 */
		p->sment.n_sclass = C_EXT;
		p->sment.n_scnum = -1;
		p->sment.n_value = value;
		p->sment.n_type = T_NULL;
		}

	if( p->sment.n_scnum == -1 )
		p->smnewval = p->sment.n_value;

	PUTSYM(p, 1);
}
