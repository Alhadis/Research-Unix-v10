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

allocate(Jukebox *j, char *vol_id, char *buf)
{
	int drive, sh;
	char nbuf[512];
	
	if(j_rdshelves(j, buf))	/* read in shelf names */
		return(-1);
	if(j_shstatus(j, buf))	/* get the jukebox status */
		return(-1);
	sh = j_shelfof(j, vol_id);
	if(sh >= 0){
		sprintf(buf, "there is an existing '%s' on shelf %d", vol_id, sh);
		return(-1);
	}
	sh = j_shelfof(j, UNALLOCATED);
	if(sh < 0){
		sprintf(buf, "no unallocated disks");
		return(-1);
	}
	printf("using unallocated disk from shelf %d\n", sh);
	drive = j->nluns-1;
	if(j_sh_to_dr(sh, SIDEB, drive, buf) < 0)
		return(-1);
	sprintf(nbuf, "%sb", vol_id);
	if(j_wvolid(drive, nbuf, buf))
		return(-1);
	j_wrshelf = 1;
	j->names[sh] = strdup(vol_id);
	j->shelves[sh] = 1;
	if(j_dr_to_sh(drive, sh, SIDEB, buf) < 0)
		return(-1);
	if(j_sh_to_dr(sh, SIDEA, drive, buf) < 0)
		return(-1);
	sprintf(nbuf, "%sa", vol_id);
	if(j_wvolid(drive, nbuf, buf))
		return(-1);
	if(j_dr_to_sh(drive, sh, SIDEA, buf) < 0)
		return(-1);
	return(0);
}
