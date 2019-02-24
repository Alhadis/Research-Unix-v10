static char ID[] = "@(#) slotvec.c: 1.5 9/20/83";
#include "system.h"

#include <stdio.h>
#if !NOSDP
#include "sdp1.h"
#include "sdpsrc/hd/define4.h"
#endif
#include "sdpsrc/hd/define2.h"
#include "structs.h"
#include "paths.h"
#include "slotvec.h"
#include "extrns.h"


/********************************************************************
 *
 *  This module administers the slot vector used during output of the
 *  object file.  A slot is used to store the information needed to permit
 *  the relocation of all references to a single symbol.
 *
 *  Using a slot vector prevents the link editor from having to look
 *  up each symbol in the global symbol table in order to relocate it.
 *
 *  The size of the slot vector is dependant on the size of the largest
 *  input symbol table, since it is rebuilt for every input object file.
 *  When the number of slots needed is too large and calloc() fails,
 *  the Software Demand Paging system is used to manipulate the slots.
 *
 ********************************************************************/

SLOTVEC *svpageptr = NULL;		/* pointer to "page-in-core"; array of slots
				 	   if( sdp_called == 0 ), pageptr points to entire
				 	   slot vector */

#if !NOSDP
#if AR16WR
#define MYSLTBSZ	1
#else
#define MYSLTBSZ	2000
#endif
#define SLTPERPAGE  (PAGESIZE/SLOTSIZ)

static int sdp_called = 0;		/* set to >0 if SDP is used */
static long *svpagtable = NULL;		/* contains ITEMIDs of the SDP pages */
static int pageincore;			/* page number currently "locked" in core */

static char svname[sizeof(TMPDIR)+20] = "";	/* holds name of directory used by SDP */
#endif

unsigned svhighslt = 0;			/* index of next available slot */
static long svhighndx = 0L;		/* sym. tbl index of symbol assigned to last assigned slot */
/*eject*/
svinit(numslots)
long numslots;		/* number of slots that are needed */
{
    char * calloc();

/*
 * Allocate a slot vector.  This vector can be either completely 
 * memory-resident or make use of SDP
 */

	register int numpages;

#if DEBUG
	if( dflag )
		fprintf( stderr, "svinit requested to set up %ld slots\n",
			numslots);
#endif

#if NOSDP
		if( (svpageptr = (SLOTVEC *) calloc((unsigned)numslots, (unsigned)SLOTSIZ)) == NULL ) {
			lderror(2,0,NULL,"fail to allocate %ld bytes for slotvec table",
			(long) numslots * (long) SLOTSIZ);
			}

/*
 * Case 1:
 *	If the number of slots needed is less then 3,000 then try to
 *	make the entire slot vector memory-resident
 */

#else
	if( numslots < MYSLTBSZ )
		if( (svpageptr = (SLOTVEC *) calloc((unsigned)numslots, (unsigned)SLOTSIZ)) != NULL ) {
			allocspc += numslots*SLOTSIZ;
			pageincore = -1;
			return;
			}

/*
 * Case 2:
 *	Use SDP to create the slot vector
 *
 *	1. Create the svpagetable, to hold SDP ITEMIDs
 *	2. Create the SDP Address Space
 *	3. Define the first page of the vector
 */

	if( sdp_called )
		lderror(2,0,NULL,"attempt to re-initialize the SDP slot space");

	numpages = (numslots + SLTPERPAGE) / SLTPERPAGE;
	svpagtable = (long *) myalloc(numpages * sizeof(long));

	sprintf(svname, "%s/%s", TMPDIR, "ldXXXXX");
	mktemp(svname);

	sdp_called++;

	if( sdpinit(svname, CREATE, SVSPACE) != SVSPACE )
		lderror(2,0,NULL,"failure to initialize the SDP slot space");
	else
		sdp_called++;

#if DEBUG
	if( dflag )
		fprintf( stderr, "svinit setting up SDP: %d pages on file %s\n",
			numpages, svname);
#endif

	pageincore = 0;
	svpagtable[0] = allocate(PAGESIZE, SVSPACE, SVPARTITION);
	svpageptr = (SLOTVEC *) lock(svpagtable[0],SVPARTITION,WRTN);
#endif
}


