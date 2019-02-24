/*
 * static char ID_wrtfrmc[] = "@(#) writeframe.c: 1.1 1/8/82";
 */

writeframe(framenbr)
int framenbr;
{

	register int index;
	register HEADER *gframe;
	int byteswritten, spacenbr, fdes;
	char file_name[NAMELENG];
	long offset;
	long pagenbr;
	
	spacenbr = XTRCTSP(manager[framenbr].page_id);
	pagenbr  = XTRCTPN(manager[framenbr].page_id);
	
	gframe = &global_frames[spacenbr];
	
	offset = 0L;
	for( index = 0; index < gframe->numbfiles; index++ ) {
		if( pagenbr < (offset + gframe->filesize[index]) )
			break;
		offset += gframe->filesize[index];
		}
	if( index == gframe->numbfiles )
		if( gframe->numbfiles == MAXFILES )
			return( fatal("address space too large") );
		else
			gframe->filedes[++gframe->numbfiles] = NOTMADE;
	
	offset = pagenbr - offset;
	
	sprintf(file_name, "%s/%s%d", direct_names[spacenbr], TEMPLATE, index);
	
	if( gframe->filedes[index] == NOTMADE ) {
		if( (fdes = creat(file_name,MODE) ) < 0 )
			return( fatal("create of new SDP file failed") );
		if( close(fdes) == SYSERR )
			return( fatal("close of new SDP file failed") );
		gframe->filedes[index] = CLOSED;
		}

	if( gframe->filedes[index] == CLOSED )
		if( (gframe->filedes[index] = open(file_name, RW)) < 0 )
			return( fatal("open of new SDP file failed") );
	
	if( lseek(gframe->filedes[index], offset*PAGESIZE, 0) < 0 )
		return( fatal("lseek on SDP file failed") );
	
	SDP_write++;
	if( dflag > 15 )
		fprintf( stderr, " w%02d ", framenbr );

	if( (byteswritten =
		write(gframe->filedes[index], manager[framenbr].frame_pntr, PAGESIZE)) < 0 )
		return( fatal("write failed to SDP file") );
	
	partusage[manager[framenbr].partition]++;
	partusage[num_partitions]++;

	return( byteswritten );
}
