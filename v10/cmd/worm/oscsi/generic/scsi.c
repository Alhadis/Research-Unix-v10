#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"fns.h"

int
gen_scsi(int niargs, int *iargs, int ncargs, char **cargs, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;

#pragma ref ncargs
#pragma ref cargs

	switch(niargs)
	{
	case 6:
		set6(cmd, iargs[0], iargs[1], iargs[2], iargs[3], iargs[4], iargs[5]);
		break;
	case 10:
		set10(cmd, iargs[0], iargs[1], iargs[2], iargs[3], iargs[4], iargs[5], iargs[6], iargs[7], iargs[8], iargs[9]);
		break;
	default:
		sprintf(err, "number of bytes (%d) must be 6 or 10\n", niargs);
		return(1);
	}
	if(n = s_io(0, &cmd, 0, &ret, 0, err))
		return(n);
	return(0);
}
