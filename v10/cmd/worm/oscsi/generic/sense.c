#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"fns.h"

int
gen_sense(int niargs, int *iargs, int ncargs, char **cargs, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;

#pragma ref ncargs
#pragma ref cargs

	if(niargs == 0)
		iargs[0] = 0;
	set6(cmd, 0x03, iargs[0]<<5, 0, 0, 4, 0);
	if(n = s_io(0, &cmd, 0, &ret, 4, err))
		return(n);
	printf("sense(%d,%d): ", s_id, iargs[0]);
	if((ret.data[0]&0x7F) == 0)
		printf("no error\n");
	else {
		printf("error class=0x%x, code=0x%x, sense=0x%x",
			(ret.data[0]>>4)&7, ret.data[0]&0xF, ret.data[2]&0xF);
		if(ret.data[0]&0x80)
			printf(", addr=0x%x", ret.data[3]+256L*ret.data[2]+256L*256*ret.data[1]);
		printf("\n");
	}
	return(0);
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
	"vendor specific (#9)",
	"copy aborted",
	"aborted command",
	"equal",
	"volume overflow",
	"miscompare",
	"reserved (#f)",
};

void
gen_extsense(uchar *data, char *dest, int ndata)
{
	int class;

	class = (data[0]>>4)&7;
	if(class == 7){
		if(data[0]&0x80)
			sprintf(dest, "extended sense: %s info=#%2.2x#%2.2x#%2.2x#%2.2x", exstab[data[2]&0xF], data[3], data[4], data[5], data[6]);
		else
			sprintf(dest, "extended sense: %s", exstab[data[2]&0xF]);
	} else {
		sprintf(dest, "sense: class=#%x, code=#%x", class, data[0]&0xF);
	}
}
