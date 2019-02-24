#include	<stdio.h>
#include	<stddef.h>
#include	<string.h>
#include	"scsi.h"
#include	"juke.h"

j_unload(char *vol_id, char *buf)
{
	Side side;
	int i, sh, dr;
	char disk_to_unload[256];

	if(j_rdshelves(buf))	/* read in shelf names */
		return(-1);
	if(j_getstatus(buf))	/* get the jukebox status */
		return(-1);
	/* now check which side we want */

	strcpy(disk_to_unload, vol_id);
	side = SIDEA;
	sh = j_shelfof(disk_to_unload);
	if(sh < 0){
		sprintf(buf, "can not find vol_id %s", disk_to_unload);
		return(-1);
	}
	dr = -1;
	for(i = 0; i < NLUN; i++){
		printf("dr:.. %d  ", i);
		printf(" rtsh: %d\n", j_status.lun[i].retshelf);

 		/* is sh = retshelf? */

		if( (j_status.lun[i].retshelf>>1 == sh) ||
                                       (j_status.lun[i].retshelf == sh*2+1) ){   					dr = i; 
				break;
		} }
	printf("dr: %d, sh: %d, side: %d, i: %d\n", dr, sh, side, i);
	if (dr == -1){
		sprintf(buf, "no drive has vol_id %s", disk_to_unload);
		return(-1);
	}
	/* put vol_id in it's shelf*/	
	if (j_drive_to_shelf(dr, sh, side, buf) >= 0){
		sprintf(buf,"/dev/worm%d\n", dr);
		return(0);
	}
	return(-1);	
}
