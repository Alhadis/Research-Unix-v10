#define	_POSIX_SOURCE
#include	<stddef.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<stdio.h>
#include	<string.h>
#include	<errno.h>
#include	<time.h>
#include	"jukeface.h"
#include	"jukebox.h"

static j_empty_drive(Jukebox *, long tlimit, char *buf);

j_load(Jukebox *j, char *vol_id, char *buf, long tlimit)
{
	int side;
	int n, sh, dr;
	char disk_to_load[256];
	struct lun *l;

	if(j_rdshelves(j, buf))	/* read in shelf names */
		return(-1);
/*
	be careful. due to a prom change the shelf indicator
	is now off when IN a drive. so do the drive status AFTER
	the shelf status.
*/
	if(j_shstatus(j, buf))	/* get the jukebox status */
		return(-1);
	if(j_drstatus(j, buf))	/* get the jukebox status */
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
	sh = j_shelfof(j, disk_to_load);
	if(sh < 0){
		sprintf(buf, "can't find vol_id %s", disk_to_load);
		return(-1);
	}
	while(tlimit >= 0){
		for(n = 0; n < j->nluns; n++){
			l = &j->luns[n];
			if(l->shelf == sh){
				if((l->side == side) && (n < j->nworms))
					return(n);
				if(l->spunup)
					goto await;
				if(j_dr_to_sh(n, -1, 0, buf))
					return(-1);
				if(j_drstatus(j, buf))	/* get the jukebox status */
					return(-1);
				break;
			}
		}
		/* disk is available */
		dr = j_empty_drive(j, tlimit, buf);
		if(dr < 0){
			sprintf(buf, "can't find a free drive");
			return(-1);
		}
		if(j_sh_to_dr(sh, side, dr, buf) < 0)
			return(-1);
		return(dr);
await:
		sleep(10);
		tlimit -= 10;
		if(j_drstatus(j, buf))	/* get the jukebox status */
			return(-1);
	}
	sprintf(buf, "disk '%s' busy", disk_to_load);
	return(-1);
}

static
j_empty_drive(Jukebox *j, long tlimit, char *buf)
{	
	int i, tstop;
	
	tstop = time((long *)0) + tlimit;
	while(time((long *)0) <= tstop){
		/* look for empty drives */
		for(i = 0; i < j->nworms; i++)
			if(!j->luns[i].occupied)
				return(i);
		/* look for spun down drives */
		for(i = 0; i < j->nworms; i++){
			if(!j->luns[i].spunup){
				if(j_dr_to_sh(i, -1, SIDEA, buf))
					return(-1);
				else
					return(i);
			}
		}
		sleep(10);
		if(j_drstatus(j, buf))	/* get the jukebox status */
			return(-1);
	}
	return(-1);
}
