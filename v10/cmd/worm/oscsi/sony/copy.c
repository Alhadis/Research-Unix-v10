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

static char good[256];	/* by default, all BAD */
typedef enum { BAD = 0, GOOD } Searchtype;
static int copy1(int, int, int, int, int, int, int, char *);
static int search(int, int, int, int, Searchtype, char *);

int
sony_copy(int niargs, int *iargs, int ncargs, char **cargs, char *err)
{
	int n;
	int sdr = iargs[0];
	int sbase = iargs[1];
	int nblocks = iargs[2];
	int ddr = iargs[3];
	int dbase = iargs[4];
	int starget = s_id;
	int dtarget = s_id;
	int wr, unwr;
	long nb = nblocks;
	long t1, t2;
	long goo;
	int lower;
	struct scsi_return ret;
#define		TALK		10000
	extern char *ctime();

#pragma ref niargs
#pragma ref ncargs
#pragma ref cargs

	printf("copying drive (%d,%d)[%d-%d] to drive (%d,%d)[%d-%d]\n",
		starget, sdr, sbase, sbase+nblocks-1,
		dtarget, ddr, dbase, dbase+nblocks-1);
	if(sony_istatus(&ret, err))
		return(1);
	if((ret.data[100]&0x80) && (sdr == (ret.data[100]&7)))
		lower = 0;
	else if((ret.data[101]&0x80) && (sdr == (ret.data[101]&7)))
		lower = 1;
	else {
		sprintf(err, "drive %d not occupied\n", sdr);
		return(1);
	}
	good[0] = good[0x81] = good[0x82] = good[0x83] = GOOD;
	time(&t1);
	goo = -1;
	while(nblocks > 0){
		/* search for a block to copy */
		while(n = min(256, nblocks)){
			wr = search(sdr, lower, sbase, n, GOOD, err);
			if(wr < 0)
				break;
			sbase += wr;
			dbase += wr;
			nblocks -= wr;
			if(wr < n)
				break;
			PROGRESS
		}
		/* now copy until the first bad block */
		while(n = min(256, nblocks)){
			unwr = search(sdr, lower, sbase, n, BAD, err);
			if(unwr < 0)
				break;
			/*printf("writing %d-%d\n", sbase, sbase+unwr-1);/**/
			if(copy1(starget, sdr, sbase, unwr, dtarget, ddr, dbase, err))
				break;
			sbase += unwr;
			dbase += unwr;
			nblocks -= unwr;
			PROGRESS
		}
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

static int
search(int dr, int lower, int sbase, int n, Searchtype s, char *err)
{
	uchar *cp;
	struct scsi_return ret;

	if(n <= 0)
		return(0);
	if(n > 256)
		n = 256;
	if(sony_media1(dr, sbase, lower, &ret, err))
		return(-1);
	for(cp = ret.data; n-- > 0; cp++)
		if(good[*cp] != s)
			break;
	return(cp-ret.data);
}
