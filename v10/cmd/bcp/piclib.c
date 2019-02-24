/* Copyright (c) 1989, 1990 AT&T --- All Rights Reserved.              */
/* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T.                */
/* The copyright notice does not imply actual or intended publication. */
/* AUTHORS:                                            */
/*     H. S. Baird - ATT-BL MH - first versions        */

/* piclib.c - picture-file public functions.
   alloc_PIC_hdr()	- allocate initialized PIC_hdr, associate with FILE *
   free_PIC_hdr()	- free PIC_hdr & all malloc'ed fields
   PIC_get_hdr()	- get picture-file header
   PIC_rline()		- read one line of picture data
   PIC_put_hdr()	- get picture-file header
   PIC_wline()		- write one line of picture data
   err_PIC_Line()	- print line on stderr
   PIC_hdr_toa()	- PIC_hdr to ascii printable string
   (PIC_* functions use system I/O, for speed)
   */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "boole.h"
#include "limits.h"	/* numeric extreme values */
#include "Units.h"
#include "Coord.h"
#include "pic.h"

#define PIC_debug (0)
double atof();

PIC_hdr *alloc_PIC_hdr( fp )
    FILE *fp;	/* already-open file descr */
{   PIC_hdr *h;
	if((h=(PIC_hdr *)malloc(sizeof(PIC_hdr)))==NULL)
		abort("piclib: alloc_PIC_hdr: can't alloc");
	*h = empty_PIC_hdr;
	h->fp = fp;
	return(h);
	}

free_PIC_hdr( h )
    PIC_hdr *h;
{	if(h->parms!=NULL) free(h->parms);
	if(h->line!=NULL) free(h->line);
	if(h->pline!=NULL) free(h->pline);
	if(h->misc!=NULL) free(h->misc);
	free(h);
	}

/* system I/O variation on fgets(3), except it replaces \n with \0,
   and returns the number of chars read (including \0) */
int PIC_fgets(bf,max,fd)
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

/* Read picture file header from file descr fileno(h->fp), set up the rest of the
   header, and return status:  1 OK, 0 EOF, <0 error */
int PIC_get_hdr( h )
    PIC_hdr *h;
