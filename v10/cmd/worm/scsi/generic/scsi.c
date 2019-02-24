#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"../tcl.h"
#include	"fns.h"

#define	A(n)	atoi(argv[n])

int
gen_scsi(ClientData cd, Tcl_Interp *it, int argc, char **argv)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;

	switch(argc)
	{
	case 7:
		set6(cmd, A(1), A(2), A(3), A(4), A(5), A(6));
		break;
	case 10:
		set10(cmd, A(1), A(2), A(3), A(4), A(5), A(6), A(7), A(8), A(9), A(10));
		break;
	default:
		sprintf(cd->err, "number of bytes (%d) must be 6 or 10\n", argc-1);
		return(TCL_ERROR);
	}
	if(s_io(0, &cmd, 0, &ret, 0, cd->err))
		ERR_RETURN
	return(TCL_OK);
}
