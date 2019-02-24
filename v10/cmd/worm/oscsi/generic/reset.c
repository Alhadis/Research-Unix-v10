#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"fns.h"
#include	<scsi.h>

int
gen_reset(int niargs, int *iargs, int ncargs, char **cargs, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;

#pragma ref niargs
#pragma ref iargs
#pragma ref ncargs
#pragma ref cargs

	set6(cmd, 0, 0, 0, 0, 0, 0);
	cmd.bus_id = s_id;
	cmd.flags |= SCSI_RESET | SCSI_BRESET;
	/* should probably test for some kind of error... */
	ss_io(0, &cmd, 0, &ret, 0, err);
	return(0);
}
