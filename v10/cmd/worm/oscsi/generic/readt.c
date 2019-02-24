#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"fns.h"

int
gen_readt(int niargs, int *iargs, int ncargs, char **cargs, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n, i;
	unsigned long ns, ss;
	long bs, addr;
	long t1, t2;

#pragma ref ncargs
#pragma ref cargs

	if(niargs == 1)
		iargs[1] = 0;
	set10(cmd, 0x25, iargs[1]<<5, 0, 0, 0, 0, 0, 0, 0, 0);
	if(n = s_io(0, &cmd, 0, &ret, 8, err))
		return(n);
	ns = longat(&ret.data[0]);
	ss = longat(&ret.data[4]);
	bs = ss? sizeof(ret.data)/ss : 1;
	time(&t1);
	srand(t1);
	addr = nrand(ns-iargs[0])-1;
	printf("read(%d,%d): %d blocks @%d (chunk=%dx%d),", s_id, iargs[1], iargs[0], addr, bs, ss);
	fflush(stdout);
	time(&t1);
	for(i = iargs[0]; i > 0; i -= bs){
		set10(cmd, 0x28, iargs[1]<<5, addr>>24, addr>>16, addr>>8, addr,
			0, 0, bs*0, 0);
		if(n = s_io(0, &cmd, 0, &ret, bs*ss, err))
			return(n);
		addr += bs;
	}
	time(&t2);
	printf(" t=%ds (%.0fKB/s)\n", t2-t1, (iargs[0]*(float)ss/1024.)/((t1 == t2)? 1:t2-t1));
	return(0);
}
