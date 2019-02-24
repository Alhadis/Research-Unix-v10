#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"../tcl.h"
#include	"fns.h"
#include	"wren.h"

static int msense(ClientData, int, char **);
static int mselect(ClientData, int, char **);
static int lsense(ClientData, int, char **);
Wren wr_elite =
{
	"ST41520N",
	"Elite-1",
	msense,
	mselect,
	0,
	lsense,
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

	GETPAGE(0x01, pcf, 12)
	printf("error recovery:\n\t");
	for(n = 7; n >= 0; n--)
		printf(" %s=%d", bit[n], !!(ret.data[14]&(1<<n)));
	printf("\n\t%d retries, max ecc span=%d, recov tlimit=%d\n",
		ret.data[15], ret.data[16], SHORT(22));
	return(0);
}

static int
dr(int pcf, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;

	GETPAGE(0x02, pcf, 16)
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
	static char *bit[8] = { "", "", "", "", "SURF", "Remove", "HardSec", "SoftSec" };

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
	static char *sync[4] = { "disabled", "slave", "master", "reserved" };

	GETPAGE(0x04, pcf, 24);
	printf("drive geometry:\n\t%d cyls, %d heads, %d RPM, spindle-sync=%s\n",
		(((long)ret.data[14])<<16)|SHORT(15), ret.data[17], SHORT(32),
		sync[ret.data[29]&0x3]);
	return(0);
}

static int
gcp(int pcf, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;
	static char *bit[] = { "RCD", "MF", "WCE" };

	GETPAGE(0x08, pcf, 12)
	printf("generic caching parameters:\n\t");
	for(n = 0; n < 3; n++)
		printf(" %s=%d", bit[n], !!(ret.data[14]&(1<<n)));
	printf("\n\trd retent priority=%d, wr retent priority=%d\n",
		ret.data[15]>>4&0xF, ret.data[15]&0xF);
	printf("\tprefetch: min=%d, max=%d, ceiling=%d\n",
		SHORT(18), SHORT(20), SHORT(22));
	return(0);
}

static int
vc(int pcf, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;
	static char *bit[8] = { "", "", "", "", "CE", 0, "WIE", "ZLR" };

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
	if(gcp(pcf, cd->err))
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
	{ "gc", 0x08, {
		{ "rcd", 2, 0, 1 },
		{ "rdrp", 3, 4, 4 },
		{ "minpref", 7, 0, 16 },
		{ "maxpref", 9, 0, 16 },
		{ (char *)0 },
	}},
	{ "vc", 0x38, {
		{ "csize", 2, 0, 4 },
		{ "ce", 2, 4, 1 },
		{ "wie", 2, 6, 1 },
		{ "zlr", 2, 7, 1 },
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

static int
cs(int pc, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;
	int n;

#define	DU(n)	((ret.data[n]&0x80)? "[DU]":"")

	GETLPAGE(0x30, 0/*ppc*/, pc, 0/*pp*/, 40)
	printf("cache statistics:\n");
	printf("\tblocks: %d read%s, %d written%s, %d read from cache%s\n", LONG(8), DU(6), LONG(16), DU(14), LONG(24), DU(22));
	printf("\trequests: %d in segment%s, %d out of segment%s\n", LONG(32), DU(30), LONG(40), DU(38));
	return(0);
}

static char *pcval[4] = { "current thresh", "current cum", "default thresh", "default cum" };

static int
lsense(ClientData cd, int argc, char **argv)
{
	int pcf;

/*	if(argc > 2){
usage:
		sprintf(cd->err, "usage: logsense [%s|%s|%s|%s]", pcfval[0], pcfval[1], pcfval[2], pcfval[3]);
		return(TCL_ERROR);
	}
	if(argc == 2){
		for(pcf = 3; pcf >= 0; pcf--)
			if(strcmp(pcval[pcf], argv[1]) == 0)
				break;
		if(pcf < 0)
			goto usage;
	} else
		pcf = 0;
*/
		pcf = 0;
	printf("logsense(id=%d,%s values):\n", s_id, pcval[pcf]);
	if(cs(pcf, cd->err))
		return(1);
	return(0);
}
