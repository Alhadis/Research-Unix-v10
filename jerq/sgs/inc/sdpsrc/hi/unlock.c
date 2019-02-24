/*
 * static char ID_unlckc[] = "@(#) unlock.c: 1.1 1/7/82";
 */

unlock(itemid, permissions)
ITEMID itemid;
int    permissions;
{

	register index;

	LD_unlock++;
#if DEBUG
	if( dflag > 15 )
		fprintf( stderr, "unlock %08lx ", itemid );
#endif

	itemid &= ~BYTEMASK;

	for( index = 0; index < NUMBERFRAMES; index++ )
		if( itemid == manager[index].page_id) {
			manager[index].lock_count--;
			if( permissions == WRTN )
				manager[index].ref_chng = WRTN;
			if( manager[index].lock_count < 0 )
				return( fatal("attempt to unlock unlocked frame") );
			if( manager[index].lock_count == 0 )
				if( enlist(index, MRU) < 0 )
					return( fatal("unable to replace frame on free list") );
			return(SUCCESS);
			}

	return( fatal("attempt to unlock non-resident item") );
}
