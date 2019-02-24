#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"../tcl.h"
#include	"fns.h"

int
gen_stop(ClientData cd, Tcl_Interp *it, int argc, char **argv)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int i, unit;

	if(argc <= 1)
		argv[i = 0] = "0";
	else
		i = 1;
	for(; i < argc; i++){
		unit = atoi(argv[i]);
		set6(cmd, 0x1B, unit<<5, 0, 0, 0, 0);
		if(s_io(0, &cmd, 0, &ret, 0, cd->err))
			ERR_RETURN
	}
	return(TCL_OK);
}
