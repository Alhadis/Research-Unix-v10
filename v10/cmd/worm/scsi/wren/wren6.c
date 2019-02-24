#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"fns.h"

#define	SHORT(n)	((ret.data[n]<<8)|(ret.data[n+1]))

static int
er_w6(int pcf, char *err)
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
	printf("\n\t%d retries, max ecc span=%d, recov tlimit=%d\n",
		ret.data[15], ret.data[16], ret.data[17]);
	return(0);
}

static int
dr_w6(int pcf, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;

	set6(cmd, 0x1A, 0, (pcf<<6)|0x02, 0, 24, 0);
	if(n = s_io(0, &cmd, 0, &ret, 24, err))
		return(n);
	printf("disconnect/reconnect:\n");
	printf("\tread reconnect=%d/256 full,", ret.data[14]);
	printf(" write reconnect=%d/256 empty\n", ret.data[15]);
	return(0);
}

static int
fp_w6(int pcf, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;
	static char *bit[8] = { "", "", "", "INS", "SURF", "Remove", "HardSec", "SoftSec" };

	set6(cmd, 0x1A, 0, (pcf<<6)|0x03, 0, 36, 0);
	if(n = s_io(0, &cmd, 0, &ret, 36, err))
		return(n);
	printf("format parameters:\n");
	printf("\tsec=%d B, trk=%d secs, interleave=%d, trk_skew=%d, cyl_skew=%d\n",
		SHORT(24), SHORT(22), SHORT(26), SHORT(28), SHORT(30));
	printf("\t%d alt_sec/%d alt_trk per zone(=%d trks), %d alt_trk per vol\n",
		SHORT(16), SHORT(18), SHORT(14), SHORT(20));
	printf("\tdrive type:");
	for(n = 7; n >= 3; n--)
		printf(" %s%s", (ret.data[32]&(1<<n))? "":"~", bit[n]);
	printf("\n");
	return(0);
}

static int
geom_w6(int pcf, char *err)
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
cc_w6(int pcf, char *err)
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

static int
er_wr2(int pcf, char *err)
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
	printf("\n\t%d retries, max ecc span=%d, %d wr retries, recov tlimit=%d\n",
		ret.data[15], ret.data[16], ret.data[20], SHORT(22));
	return(0);
}

static int
geom_wr2(int pcf, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;
	static char *sspin[4] = {
		"no spindle synch",
		"synch-spindle slave",
		"synch-spindle master",
		"synch-spindle master control",
	};

	set6(cmd, 0x1A, 0, (pcf<<6)|0x04, 0, 32, 0);
	if(n = s_io(0, &cmd, 0, &ret, 32, err))
		return(n);
	printf("drive geometry:\n\t%d cyls, %d heads, %s, rotation rate %d\n",
		(ret.data[14]<<16)|SHORT(15), ret.data[17],
		sspin[ret.data[29]&3], SHORT(32));
	return(0);
}

static int
cp_wr2(int pcf, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;
	static char *bit[8] = { "ReadCacheDisable", "", "WriteCacheEnable", "", "", "", "", "" };

	set6(cmd, 0x1A, 0, (pcf<<6)|0x38, 0, 28, 0);
	if(n = s_io(0, &cmd, 0, &ret, 28, err))
		return(n);
	printf("caching parameters:\n\t");
	for(n = 2; n >= 0; n -= 2)
		printf(" %s%s", (ret.data[14]&(1<<n))? "":"~", bit[n]);
	printf("\n\tprefetch: min=%d, max=%d, ceiling=%d\n",
		SHORT(18), SHORT(20), SHORT(22));
	return(0);
}

static int
cc_wr2(int pcf, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;
	static char *bit[8] = { "", "", "", "", "CacheEnable", "SSM", "WIE", "CCEN" };

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

typedef (*Fn)(int, char *);
static struct Drive
{
	char *type;		/* match inq field */
	char *desc;		/* print at the user */
	Fn fns[10];
} drive[] = {		/* first one is default when none match */
	{ "94181-15", "Wren VI", er_w6, dr_w6, fp_w6, geom_w6, cc_w6, 0 },
	{ "ST4767", "Wren Runner-2", er_wr2, dr_w6, fp_w6, geom_wr2, cp_wr2, cc_wr2, 0 },
	{ 0 }
};

int
wr_modesense(int niargs, int *iargs, int ncargs, char **cargs, char *err)
{
	int n, i, retv;
	char product[17];
	int found;
	struct scsi_cmd cmd;
	struct scsi_return ret;

#pragma ref ncargs
#pragma ref cargs
#pragma ref niargs
#pragma ref iargs

#define	PCF	0	/* current values */

	/* find drive type */
	set6(cmd, 0x12, 0, 0, 0, 32, 0);
	if(n = s_io(0, &cmd, 0, &ret, 32, err))
		return(n);
	fixedstr(&ret.data[16], 16, product);
	for(n = 0, found = 0; drive[n].type; n++)
		if(strcmp(product, drive[n].type) == 0){
			found = 1;
			break;
		}
	if(!found)
		n = 0;

	if(found)
		printf("mode sense(%d,0)[%s(%s)]:\n", s_id, drive[n].desc, product);
	else
		printf("mode sense(%d,0)[using %s, found '%s']:\n", s_id, drive[n].desc, product);
	for(i = 0; drive[n].fns[i]; i++)
		if(retv = (*drive[n].fns[i])(PCF, err))
			return(retv);
	return(0);
}
#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"fns.h"

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
