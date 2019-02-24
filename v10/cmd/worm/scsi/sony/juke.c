#include	<stdio.h>
#include	<stdlib.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"../tcl.h"
#include	"fns.h"
#include	"../generic/fns.h"
#include	"../jukeface.h"
#include	<scsi.h>

int
j_config(Jukebox *j, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	char buf[512];

	j->nshelves = 50;
	j->nluns = 8;
	set6(cmd, 0x12, 0, 0, 0, 44, 0);
	if(s_io(0, &cmd, 0, &ret, 44, err))
		return(-1);
	switch(ret.data[37])
	{
	case 1:		j->ndrives = 1; break;
	case 2: case 3:	j->ndrives = 2; break;
	default:	j->ndrives = 0; break;
	}
	j->luns = (struct lun *)malloc(j->nluns * sizeof(struct lun));
	j->shelves = (char *)malloc(j->nshelves * sizeof(char));
	j->names = (char **)malloc(j->nshelves * sizeof(char *));

	for(j->nworms = 0; j->nworms < j->nluns; j->nworms++){
		sprintf(buf, "/dev/worm%d", j->nworms);
		if(access(buf, 0) < 0)
			return(0);
	}
	return(0);
}

extern j_drstatus(Jukebox *j, char *err)
{
	struct scsi_return ret;
	int i;
	char *where[8];
	unsigned char *d;

	if(sony_istatus(&ret, err))
		return(-1);
	for(i = 0; i < 8; i++)
		where[i] = "shelf";
	d = &ret.data[100];
	if(*d&0x80)
		where[*d&7] = "upper";
	d++;
	if(*d&0x80)
		where[*d&7] = "lower";
	d = &ret.data[16];
	for(i = 0; i < 8; i++, d += 4){
		j->luns[i].spunup = (d[0]&1) != 0;
		j->luns[i].desc = "shelf";
		j->luns[i].shelf = -1;
		j->luns[i].side = 0;
		if(j->luns[i].occupied = (d[0]&0x40) != 0){
			if(d[1]&0x80){
				j->luns[i].desc = where[i];
				if(d[2]&0x80){
					j->luns[i].shelf = (d[2]&0x7F)/2;
					j->luns[i].side = d[2]&1;
				}
			} else {
				j->luns[i].shelf = (d[1]&0x7F)/2;
				j->luns[i].side = d[1]&1;
			}
			j->shelves[j->luns[i].shelf] = 1;
		}
	}
	return(0);
}

extern j_shstatus(Jukebox *j, char *err)
{
	struct scsi_return ret;
	int i;
	unsigned char *d;

	if(sony_istatus(&ret, err))
		return(-1);
	d = &ret.data[48];
	for(i = 0; i < j->nshelves; i++)
		j->shelves[i] = (d[i]&0x80) && (d[i]&0x40);
	return(0);
}

j_eject(int dr, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;

	set6(cmd, 0xC0, dr<<5, 0, 0, 0, 0);
	return(s_io(0, &cmd, 0, &ret, 0, err));
}

j_sh_to_dr(int sh, int side, int dr, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;

	set6(cmd, 0xD6, dr<<5, 0, (sh<<1)|side, 0, 0);
	return(s_io(0, &cmd, 0, &ret, 0, err));
}

j_dr_to_sh(int dr, int sh, int side, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;

	if(sh < 0)
		set6(cmd, 0xD7, dr<<5, 0, 0, 0, 0);
	else
		set6(cmd, 0xD7, (dr<<5)|1, 0, (sh<<1)|side, 0, 0);
	return(s_io(0, &cmd, 0, &ret, 0, err));
}

j_start(int dr, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;

	set6(cmd, 0x1B, dr<<5, 0, 0, 1, 0);
	if(s_io(0, &cmd, 0, &ret, 0, err))
		return(-1);
	return(0);
}

j_stop(int dr, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;

	set6(cmd, 0x1B, dr<<5, 0, 0, 0, 0);
	if(s_io(0, &cmd, 0, &ret, 0, err))
		return(-1);
	return(0);
}
extern j_read(int, unsigned long, char *, int, char *);
extern j_write(int, unsigned long, char *, int, char *);
extern j_capacity(int, unsigned long *, unsigned long *);

j_load_unloaded(int dr, char *err)
{
	/* this is wrong;
		we should do a status every time to see ifthere are any temps
	*/
	if(j_sh_to_dr(127, SIDEA, dr, err))
		return(0);
	else
		return(1);
}

void
j_reset(void)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	char err[1024];

	set6(cmd, 0, 0, 0, 0, 0, 0);
	cmd.bus_id = s_id;
	cmd.flags |= SCSI_RESET | SCSI_BRESET;
	/* should probably test for some kind of error... */
	ss_io(0, &cmd, 0, &ret, 0, err);
}
