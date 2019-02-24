#include	<stdio.h>
#include	"scsi.h"
#include	"scsish.h"
#include	"generic/fns.h"
#include	"sony/fns.h"

s_start(int dr, char *err)
{
	int iargs[1];
	char *cargs[1];

	iargs[0] = dr;
	return(gen_start(1, iargs, 0, cargs, err));
}

s_stop(int dr, char *err)
{
	int iargs[1];
	char *cargs[1];

	iargs[0] = dr;
	return(gen_stop(1, iargs, 0, cargs, err));
}

s_eject(int dr, char *err)
{
	int iargs[1];
	char *cargs[1];

	iargs[0] = dr;
	return(sony_eject(1, iargs, 0, cargs, err));
}
