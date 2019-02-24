#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"../tcl.h"
#include	"fns.h"

static
internal(int n, int b1, int nb, struct scsi_return *ret, char *err)
{
	struct scsi_cmd cmd;

	set6(cmd, 0x1D, b1, 0, 0, 10, 0);
	setlongtimeout(&cmd);
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
sony_internal(ClientData cd, Tcl_Interp *it, int argc, char **argv)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	register unsigned char *d;
	int i, drive, lower, arg, j;
	long t1, t2;
	char c;
	extern char *cmesg[];
	extern char *i0com[], *i1err[], *scsicmd[], *busid[], *scsiident[];

	if(argc <= 1){
		printf("available internal commands:\n");
		for(i = 0; cmds[i]; i++)
			printf("\tinternal %d: %s\n", i, cmds[i]);
		return(TCL_OK);
	}
	switch(arg = atoi(argv[1]))
	{
	case 0:
		if(internal(0xE5, 0, 256, &ret, cd->err))
			ERR_RETURN
		printf("internal 0 (%s):\n", cmds[arg]);
		printf("Diagnostic #E5: last 16 internal tasks (drive,shelf)\n");
		for(i = 0, d = ret.data; i < 16; i++, d += 16){
			printf("[%d] %s (%d,%d) ",
				d[0], i0com[d[1]], d[2], d[3]);
			c = '[';
			for(j = 0; j < 16; j += 4){
				printf("%c#%x#%x#%x#%x", c, d[j], d[j+1], d[j+2], d[j+3]);
				c = ' ';
			}
			printf("]\n");
		}
		break;
	case 1:
		if(internal(0xE4, 0, 256, &ret, cd->err))
			ERR_RETURN
		printf("internal 1 (%s):\n", cmds[arg]);
		printf("Diagnostic #E4: last 16 errors; initiator[identify] error[sense] (cmd)\n");
		for(i = 0, d = ret.data; i < 16; i++, d += 16){
			printf("%s[%s]: %s[#%x] (%s) ",
				busid[d[0]], scsiident[d[1]], i1err[d[14]], d[15], scsicmd[d[4]]);
			c = '[';
			for(j = 0; j < 16; j += 4){
				printf("%c#%x#%x#%x#%x", c, d[j], d[j+1], d[j+2], d[j+3]);
				c = ' ';
			}
			printf("]\n");
		}
		break;
	case 2:
		printf("internal 2 (%s):\n", cmds[arg]);
		fflush(stdout);
		time(&t1);
		if(internal(0x90, 0, 8, &ret, cd->err))
			ERR_RETURN
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
		printf("internal 3 (%s):\n", cmds[arg]);
		fflush(stdout);
		time(&t1);
		if(internal(0xe0, 0, 8, &ret, cd->err))
			ERR_RETURN
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
		drive = (argc >= 3)? atoi(argv[2]) : 0;
		if(sony_istatus(&ret, cd->err))
			ERR_RETURN
		if((ret.data[100]&0x80) && (drive == (ret.data[100]&7)))
			lower = 0x100;
		else if((ret.data[101]&0x80) && (drive == (ret.data[101]&7)))
			lower = 0x200;
		else {
			fprintf(stderr, "drive %d not occupied\n", drive);
			ERR_RETURN
		}
		printf("drive %d[%ser]: %s\n", drive, (lower == 0x200)?"low":"upp", cmds[arg]);
		fflush(stdout);
		time(&t1);
		if(internal(0x18, lower, 256, &ret, cd->err))
			ERR_RETURN
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
		if(s_io(0, &cmd, 0, &ret, 20, cd->err))
			ERR_RETURN
		printf("%s: component(fatal err/err/cmds)\n", cmds[arg]);
		d = ret.data;
#define	ONE(str, x, sep)	printf("%s(%d/%d/%d)%c", str, d[x+3], d[x+2], d[x+1]+256*d[x], sep)
	
		ONE("upper drive", 4, ' ');
		ONE("lower drive", 8, ' ');
		ONE("sys control", 12, ' ');
		printf("backup mem(0/%d/%d)\n", d[19]+256*d[18], d[17]+256*d[16]);
		break;
	}
	return(TCL_OK);
}
