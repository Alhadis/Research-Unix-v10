#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"../tcl.h"
#include	"fns.h"

static char mtab[5][2] =
{
	'0', '0', '1', '1', '1', '2', '2', '2', '?', '?'
};
static char *brdname[] = {
	"no doard", "T.D. Systems Viking", "U.S. Design 1158"
};

int
sony_conf(ClientData cd, Tcl_Interp *it, int argc, char **argv)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n, i;
	char buf[512];

#pragma ref argc
#pragma ref argv

	set6(cmd, 0x12, 0, 0, 0, 44, 0);
	if(n = s_io(0, &cmd, 0, &ret, 44, cd->err))
		ERR_RETURN
	i = min(ret.data[37], 4);
	fixedstr(&ret.data[8], 28, buf);
	printf("config(%d,%d): %s device, '%s', %c controller%s, %c drive%s\n",
		s_id, 0, (ret.data[0] == 0x4)? "WORM":"Unknown",
		buf, mtab[i][0], (mtab[i][0] == '1')?"":"s",
		mtab[i][1], (mtab[i][1] == '1')?"":"s");
	printf("\tUnibus-SCSI controller=%s\n", brdname[ret.type]);
	printf("\tROMS:");
	if(ret.data[38] != 0xFF)
		printf(" up cont.=0x%x,", ret.data[38]);
	if(ret.data[40] != 0xFF)
		printf(" lo cont.=0x%x,", ret.data[40]);
	printf( " IF-129=0x%x, SY-46=0x%x, SS-30=0x%x\n", ret.data[36],
		ret.data[42], ret.data[43]);
	return(TCL_OK);
}
