#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"../tcl.h"
#include	"fns.h"
#include	"wren.h"

static int msense(ClientData, int, char **);
static int mselect(ClientData, int, char **);
Wren wr_wren5 =
{
	"94181-15",
	"Wren V",
	msense,
	mselect,
	0,
	0,
	0
};

#define	SHORT(n)	((ret.data[n]<<8)|(ret.data[n+1]))
#define	LONG(n)		((((long)SHORT(n))<<16) | SHORT(n+2))
#define	GETPAGE(page, pcf, ndata)\
	{\
		set6(cmd, 0x1A, 0, (pcf<<6)|page, 0, 12+ndata, 0);\
		if(s_io(0, &cmd, 0, &ret, 12+ndata, err)) return(-1);\
		if(((ret.data[12]&0x3F) != page) || (ret.data[13]+2!= ndata))\
			printf("pg=#%x(#%x) data=#%x(#%x)\n",\
				ret.data[12], page, ret.data[13], ndata);\
	}
#define	GETLPAGE(page, ppc, pc, pp, ndata)\
	{\
		set10(cmd, 0x4D, ppc<<1, (pc<<6)|page, 0, 0, pp>>8, pp, (ndata+4)>>8, (ndata+4), 0);\
		if(s_io(0, &cmd, 0, &ret, 4+ndata, err)) return(-1);\
		if(((ret.data[0]&0x3F) != page) || (SHORT(2)!= ndata))\
			printf("pg=#%x(#%x) data=#%x(#%x)\n",\
				ret.data[0], page, SHORT(2), ndata);\
	}

static int
er(int pcf, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;
	static char *bit[8] = { "DCR", "DTE", "PER", "EER", "RC", "TB", "ARRE", "AWRE" };

	GETPAGE(0x01, pcf, 8)
	printf("error recovery:\n\t");
	for(n = 7; n >= 0; n--)
		printf(" %s=%d", bit[n], !!(ret.data[14]&(1<<n)));
	printf("\n\t%d retries, max ecc span=%d, recov tlimit=%d\n",
		ret.data[15], ret.data[16], ret.data[19]);
	return(0);
}

static int
dr(int pcf, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;

	GETPAGE(0x02, pcf, 12)
	printf("disconnect/reconnect:\n");
	printf("\tread reconnect=%d/256 full,", ret.data[14]);
	printf(" write reconnect=%d/256 empty\n", ret.data[15]);
	return(0);
}

static int
fp(int pcf, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;
	static char *bit[8] = { "", "", "", "INS", "SURF", "Remove", "HardSec", "SoftSec" };

	GETPAGE(0x03, pcf, 24);
	printf("format parameters:\n");
	printf("\tdrive type:");
	for(n = 7; n >= 3; n--)
		printf(" %s=%d", bit[n], !!(ret.data[32]&(1<<n)));
	printf("\n");
	printf("\tsec=%d B, trk=%d secs, interleave=%d, trk_skew=%d, cyl_skew=%d\n",
		SHORT(24), SHORT(22), SHORT(26), SHORT(28), SHORT(30));
	printf("\t%d alt_sec/%d alt_trk per zone(=%d trks), %d alt_trk per vol\n",
		SHORT(16), SHORT(18), SHORT(14), SHORT(20));
	return(0);
}

static int
geom(int pcf, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;

	GETPAGE(0x04, pcf, 20);
	printf("drive geometry:\n\t%d cyls, %d heads\n",
		(((long)ret.data[14])<<16)|SHORT(15), ret.data[17]);
	return(0);
}

static int
vc(int pcf, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;
	static char *bit[8] = { "", "", "", "", "CE", 0, "WIE", 0 };

	GETPAGE(0x38, pcf, 16)
	printf("vendor caching parameters:\n\t");
	for(n = 7; n >= 4; n--)
		if(bit[n])
			printf(" %s=%d", bit[n], !!(ret.data[14]&(1<<n)));
	printf(", cache size=%d\n", ret.data[14]&0xF);
	printf("\tprefetch: thr=%d max=%d(mult %d) min=%d(mult %d)\n",
		ret.data[15], ret.data[16], ret.data[17], ret.data[18], ret.data[19]);
	return(0);
}

