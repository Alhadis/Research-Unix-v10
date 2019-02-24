#define	_POSIX_SOURCE
#include	<stddef.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<stdio.h>
#include	<string.h>
#include	<ctype.h>
#include	"jukeface.h"
#include	"jukebox.h"
#include	"tcl.h"
#include	"generic/fns.h"

static sort(char *buf);

j_cold(Jukebox *j, char *type, char *err)
{
	int side;
	int drive, sh, nsh;
	int n;
	char vol_id[512];
	char *didit;

	if(j_drstatus(j, err))
		return(-1);
	if(j_shstatus(j, err))
		return(-1);
	/* first clear out nonexistent labels */
	n = 0;
	didit = (char *)malloc(j->nshelves*sizeof(char));
	for(sh = 0; sh < j->nshelves; sh++){
		if(j->shelves[sh]){
			n++;
			j->names[sh] = "there";
		} else
			j->names[sh] = 0;
		didit[sh] = 0;
	}
	printf("%d disks in shelves.\n", n);
	/* second, clear the drives */
	for(sh = 0; sh < j->nshelves; sh++)
		if(j->shelves[sh] == 0)
			break;
	for(drive = 0; drive < j->nluns; drive++){
		if(!j->luns[drive].occupied)
			continue;		/* no disk in drive */
		if(j->luns[drive].shelf < 0){
			if(j_dr_to_sh(drive, sh, SIDEA, err))
				return(-1);
			for(sh++; sh < j->nshelves; sh++)
				if(j->shelves[sh] == 0)
					break;
			n++;
		} else
			if(j_dr_to_sh(drive, -1, SIDEA, err))
				return(-1);
	}
	drive = j->nluns-1;
	if(isdigit(*type)){	/* just do one shelf */
		sh = atol(type);
		printf("single shelf %d reload\n", sh);
		if(j_rdshelves(j, err))
			return(-1);
		if(getvol(sh, drive, vol_id, &side)){
			strcpy(err, vol_id);
			return(-1);
		}
		printf("%s@%d -> %d\n", vol_id, sh, sh);
		if(j_dr_to_sh(drive, -1, side, err) < 0)
			return(-1);
		j->names[sh] = strdup(vol_id);
		j->shelves[sh] = 1;
		j_wrshelf = 1;
		return(0);
	}
	printf("reloading %d disks.\n", n);
	side = SIDEA;
	j_wrshelf = 1;
	for(sh = 0; sh < j->nshelves; sh++){
		if(didit[sh])
			continue;
		j->names[sh] = 0;
		if(j->shelves[sh]){
			printf("%d: ", sh); fflush(stdout);
			if(getvol(sh, drive, vol_id, &side)){
				strcpy(err, vol_id);
				return(-1);
			}
			j->shelves[sh] = 0;
			switch(*type)
			{
			case 'c':
				for(nsh = 0; j->shelves[nsh]; nsh++)
					;
				break;
			case 's':
			case 'r':
				while(j->shelves[nsh = nrand(j->nshelves)])
					;
				break;
			case 'u':
			default:
				nsh = sh;
				break;
			}
			printf("%s@%d -> %d\n", vol_id, sh, nsh);
			if(j_dr_to_sh(drive, nsh, side, err) < 0)
				return(-1);
			j->names[nsh] = strdup(vol_id);
			j->shelves[nsh] = 1;
			didit[nsh] = 1;
			sleep(5);
		}
	}
	printf("process any new disks.\n");
	if(j_warm(j, err))
		return(-1);
/*	if(type == 's')
		return(sort(err));
*/
	return(0);
} 

getvol(int sh, int drive, char *vol_id, int *side)
{
	int i;
	char buf[512];

	if(j_sh_to_dr(sh, SIDEA, drive, vol_id) < 0)
		return(-1);
	if((i = j_rvolid(drive, buf)) < 0)
		goto softerr;
	if(i == 0)
		*side = SIDEA;
	else {
		*side = SIDEB;
		if(j_dr_to_sh(drive, sh, SIDEA, vol_id) < 0)
			return(-1);
		if(j_sh_to_dr(sh, SIDEB, drive, vol_id) < 0)
			return(-1);
		if((i = j_rvolid(drive, buf)) < 0)
			goto softerr;
	}
	if(i > 0)
		strcpy(vol_id, UNALLOCATED);
	else {
		strcpy(vol_id, buf);
		i = strlen(vol_id)-1;
		if(i < 0){
			sprintf(buf, "apparently good superblock but null vol_id");
			goto softerr;
		} else if(vol_id[i] == 'a')
			vol_id[i] = 0;
		else if(vol_id[i] == 'b'){
			vol_id[i] = 0;
			*side = !*side;
		} else {
			sprintf(buf, "vol_id '%s' must end in a or b", vol_id);
			strcpy(vol_id, buf);
			return(-1);
		}
	}
	return(0);
softerr:
	*side = SIDEA;
	fprintf(stderr, "error in reading shelf %d: %s; proceeding\n", sh, buf);
	sprintf(vol_id, "DISK_ERROR%d", sh);
	j_reset();
	return(0);
}
#ifdef	CRAP
static int index[j->nshelves];
static
cmp(int *a, int *b)
{
	char *sa = j->shelves[*a], *sb = j->shelves[*b];

	if((sa == 0) && (sb == 0)) return(0);
	if(sa == 0) return(-1);
	if(sb == 0) return(1);
	return(strcmp(sa, sb));
}

static char *disk[8];

static
sd(int a, int b, char *err)
{
	disk[b] = j->shelves[a];
	return(j->shelves_to_drive(a, SIDEB, b, err));
}
static
ds(int a, int b, char *err)
{
	j->shelves[b] = disk[a];
	return(j_drive_to_shelf(a, b, SIDEB, err));
}

static
sort(char *errbuf)
{
	int i, j, org;

	for(i = 0; i < j->nshelves; i++)
		index[i] = i;
	qsort(index, j->nshelves, sizeof index[0], cmp);
	for(i = 0; i < j->nshelves; i++){
		if(index[i] < 0) continue;
		if(sd(org = i, NLUN-1, errbuf) < 0)
			return(-1);
		j = index[i];
		index[i] = -1;
		while(j != org){
			if(sd(j, NLUN-2, errbuf) < 0)
				return(-1);
			if(ds(NLUN-2, i, errbuf) < 0)
				return(-1);
			i = j;
			if(index[i] < 0)
				break;
			j = index[i];
			index[i] = -1;
		}
		if(ds(NLUN-1, i, errbuf) < 0)
			return(-1);
	}
	return(0);
}
#endif
