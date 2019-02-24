#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"fns.h"

int
sony_set(int niargs, int *iargs, int ncargs, char **cargs, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n, i;

#pragma ref niargs
#pragma ref ncargs

	if((i = shelfside(cargs[0], err)) < 0)
		return(1);
	set6(cmd, 0xD6, iargs[0]<<5, 0, i, 0, 0);
	if(n = s_io(0, &cmd, 0, &ret, 0, err))
		return(n);
	return(0);
}
