/*
 * static char ID_chgc[] = "@(#) change.c: 1.1 1/7/82";
 */

change(pointer)
char *pointer;
{

	int scan;

	pointer = (char *) (((long)pointer) & ~BYTEMASK);
	for(scan=0;scan<NUMBERFRAMES;scan++)

		if( manager[scan].frame_pntr == pointer)
		{
			manager[scan].ref_chng = WRTN;
			return(SUCCESS);
		}

	return(fatal("CH01--attempt to set ref/chng flag on item not in core"));
}

