#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"../tcl.h"
#include	"fns.h"

static
table(int drive, int tab, uchar *data)
{
	int n, i;

	n = data[6];
	printf("(%d,%d): alternate table %d (%d entries)\n", s_id, drive, tab, n);
	for(data += 0x18, i = 0; i < n; data += 4, i++)
		printf("%ld%c", data[0]+256L*data[1]+256L*256*data[2],
			(i%10 == 9)? '\n':' ');
	if((i%10) && n)
		putchar('\n');
}

int
sony_alt(ClientData cd, Tcl_Interp *it, int argc, char **argv)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int unit, i;

	if(argc <= 1)
		argv[i = 0] = "0";
	else
		i = 1;
	for(; i < argc; i++){
		unit = atoi(argv[i]);
		set6(cmd, 0xC3, unit<<5, 0, 0, 0, 0);
		if(s_io(0, &cmd, 0, &ret, 4096, cd->err))
			ERR_RETURN
		for(i = 0; i < 4; i++)
			table(unit, i+1, &ret.data[1024*i]);
	}
	return(TCL_OK);
}
