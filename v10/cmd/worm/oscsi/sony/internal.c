#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"fns.h"

static
internal(int n, int b1, int nb, struct scsi_return *ret, char *err)
{
	struct scsi_cmd cmd;

	set6(cmd, 0x1D, b1, 0, 0, 10, 0);
	cmd.data[0] = n;
	cmd.data[1] = b1>>8;
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
	setdiag(cmd, 0, nb);
	if(n = s_io(0, &cmd, 0, ret, nb, err))
		return(n);
	return(0);
}

static char *cmds[] = {
	"internal command table",
	"error information table",
	"arm controller diagnostics",
	"scsi control board diagnostics",
	"drive controller diagnostics",
	"jukebox status",
	0
};

static char *msg1[16] =
{
	"drive not connected or powered off",
	"drive connected but no disk",
	"diagnostic aborted: write-protect",
	"diagnostic aborted: write area full",
	"urk 4", "urk 5", "urk 6", "urk 7", "urk 8", "urk 9", "urk 10",
	"urk 11", "urk 12", "urk 13", "urk 14", "urk 15"
};

static char *testn[10] =
{
	"drive on/off",
	"read disk id",
	"move",
	"seek",
	"blank sector search",
	"written sector search",
	"search writable area",
	"write",
	"ECC margin check",
	"read data compare"
};

int
sony_internal(int niargs, int *iargs, int ncargs, char **cargs, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;
	register unsigned char *d;
	int i, drive, lower;
	long t1, t2;
	extern char *cmesg[];
	extern char *i0com[], *i1err[], *scsicmd[], *busid[], *scsiident[];

#pragma ref ncargs
#pragma ref cargs

	switch(iargs[0])
	{
	case -1:
		printf("available internal commands:\n");
		for(i = 0; cmds[i]; i++)
			printf("\tinternal %d: %s\n", i, cmds[i]);
		break;
	case 0:
		if(internal(0xE5, 0, 256, &ret, err))
			return(1);
		printf("internal 0 (%s):\n", cmds[iargs[0]]);
		printf("Diagnostic #E5: last 16 internal tasks (drive,shelf)\n");
		for(i = 0, d = ret.data; i < 16; i++, d += 16){
			printf("[%d] %s (%d,%d)\n",
				d[0], i0com[d[1]], d[2], d[3]);
		}
		break;
	case 1:
		if(internal(0xE4, 0, 256, &ret, err))
			return(1);
		printf("internal 1 (%s):\n", cmds[iargs[0]]);
		printf("Diagnostic #E4: last 16 errors; initiator[identify] error[sense] (cmd)\n");
		for(i = 0, d = ret.data; i < 16; i++, d += 16){
			printf("%s[%s]: %s[#%x] (%s)\n",
				busid[d[0]], scsiident[d[1]], i1err[d[14]], d[15], scsicmd[d[4]]);
		}
		break;
	case 2:
		printf("internal 2 (%s):\n", cmds[iargs[0]]);
		fflush(stdout);
		time(&t1);
		if(internal(0x90, 0, 8, &ret, err))
			return(1);
		time(&t2);
		d = ret.data;
		if(d[0] == 0)
			printf("\tended normally");
		else
			printf("\tfailed, error codes=#%x, #%x, #%x",
				d[0], d[1], d[2]);
		printf(" (time: %lds)\n", t2-t1);
		break;
	case 3:
		printf("internal 3 (%s):\n", cmds[iargs[0]]);
		fflush(stdout);
		time(&t1);
		if(internal(0xe0, 0, 8, &ret, err))
			return(1);
		time(&t2);
		d = ret.data;
		if(d[0] == 0)
			printf("\tended normally");
		else
			printf("\tfailed, error codes=#%x, #%x, #%x",
				d[0], d[1], d[2]);
		printf(" (time: %lds)\n", t2-t1);
		break;
	case 4:
		if(niargs == 1)
			iargs[1] = 0;		/* zero default */
		drive = iargs[1];
		if(sony_istatus(&ret, err))
			return(1);
		if((ret.data[100]&0x80) && (drive == (ret.data[100]&7)))
			lower = 0x100;
		else if((ret.data[101]&0x80) && (drive == (ret.data[101]&7)))
			lower = 0x200;
		else {
			fprintf(stderr, "drive %d not occupied\n", drive);
			return(1);
		}
		printf("drive %d[%ser]: %s\n", drive, (lower == 0x200)?"low":"upp", cmds[iargs[0]]);
		fflush(stdout);
		time(&t1);
		if(internal(0x18, lower, 256, &ret, err))
			return(1);
		time(&t2);
		d = ret.data;
		if(d[1]&0x80){
			printf("diagnostic result:");
			if((d[1]&0x70) == 0)
				printf(" no faults");
			else {
				if(d[1]&0x10)
					printf(" controller-fault");
				if(d[1]&0x20)
					printf(" drive-fault");
				if(d[1]&0x10)
					printf(" disk-fault");
				printf(" (last error code 0x%2.2ux)", d[4]);
			}
		} else
			printf("diagnostic not performed: %s", msg1[d[1]&0xF]);
		printf(" (time: %lds)\n", t2-t1);
		for(i = 0; i < 10; i++)
			printf("test %d[%s]: %s\n", i, testn[i], cmesg[d[i*8+drive+8]]);
		printf("diagnostic count (drive:avail):");
		for(d += 104, i = 0; i < 8; i++, d += 2)
			printf(" %d:%d", i, d[0]+d[1]*256);
		printf("\n");
		break;
	case 5:
		set10(cmd,  0xD3, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		if(n = s_io(0, &cmd, 0, &ret, 20, err))
			return(n);
		printf("%s: component(fatal err/err/cmds)\n", cmds[iargs[0]]);
		d = ret.data;
#define	ONE(str, x, sep)	printf("%s(%d/%d/%d)%c", str, d[x+3], d[x+2], d[x+1]+256*d[x], sep)
	
		ONE("upper drive", 4, ' ');
		ONE("lower drive", 8, ' ');
		ONE("sys control", 12, ' ');
		printf("backup mem(0/%d/%d)\n", d[19]+256*d[18], d[17]+256*d[16]);
		break;
	}
	return(0);
}
