#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"fns.h"

int
sony_eject(int niargs, int *iargs, int ncargs, char **cargs, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;

#pragma ref niargs
#pragma ref ncargs
#pragma ref cargs

	set6(cmd, 0xC0, iargs[0]<<5, 0, 0, 0, 0);
	if(n = s_io(0, &cmd, 0, &ret, 0, err))
		return(n);
	return(0);
}
