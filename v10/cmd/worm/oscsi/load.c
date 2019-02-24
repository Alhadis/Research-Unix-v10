#include	<stdio.h>
#include	<stddef.h>
#include	<string.h>
#include	"scsi.h"
#include	"juke.h"

j_load(char *vol_id, char *buf, long tlimit)
{
	Side side;
	int n, sh, dr;
	char disk_to_load[256];
	struct Lunstatus *l;

	if(j_rdshelves(buf))	/* read in shelf names */
		return(-1);
	if(j_getstatus(buf))	/* get the jukebox status */
		return(-1);
	/* now check which side we want */
	n = strlen(vol_id);
	strcpy(disk_to_load, vol_id);
	if(disk_to_load[n-1] == 'a')
		side = SIDEA;
	else if(disk_to_load[n-1] == 'b')
		side = SIDEB;
	else {
		sprintf(buf, "vol_id '%s' must end in a or b", vol_id);
		return(-1);
	}
	disk_to_load[n-1] = 0;
	/* which shelf is that? */
	sh = j_shelfof(disk_to_load);
	if(sh < 0){
		sprintf(buf, "can't find vol_id %s", disk_to_load);
		return(-1);
	}
	while(tlimit >= 0){
		for(n = 0; n < NLUN; n++){
			l = &j_status.lun[n];
			if(l->diskin && l->shelfvalid && (sh == (l->retshelf>>1))){
				if(((l->retshelf&1) == side) && (n < nlun))
					return(n);
				if(l->ready)
					goto await;
				if(j_drive_to_shelf(n, -1, 0, buf))
					return(-1);
				if(j_getstatus(buf))	/* get the jukebox status */
					return(-1);
				break;
			}
		}
		/* disk is available */
		dr = j_empty_drive(tlimit, buf);
		if(dr < 0){
			sprintf(buf, "can't find a free drive");
			return(-1);
		}
		if(j_shelf_to_drive(sh, side, dr, buf) < 0)
			return(-1);
		return(dr);
await:
		sleep(10);
		tlimit -= 10;
		if(j_getstatus(buf))	/* get the jukebox status */
			return(-1);
	}
	sprintf(buf, "disk '%s' busy", disk_to_load);
	return(-1);
}
