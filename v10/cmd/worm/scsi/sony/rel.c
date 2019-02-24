#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"../tcl.h"
#include	"fns.h"

int
sony_rel(ClientData cd, Tcl_Interp *it, int argc, char **argv)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int i, j;

	if(argc < 3){
		i = 0;
		j = 0;		/* its ignored anyway */
	} else {
		i = 1;
		if((j = shelfside(argv[2], cd->err)) < 0)
			ERR_RETURN
	}
	set6(cmd, 0xD7, (atoi(argv[1])<<5)|i, 0, j, 0, 0);
	if(s_io(0, &cmd, 0, &ret, 0, cd->err))
		ERR_RETURN
	return(TCL_OK);
}
