#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"../tcl.h"
#include	"fns.h"

int
sony_sense(ClientData cd, Tcl_Interp *it, int argc, char **argv)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int i, unit;
	char buf[4096];

#pragma ref ncargs
#pragma ref cargs

	if(argc <= 1)
		argv[i = 0] = "0";
	else
		i = 1;
	for(; i < argc; i++){
		unit = atoi(argv[i]);
		set6(cmd, 0x03, unit<<5, 0, 0, 32, 0);
		if(s_io(0, &cmd, 0, &ret, -32, cd->err))
			ERR_RETURN
		printf("sense(%d,%d): ", s_id, unit);
		sony_extsense(ret.data, buf, sizeof buf);
		printf("%s\n", buf);
	}
	return(TCL_OK);
}

static char *exstab[16] =
{
	"no sense",
	"recovered error",
	"not ready",
	"medium error",
	"hardware error",
	"illegal request",
	"unit attention",
	"data protect",
	"blank check",
	"key #9",
	"copy aborted",
	"aborted command",
	"key #c",
	"volume overflow",
	"miscompare",
	"key #f",
};

void
sony_extsense(uchar *data, char *dest, int ndata)
{
	char buf[4096];
	extern char *nesd[];

	dest[0] = 0;
	switch(data[2])
	{
	case 0:
		sprintf(dest, "no error");
		break;
	case 0x1:	/* recovered error */
		sprintf(dest, "recovered error");
		break;
	case 0xA:	/* recovered error */
		sprintf(dest, "recovered error");
		break;
	default:
		if(data[7] != 4)
			sprintf((char *)data, "warning: extra data is %d, not 4! ", data[7]);
		sprintf(buf, "sense: %s", nesd[data[8]&0x7f]);
		strcat(dest, buf);
		if(data[8]&0x80){
			sprintf(buf, " at addr #%x", data[11]+256L*data[10]+256L*256*data[9]);
			strcat(dest, buf);
		}
		sprintf(buf, ", ext sense: %s", exstab[data[2]]);
		strcat(dest, buf);
		if(data[0]&0x80){
			sprintf(buf, " info=#%x", data[6]+256L*data[5]+256L*256L*data[4]+256L*256L*256L*data[3]);
			strcat(dest, buf);
		}
		break;
	}
}
