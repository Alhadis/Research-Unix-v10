#include	<stddef.h>
#include	<stdio.h>
#include	<string.h>
#include	"scsi.h"
#include	"juke.h"

allocate(char *vol_id, char *buf)
{
	int drive, sh;
	char nbuf[512];
	
	if(j_rdshelves(buf))	/* read in shelf names */
		return(-1);
	if(j_getstatus(buf))	/* get the jukebox status */
		return(-1);
	sh = j_shelfof(vol_id);
	if(sh >= 0){
		sprintf(buf, "there is an existing '%s' on shelf %d", vol_id, sh);
		return(-1);
	}
	sh = j_shelfof(UNALLOCATED);
	if(sh < 0){
		sprintf(buf, "no unallocated disks");
		return(-1);
	}
	printf("using unallocated disk from shelf %d\n", sh);
	drive = min(nlun+1, NLUN-1);
	if(j_shelf_to_drive(sh, SIDEB, drive, buf) < 0)
		return(-1);
	sprintf(nbuf, "%sb", vol_id);
	if(j_wvolid(drive, nbuf, buf))
		return(-1);
	j_wrshelf = 1;
	j_shelf[sh] = strdup(vol_id);
	if(j_drive_to_shelf(drive, sh, SIDEB, buf) < 0)
		return(-1);
	if(j_shelf_to_drive(sh, SIDEA, drive, buf) < 0)
		return(-1);
	sprintf(nbuf, "%sa", vol_id);
	if(j_wvolid(drive, nbuf, buf))
		return(-1);
	if(j_drive_to_shelf(drive, sh, SIDEA, buf) < 0)
		return(-1);
	return(0);
}
