#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"fns.h"

int
sony_rel(int niargs, int *iargs, int ncargs, char **cargs, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n, i, j;

#pragma ref niargs

	if(ncargs == 0){
		i = 0;
		j = 0;		/* its ignored anyway */
	} else {
		i = 1;
		if((j = shelfside(cargs[0], err)) < 0)
			return(1);
	}
	set6(cmd, 0xD7, (iargs[0]<<5)|i, 0, j, 0, 0);
	if(n = s_io(0, &cmd, 0, &ret, 0, err))
		return(n);
	return(0);
}