#if SDP
SLOTVEC *
getslot(sltindex, action)
unsigned sltindex;
int action;
{

/*
 * Return the address of the slot whose index is given by "sltindex"
 */

	register SLOTVEC *p;
	register int pagenum;
	static  int  unlockact;
	
	if( sdp_called == 0 )
		p = &svpageptr[sltindex];
	else {
		pagenum = sltindex / SLTPERPAGE;
		if( pagenum != pageincore ) {
			unlock(svpagtable[pageincore], unlockact);
			if( svpagtable[pagenum] == 0L )
				svpagtable[pagenum] = allocate(PAGESIZE, SVSPACE, SVPARTITION);
			svpageptr = (SLOTVEC *) lock(svpagtable[pagenum], SVPARTITION, action);
			pageincore = pagenum;
			unlockact = action;
			}
		else
			unlockact = (action==WRTN) ? WRTN : unlockact;
		p = svpageptr + sltindex - pagenum*SLTPERPAGE;
		}

	return(p);
}
#endif
/*eject*/
svcreate(symindex, ovaddr, nvaddr, newindex, secnum, flags)
long symindex, ovaddr, nvaddr, newindex;
int secnum, flags;
{

/*
 * Get and initialize the next available slot in the slot vector
 */

	register SLOTVEC *p;

	p = GETSLOT(svhighslt++, WRTN);

	svhighndx = symindex;

	p->svsymndx = symindex;
	p->svovaddr = ovaddr;
	p->svnvaddr = nvaddr;
	p->svnewndx = newindex;
	p->svsecnum = secnum;
	p->svflags = flags;

}
/*eject*/
SLOTVEC *
svread(symndx)
long symndx;
{

/*
 * Return the pointer to the slot belonging to the symbol whose 
 * old (e.g., input) symbol table index is given by "symndx"
 *
 * Return NULL upon a failure to find a slot
 */

	register SLOTVEC *p;
	register unsigned high, low, diff, guess;

/*
 * Optimize: see if asking for the slot of the last symbol given a slot
 */

	if( symndx == svhighndx )
		return(GETSLOT(svhighslt-1, RNLY));

/*
 * Perform a binary search on the slot vector, looking for the 
 * requested slot
 */

	high = svhighslt - 1;
	low = 0;

	while( (diff = high - low) != 0 ) {
		guess = low + (diff / 2);
		p = GETSLOT(guess, RNLY);
		if( p->svsymndx == symndx )
			return(p);
		if( p->svsymndx > symndx ) { 
			if( high == guess )
				break;		/* not found */
			high = guess;
			}
		else {
			if( low == guess )
				break;		/* not found */
			low = guess;
			}
		}

/*
 * ERROR: the specified symbol was never assigned a slot
 */

	return( NULL );

}
/*eject*/
#if !NOSDP
svfini()
{

/*
 * Do any necessary clean-up with regard to the slot vector
 */

	char buf[sizeof(svname)*2 + 20];

#if DEBUG
	if( dflag )
		fprintf( stderr, "svfini: next avail slot: %u    last symbol index: %.2lo\n",
			svhighslt, svhighndx);
#endif

	if( sdp_called == 0 ) {
		sdp_called--;
		if( svpageptr != NULL )
			free((char *) svpageptr);
		return;
		}

	if( sdp_called == 2 ) {
		sdp_called--;
		unlock(svpagtable[pageincore], RNLY);
		free((char *) svpagtable);
		if( sdpterm(SVSPACE) < 0 )
			lderror(2,0,NULL,"failure to close the SDP slot space");
		}

	if( sdp_called != 1 )
		return;
	sdp_called--;

#if TS || RT
	sprintf(buf, "rm -r %s", svname);
#else
	sprintf(buf, "rm -r %s; rmdir %s", svname, svname);
#endif

	system(buf);
}
#endif
