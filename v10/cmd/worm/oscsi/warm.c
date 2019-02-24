#include	<stddef.h>
#include	<stdio.h>
#include	<string.h>
#include	"scsi.h"
#include	"juke.h"

warm_inv(char *buf)
{
	Side side;
	int drive, sh;
	char vol_id[512];
	
	if(j_rdshelves(buf))	/* read in shelf names */
		return(-1);
	side = SIDEA;
	drive = min(nlun+1, NLUN-1);
	for(;;){
		if(j_getstatus(buf))	/* get the jukebox status */
			return(-1);
		for(sh = 0; sh < NSHELF; sh++)
			if(j_status.shelf[sh]&0x10) break;
		if(sh >= NSHELF)
			break;
		if(getvol(127, drive, vol_id, &side)){
			strcpy(buf, vol_id);
			return(-1);
		}
		for(sh = 0; j_shelf[sh]; sh++)
			;
		printf("%s -> %d\n", vol_id, sh);
		if(j_drive_to_shelf(drive, sh, side, buf) < 0)
			return(-1);
		j_wrshelf = 1;
		j_shelf[sh] = strdup(vol_id);
		sleep(1);
	}
	return(0);
}
