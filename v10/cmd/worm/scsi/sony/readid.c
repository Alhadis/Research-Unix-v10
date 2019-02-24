#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"../tcl.h"
#include	"fns.h"

static int
my_read(int lun, long blk, struct scsi_return *ret, char *err)
{
	struct scsi_cmd cmd;
	int n;

	cmd.bus_id = s_id;
	set10(cmd, 0x28, lun<<5, blk>>24, blk>>16, blk>>8, blk, 0, 0, 1, 0);
	n = ss_io(0, &cmd, 0, ret, 1024, err);
	return(n);
}

int
sony_readid(ClientData cd, Tcl_Interp *it, int argc, char **argv)
{
	struct scsi_return ret;
	char buf[128];
	int drive;
	long blk, lastb, zero;
	int pr = 0;
	int c;
	extern int optind;
	extern char *optarg;

	zero = 0;
	while((c = getopt(argc, argv, "vz:")) != -1)
		switch(c)
		{
		case 'v':	pr = 1; break;
		case 'z':	zero = atol(optarg); break;
		default:	USAGE_RETURN
		}
	if(optind >= argc)
		argv[--optind] = "0";
	for(; optind < argc; optind++){
		drive = atoi(argv[optind]);
		buf[0] = 0;
		blk = zero;
		if(blk == 0){
			if(my_read(drive, blk, &ret, cd->err) == 0)
				goto done;
			blk++;
		}
		for(lastb = -1;;){
			if(pr){
				printf("%d: ", blk);
				fflush(stdout);
			}
			if(my_read(drive, blk, &ret, cd->err))
				break;
			lastb = blk;
			blk = ((long *)ret.data)[9];
		}
		if(lastb < 0){
			printf("read(blk=%d) failed\n", blk);
			ERR_RETURN
		}
		if(my_read(drive, lastb, &ret, cd->err) != 0)
			ERR_RETURN
	done:
		strncpy(buf, (char *)&ret.data[42], 128);
		buf[127] = 0;
		printf("(%d,%d): '%s'\n", s_id, drive, buf);
		fflush(stdout);
	}
	return(TCL_OK);
}