#define HL_MAX 300
#define HTERM "=, 	\n"	/* terminations for header words: "=,<sp><tab>" */
{   char *cp,*parm,hline[HL_MAX],oline[HL_MAX],*hl;
    int status,nrd,ch,mny;
	/* synchronize FILE * ptr with file descr ptr */
	fseek(h->fp,h->seek = lseek(fileno(h->fp),0L,1),0);
	hline[0]='\0';
	if((mny=read(fileno(h->fp),hline,5))!=5)
		return(0);
	if(!(hline[0]=='T' && hline[1]=='Y' && hline[2]=='P'
		&& hline[3]=='E' && hline[4]=='=')) {
		/* no "TYPE=" header; don't accept */
		if(0) /* don't accept */ {
			if(PIC_debug)
				err("PIC_get_hdr: can't find TYPE=... header");
			return(-1);
			}
		else /* accept as bitfile format (OBSOLESCENT) */ {
			strcpy(h->type,"bitfile");
			h->res_x = h->res_y = 0;
			fseek(h->fp,h->seek+2L,0);	/* back up to byte no. 2 */
			/* read window */
			if((ch=getc(h->fp))!=EOF) { h->bx.a.x = ch;
			 if((ch=getc(h->fp))!=EOF) { h->bx.a.x += (ch<<8);
			  if((ch=getc(h->fp))!=EOF) { h->bx.a.y = ch;
			   if((ch=getc(h->fp))!=EOF) { h->bx.a.y += (ch<<8);
			    if((ch=getc(h->fp))!=EOF) { h->bx.b.x = ch;
			     if((ch=getc(h->fp))!=EOF) { h->bx.b.x += (ch<<8);
			      if((ch=getc(h->fp))!=EOF) { h->bx.b.y = ch;
			       if((ch=getc(h->fp))!=EOF) { h->bx.b.y += (ch<<8); }}}}}}}}
			else return(0/*EOF*/);
			h->bx.b.x--;  h->bx.b.y--;
			/* synchronize file descr ptr with FILE * ptr */
			lseek(fileno(h->fp),h->seek = ftell(h->fp),0);
			};
		}
	else {
	    if((status=PIC_fgets(hline+5,HL_MAX-5,fileno(h->fp)))<=0)
		return(status);
	    strcpy(oline,hline);
	    hl=hline;  while(*hl==' ') hl++;  /* strip initial blanks */
	    while(strlen(hl)>1) {
		if(PIC_debug) err("PIC_get_hdr:  hl \"%s\"",hl);
		parm=strtok(hl,HTERM);
		if(parm!=NULL&&strcmp(parm,"TYPE")==0) {
			if((parm=strtok(0,HTERM))!=NULL) {
				strcpy(h->type,parm);
				if( (strcmp(h->type,"dump")==0)
				    || (strcmp(h->type,"pico")==0)
				    || (strcmp(h->type,"rle")==0)
				    || (strcmp(h->type,"binary")==0)
				    || (strcmp(h->type,"bitmap")==0)
				    || (strcmp(h->type,"dim")==0)
				    || (strcmp(h->type,"document-image")==0)
				    || (strcmp(h->type,"ccitt-g31")==0)
				    || (strcmp(h->type,"ccitt-g32")==0)
				    || (strcmp(h->type,"ccitt-g4")==0)
				    || (strcmp(h->type,"cdf")==0)
				    || (strcmp(h->type,"tiff")==0)
				    ) /* these types are expected */ {
					if(PIC_debug)
						err("PIC_get_hdr:  good TYPE=\"%s\"",h->type);
					}
				else {	if(PIC_debug)
						err("PIC_get_hdr:  unexpected TYPE=\"%s\"",h->type);
					return(-1);
					};
				}
			else {	if(PIC_debug)
					err("PIC_get_hdr:  1st line must be TYPE=...");
					
				return(-1);
				};
			}
		else if(parm!=NULL&&strcmp(parm,"WINDOW")==0) {
			if((parm=strtok(0,HTERM))!=NULL) h->bx.a.x=atoi(parm);
			else return(-1);
			if((parm=strtok(0,HTERM))!=NULL) h->bx.a.y=atoi(parm);
			else return(-1);
			if((parm=strtok(0,HTERM))!=NULL) h->bx.b.x=atoi(parm)-1;
			else return(-1);
			if((parm=strtok(0,HTERM))!=NULL) h->bx.b.y=atoi(parm)-1;
			else return(-1);
			}
		else if(parm!=NULL&&strcmp(parm,"RES")==0) {
			if((parm=strtok(0,HTERM))!=NULL)
				h->res_x=(int)(atof(parm)+0.5);
			else return(-1);
			if((parm=strtok(0,HTERM))!=NULL)
				h->res_y=(int)(atof(parm)+0.5);
			else return(-1);
			}
		else if(parm!=NULL&&strcmp(parm,"NCHAN")==0) {
		    int nchan;
			if((parm=strtok(0,HTERM))!=NULL) nchan=atoi(parm);
			else return(-1);
			if(nchan!=1) {
				err("PIC_get_hdr: NCHAN=%d illegal: must be 1",nchan);
				return(-1);
				};
			}
		else if(parm!=NULL&&strcmp(parm,"PARMS")==0) {
		    int nchan;
			if((parm=strtok(0,"\n"))!=NULL) {
				h->parms = strdup(parm);
				}
			else return(-1);
			}
		else if(parm!=NULL) {
		    char *cat;
			/* save all header lines with unexpected keywords */
			if(h->misc==NULL) h->misc=(char *)malloc(strlen(oline)+2);
			else {	cat=(char *)malloc(strlen(h->misc)+strlen(oline)+2);
				strcpy(cat,h->misc);
				free(h->misc);
				h->misc=cat;
				};
			strcat(h->misc,oline);
			strcat(h->misc,"\n");
			}
		else return(-1);
		if((status=PIC_fgets(hline,HL_MAX,fileno(h->fp)))<=0)
			return(status);
	        strcpy(oline,hline);
	    	hl=hline;  while(*hl==' ') hl++;  /* strip initial blanks */
		};
	    };

	if(strcmp(h->type,"cdf")==0) {
	    int stat;
		/* Compund Document Format has binary header */
		/* synchronize FILE * ptr with file descr ptr */
		fseek(h->fp,h->seek = lseek(fileno(h->fp),0L,1),0);
		if((stat=CDF_get_hdr(h)) != 1) return(stat);
		};

	if(strcmp(h->type,"tiff")==0) {
	    int stat;
		/* Tagged Image File Format has abinary header */
		/* synchronize FILE * ptr with file descr ptr */
		fseek(h->fp,h->seek = lseek(fileno(h->fp),0L,1),0);
		if((stat=TIFF_get_hdr(h)) != 1) return(stat);
		};

	/* may want to allocate a line buffer here */
	h->bpl = bbx_wid(&(h->bx));
	if(strcmp(h->type,"binary")==0)
		h->bpl = (h->bpl+7)/8;		/* round up to byte boundary */
	else if(strcmp(h->type,"bitfile")==0)
		h->bpl = 2*((h->bpl+15)/16);	/* round up to 2-byte boundary */
	else if(strcmp(h->type,"bitmap")==0)
		h->bpl = 2*((h->bpl+15)/16);	/* round up to 2-byte boundary */
	else if( strcmp(h->type,"rle")==0
		|| strcmp(h->type,"dim")==0
		|| strcmp(h->type,"document-image")==0
		|| strcmp(h->type,"cdf")==0
		|| strcmp(h->type,"cdf-mrlc")==0
		|| strcmp(h->type,"cdf-g31")==0
		|| strcmp(h->type,"cdf-g32")==0
		|| strcmp(h->type,"cdf-g4")==0
		|| strcmp(h->type,"tiff")==0 )
		h->bpl = 0;
	if(h->bpl==0) h->line = NULL;
	else {	/* allocate one extra byte in line buffer as a favor to RLE */
		if((h->line = (unsigned char *) malloc(h->bpl+1))==NULL) {
			fprintf(stderr,
				"piclib: PIC_get_hdr: can't alloc h->line (%d bytes) - abort\n",
				h->bpl+1);
			return(-1);
			};
		memset(h->line,'\0',h->bpl);
		if(strcmp(h->type,"bitfile")==0) {
			if((h->pline = (unsigned char *) malloc(h->bpl+1))==NULL) {
				fprintf(stderr,
					"piclib: PIC_get_hdr: can't alloc h->pline (%d bytes) - abort\n",
					h->bpl+1);
				return(-1);
				};
			};
		};
	h->cy = h->bx.a.y-1;	/* just prior to 1st line */
	/* synchronize FILE * ptr with file descr ptr */
	fseek(h->fp,h->seek = lseek(fileno(h->fp),0L,1),0);
	return(1);
	}

