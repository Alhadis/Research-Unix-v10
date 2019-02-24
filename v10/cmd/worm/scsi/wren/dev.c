#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"../tcl.h"
#include	"fns.h"

static Function fns[] = {
	{ "diag", "diag", "", wr_diag },
	{ "extinq", "extinq", "", wr_extinq },
	{ "modesense", "modesense", "", wr_modesense },
	{ "modeselect", "modeselect {page fields}*", "IIIIIIII", wr_modeselect },
	{ "logsense", "logsense", "", wr_logsense },
	{ "logselect", "logselect {page fields}*", "IIIIIIII", wr_logselect },
	{ 0 }
};
Device wrendev = {
	"wren", "Wrens/Elite disks",
	gen_extsense,
	fns
};
