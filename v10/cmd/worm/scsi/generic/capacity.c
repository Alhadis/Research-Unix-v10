#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"../tcl.h"
#include	"fns.h"

int
gen_capacity(ClientData cd, Tcl_Interp *it, int argc, char **argv)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int i, unit;
	unsigned long ns, ss;

	if(argc <= 1)
		argv[i = 0] = "0";
	else
		i = 1;
	for(; i < argc; i++){
		unit = atoi(argv[i]);
		set10(cmd, 0x25, unit<<5, 0, 0, 0, 0, 0, 0, 0, 0);
		if(s_io(0, &cmd, 0, &ret, 8, cd->err))
			ERR_RETURN
		ns = longat(&ret.data[0]);
		ss = longat(&ret.data[4]);
		printf("capacity(%d,%d): %ld blocks of %ld bytes (#%xx#%x)\n",
			s_id, unit, ns, ss, ns, ss);
	}
	return(TCL_OK);
}
