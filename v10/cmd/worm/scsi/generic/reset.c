#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"../tcl.h"
#include	"fns.h"
#include	<scsi.h>

int
gen_reset(ClientData cd, Tcl_Interp *it, int argc, char **argv)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;

#pragma ref argc
#pragma ref argv
#pragma ref it

	set6(cmd, 0, 0, 0, 0, 0, 0);
	cmd.bus_id = s_id;
	cmd.flags |= SCSI_RESET | SCSI_BRESET;
	/* should probably test for some kind of error... */
	ss_io(0, &cmd, 0, &ret, 0, cd->err);
	return(TCL_OK);
}

void
soft_reset(void)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	char buf[1024];

	set6(cmd, 0, 0, 0, 0, 0, 0);
	cmd.bus_id = s_id;
	cmd.flags |= SCSI_RESET | SCSI_BRESET;
	/* should probably test for some kind of error... */
	ss_io(0, &cmd, 0, &ret, 0, buf);
}
