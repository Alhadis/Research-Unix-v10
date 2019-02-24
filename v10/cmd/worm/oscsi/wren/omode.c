#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"fns.h"

#define	SHORT(n)	((ret.data[n]<<8)|(ret.data[n+1]))

static int
er(int pcf, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;
	static char *bit[8] = { "DCR", "DTE", "PER", "EEC", "RC", "TB", "ARRE", "AWRE" };

	set6(cmd, 0x1A, 0, (pcf<<6)|0x01, 0, 20, 0);
	if(n = s_io(0, &cmd, 0, &ret, 20, err))
		return(n);
	printf("error recovery:\n\t");
	for(n = 7; n >= 0; n--)
		printf(" %s%s", (ret.data[14]&(1<<n))? "":"~", bit[n]);
	printf("\n\t%d retries, max ecc span=%d\n", ret.data[15], ret.data[16]);
	return(0);
}

static int
dr(int pcf, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;

	set6(cmd, 0x1A, 0, (pcf<<6)|0x02, 0, 24, 0);
	if(n = s_io(0, &cmd, 0, &ret, 24, err))
		return(n);
	printf("disconnect/reconnect:\n");
	printf("\tread reconnect=%d/256,", ret.data[14]);
	printf(" write reconnect=%d/256\n", ret.data[15]);
	return(0);
}

static int
fp(int pcf, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;
	static char *bit[8] = { "", "", "", "INS", "SURF", "Remove", "HardSec", "SoftSec" };

	set6(cmd, 0x1A, 0, (pcf<<6)|0x03, 0, 36, 0);
	if(n = s_io(0, &cmd, 0, &ret, 36, err))
		return(n);
	printf("format parameters:\n");
	printf("\tsec=%dB, trk=%d secs, interleave=%d trk skew=%d cyl skew=%d\n",
		SHORT(24), SHORT(22), SHORT(26), SHORT(28), SHORT(30));
	printf("\t%d alt sec/%d alt trk per zone(=%d trks), %d alt trks per vol\n",
		SHORT(16), SHORT(18), SHORT(14), SHORT(20));
	printf("\tdrive type:");
	for(n = 7; n >= 3; n--)
		printf(" %s%s", (ret.data[32]&(1<<n))? "":"~", bit[n]);
	printf("\n");
	return(0);
}

static int
geom(int pcf, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;

	set6(cmd, 0x1A, 0, (pcf<<6)|0x04, 0, 32, 0);
	if(n = s_io(0, &cmd, 0, &ret, 32, err))
		return(n);
	printf("drive geometry:\n\t%d cyls, %d heads\n",
		(ret.data[14]<<16)|SHORT(15), ret.data[17]);
	return(0);
}

static int
cc(int pcf, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;
	static char *bit[8] = { "", "", "", "", "CacheEnable", "RSVD", "WIE", "RSVD" };

	set6(cmd, 0x1A, 0, (pcf<<6)|0x38, 0, 28, 0);
	if(n = s_io(0, &cmd, 0, &ret, 28, err))
		return(n);
	printf("cache control:\n\t");
	for(n = 7; n >= 4; n--)
		printf(" %s%s", (ret.data[14]&(1<<n))? "":"~", bit[n]);
	printf(", cache size=%d\n", ret.data[14]&0xF);
	printf("\tprefetch: thr=%d max=%d(mult %d) min=%d(mult %d)\n",
		ret.data[15], ret.data[16], ret.data[17], ret.data[18], ret.data[19]);
	return(0);
}

int
wr_modesense(int niargs, int *iargs, int ncargs, char **cargs, char *err)
{
	int n;

#pragma ref ncargs
#pragma ref cargs
#pragma ref niargs
#pragma ref iargs

#define	PCF	0	/* current values */

	printf("mode sense(%d,0):\n", s_id);
	if(n = er(PCF, err))
		return(n);
	if(n = dr(PCF, err))
		return(n);
	if(n = fp(PCF, err))
		return(n);
	if(n = geom(PCF, err))
		return(n);
	if(n = cc(PCF, err))
		return(n);
	return(0);
}

