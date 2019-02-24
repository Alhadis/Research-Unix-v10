#define	_POSIX_SOURCE
#include	<stddef.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<errno.h>
#include	"jukeface.h"
#include	"jukebox.h"

int j_wrshelf = 0;

j_rdshelves(Jukebox *j, char *err)
{
	FILE *fp;
	static haveread = 0;
	int shno;
	char vname[256];

	if(haveread)
		return(0);
	for(shno = 0; shno < j->nshelves; shno++){
		j->names[shno] = 0;
		j->shelves[shno] = 0;
	}
	if((fp = fopen(JUKEDIR, "r")) == NULL){
		sprintf(err, "%s: %s", JUKEDIR, strerror(errno));
		return(-1);
	}
	while(fscanf(fp, "%d %s\n", &shno, vname) == 2){
		if((shno < 0) || (shno >= j->nshelves)){
			fprintf(stderr, "Warning: bad shelf number in %s: %d (vol_id=%s)\n",
				JUKEDIR, shno, vname);
			continue;
	    	
		}
		j->names[shno] = strdup(vname);
		j->shelves[shno] = 1;
	}
	fclose(fp);
	haveread = 1;
	return(0);
}

j_wrshelves(Jukebox *j, char *err)
{
	FILE *fp;
	int shno;

	if((fp = fopen(JUKEDIR, "w")) == NULL){
		sprintf(err, "%s: %s", JUKEDIR, strerror(errno));
		return(-1);
	}
	for(shno = 0; shno < j->nshelves; shno++)
		if(j->names[shno])
			fprintf(fp, "%d %s\n", shno, j->names[shno]);
	fclose(fp);
	return(0);
}

char *
j_name(Jukebox *j, int n)
{
	static char err[1024];

	if(j_rdshelves(j, err) < 0)
		return(err);
	if((n >= j->nshelves) || (j->names[n] == 0))
		return(NONAME);
	return(j->names[n]);
}

j_shelfof(Jukebox *j, char *vol_id)
{
	int i;
	char buf[512];

	for(;;){
		for(i = 0; i < j->nshelves; i++)
			if(j->shelves[i] && (strcmp(j->names[i], vol_id) == 0))
				return(i);
		if((i = j_warm(j, buf)) <= 0)
			break;
	}
	if(i < 0)
		fprintf(stderr, "jukebox: %s\n", buf);
	return(-1);
}

j_driveof(Jukebox *j, char *vol_id)
{
	int i, sh;

	if((sh = j_shelfof(j, vol_id)) < 0)
		return(-1);
	for(i = 0; i < j->nluns; i++)
		if(j->luns[i].shelf == sh)
			return(i);
	return(-1);
}
