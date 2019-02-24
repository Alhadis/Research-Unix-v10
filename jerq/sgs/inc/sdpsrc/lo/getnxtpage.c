/*
 * static char ID_gnxpagc[] = "@(#) getnxtpage.c: 1.1 1/7/82";
 */

long getnxtpage(spacenbr)
int spacenbr;
{
	register long pagenbr;

	pagenbr = global_frames[spacenbr].maxpage;

	if( ++pagenbr == SPSIZE )
		return( (long) fatal("SDP page limit reached") );

	global_frames[spacenbr].maxpage = pagenbr;

	return( pagenbr );
}
