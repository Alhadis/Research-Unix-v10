/*
 * static char ID_nlstc[] = "@(#) enlist.c: 1.1 1/7/82";
 */

enlist(framenbr, position)
int framenbr, position;
{

	register int partnbr;

	partnbr = manager[framenbr].partition;

	if( manager[framenbr].forward != INVALID )
		return( fatal("attempt to link a linked frame") );

	if( num_linked[partnbr] == 0 ) {
		lru[partnbr] = mru[partnbr] = framenbr;
		manager[framenbr].forward = LAMDA;
		manager[framenbr].backward = LAMDA;
		}
	else if( position == MRU ) {
		manager[mru[partnbr]].forward = framenbr;
		manager[framenbr].backward = mru[partnbr];
		manager[framenbr].forward = LAMDA;
		mru[partnbr] = framenbr;
		}
	else if( position == LRU ) {
		manager[lru[partnbr]].backward = framenbr;
		manager[framenbr].forward = lru[partnbr];
		manager[framenbr].backward = LAMDA;
		lru[partnbr] = framenbr;
		}
	else
		return( fatal("attempt to insert frame in middle of list") );

	num_linked[partnbr]++;

	return(SUCCESS);
}
