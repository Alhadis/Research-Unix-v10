/*
 * static char ID_allocc[] = "@(#) allocate.c: 1.1 1/7/82";
 */

ITEMID allocate(request, spacenbr, partnbr)
int request, spacenbr, partnbr;
{

	register HEADER *gframe;
	int framenbr;
	long pagenbr;
	long itemid;
	long getnxtpage();

/*
 * The request must be for an even number of bytes, and must hold an
 * itemid. No request can exceed one page
 */

	request = (request < ITEMIDSIZE) ? (ITEMIDSIZE) : ((request+1) & (~1));

	if( (request == 0)  ||  (request > PAGESIZE) )
		return( (ITEMID) fatal("allocate request out of range") );

	if( open_spacenumbers[spacenbr] != YES )
		return( (ITEMID) fatal("address space not yet opened") );

	if( (partnbr < 0)  ||  (partnbr > num_partitions) )
		return( (ITEMID) fatal("allocate partition out of range") );


/*
 * Satisfy the request. There are two cases:
 *
 *	1. Allocate a piece of the current page
 *	2. Allocate an entirely new page
 */

	gframe = &global_frames[spacenbr];

#if DEBUG
	if( dflag > 15 )
		fprintf( stderr, " alloc %04x %d %d ",
			request, spacenbr, partnbr);
#endif
		
	if( gframe->curoffset + request <= PAGESIZE ) {

		itemid = BUILDPN(gframe->curpage, spacenbr) + gframe->curoffset;
		gframe->curoffset += request;

		return( (ITEMID) itemid );

		}
	
	if( (pagenbr = getnxtpage(spacenbr)) < 0 )
		return( (ITEMID) fatal("unable to allocate new page") );

	itemid = BUILDPN(pagenbr, spacenbr);

	if( (framenbr = sojourn(itemid, partnbr, NO)) < 0 )
		return( (ITEMID) fatal("unable to find frame for new page") );

	if( delist(framenbr) < 0 )
		return( (ITEMID) fatal("unable to delist frame of new page") );
	if( enlist(framenbr, MRU) < 0 )
		return( (ITEMID) fatal("unable to enlist frame of new page") );

	gframe->curpage = pagenbr;
	gframe->curoffset = request;

	return( (ITEMID) itemid );
}
