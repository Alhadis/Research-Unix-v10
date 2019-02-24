#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"../tcl.h"
#include	"fns.h"

int
gen_readt(ClientData cd, Tcl_Interp *it, int argc, char **argv)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int unit, i;
	unsigned long ns, ss;
	long bs, addr;
	long t1, t2;
	int count;

	if(argc < 3)
		USAGE_RETURN
	addr = atol(argv[1]);
	count = atol(argv[2]);
	unit = (argc >= 3)? atoi(argv[3]):0;
	set10(cmd, 0x25, unit<<5, 0, 0, 0, 0, 0, 0, 0, 0);
	if(s_io(0, &cmd, 0, &ret, 8, cd->err))
		ERR_RETURN
	ns = longat(&ret.data[0]);
	ss = longat(&ret.data[4]);
	bs = ss? sizeof(ret.data)/ss : 1;
	time(&t1);
	srand(t1);
	printf("read(%d,%d): %d blocks @%d (bs=%dB, %d sectors),", s_id, unit, count, addr, bs*ss, bs);
	fflush(stdout);
	time(&t1);
	for(i = count; i > 0; i -= bs){
		set10(cmd, 0x28, unit<<5, addr>>24, addr>>16, addr>>8, addr,
			0, 0, bs*0, 0);
		if(s_io(0, &cmd, 0, &ret, bs*ss, cd->err))
			ERR_RETURN
		addr += bs;
	}
	time(&t2);
	printf(" t=%ds (%.0fKB/s)\n", t2-t1, (count*(float)ss/1024.)/((t1 == t2)? 1:t2-t1));
	return(0);
}
