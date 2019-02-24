#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"../tcl.h"
#include	"fns.h"

static
shelf(int i)
{
	printf(": ");
	if(i&0x80){
		printf("%s,", (i&0x40)? "disk":"temporary");
		if(i&0x10) printf("wait loading,");
		if(i&0x08) printf("wait ejection,");
		if(i&0x20) printf("use shelf instead of drive for LUN %d", i&7);
	} else
		printf("no disk");
	printf("\n");
}

int
sony_istatus(struct scsi_return *ret, char *err)
{
	struct scsi_cmd cmd;
	int n;

	set6(cmd, 0x1D, 0, 0, 0, 10, 0);
	cmd.data[0] = 0xE2;	/* internal status */
	cmd.data[1] = 0;
	cmd.data[2] = 0;
	cmd.data[3] = 0;
	cmd.data[4] = 0;
	cmd.data[5] = 0;
	cmd.data[6] = 0;
	cmd.data[7] = 0;
	cmd.data[8] = 0;
	cmd.data[9] = 0;
	if(n = s_io(0, &cmd, 10, ret, 0, err))
		return(n);
	setdiag(cmd, 0, 128);
	if(n = s_io(0, &cmd, 0, ret, 128, err))
		return(n);
	return(0);
}

int
sony_status(ClientData cd, Tcl_Interp *it, int argc, char **argv)
{
	struct scsi_return ret;
	int i, start;
	uchar *d;

#pragma ref argc
#pragma ref argv

	if(sony_istatus(&ret, cd->err))
		ERR_RETURN
	d = &ret.data[16];
	for(i = 0; i < 8; i++, d += 4){
		printf("drive %d: %sready,%sdisk in LUN,power %s,", i,
			(d[0]&1)?"":"not ", (d[0]&0x40)?"":"no ",
			(d[0]&0x80)?"off":"on");
		if(d[0]&0x40){
			if(d[1]&0x80){
				printf("disk in drive %d", d[1]&0x7f);
				if(d[2]&0x80)
					printf(", return shelf %d%c", (d[2]&0x7F)/2, "ab"[d[2]&1]);
			} else
				printf("disk in shelf %d%c (%d)", (d[1]&0x7f)/2, (d[1]&1)+'a', d[1]&0x7f);
		}
		printf("\n");
	}
	for(i = 0; i < 50;){
		for(start = i; ++i < 50;)
			if(d[i] != d[start])
				break;
		if(i == start+1)
			printf("%d", start);
		else
			printf("%d-%d", start, i-1);
		shelf(d[start]);
	}
	d += 50;
	printf("I/O shelf");
	shelf(*d);
	d++;
	printf("carrier: ");
	i = *d&0x7F;
	if(*d&0x80)
		printf("disk shelf=%d%c (%d)\n", i/2, 'a'+(i&1), i);
	else
		printf("no disk\n");
	d++;
	if(*d&0x80)
		printf("upper drive: disk, LUN=%d\n", *d&7);
	else
		printf("upper drive: no disk\n");
	d++;
	if(*d&0x80)
		printf("lower drive: disk, LUN=%d\n", *d&7);
	else
		printf("lower drive: no disk\n");
	return(TCL_OK);
}
