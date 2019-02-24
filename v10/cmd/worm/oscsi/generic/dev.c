#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"fns.h"

static int gen_id(int, int *, int, char **, char *);

static Function fns[] = {
	{ "capacity", "capacity [lun=0]", "L?", gen_capacity },
	{ "copy", "copy srclun start n destlun dest", "LIILII?", gen_copy },
	{ "display", "display", "", gen_display },
	{ "dev", "dev [type] # dev ? for list", "S?", gen_dev },
	{ "help", "help [cmd]", "S?", gen_help },
	{ "id", "id [target=0]", "L?", gen_id },
	{ "inq", "inq [lun=0]", "L?", gen_inq },
	{ "readt", "readt count [lun=0]", "IL?", gen_readt },
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
gen_id(int niargs, int *iargs, int ncargs, char **cargs, char *err)
{
#pragma ref ncargs
#pragma ref cargs
#pragma ref err

	if(niargs == 0)
		printf("current SCSI id = %d\n", s_id);
	else
		scsi_target(iargs[0]);
	return(0);
}
