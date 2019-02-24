#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"fns.h"

extern char *gen_rmb[2];
extern char *gen_devtype[256];

int
wr_extinq(int niargs, int *iargs, int ncargs, char **cargs, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;
	char vendor[9], product[17];

#pragma ref niargs
#pragma ref iargs
#pragma ref ncargs
#pragma ref cargs

	set6(cmd, 0x12, 0, 0, 0, 96, 0);
	if(n = s_io(0, &cmd, 0, &ret, 96, err))
		return(n);
	fixedstr(&ret.data[8], 8, vendor);
	fixedstr(&ret.data[16], 16, product);
	printf("inq(%d,%d): %s %s, %s/%s rev=%0.4s serial=%0.8s\n",
		s_id, 0, gen_rmb[ret.data[1]>>7], gen_devtype[ret.data[0]],
		vendor, product, &ret.data[32], &ret.data[36]);
	return(0);
}
