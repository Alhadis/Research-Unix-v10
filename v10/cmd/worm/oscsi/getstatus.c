#include	<stddef.h>
#include	<stdio.h>
#include	"scsi.h"
#include	"juke.h"

struct Jstatus j_status;

static
dolun(struct Lunstatus *lun, uchar *u)
{
	lun->poweron = ((*u)&0x80) == 0;
	lun->diskin = ((*u)&0x40) != 0;
	lun->ready = ((*u)&0x01) != 0;
	u++;
	lun->diskindrive = ((*u)&0x80) != 0;
	lun->driveshelf = (*u)&0x7F;
	u++;
	lun->shelfvalid = ((*u)&0x80) != 0;
	lun->retshelf = (*u)&0x7F;
}

j_getstatus(char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int i;

	set6(cmd, 0x1D, 0, 0, 0, 10, 0);
	memset(cmd.data, 0, 10);
	cmd.data[0] = 0xE2;
	if(s_io(1, &cmd, 10, &ret, 0, err))
		return(-1);
	set6(cmd, 0x1C, 0, 0, 0, 128, 0);
	if(s_io(0, &cmd, 0, &ret, 128, err))
		return(-1);
	for(i = 0; i < 8; i++)
		dolun(&j_status.lun[i], &ret.data[16+4*i]);
	for(i = 0; i < NSHELF; i++)
		j_status.shelf[i] = ret.data[48+i];
	j_status.iounit = ret.data[98];
	j_status.carrier = ret.data[99];
	j_status.udrive = ret.data[100];
	j_status.ldrive = ret.data[101];
	return(0);
}
