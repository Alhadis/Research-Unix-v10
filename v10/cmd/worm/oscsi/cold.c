#include	<stddef.h>
#include	<stdio.h>
#include	<string.h>
#include	"scsi.h"
#include	"juke.h"

static sort(char *buf);

cold_inv(char type, char *buf)
{
	Side side;
	int drive, sh, nsh;
	int n;
	char vol_id[512];
	int didit[NSHELF];

	if(j_getstatus(buf))	/* get the jukebox status */
		return(-1);
printf("getstatus done\n");
	/* first clear out nonexistent labels */
	n = 0;
	for(sh = 0; sh < NSHELF; sh++){
		if((j_status.shelf[sh]&0xC0) == 0xC0){
			n++;
			j_shelf[sh] = "there";
		} else
			j_shelf[sh] = 0;
		didit[sh] = 0;
	}
	printf("%d disks in shelves.\n", n);
	/* second, clear the drives */
	for(sh = 0; sh < NSHELF; sh++)
		if(j_shelf[sh] == 0)
			break;
	for(drive = 0; drive < 8; drive++){
		if(!j_status.lun[drive].diskin)
			continue;		/* no disk in drive */
printf("clearing drive %d:\n", drive);
		if(j_status.lun[drive].diskindrive && !j_status.lun[drive].shelfvalid){
			if(j_drive_to_shelf(drive, sh, SIDEA, buf))
				return(-1);
			for(sh++; sh < NSHELF; sh++)
				if(j_shelf[sh] == 0)
					break;
			n++;
		} else
			if(j_drive_to_shelf(drive, -1, SIDEA, buf))
				return(-1);
		printf("\n");
	}
	printf("reloading %d disks.\n", n);
	side = SIDEA;
	drive = min(nlun+1, NLUN-1);
	j_wrshelf = 1;
	for(sh = 0; sh < NSHELF; sh++){
		if(didit[sh])
			continue;
		j_shelf[sh] = 0;
		/* the C0 means disk properly present (not temp) */
		if((j_status.shelf[sh]&0xC0) == 0xC0){
			if(getvol(sh, drive, vol_id, &side))
				errexit(vol_id);
			switch(type)
			{
			case 'c':
				for(nsh = 0; j_shelf[nsh]; nsh++)
					;
				break;
			case 's':
			case 'r':
				while(j_shelf[nsh = nrand(NSHELF)])
					;
				break;
			case 'u':
			default:
				nsh = sh;
				break;
			}
			printf("%s@%d -> %d\n", vol_id, sh, nsh);
			if(j_drive_to_shelf(drive, nsh, side, buf) < 0)
				return(-1);
			j_shelf[nsh] = strdup(vol_id);
			didit[nsh] = 1;
			sleep(2);
		}
	}
	printf("process any new disks.\n");
	if(warm_inv(buf))
		return(-1);
	if(type == 's')
		return(sort(buf));
	return(0);
} 

getvol(int sh, int drive, char *vol_id, int *side)
{
	int i;
	char buf[512];

	if(j_shelf_to_drive(sh, SIDEA, drive, vol_id) < 0)
		return(-1);
	if((i = j_rvolid(drive, buf)) < 0)
		goto softerr;
	if(i == 0)
		*side = SIDEA;
	else {
		*side = SIDEB;
		if(j_drive_to_shelf(drive, sh, SIDEA, vol_id) < 0)
			return(-1);
		if(j_shelf_to_drive(sh, SIDEB, drive, vol_id) < 0)
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
	gen_reset(0, (int *)0, 0, (char **)0, buf);
	return(0);
}

static int index[NSHELF];
static
cmp(int *a, int *b)
{
	char *sa = j_shelf[*a], *sb = j_shelf[*b];

	if((sa == 0) && (sb == 0)) return(0);
	if(sa == 0) return(-1);
	if(sb == 0) return(1);
	return(strcmp(sa, sb));
}

static char *disk[8];

static
sd(int a, int b, char *err)
{
	disk[b] = j_shelf[a];
	return(j_shelf_to_drive(a, SIDEB, b, err));
}
static
ds(int a, int b, char *err)
{
	j_shelf[b] = disk[a];
	return(j_drive_to_shelf(a, b, SIDEB, err));
}

static
sort(char *errbuf)
{
	int i, j, org;

	for(i = 0; i < NSHELF; i++)
		index[i] = i;
	qsort(index, NSHELF, sizeof index[0], cmp);
	for(i = 0; i < NSHELF; i++){
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
