/*
 * static char ID_initc[] = "@(#) init.c: 1.1 1/7/82";
 */

sdpinit(dir_name, file_status, spacenbr)
char *dir_name;
int  file_status, spacenbr;
{

	register int index1, index2, limit1, limit2;
	char call_string[NAMELENG+7];
	int  fdes;
	HEADER *gframe;

/*
 * The UN*X directory name must fit into local storage.
 *
 * The file status and space number must be valid.
 */

	if( (strlen(dir_name)+1+strlen(TEMPLATE)+1 > NAMELENG)
	    	||  (strlen(dir_name) == 0) )
		return( fatal("SDP directory name is illegal") );

	if( (spacenbr < 0) || (spacenbr > MAXSP) )
		return( fatal("illegal address space number") );

	if( file_status != CREATE )
		return( fatal("unknown file status") );

/*
 * If sdpinit has not been called before, set up the frame managers
 */

	if( callstatus == NOTCALLEDBEFORE ) {

		limit1 = 0;
		for( index1 = 0; index1 < num_partitions; index1++ ) {

			limit2 = limit1 + partitions[index1];
			for( index2 = limit1; index2 < limit2; index2++ ) {
				manager[index2].page_id     = EMPTY;
				manager[index2].lock_count  = 0;
				manager[index2].forward     = index2+1;
				manager[index2].backward    = index2-1;
				manager[index2].ref_chng    = RNLY;
				manager[index2].partition   = index1;
				manager[index2].frame_pntr  =  &(frames[index2][0]);
				}

			manager[limit1].backward = LAMDA;
			manager[limit2-1].forward = LAMDA;

			lru[index1] = limit1;
			mru[index1] = limit2-1;
			num_linked[index1] = limit2 - limit1;
			partusage[index1] = 0;

			limit1 = limit2;

			}
		partusage[num_partitions] = 0;

		for( index1 = 0; index1 < MAXATTACH; index1++ )
			open_spacenumbers[index1] = NO;

		}
	else
		if( open_spacenumbers[spacenbr] == YES )
			return( fatal("address space already opened") );

/*
 * Create the paging file
 */

	gframe = &global_frames[spacenbr];

	gframe->maxpage = 0;
	gframe->curpage = EMPTY;
	gframe->curoffset = PAGESIZE;
	gframe->numbfiles = DFILES;
	for( index1 = 0; index1 < DFILES; index1++ ) {
		gframe->filesize[index1] = file_size[index1];
		gframe->filedes[index1] = NOTMADE;
		}

	sprintf(call_string, "mkdir %s", dir_name);
	if( system(call_string) != 0 )
		return( fatal("creation of SDP directory failed") );

	sprintf(call_string, "%s/%s0", dir_name, TEMPLATE);
	if( (fdes = creat(call_string,MODE)) == SYSERR )
		return( fatal("creation of SDP paging file failed") );

	if( close(fdes) == SYSERR )
		return( fatal("close of SDP paging file failed") );

	if( (gframe->filedes[0] = open(call_string,RW)) == SYSERR )
		return( fatal("open of SDP paging file failed") );

	open_spacenumbers[spacenbr] = YES;
	sprintf(&direct_names[spacenbr][0], "%s", dir_name);

	num_spaces++;

	callstatus = CALLEDBEFORE;

	return( spacenbr );
}
