/* Copyright (c) 1989, 1990 AT&T --- All Rights Reserved.              */
/* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T.                */
/* The copyright notice does not imply actual or intended publication. */
/* AUTHORS:                                            */
/*     H. S. Baird - ATT-BL MH - first versions        */
/* riclib.c - Ricoh scanner-file public functions:

RIC_* functions use system I/O, for speed...
   int RIC_get_hdr(fd,RIC_hdr *) - get scanner-file header
   int RIC_line(char **)	- read one line of scanner data
	err_RIC_Line(char *,RIC_hdr) - print on stderr
   int RIC_oline(char **)	- write one line of scanner data
   char *RIC_hdr_toa()	- RIC_hdr to ascii printable string
   */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "CPU.h"
#include "stdocr.h"

#define RIC_debug 0

/* treated as local static: */
int RIC_fd;		/* file descr. */
int RIC_bpl;		/* bytes per line */
char *RIC_bf = NULL;	/* (malloc space *) holds one line of scanner data */

/* system I/O variation on fgets(3), except it replaces \n with \0,
   and returns the number of chars read (including \0) */
int RIC_fgets(bf,max,fd)
	char *bf;
	int max;
	int fd;
{   char *cp;
    int stat,mny;
	cp=bf; mny=0;
	while(((stat=read(fd,cp,1))==1)&&(++mny<max)&&((*cp)!='\n')) cp++;
	if(stat!=1) return(stat);
	else if(mny>=max) return(-1);
	*cp='\0';
	return(mny);
	}

/* read header from scanner file, return status:  1 OK, 0 EOF, -1 error */
int RIC_get_hdr( fd, hp )
	int fd;		/* should have been open'ed earlier */
	RIC_hdr *hp;
#define HL_MAX 80
#define HTERM "=, 	\n"	/* terminations for header words: "=,<sp><tab>" */
{   char *cp,*parm,hline[HL_MAX];
    int status,nrd;
	RIC_fd = fd;
	if((status=RIC_fgets(hline,HL_MAX,RIC_fd))<=0) return(status);
	while(strlen(hline)>1) {
		if(RIC_debug) err("hline \"%s\"",hline);
		parm=strtok(hline,HTERM);
		if(parm!=NULL&&strcmp(parm,"TYPE")==0) {
			if((parm=strtok(0,HTERM))!=NULL
				&&strcmp(parm,"binary")==0) ;
			else return(-1);
			}
		else if(parm!=NULL&&strcmp(parm,"WINDOW")==0) {
			if((parm=strtok(0,HTERM))!=NULL) hp->bx.a.x=atoi(parm);
			else return(-1);
			if((parm=strtok(0,HTERM))!=NULL) hp->bx.a.y=atoi(parm);
			else return(-1);
			if((parm=strtok(0,HTERM))!=NULL) hp->bx.b.x=atoi(parm)-1;
			else return(-1);
			hp->bpl=(hp->bx.b.x-hp->bx.a.x+1+7)/8;
			if((parm=strtok(0,HTERM))!=NULL) hp->bx.b.y=atoi(parm)-1;
			else return(-1);
			}
		else if(parm!=NULL&&strcmp(parm,"RES")==0) {
			if((parm=strtok(0,HTERM))!=NULL) hp->res_x=atoi(parm);
			else return(-1);
			if((parm=strtok(0,HTERM))!=NULL) hp->res_y=atoi(parm);
			else return(-1);
			}
		else return(-1);
		if((status=RIC_fgets(hline,HL_MAX,RIC_fd))<=0) return(status);
		};
	RIC_bpl=hp->bpl;
	/* allocate one extra byte in scanner buffer as a favor to RLE */
	if((RIC_bf = (char *) malloc(RIC_bpl+1))==NULL) {
		fprintf(stderr,
			"riclib:  can't alloc RIC_bf (%d bytes) - abort\n",
			RIC_bpl+1);
		return(-1);
		};
	return(1);
	}

/* write header to scanner file */
RIC_put_hdr( fd, hp )
	int fd;		/* should have been open'ed earlier */
	RIC_hdr *hp;
{   char *cp,*parm,hline[HL_MAX];
    int status,nrd;
	sprintf(hline,"TYPE=binary\n");
	write(fd,hline,strlen(hline));
	sprintf(hline,"WINDOW=%d %d %d %d\n",
			hp->bx.a.x,hp->bx.a.y,hp->bpl,hp->bx.b.y+1);
	write(fd,hline,strlen(hline));
	sprintf(hline,"RES=%d %d\n\n",hp->res_x,hp->res_y);
	write(fd,hline,strlen(hline));
	}

err_RIC_line(sl,shdr)
	char *sl;
	RIC_hdr shdr;
#define BPL 20	/* bytes to display per line */
{   char *cp,*ep;
    int bpl;		/* bytes per display line */
	bpl=0;
	for(cp=sl,ep=sl+shdr.bpl; cp!=ep; cp++) {
		fprintf(stderr,"%o ",0377&(*cp));
		if((++bpl)%BPL==0) fprintf(stderr,"\n   ");
		};
	if((bpl)%BPL!=0)fprintf(stderr,"\n");
	}

/* skip `y' lines, starting from current read pointer */
RIC_skip(y)
	int y;
{	lseek(RIC_fd,(long)(y*RIC_bpl),1);
	}

/* read next full line of scanner data, return status:  1 OK, 0 EOF, -1 ERR */
int RIC_line(lbpp)
	char **lbpp;
{   int stat;
	if( (stat=read(RIC_fd,RIC_bf,RIC_bpl)) == RIC_bpl) {
		*lbpp=RIC_bf;
		if(RIC_debug) err("read %d bytes from RIC_fd - OK",stat);
		return(1);
		}
	else { /* EOF or ERR */
		*lbpp=NULL;
		free(RIC_bf);
		if(RIC_debug) err("read from RIC_fd stat%d",stat);
		if((stat>=0)&&(stat<RIC_bpl)) return(0 /*EOF*/);
		else return(-1);
		};
	}

/* write a full line of scanner data, returning status:  1 OK, 0 EOF, -1 ERR */
int RIC_oline(fd,bf)
	int fd;
	char *bf;
{   int stat;
	if( (stat=write(fd,bf,RIC_bpl)) == RIC_bpl) {
		if(RIC_debug) err("wrote %d bytes to fd%d - OK",stat,fd);
		return(1);
		}
	else { /* ERR */
		err("write to fd%d stat%d",fd,stat);
		if((stat>=0)&&(stat<RIC_bpl)) return(0 /*EOF*/);
		else return(-1);
		};
	}

char *RIC_hdr_toa(hp)
	RIC_hdr *hp;
{   static char s[40];
	sprintf(s,"{res%d,%d bpl%d bx%s}\n",
		hp->res_x,hp->res_y,
		hp->bpl,
		bbx_toa(&(hp->bx)));
	return(s);
	}