/* Read next page from AT&T Image Director file header from file descr
   fileno(h->fp), set up the rest of the header, and return status:
   1 OK, 0 EOF, <0 error */
int CDF_next_page( h )
    PIC_hdr *h;
{	return(-1);	/* unimplemented */
	}

#define HASHEADER(h) ((strcmp((h)->type,"postscript")!=0&&strcmp((h)->type,"sunraster")!=0))

/* Read AT&T Image Director file header from file descr fileno(h->fp),
   set up the rest of the header, and return status:  1 OK, 0 EOF, <0 error */
int CDF_get_hdr( h )
    PIC_hdr *h;
{   char *cp,*parm,hline[HL_MAX];
    int status,nrd,ch,mny;
#define dbg_cdf (0)
typedef struct CDF_file {
	unsigned char raw[16];
	unsigned char op,np,at;
	unsigned int l1,l2,len;
	} CDF_file;
typedef struct CDF_page {
	unsigned char raw[16];
	unsigned char op,pn,at;
	unsigned char f1,f2,f3,f4;
        unsigned int ptr;
	} CDF_page;
typedef struct CDF_recd {
	unsigned char raw[16];
	unsigned char op,a1,a2,a3;
        unsigned int len;
	Bbx bx;
	} CDF_recd;
    CDF_file fh;
    CDF_page ph;
    CDF_recd rh;
    
	if(dbg_cdf) err("CDF_get_hdr: enter %s",PIC_hdr_toa(h));
	/* synchronize FILE * ptr with file descr ptr */
	fseek(h->fp,h->seek = lseek(fileno(h->fp),0L,1),0);
	strcpy(h->type,"cdf");

	/* Read CDF File header */
	if(fread(fh.raw,1,16,h->fp)!=16) return(0);
	fh.op = fh.raw[0];
	fh.np = fh.raw[1];
	fh.at = fh.raw[2];
	fh.l1 = fh.raw[4]+256*fh.raw[5];
	fh.l2 = fh.raw[6]+256*fh.raw[7];
	fh.len = fh.raw[12]+256*(fh.raw[13]+256*(fh.raw[14]+256*fh.raw[15]));
	if(dbg_cdf) err("CDF_get_hdr: file hdr: op0x%x p%d at0x%x lim1:%u lim2:%u len%u",
		fh.op,fh.np,fh.at,fh.l1,fh.l2,fh.len);
	if(fh.op!=0xA0&&fh.op!=0xAF) abort("CDF_get_hdr: file must be CDF type");
	if(fh.np!=1) err("CDF_get_hdr: file has %d pages - ignore all but 1st",fh.np);
	switch(fh.at>>4) {
	    case 0:	h->res_x = h->res_y = 100;  break;
	    case 1:	h->res_x = h->res_y = 200;  break;
	    case 2:	h->res_x = h->res_y = 300;  break;
	    default:	err("CDF_get_hdr: file has unknown resolution nibble 0x%x -- assume 400 dpi",fh.at>>4);
			h->res_x = h->res_y = 400;
			break;
	    };
	h->bx.a.x = h->bx.a.y = 0;
	h->bx.b.x = fh.l1;
	h->bx.b.y = fh.l2;
	if(fh.op==0xAF) {
		abort("CDF_get_hdr: file has continuation header");
		};

	/* Read 1st page table entry */
	if(fread(ph.raw,1,16,h->fp)!=16) return(0);
	ph.op = ph.raw[0];
	ph.pn = ph.raw[1];
	ph.at = ph.raw[2];
	ph.ptr = ph.raw[12]+256*(ph.raw[13]+256*(ph.raw[14]+256*ph.raw[15]));
	if(dbg_cdf) err("CDF_get_hdr: page hdr: op0x%x p%d at0x%x ptr%u",
		ph.op,ph.pn,ph.at,ph.ptr);
	if(ph.op!=0x80&&ph.op!=0x8F)
		abort("CDF_get_hdr: page opcode 0x%x is peculiar",ph.op);
	if(ph.pn!=1) err("CDF_get_hdr: page %d out of order - read anyway",ph.pn);
	switch(ph.at&0x0F) {	/* low nibble */
	    case 0:	/* portrait */  break;
	    case 1:	/* landscape */
	    default:	err("CDF_get_hdr: page has peculiar format nibble 0x%x -- assume 40portrait",ph.at&0x0F);
			break;
	    };
	switch(ph.at>>4) {	/* high nibble */
	    case 0xF:	/* background is white */  break;
	    default:	err("CDF_get_hdr: page has peculiar color nibble 0x%x -- assume white",ph.at>>4);
			break;
	    };
	/* seek top of Page */
	fseek(h->fp,h->seek+=ph.ptr,0);

	/* Read CDF Record header */
	if(fread(rh.raw,1,16,h->fp)!=16) return(0);
	rh.op = rh.raw[0];
	rh.a1 = rh.raw[1];
	rh.a2 = rh.raw[2];
	rh.a3 = rh.raw[3];
	rh.len = rh.raw[4]+256*rh.raw[5];
	rh.bx.a.x = rh.raw[8]+256*rh.raw[9];
	rh.bx.a.y = rh.raw[10]+256*rh.raw[11];
	rh.bx.b.x = rh.raw[12]+256*rh.raw[13];
	rh.bx.b.y = rh.raw[14]+256*rh.raw[15];
	if(dbg_cdf) err("CDF_get_hdr: record hdr: op0x%x a0x%x,0x%x,0x%x len%u bx%s",
		rh.op,rh.a1,rh.a2,rh.a3,rh.len,bbx_toa(&rh.bx));
	switch(rh.op) {
	    case 0x20: /* Image */ break;
	    case 0x01: /* End of Page */ 
	    case 0x10: /* Text */ 
	    case 0x1F: /* Text + cont. */ 
	    case 0x50: /* Text Document */ 
	    case 0x5F: /* Text Document + cont. */ 
	    case 0x40: case 0x41: case 0x42: case 0x43: case 0x44:
	    case 0x45: case 0x46: case 0x47: case 0x48: case 0x49: case 0x4A:
	    case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F:
			/* Graphics */ 
	    default:
		abort("CDF_get_hdr: record not Image");
	    };
	if(rh.a2!=0x01) err("CDF_get_hdr: record not opaque - assume opaque");
	switch(rh.a3&0x0F) {	/* compression algorithm */
	    case 1:	/* modified run-length code */
		strcat(h->type,"-mrlc");
		break;
	    case 0:	/* raw */
		abort("CDF_get_hdr: record compression 0x%x is legal but unsupported",rh.a3&0x0F);
		break;
	    case 2:	/* ccitt_g31 */
		strcat(h->type,"-g31");
		abort("CDF_get_hdr: record compression 0x%x is legal but unsupported",rh.a3&0x0F);
		break;
	    case 3:	/* ccitt_g32 or ccitt_g4 */
		strcat(h->type,"-g32");
		abort("CDF_get_hdr: record compression 0x%x is legal but unsupported",rh.a3&0x0F);
		break;
	    default:
		abort("CDF_get_hdr: record compression 0x%x is peculiar",rh.a3&0x0F);
		break;
	    };
	switch(rh.a3>>4) {	/* resolution */
	    case 0:
		h->res_y = 100;
		break;
	    case 1:
		h->res_y = 200;
		break;
	    case 2:
		h->res_y = 300;
		break;
	    default:
		err("CDF_get_hdr: record vert resolution peculiar 0x%x - assume 200 dpi",
			rh.a3>>4);
		h->res_y = 200;
		break;
	    };
	/* synchronize file descr ptr with FILE * ptr */
	lseek(fileno(h->fp),h->seek = ftell(h->fp),0);
	if(dbg_cdf) err("CDF_get_hdr: normal exit %s",PIC_hdr_toa(h));
	return(1);
	}

