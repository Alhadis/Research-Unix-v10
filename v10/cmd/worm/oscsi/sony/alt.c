#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
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
sony_alt(int niargs, int *iargs, int ncargs, char **cargs, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n, i;

#pragma ref ncargs
#pragma ref cargs

	if(niargs == 0)
		iargs[0] = 0;
	set6(cmd, 0xC3, iargs[0]<<5, 0, 0, 0, 0);
	if(n = s_io(0, &cmd, 0, &ret, 4096, err))
		return(n);
	for(i = 0; i < 4; i++)
		table(iargs[0], i+1, &ret.data[1024*i]);
	return(0);
}
