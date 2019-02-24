#include	<stdio.h>
#include	<stddef.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"../tcl.h"
#include	"fns.h"

static int cnts[256];
static char *cmsg[256];

sony_media1(int drive, long lbn, int lower, struct scsi_return *ret, char *err)
{
	struct scsi_cmd cmd;
	int n;

	set6(cmd, 0x1D, drive<<5, 0, 0, 10, 0);
	cmd.data[0] = 0x0A;	/* error margin check */
	cmd.data[1] = lower? 2:1;
	cmd.data[2] = 0;
	cmd.data[3] = 0;
	cmd.data[4] = drive;
	cmd.data[5] = lbn;
	cmd.data[6] = lbn>>8;
	cmd.data[7] = lbn>>16;
	cmd.data[8] = 0;
	cmd.data[9] = 0;
	if(n = s_io(0, &cmd, 10, ret, 0, err))
		return(n);
	setdiag(cmd, drive, 256);
	if(n = s_io(0, &cmd, 0, ret, 256, err))
		return(n);
	return(0);
}

int
sony_media(ClientData cd, Tcl_Interp *it, int argc, char **argv)
{
	struct scsi_return ret;
	uchar *d;
	int bn, c;
	char buf[256];
	int lower;
	int nline;
	int cur, curb;
	int drive;
	long lbn;
	int count;
	extern char *strdup(char *);
	int verbose = 0;
	FILE *fp = 0;
	char *fout = 0;
	extern char *optarg;
	extern int optind;

#pragma ref niargs

	while((c = getopt(argc, argv, "vf:")) != -1)
		switch(c)
		{
		case 'v':	verbose = 1; break;
		case 'f':	fout = optarg; break;
		default:	USAGE_RETURN
		}
	if(optind+3 != argc)
		USAGE_RETURN
	drive = atoi(argv[optind]);
	lbn = atol(argv[optind+1]);
	count = atoi(argv[optind+2]);
	if(fout){
		if((fp = fopen(fout, "w")) == NULL){
			pperror(cd->err, fout);
			ERR_RETURN
		}
	}
	if(sony_istatus(&ret, cd->err))
		ERR_RETURN
	if((ret.data[100]&0x80) && (drive == (ret.data[100]&7)))
		lower = 0;
	else if((ret.data[101]&0x80) && (drive == (ret.data[101]&7)))
		lower = 1;
	else {
		sprintf(cd->err, "drive %d not occupied and ready\n", drive);
		ERR_RETURN
	}
	printf("media margin check for %d blocks [%d-%d] on %s drive (%d,%d):",
		count, lbn, lbn+count-1, lower? "lower":"upper", s_id, drive);
	if(fp)
		printf(" stored in '%s'", fout);
	putchar('\n');
	if(cmsg[0] == 0){
		for(bn = 0; bn < 256; bn++){
			sprintf(buf, "rare error 0x%x", bn);
			cmsg[bn] = strdup(buf);
		}
		cmsg[0] = "good";
		cmsg[0x40] = "seek error 1 (alternated)";
		cmsg[0x41] = "seek error 2 (alternated)";
		cmsg[0x42] = "seek error 3 (alternated)";
		cmsg[0x44] = "read error 1 (alternated)";
		cmsg[0x45] = "unwritten";
		cmsg[0x46] = "read error 3 (alternated)";
		cmsg[0x81] = "<50% burst";
		cmsg[0x82] = "50-96% burst (alternated)";
		cmsg[0x83] = ">96% burst (alternated)";
		cmsg[0x84] = "uncorrectable (alternated)";
	}
#define	DO(ch,cp) if(fp) putc(ch,fp); else if(ch != cur){\
			int newb = bn+cp-ret.data;\
			if(verbose && (curb>=0)){\
				printf("%d %s@%d, ", newb-curb, cmsg[cur], curb);\
				if(++nline == 5){nline = 0; putchar('\n');}\
			}\
			cur = ch;\
			curb = newb;\
		}
	cur = 256;
	curb = -1;
	nline = 0;
	for(bn = 0; bn < 256; bn++)
		cnts[bn] = 0;
	for(bn = lbn, c = count; c >= 256; c -= 256, bn += 256){
		if(sony_media1(drive, bn, lower, &ret, cd->err))
			ERR_RETURN
		for(d = ret.data; d < &ret.data[256];){
			DO(*d, d);
			cnts[*d++]++;
		}
	}
	if(c){
		if(sony_media1(drive, bn, lower, &ret, cd->err))
			ERR_RETURN
		for(d = ret.data; c; c--){
			DO(*d, d);
			cnts[*d++]++;
		}
	}
	DO(256, d);
	if(nline)
		putchar('\n');
	printf("\t");
	for(c = 0; c < 256; c++)
		if(cnts[c])
			printf("%d %s, ", cnts[c], cmsg[c]);
	printf("\n");
	return(TCL_OK);
}
