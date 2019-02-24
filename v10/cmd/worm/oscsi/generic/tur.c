#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"fns.h"

int
gen_tur(int niargs, int *iargs, int ncargs, char **cargs, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;

#pragma ref ncargs
#pragma ref cargs

	if(niargs == 0)
		iargs[0] = 0;
	set6(cmd, 0x00, iargs[0]<<5, 0, 0, 0, 0);
	if(n = s_io(0, &cmd, 0, &ret, 0, err))
		return(n);
	printf("(%d,%d): good status\n", s_id, iargs[0]);
	return(0);
}
