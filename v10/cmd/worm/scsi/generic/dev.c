#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"../tcl.h"
#include	"fns.h"

static int gen_id(ClientData, Tcl_Interp *it, int argc, char **argv);

static Function fns[] = {
	{ "capacity", "capacity [lun=0 ...]", "L?", gen_capacity },
	{ "dev", "dev [type] # dev ? for list", "S?", gen_dev },
	{ "display", "display", "", gen_display },
	{ "help", "help [cmd]", "S?", gen_help },
	{ "id", "id [target=0]", "L?", gen_id },
	{ "inq", "inq [lun=0]", "L?", gen_inq },
	{ "read", "read lun start [count [file]]", "LII?", gen_read },
	{ "readt", "readt base count [lun=0]", "IIL?", gen_readt },
	{ "reset", "reset", "", gen_reset },
	{ "scsi", "scsi bytes... # 6 or 10", "I?I?I?I?I?I?I?I?I?I?", gen_scsi },
	{ "sense", "sense [lun=0]", "L?", gen_sense },
	{ "start", "start [lun=0]", "L?", gen_start },
	{ "stop", "stop [lun=0]", "L?", gen_stop },
	{ "testunit", "testunit [lun=0", "L?", gen_tur },
	{ 0 }
};

Device genericdev = {
	"scsi", "generic scsi",
	gen_extsense,
	fns
};

static int
gen_id(ClientData err, Tcl_Interp *it, int argc, char **argv)
{
#pragma ref it
#pragma ref err

	if(argc <= 1)
		printf("current SCSI id = %d\n", s_id);
	else
		scsi_target(atoi(argv[1]));
	return(TCL_OK);
}
