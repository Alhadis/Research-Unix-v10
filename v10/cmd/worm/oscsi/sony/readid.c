#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"fns.h"

static int
my_read(int lun, long blk, struct scsi_return *ret, char *err)
{
	struct scsi_cmd cmd;
	int n;

	cmd.bus_id = s_id;
	set10(cmd, 0x28, lun<<5, blk>>24, blk>>16, blk>>8, blk, 0, 0, 1, 0);
	n = ss_io(0, &cmd, 0, ret, 1024, err);
	return(n);
}

int
sony_readid(int niargs, int *iargs, int ncargs, char **cargs, char *err)
{
	struct scsi_return ret;
	char buf[128];
	int drive = iargs[0];
	long blk, lastb;
	int pr = 0;

#pragma ref ncargs
#pragma ref cargs

	buf[0] = 0;
	if(niargs == 2){
		if((blk = iargs[1]) < 0){
			blk = -blk;
			pr = 1;
		}
	} else {
		if(my_read(drive, 0L, &ret, err) == 0)
			goto done;
		blk = 1;
	}
	for(lastb = -1;;){
		if(pr){
			printf("%d: ", blk);
		}
		if(my_read(drive, blk, &ret, err))
			break;
		lastb = blk;
		blk = ((long *)ret.data)[9];
	}
	if(lastb < 0){
		printf("read(blk=%d) failed\n", blk);
		return(1);
	}
	if(my_read(drive, lastb, &ret, err) != 0)
		return(1);
done:
	strncpy(buf, (char *)&ret.data[42], 128);
	buf[127] = 0;
	printf("(%d,%d): '%s'\n", s_id, drive, buf);
	return(0);
}