int
wr_modeselect(int niargs, int *iargs, int ncargs, char **cargs, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;

#pragma ref niargs
#pragma ref ncargs
#pragma ref cargs

	printf("changing modes to ");
	if((iargs[0] < 256) && (iargs[0] >= 0))
		printf("er-param=%d(=#%x), ", iargs[0], iargs[0]);
	if((iargs[1] < 256) && (iargs[1] >= 0))
		printf("er-retries=%d, ", iargs[1]);
	if((iargs[2] < 256) && (iargs[2] >= 0))
		printf("read-recon=%d/256, ", iargs[2]);
	if((iargs[3] < 256) && (iargs[3] >= 0))
		printf("write-recon=%d/256, ", iargs[3]);
	if((iargs[4] < 256) && (iargs[4] >= 0))
		printf("cache %sable, ", iargs[4]?"en":"dis");
	if((iargs[5] < 256) && (iargs[5] >= 0))
		printf("cache threshold=%d, ", iargs[5]);
	if((iargs[6] < 256) && (iargs[6] >= 0))
		printf("cache max prefetch=%d, ", iargs[6]);
	if((iargs[7] < 256) && (iargs[7] >= 0))
		printf("cache size=%d, ", iargs[7]);
	printf("\nsleep(10); kill me if you disagree\n");
	fflush(stdout);
	sleep(10);
	/* do error recovery */
	if(((iargs[0] < 256) && (iargs[0] >= 0)) || ((iargs[1] < 256) && (iargs[1] >= 0))){
		set6(cmd, 0x1A, 0, (0<<6)|0x01, 0, 20, 0);
		if(n = s_io(0, &cmd, 0, &ret, 20, err))
			return(n);
		memcpy(cmd.data, ret.data, 20);
		cmd.data[14] &= ~0x10;
		if((iargs[0] < 256) && (iargs[0] >= 0))
			cmd.data[14] = iargs[0];
		if((iargs[1] < 256) && (iargs[1] >= 0))
			cmd.data[15] = iargs[1];
		set6(cmd, 0x15, 0x11, 0, 0, 20, 0);
		if(n = s_io(0, &cmd, 20, &ret, 0, err))
			return(n);
	}
	/* reconnect */
	if(((iargs[2] < 256) && (iargs[2] >= 0)) || ((iargs[3] < 256) && (iargs[3] >= 0))){
		set6(cmd, 0x1A, 0, (0<<6)|0x02, 0, 24, 0);
		if(n = s_io(0, &cmd, 0, &ret, 24, err))
			return(n);
		memcpy(cmd.data, ret.data, 24);
		if((iargs[3] < 256) && (iargs[3] >= 0))
			cmd.data[14] = iargs[3];
		if((iargs[4] < 256) && (iargs[4] >= 0))
			cmd.data[15] = iargs[4];
		set6(cmd, 0x15, 0x11, 0, 0, 24, 0);
		if(n = s_io(0, &cmd, 24, &ret, 0, err))
			return(n);
	}
	/* do cache control */
	if(((iargs[4] < 256) && (iargs[4] >= 0))
			|| ((iargs[5] < 256) && (iargs[5] >= 0))
			|| ((iargs[6] < 256) && (iargs[6] >= 0))
			|| ((iargs[7] < 256) && (iargs[7] >= 0))){
		set6(cmd, 0x1A, 0, (0<<6)|0x38, 0, 28, 0);
		if(n = s_io(0, &cmd, 0, &ret, 28, err))
			return(n);
		memcpy(cmd.data, ret.data, 28);
		cmd.data[14] &= ~0x10;
		if(iargs[4])
			cmd.data[14] |= 0x10;
		if((iargs[7] < 256) && (iargs[7] >= 0)){
			cmd.data[14] &= 0xF0;
			cmd.data[14] |= iargs[7]&0xF;
		}
		if((iargs[5] < 256) && (iargs[5] >= 0))
			cmd.data[15] = iargs[5];
		if((iargs[6] < 256) && (iargs[6] >= 0))
			cmd.data[16] = iargs[6];
		set6(cmd, 0x15, 0x11, 0, 0, 28, 0);
		if(n = s_io(0, &cmd, 28, &ret, 0, err))
			return(n);
	}
	return(0);
}
