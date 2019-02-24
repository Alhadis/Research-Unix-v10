#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"fns.h"

int
sony_inq(int niargs, int *iargs, int ncargs, char **cargs, char *err)
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
		set6(cmd, 0x12, iargs[i]<<5, 0, 0, 6, 0);
		if(n = s_io(0, &cmd, 0, &ret, 6, err))
			return(n);
		printf("inq(%d,%d): ", s_id, iargs[i]);
		if(ret.data[5]&0x80)
			printf("power off (0x%x)\n", ret.data[5]&0xFF);
		else if(ret.data[5]&0x40)
			printf("empty (0x%x)\n", ret.data[5]&0xFF);
		else
			printf("%s,%s,%s,%s (0x%x)\n",
				(ret.data[5]&0x08)?"write protect":"writable",
				(ret.data[5]&0x04)?"no alternate":"",
				(ret.data[5]&0x02)?"drive error":"",
				(ret.data[5]&0x01)?"ready":"not ready",
				ret.data[5]&0xFF);
	}
	return(0);
}
