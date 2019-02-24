#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"../tcl.h"
#include	"fns.h"
#include	"wren.h"

int
wr_mpage(int pcf, int page, Field **fields, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int ndata;
	int m, msk;
	Field *f, **fp;
	static int mask[8] = { 0x0, 0x1, 0x3, 0x7, 0xF, 0x1F, 0x3F, 0x7F };

	set6(cmd, 0x1A, 0, (pcf<<6)|page, 0, 60, 0);
	if(s_io(0, &cmd, 0, &ret, -72, err))
		return(-1);
	ndata = ret.nread;
	if(((ret.data[12]&0x3F) != page) || (ret.data[13]+14!= ndata))
		printf("pg=#%x(#%x) data=#%x(#%x)\n", ret.data[12], page, ret.data[13], ndata);

	printf("setting");
	for(fp = fields; f = *fp; fp++)
		printf(" %s '%s'=%d", ((*fp)->len==1)? "bit":"count", (*fp)->name, (*fp)->nval);
	printf("; kill me if that's wrong\n");
	fflush(stdout);
	sleep(5);
	for(fp = fields; f = *fp; fp++)
		switch(f->len)
		{
		case 8:
			ret.data[f->byteoff+12] = f->nval;
			break;
		case 16:
			ret.data[f->byteoff+11] = f->nval>>8;
			ret.data[f->byteoff+12] = f->nval;
			break;
		case 1: case 2: case 3: case 4: case 5: case 6: case 7:
			msk = mask[f->len]<<f->bitoff;
			m = ret.data[f->byteoff+12]&~msk;
			ret.data[f->byteoff+12] = m|((f->nval<<f->bitoff) & msk);
			break;
		default:
			printf("unknown size %d for field %s\n", f->len, f->name);
			return(-1);
		}
	memcpy(cmd.data, ret.data, ndata);
	cmd.data[0] = cmd.data[2] = 0;
	cmd.data[12] &= 0x3F;	/* clear out top two bits */
	set6(cmd, 0x15, 0x11, 0, 0, ndata, 0);
	if(s_io(0, &cmd, ndata, &ret, 0, err))
		return(-1);
	return(TCL_OK);
}
