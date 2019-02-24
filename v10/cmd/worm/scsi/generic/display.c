#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"../tcl.h"
#include	"fns.h"

extern char *gen_rmb[2];
extern char *gen_devtype[256];

int
gen_display(ClientData cd, Tcl_Interp *it, int argc, char **argv)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n, i, old_id;
	int retv = TCL_OK;
	char rev[100], vendor[100], product[100];

#pragma ref argc
#pragma ref argv

	old_id = s_id;
	for(s_id = 0; s_id < 8; s_id++){
		printf("target %d:\n");
		set6(cmd, 0x00, 0, 0, 0, 0, 0);
		if(s_io(0, &cmd, 0, &ret, 0, cd->err))
			continue;
	printf("responded to test unit ready\n");
	continue;
		for(i = 0; i < 8; i++){
			set6(cmd, 0x12, i<<5, 0, 0, 36, 0);
			if(s_io(0, &cmd, 0, &ret, -36, cd->err)){
				it->result = cd->err;
				retv = TCL_ERROR;
				break;
			}
			if(ret.nread >= 16)
				fixedstr(&ret.data[8], 8, vendor);
			else
				sprintf(vendor, "??");
			if(ret.nread >= 32)
				fixedstr(&ret.data[16], 16, product);
			else
				sprintf(product, "??");
			if(ret.nread >= 16)
				fixedstr(&ret.data[32], 4, rev);
			else
				sprintf(vendor, "??");
			printf("\tlun(%d): %s %s, %s/%s rev=%s\n", i,
				gen_rmb[ret.data[1]>>7], gen_devtype[ret.data[0]],
				vendor, product, rev);
		}
	}
	s_id = old_id;
	return(retv);
}
