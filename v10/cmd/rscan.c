/*	scanning program for ricoh scanner:
    	OPTIONS:
		-rN	resolution, N :  (NOT TESTED)
				0 - 200x200
				1 - 200x100
				2 - 300x300 (default)
				3 - 240x240
		-D	dither
		-t	trace stages
		-d	detailed debugging
   16 Jan 89 (hsb):  WINDOW=0 0 wid hgt changed:
     1. wid pixels, not;  2. hgt is calculated at 11.7 inches (still in pixels)
   */
#include <stdio.h>
#include "rcocmd.h"
/*			don't change this number - hardware puts out some number
				of lines +- 5% - this makes sure we never
				overrun the mapping registers */
#define MAX 1200000
#define IOALIGN	4*512
#define IMAX  MAX  + IOALIGN
char Buf[IMAX];
int cmdstat[]	= {0, 0, 0};
int *cs = cmdstat;
char *resolution[]={
	"200 200", "200 100", "300 300", "240 240" };
int width[]={	/* in bytes, not pixels */
	216, 216, 324, 260 };
int height[]={	/* 11.7 inches high */
	2340, 1170, 3510, 2808 };
char *ares = "300 300";
int wid = 324;	/* in bytes, not pixels */
int ht = 3510;
int dither = 0;
int resol = 0;
int trace = 0;
int debug = 0;

char *device = "/dev/ricoh";

main(argc, argv)
char **argv;
{
	int scanner, i, outp, count;
	char *arg, *ptr, *b;

	argv++;
	while(--argc > 0){
		if(**argv == '-'){
			arg = *argv++;
			switch(*++arg){
			case 't':
				trace = 1;
				break;
			case 'd':
				debug = 1;
				break;
			case 'D':
				dither = 0;
				break;
			case 'r':
				resol = *++arg;
				resol -= '0';
				if(resol <0 || resol > 3)
					fprintf(stderr,"strange resolution %d using default\n",resol);
				ares = resolution[resol];
				wid = width[resol];
				ht = height[resol];
				break;

			case 'f':
				device = ++arg;
				break;
			default:
				fprintf(stderr,"unknown flag %s\n",arg);
				exit(1);
			}
		}
		else break;
	}
	if((scanner = open(device, 2)) == -1){
		fprintf(stderr,"Can't open %s\n", device);
		exit(1);
	}
	if(trace) fprintf(stderr,"opened ricoh\n");
	if(argc != 0){
		if((outp = creat(*argv,0666)) == -1){
			fprintf(stderr,"Can't create output file %s\n",*argv);
			exit(1);
		}
	}
	else if((outp = creat("Junk", 0666)) == -1){
		fprintf(stderr,"Can't create default output file junk\n");
		exit(1);
	}
	if(trace) fprintf(stderr,"created output file\n");
	sprintf(Buf,
		"TYPE=binary\nWINDOW=0 0 %d %d\nRES=%s\n\n",
		wid*8,ht,ares);
	for(b=Buf; ;b++){
		if(*b == '\n' && *(b+1) == '\n'){
			count = b - Buf +2;
			break;
		}
	}
	if(write(outp, Buf, count) != count){
		fprintf(stderr,"error while writing header\n");
		exit(1);
	}
	if(dither){
		*cs = dither;
		if((i=ioctl(scanner,RCODITHER,cmdstat)) != 0){
			fprintf(stderr,"ioctl error setting dither\n");
			exit(1);
		}
	}
	if(resol){
		*cs = resol;
		if((i=ioctl(scanner,RCORES,cmdstat)) != 0){
			fprintf(stderr,"ioctl error setting resolution\n");
			exit(1);
		}
	}
	if(trace) fprintf(stderr,"about to read\n");
	ptr = (char *)(((int)(Buf + IOALIGN-1)) & ~(IOALIGN-1));
	if((i=read(scanner, ptr, MAX)) != MAX){
		fprintf(stderr,"read error on scanner %d\n",i);
		exit(1);
	}
	if(trace) fprintf(stderr,"returned from read\n");
	if((ioctl(scanner, RCOHACK, cmdstat)) == 0) {	/* obsolete */
		fprintf(stderr, "offset %d, base = 0x%x\n", cmdstat[0], ptr);
		ptr += cmdstat[0];
	}
	if((i=write(outp, ptr, MAX))!= MAX){
		fprintf(stderr,"write error on output file\n");
		exit(1);
	}
	close(scanner);
	exit(0);
}
