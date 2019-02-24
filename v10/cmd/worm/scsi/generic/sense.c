#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"../tcl.h"
#include	"fns.h"

char *argv0;		/* not a good place, is it */

int
gen_sense(ClientData cd, Tcl_Interp *it, int argc, char **argv)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int i, unit;

#pragma ref it

	if(argc <= 1)
		argv[i = 0] = "0";
	else
		i = 1;
	for(; i < argc; i++){
		unit = atoi(argv[i]);
		set6(cmd, 0x03, unit<<5, 0, 0, 4, 0);
		if(s_io(0, &cmd, 0, &ret, 4, cd->err))
			ERR_RETURN;
		printf("sense(%d,%d): ", s_id, unit);
		if((ret.data[0]&0x7F) == 0)
			printf("no error\n");
		else {
			printf("error class=0x%x, code=0x%x, sense=0x%x",
				(ret.data[0]>>4)&7, ret.data[0]&0xF, ret.data[2]&0xF);
			if(ret.data[0]&0x80)
				printf(", addr=0x%x", ret.data[3]+256L*ret.data[2]+256L*256*ret.data[1]);
			printf("\n");
		}
	}
	return(TCL_OK);
}

static char *exstab[16] =
{
	"no sense",
	"recovered error",
	"not ready",
	"medium error",
	"hardware error",
	"illegal request",
	"unit attention",
	"data protect",
	"blank check",
	"vendor specific (#9)",
	"copy aborted",
	"aborted command",
	"equal",
	"volume overflow",
	"miscompare",
	"reserved (#f)",
};

void
gen_extsense(uchar *data, char *dest, int ndata)
{
	int class;

#pragma ref ndata

	class = (data[0]>>4)&7;
	if(class == 7){
		if(data[0]&0x80)
			sprintf(dest, "extended sense: %s info=#%2.2x#%2.2x#%2.2x#%2.2x", exstab[data[2]&0xF], data[3], data[4], data[5], data[6]);
		else
			sprintf(dest, "extended sense: %s", exstab[data[2]&0xF]);
	} else {
		sprintf(dest, "sense: class=#%x, code=#%x", class, data[0]&0xF);
	}
}