/* Read Tagged Image Format File (TIFF) header, from file descr fileno(h->fp),
   set up the rest of the PIC header, and return status:  1 OK, 0 EOF, <0 error */
int TIFF_get_hdr( h )
    PIC_hdr *h;
{	err("TIFF not yet supported");
	return(-1);
	}

/* Write header to picture file, to file descriptor fileno(h->fp), and initialize
   line buffer and cy. */
PIC_put_hdr( h )
   PIC_hdr *h;
{   char *cp,*parm,hline[HL_MAX];
    int status,nrd;
    unsigned short s;
#define put_ushort(S) { s=(S); putc(s&0377,h->fp); putc(s>>8,h->fp); }

	/* write type header */
	if( strcmp(h->type,"bitfile")==0 ) {	/* bitfile header */
		put_ushort(0);
		}
	else if(HASHEADER(h)) {
		sprintf(hline,"TYPE=%s\n",h->type);
		  write(fileno(h->fp),hline,strlen(hline));
		};
	if(strcmp(h->type,"pico")==0
	    || strcmp(h->type,"dump")==0) {
		sprintf(hline,"NCHAN=1\n");
		  write(fileno(h->fp),hline,strlen(hline));
		};

	/* write window */
	h->bpl = bbx_wid(&(h->bx));
	if(strcmp(h->type,"binary")!=0
		&& strcmp(h->type,"bitfile")!=0
		&& strcmp(h->type,"bitmap")!=0
		&& HASHEADER(h)) {
		sprintf(hline,"WINDOW=%d %d %d %d\n",
				h->bx.a.x,h->bx.a.y,h->bx.b.x+1,h->bx.b.y+1);
		  write(fileno(h->fp),hline,strlen(hline));
		}
	else if(strcmp(h->type,"binary")==0) {
		h->bpl = (h->bpl+7)/8;	/* round up to byte boundary */
		sprintf(hline,"WINDOW=%d %d %d %d\n",
				h->bx.a.x,h->bx.a.y,h->bx.b.x+1,h->bx.b.y+1);
		  write(fileno(h->fp),hline,strlen(hline));
		}
	else if(strcmp(h->type,"bitmap")==0) {
		h->bpl = 2*((h->bpl+15)/16);	/* round up to 2-byte boundary */
		sprintf(hline,"WINDOW=%d %d %d %d\n",
				h->bx.a.x,h->bx.a.y,h->bx.b.x+1,h->bx.b.y+1);
		  write(fileno(h->fp),hline,strlen(hline));
		}
	else if(strcmp(h->type,"sunraster")!=0 ) {	/* bitfile window */
		h->bpl = 2*((h->bpl+15)/16);  /* round up to 2-byte boundary */
		put_ushort(h->bx.a.x);  put_ushort(h->bx.a.y);
		put_ushort(h->bx.b.x+1);  put_ushort(h->bx.b.y+1);
		fflush(h->fp);
		};
	if( ((h->res_x!=0||h->res_y!=0)) && (HASHEADER(h))
		&& strcmp(h->type,"bitfile")!=0 ) {
		sprintf(hline,"RES=%d %d\n",h->res_x,h->res_y);
		  write(fileno(h->fp),hline,strlen(hline));
		};
	if(h->parms!=NULL && strlen(h->parms)>0 && strcmp(h->type,"bitfile")!=0 ) {
		sprintf(hline,"PARMS=%s\n",h->parms);
		  write(fileno(h->fp),hline,strlen(hline));
		};
	if(h->misc!=NULL && strlen(h->misc)>0 && strcmp(h->type,"bitfile")!=0 ) {
		write(fileno(h->fp),h->misc,strlen(h->misc));
		};
	/* terminating blank line */
	if( HASHEADER(h) && strcmp(h->type,"bitfile")!=0 ) {
		hline[0] = '\n';
		write(fileno(h->fp),hline,1);
		};

	if(strcmp(h->type,"rle")==0) { h->bpl = 0;  h->line = NULL; }
	else {	/* allocate one extra byte in line buffer as a favor to RLE */
		if((h->line = (unsigned char *) malloc(h->bpl+1))==NULL) {
			fprintf(stderr,
				"piclib: PIC_put_hdr: can't alloc h->line (%d bytes) - abort\n",
				h->bpl+1);
			return(-1);
			};
		if(strcmp(h->type,"bitfile")==0) {
			memset(h->line,'\0',h->bpl);
			if((h->pline = (unsigned char *) malloc(h->bpl+1))==NULL) {
				fprintf(stderr,
					"piclib: PIC_put_hdr: can't alloc h->pline (%d bytes) - abort\n",
					h->bpl+1);
				return(-1);
				};
			};
		};
	h->cy = h->bx.a.y-1;	/* just prior to 1st line */
	/* synchronize FILE * ptr with file descr ptr */
	fseek(h->fp,h->seek = lseek(fileno(h->fp),0L,1),0);
	}

