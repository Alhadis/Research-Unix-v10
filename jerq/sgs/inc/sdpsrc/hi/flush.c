/*
 * static char ID_flshc[] = "@(#) flush.c: 1.1 1/7/82";
 */

sdpterm(spacenbr)
int spacenbr;
{
	register int index, fdes;

	if( (spacenbr < 0)  ||  (spacenbr > MAXATTACH) )
		return( fatal("illegal space number") );

	if( open_spacenumbers[spacenbr] != YES )
		return( fatal("address space not open") );

	num_spaces--;
	open_spacenumbers[spacenbr] = NO;

	for( index = 0; index < NUMBERFRAMES; index++ ) {
	
		if( spacenbr != XTRCTSP(manager[index].page_id) )
			continue;

		if( manager[index].lock_count > 0 )
			return( fatal("not all in-core pages unlocked") );
		else if( manager[index].lock_count < 0 )
			return( fatal("page lock count has gone negative") );
		
		if( (manager[index].ref_chng != WRTN)  &&
		    (manager[index].ref_chng != RNLY) )
			return( fatal("illegal page ref/change value") );
		}
		
	for( index = 0; index < global_frames[spacenbr].numbfiles; index++ ) {
		fdes = global_frames[spacenbr].filedes[index];
		if( (fdes == NOTMADE) || (fdes == CLOSED) )
			continue;
		if( close(fdes) == SYSERR )
			return( fatal("can't close pageing file") );
		}

	return(SUCCESS);
}
