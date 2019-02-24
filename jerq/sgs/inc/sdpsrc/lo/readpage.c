/*
 * static char ID_rdpagc[] = "@(#) readpage.c: 1.2 11/10/82";
 */

readpage(itemid, framenbr)
int framenbr;
ITEMID itemid;
{

	register int index;
	register HEADER *gframe;
	int spacenbr, bytesread;
	char file_name[NAMELENG];
	long offset, pagenbr;

	pagenbr  = XTRCTPN(itemid);
	spacenbr = XTRCTSP(itemid);

	gframe = &global_frames[spacenbr];
	
	offset = 0L;
	for( index = 0; index < gframe->numbfiles; index++ ) {
		if( pagenbr < (offset + gframe->filesize[index]) )
			break;
		offset += gframe->filesize[index];
		}
	if( index == gframe->numbfiles )
		return( fatal("attempt to read page outside of address space") );
	
	offset = pagenbr - offset;
	
	if( gframe->filedes[index] == NOTMADE )
		return( fatal("attempt to read from nonexistant file") );
	
	if( gframe->filedes[index] == CLOSED ) {
		sprintf(file_name, "%s/%s%d", direct_names[spacenbr], TEMPLATE, index);
		if( (gframe->filedes[index] = open(file_name,RW)) < 0 )
			return( fatal("unable to open SDP file for read") );
		}
	
	if( lseek(gframe->filedes[index], offset*PAGESIZE, 0) < 0 )
		return( fatal("lseek failure on SDP file") );
	
	SDP_read++;
	if( dflag > 15 )
		fprintf( stderr, " r%02d ", framenbr );

	if( (bytesread =
		read(gframe->filedes[index], manager[framenbr].frame_pntr, PAGESIZE)) < 0 )
		return( fatal("read feilure on SDP file") );

	manager[framenbr].page_id = itemid;
	
	partusage[manager[framenbr].partition]++;
#if DPARTRESET == -1
	partusage[num_partitions]++;
#else
	if( ++partusage[num_partitions] >= partreset )
		repartition();
#endif

	return( bytesread );
}
