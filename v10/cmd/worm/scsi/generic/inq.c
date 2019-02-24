#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"../tcl.h"
#include	"fns.h"

char *gen_rmb[2] = { "nonremovable", "removable" };
char *gen_devtype[256] = {
	"direct access",
	"sequential access",
	"printer",
	"processor",
	"worm",
	"cd-rom"
};

int
gen_inq(ClientData cd, Tcl_Interp *it, int argc, char **argv)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int i;
	int args[8], na;

#pragma ref it

	na = 0;
	if(argc == 1)
		args[na++] = 0;
	else if((argc == 2) && (atoi(argv[1]) < 0)){
		for(i = 0; i < 8; i++)
			args[na++] = i;
	} else {
		for(i = 1; i < argc; i++)
			args[na++] = atoi(argv[i]);
	}
	for(i = 0; i < na; i++){
		set6(cmd, 0x12, args[i]<<5, 0, 0, 36, 0);
		if(s_io(0, &cmd, 0, &ret, -36, cd->err))
			ERR_RETURN
		printf("inq(%d,%d): %s %s;", s_id, args[i],
			gen_rmb[ret.data[1]>>7], gen_devtype[ret.data[0]]);
		if(ret.data[4] >= 16){
			char buf[256];

			fixedstr(&ret.data[8], 8, buf);
			printf(" %s", buf);
			if(ret.data[4] >= 32){
				fixedstr(&ret.data[16], 16, buf);
				printf("/%s", buf);
				if(ret.data[4] >= 36){
					fixedstr(&ret.data[32], 4, buf);
					printf(" rev=%s", buf);
				}
			}
		}
		printf(" [%d bytes]\n", ret.data[4]);
	}
	return(TCL_OK);
}
