#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"fns.h"

int
gen_capacity(int niargs, int *iargs, int ncargs, char **cargs, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;
	unsigned long ns, ss;

#pragma ref ncargs
#pragma ref cargs

	if(niargs == 0)
		iargs[0] = 0;
	set10(cmd, 0x25, iargs[0]<<5, 0, 0, 0, 0, 0, 0, 0, 0);
	if(n = s_io(0, &cmd, 0, &ret, 8, err))
		return(n);
	ns = longat(&ret.data[0]);
	ss = longat(&ret.data[4]);
	printf("capacity(%d,%d): %ld blocks of %ld bytes (#%xx#%x)\n", s_id, iargs[0],
		ns, ss, ns, ss);
	return(0);
}
