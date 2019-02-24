#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
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
gen_inq(int niargs, int *iargs, int ncargs, char **cargs, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n, i;

#pragma ref ncargs
#pragma ref cargs

	if(niargs == 0)
		for(niargs = 0; niargs < 8; niargs++)
			iargs[niargs] = niargs;
	for(i = 0; i < niargs; i++){
		set6(cmd, 0x12, iargs[i]<<5, 0, 0, 36, 0);
		if(n = s_io(0, &cmd, 0, &ret, -36, err))
			return(n);
		printf("inq(%d,%d): %s %s;", s_id, iargs[i],
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
	return(0);
}