err_PIC_line(h,sl)
	PIC_hdr *h;
	char *sl;
#define BPL 20	/* bytes to display per line */
{   char *cp,*ep;
    int bpl;		/* bytes per display line */
	bpl=0;
	for(cp=sl,ep=sl+h->bpl; cp!=ep; cp++) {
		fprintf(stderr,"%o ",0377&(*cp));
		if((++bpl)%BPL==0) fprintf(stderr,"\n   ");
		};
	if((bpl)%BPL!=0)fprintf(stderr,"\n");
	}

/* Skip `y' lines of PIC file */
PIC_skip(h,y)
    PIC_hdr *h;
    int y;
{	lseek(fileno(h->fp),(long)(y*h->bpl),1);
	h->seek += y*h->bpl;
	h->cy += y;
	}

/* Read next full line of picture data, and if OK update line & cy;
   return status:  1 OK, 0 EOF, -1 ERR.
   Free buffer in header on EOF. */
int PIC_rline(h,lbpp)
    PIC_hdr *h;
    unsigned char **lbpp;
{   int stat;
	if( (stat=read(fileno(h->fp),h->line,h->bpl)) == h->bpl) {
		*lbpp=h->line;
		h->seek += h->bpl;
		h->cy++;
		if(PIC_debug) err("read %d bytes from fileno(h->fp) - OK",stat);
		return(1);
		}
	else { /* EOF or ERR */
		*lbpp=NULL;
		free(h->line);  h->line = NULL;
		if(PIC_debug) err("read from fileno(h->fp) stat%d",stat);
		if((stat>=0)&&(stat<h->bpl)) return(0 /*EOF*/);
		else return(-1);
		};
	}

/* write a full line of picture data, returning status:  1 OK, 0 EOF, -1 ERR */
int PIC_wline(h,line)
    PIC_hdr *h;
    unsigned char *line;
{   int stat;
	if( (stat=write(fileno(h->fp),line,h->bpl)) == h->bpl) {
		h->seek += h->bpl;
		h->cy++;
		if(PIC_debug) err("wrote %d bytes to fd%d - OK",stat,fileno(h->fp));
		return(1);
		}
	else { /* ERR */
		err("write to fd%d stat%d",fileno(h->fp),stat);
		if((stat>=0)&&(stat<h->bpl)) return(0 /*EOF*/);
		else return(-1);
		};
	}

char *PIC_hdr_toa(h)
    PIC_hdr *h;
{   static char s[120];
	sprintf(s,"{type=%s bx%s res%d,%d bpl%d cy%d sk%d}",
		h->type,
		bbx_toa(&(h->bx)),
		h->res_x,h->res_y,
		h->bpl,
		h->cy,
		h->seek
		);
	return(s);
	}
