/*
 * static char ID_dlstc[] = "@(#) delist.c: 1.1 1/7/82";
 */

delist(framenbr)
int framenbr;
{

	register int partnbr;

	partnbr = manager[framenbr].partition;

	if( num_linked[partnbr] == 0 )
		return( fatal("no free frames in partition") );

	if( manager[framenbr].forward == INVALID )
		return( fatal("attempt to unlink an unlinked frame") );

	if( num_linked[partnbr] == 1 )
		lru[partnbr] = mru[partnbr] = NOFRAMES;
	else if( framenbr == lru[partnbr] ) {
		lru[partnbr] = manager[lru[partnbr]].forward;
		manager[lru[partnbr]].backward = LAMDA;
		}
	else if( framenbr == mru[partnbr] ) {
		mru[partnbr] = manager[mru[partnbr]].backward;
		manager[mru[partnbr]].forward = LAMDA;
		}
	else {
		manager[manager[framenbr].backward].forward = 
			manager[framenbr].forward;
		manager[manager[framenbr].forward].backward =
			manager[framenbr].backward;
		}

	manager[framenbr].forward  = INVALID;
	manager[framenbr].backward = INVALID;

	num_linked[partnbr]--;

	return(SUCCESS);
}