static char *pcfval[4] = { "current", "changeable", "default", "saved" };

static int
msense(ClientData cd, int argc, char **argv)
{
	int pcf;

	if(argc > 2){
usage:
		sprintf(cd->err, "usage: modesense [%s|%s|%s|%s]", pcfval[0], pcfval[1], pcfval[2], pcfval[3]);
		return(TCL_ERROR);
	}
	if(argc == 2){
		for(pcf = 3; pcf >= 0; pcf--)
			if(strcmp(pcfval[pcf], argv[1]) == 0)
				break;
		if(pcf < 0)
			goto usage;
	} else
		pcf = 0;
	printf("modesense(id=%d,%s values):\n", s_id, pcfval[pcf]);
	if(er(pcf, cd->err))
		return(1);
	if(dr(pcf, cd->err))
		return(1);
	if(fp(pcf, cd->err))
		return(1);
	if(geom(pcf, cd->err))
		return(1);
	if(vc(pcf, cd->err))
		return(1);
	return(0);
}

static Page pages[] =
{
	{ "er", 0x1, {
		{ "dcr", 2, 0, 1 },
		{ "dte", 2, 1, 1 },
		{ "per", 2, 2, 1 },
		{ "eer", 2, 3, 1 },
		{ "rc", 2, 4, 1 },
		{ "tb", 2, 5, 1 },
		{ "arre", 2, 6, 1 },
		{ "awre", 2, 7, 1 },
		{ "read retries", 3, 0, 8 },
		{ (char *)0 },
	}},
	{ "dr", 0x2, {
		{ "buffer full", 2, 0, 8 },
		{ "buffer empty", 3, 0, 8 },
		{ (char *)0 },
	}},
	{ "vc", 0x38, {
		{ "ce", 2, 4, 1 },
		{ (char *)0 },
	}},
	{ (char *)0 }
};

static int
mselect(ClientData cd, int argc, char **argv)
{
	int page, i;
	int pcf = 0;
	Field *f, *todo[64], **fp = todo;

#pragma ref argc

	argv++;
	if(*argv){
		for(i = 0; i < 4; i++)
			if(strcmp(pcfval[i], *argv) == 0){
				pcf = i;
				argv++;
			}
	}
	if(*argv == 0){
usage:
		printf("Usage: modeselect ");
		for(i = 0; i < 4; i++)
			printf("%c%s", i?'|':'[', pcfval[i]);
		printf("]");
		for(i = 0; pages[i].name; i++)
			printf("%c%s", i?'|':' ', pages[i].name);
		printf(" fields ...\n");
		return(TCL_OK);
	}
	for(i = 0; pages[i].name; i++)
		if(strcmp(pages[i].name, *argv) == 0)
			break;
	if(pages[i].name == 0)
		goto usage;
	page = i;
	if(*++argv == 0){
fusage:
		printf("fields for page %s:", pages[page].name);
		for(i = 0; pages[page].fields[i].name; i++)
			printf(" '%s'", pages[page].fields[i].name);
		printf("\n");
		return(TCL_OK);
	}
	while(*argv){
		for(f = pages[page].fields; f->name; f++)
			if(strcmp(f->name, *argv) == 0)
				break;
		if(f->name == 0)
			goto fusage;
		if(*++argv == 0){
			sprintf(cd->err, "expected val for field %s", f->name);
			return(-1);
		}
		f->nval = atol(*argv++);
		*fp++ = f;
	}
	*fp = 0;
	return(wr_mpage(pcf, pages[page].page, todo, cd->err));
}

#ifdef	NO
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
#endif
