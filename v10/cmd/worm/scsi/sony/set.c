#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"../tcl.h"
#include	"fns.h"

int
sony_set(ClientData cd, Tcl_Interp *it, int argc, char **argv)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int i;

	if(argc < 3)
		USAGE_RETURN
	if((i = shelfside(argv[1], cd->err)) < 0)
		return(1);
	set6(cmd, 0xD6, atoi(argv[2])<<5, 0, i, 0, 0);
	if(s_io(0, &cmd, 0, &ret, 0, cd->err))
		ERR_RETURN
	return(0);
}
