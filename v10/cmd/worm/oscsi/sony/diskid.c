#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"fns.h"

int
sony_diskid(int niargs, int *iargs, int ncargs, char **cargs, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;

#pragma ref ncargs
#pragma ref cargs

	if(niargs == 0)
		iargs[0] = 0;
	set6(cmd, 0xC2, iargs[0]<<5, 0, 0, 0, 0);
	if(n = s_io(0, &cmd, 0, &ret, 1024, err))
		return(n);
	printf("(%d,%d) disk id block:\n", s_id, iargs[0]);
	xd(ret.data, 1024, stdout);
	return(0);
}
