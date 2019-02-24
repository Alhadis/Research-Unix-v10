#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"fns.h"

#define	PROGRESS	\
		if(sbase/TALK != goo){\
			goo = sbase/TALK;\
			time(&t2);\
			printf("\tdoing block %ld at %s", goo*TALK, ctime(&t2));\
		}

static int copy1(int, int, int, int, int, int, int, char *);

int
gen_copy(int niargs, int *iargs, int ncargs, char **cargs, char *err)
{
	int n;
	int sdr = 0;
	int sbase = iargs[1];
	int nblocks = iargs[2];
	int ddr = 0;
	int dbase = iargs[4];
	int starget = iargs[0];
	int dtarget = iargs[3];
	int block = 256;
	long nb = nblocks;
	long t1, t2;
	long goo;

#define		TALK		10000
	extern char *ctime();

#pragma ref ncargs
#pragma ref cargs

	if(niargs == 6)
		block = iargs[5];
	printf("copying drive (%d,%d)[%d-%d] to drive (%d,%d)[%d-%d] blk=%dK\n",
		starget, sdr, sbase, sbase+nblocks-1,
		dtarget, ddr, dbase, dbase+nblocks-1, block);
	time(&t1);
	goo = -1;
	while(nblocks > 0){
		n = min(block, nblocks);
		printf("writing %d-%d\n", sbase, sbase+n-1);/**/
		if(copy1(starget, sdr, sbase, n, dtarget, ddr, dbase, err))
			break;
		sbase += n;
		dbase += n;
		nblocks -= n;
		PROGRESS
	}
	time(&t2);
	t2 -= t1;
	if(t2 == 0) t2 = 1;
	printf("%ds: ", t2);
	if(nblocks){
		printf("copy buggered up: sbase=%d nblks=%d dbase=%d\n",
			sbase, nblocks, dbase);
		return(1);
	}
	printf("%d blocks at %.1fKB/s\n", nb, nb/(float)t2);
	return(0);
}

static int
copy1(int st, int sd, int sb, int n, int dt, int dd, int db, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;

	set6(cmd, 0x18, sd<<5, 0, 0, 20, 0);
	cmd.data[0] = 0x10;	/* copy */
	cmd.data[1] = 0;
	cmd.data[2] = 0;
	cmd.data[3] = 0;
	cmd.data[4] = (st<<5)|sd;
	cmd.data[5] = (dt<<5)|dd;
	cmd.data[6] = 0;
	cmd.data[7] = 0;
	cmd.data[8] = n>>24;
	cmd.data[9] = n>>16;
	cmd.data[10] = n>>8;
	cmd.data[11] = n;
	cmd.data[12] = sb>>24;
	cmd.data[13] = sb>>16;
	cmd.data[14] = sb>>8;
	cmd.data[15] = sb;
	cmd.data[16] = db>>24;
	cmd.data[17] = db>>16;
	cmd.data[18] = db>>8;
	cmd.data[19] = db;
	return(s_io(0, &cmd, 20, &ret, 0, err));
}
