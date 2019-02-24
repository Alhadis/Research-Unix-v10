#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"../tcl.h"
#include	"fns.h"

int
gen_read(ClientData cd, Tcl_Interp *it, int argc, char **argv)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int unit, i;
	unsigned long ns, ss;
	long bs, addr, n;
	long t1, t2;
	int count;
	char *file;
	FILE *fp;

	if(argc < 2)
		USAGE_RETURN
	unit = atoi(argv[1]);
	addr = atol(argv[2]);
	count = (argc >= 3)? atoi(argv[3]):1;
	file = (argc >= 4)? argv[4]:0;
	printf("lun=%d addr=%d count=%d file=%s\n", unit, addr, count, file);
	if(file){
		if((fp = fopen(file, "w")) == NULL){
			pperror(cd->err, file);
			ERR_RETURN
		}
	} else
		fp = 0;
	set10(cmd, 0x25, unit<<5, 0, 0, 0, 0, 0, 0, 0, 0);
	if(s_io(0, &cmd, 0, &ret, 8, cd->err))
		ERR_RETURN
	ns = longat(&ret.data[0]);
	ss = longat(&ret.data[4]);
	bs = ss? sizeof(ret.data)/ss : 1;
	printf("read(%d,%d): %d blocks @%d (bs=%dB, %d sectors)\n", s_id, unit, count, addr, bs*ss, bs);
	fflush(stdout);
	for(i = count, n = bs; i > 0; i -= n){
		if(i < n) n = i;
		set10(cmd, 0x28, unit<<5, addr>>24, addr>>16, addr>>8, addr,
			0, 0, n, 0);
		if(s_io(0, &cmd, 0, &ret, n*ss, cd->err))
			ERR_RETURN
		addr += n;
		if(fp)
			fwrite(ret.data, ss, n, fp);
	}
	return(0);
}
