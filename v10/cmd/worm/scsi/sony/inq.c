#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"../tcl.h"
#include	"fns.h"

int
sony_inq(ClientData cd, Tcl_Interp *it, int argc, char **argv)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int i;
	int na, args[8];

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
		set6(cmd, 0x12, args[i]<<5, 0, 0, 6, 0);
		if(s_io(0, &cmd, 0, &ret, 6, cd->err))
			ERR_RETURN
		printf("inq(%d,%d): ", s_id, args[i]);
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
