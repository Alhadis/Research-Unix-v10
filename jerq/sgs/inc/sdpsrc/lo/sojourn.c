/*
 * static char ID_sjrnc[] = "@(#) sojourn.c: 1.1 1/8/82";
 */

sojourn(itemid, partnbr, readaction)
long itemid;
int partnbr, readaction;
{

	register int   index1;
	register char *index2;
	int framenbr;

/*
 * Search the page frame contents, to see if the desired item is
 * already present
 */

	for( index1 = 0; index1 < NUMBERFRAMES; index1++ )
		if( itemid == manager[index1].page_id )
			return( index1 );

	if( (partnbr < 0)  ||  (partnbr > num_partitions) )
		return( fatal("illegal frame partition supplied") );

/*
 * A frame must be selected for a item not currently in memory
 */

	if( (framenbr = lru[partnbr]) == NOFRAMES )
		return( fatal("no unlocked frames available") );

	if( manager[framenbr].ref_chng == WRTN )
		if( writeframe(framenbr) != PAGESIZE )
			return( fatal("page write failed to output entire page") );

	if( readaction == YES ) {
		if( readpage(itemid, framenbr) != PAGESIZE )
			return( fatal("page read failed to input entire page") );
		}
	else
		for( index2 = manager[framenbr].frame_pntr; index2 < manager[framenbr].frame_pntr+PAGESIZE; *index2++ = 0 )
			;

	manager[framenbr].page_id = itemid;
	manager[framenbr].ref_chng = RNLY;

	return( framenbr );
}
