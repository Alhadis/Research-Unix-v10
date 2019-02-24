#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"fns.h"

int
wr_diag(int niargs, int *iargs, int ncargs, char **cargs, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;
	long t;

#pragma ref niargs
#pragma ref iargs
#pragma ref ncargs
#pragma ref cargs

	t = time((long *)0);
	set6(cmd, 0x1D, 0x04, 0, 0, 0, 0);
	if(n = s_io(0, &cmd, 0, &ret, 0, err))
		return(n);
	set6(cmd, 0x1C, 0, 0, 0, 8, 0);
	if(n = s_io(0, &cmd, 0, &ret, 8, err))
		return(n);
	t = time((long *)0)-t;
	printf("selftest diagnostic (%ds)\n", t);
	if((ret.data[7] == 0) && (ret.data[2] == 0))
		printf("\tno errors\n");
	else
		printf("\terror==#%x,#%x FRU=(#%x,#%x,#%x,#%x)\n",
			ret.data[6], ret.data[7], ret.data[2],
			ret.data[3], ret.data[4], ret.data[5]);
	return(0);
}
