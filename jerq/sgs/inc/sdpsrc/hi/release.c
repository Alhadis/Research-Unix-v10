/*
 * static char ID_relc[] = "@(#) release.c: 1.1 1/7/82";
 */

release(what,howmuch)
ITEMID what;
int    howmuch;
{
	int frameno,ordinal,tops;
	char *place, *scan;

	frameno = sojourn(what,YES);
	if(frameno<0) return(fatal("RL01 -- unable to find frame for release"));


	if( (ordinal = sp2or(XTRCTSP(what))) <0 )
		return(fatal("RL02 -- unknown space number"));

	place = manager[frameno].frame_pntr + (what & BYTEMASK);

	for(tops=global_frames[ordinal]->numbstax-1; tops >= 0; tops--)
		if( howmuch >= global_frame[ordinal]->sizestax[tops] )
		{
			(*( (ITEMID *)place )) = global_frame[ordinal] -> topstax[tops];
			global_frame[ordinal] -> topstax[tops] = what;
			manager[frameno].ref_chng = WRTN;
			break;
		}

	for(scan=place; scan<place+howmuch; *scan++=0);

	return(SUCCESS);

}
