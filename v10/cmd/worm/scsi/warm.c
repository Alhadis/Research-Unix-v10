#define	_POSIX_SOURCE
#include	<stddef.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<unistd.h>
#include	<string.h>
#include	"jukeface.h"
#include	"jukebox.h"

j_warm(Jukebox *j, char *buf)
{
	int side, i;
	int drive, sh;
	char vol_id[512];
	
	if(j_rdshelves(j, buf))	/* read in shelf names */
		return(-1);
	drive = j->nluns-1;
	if(j_shstatus(j, buf))	/* get the jukebox status */
		return(-1);
	for(;;){
		for(sh = 0; sh < j->nshelves; sh++)
			if(j->shelves[sh] == 0) break;
		if(sh >= j->nshelves)
			break;
		if((i = j_load_unloaded(drive, buf)) < 0)
			return(-1);
		if(i == 0)
			break;		/* no more disks */
		if(j_dr_to_sh(drive, sh, SIDEA, buf))
			return(-1);
		if(getvol(sh, drive, vol_id, &side)){
			strcpy(buf, vol_id);
			return(-1);
		}
		printf("%s -> %d\n", vol_id, sh);
		if(j_dr_to_sh(drive, sh, side, buf) < 0)
			return(-1);
		j_wrshelf = 1;
		j->names[sh] = strdup(vol_id);
		j->shelves[sh] = 1;
		sleep(3);
	}
	return(0);
}
