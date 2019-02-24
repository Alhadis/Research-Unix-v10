#include	<stddef.h>
#include	<stdio.h>
#include	<string.h>
#include	"jukeface.h"
#include	"jukebox.h"

char *j_shelf[NSHELF];
int j_wrshelf = 0;

j_rdshelves(char *err)
{
	FILE *fp;
	static haveread = 0;
	int shno;
	char vname[256];

	if(haveread)
		return(0);
	for(shno = 0; shno < NSHELF; shno++)
		j_shelf[shno] = 0;
	if((fp = fopen(JUKEDIR, "r")) == NULL){
		pperror(err, JUKEDIR);
		return(-1);
	}
	while(fscanf(fp, "%d %s\n", &shno, vname) == 2){
		if((shno < 0) || (shno >= NSHELF)){
		fprintf(stderr, "Warning: bad shelf number in %s: %d (vol_id=%s)\n",
				JUKEDIR, shno, vname);
			continue;
	    	
		}
		j_shelf[shno] = strdup(vname);
	}
	fclose(fp);
	haveread = 1;
	return(0);
}

j_wrshelves(char *err)
{
	FILE *fp;
	int shno;

	if((fp = fopen(JUKEDIR, "w")) == NULL){
		pperror(err, JUKEDIR);
		return(-1);
	}
	for(shno = 0; shno < NSHELF; shno++)
		if(j_shelf[shno])
			fprintf(fp, "%d %s\n", shno, j_shelf[shno]);
	fclose(fp);
	return(0);
}

int
j_shelfof(char *vol_id)
{
	int i;
	char buf[512];

	for(;;){
		for(i = 0; i < NSHELF; i++)
			if(j_shelf[i] && (strcmp(j_shelf[i], vol_id) == 0))
				return(i);
		if((i = warm_inv(buf)) <= 0)
			break;
	}
	if(i < 0)
		fprintf(stderr, "jukebox: %s\n", buf);
	return(-1);
}

int
j_driveof(char *vol_id)
{
	int i, sh;

	if((sh = j_shelfof(vol_id)) < 0)
		return(-1);
	for(i = 0; i < NLUN; i++)
		if(j_status.lun[i].shelfvalid && (j_status.lun[i].retshelf == sh))
			return(i);
	return(-1);
}
