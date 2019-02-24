/* Copyright (c) 1989, 1990 AT&T --- All Rights Reserved.              */
/* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T.                */
/* The copyright notice does not imply actual or intended publication. */
/* AUTHORS:                                            */
/*     T. Pavlidis - ATT-BL MH - first versions        */
/*     H. S. Baird - ATT-BL MH - first versions        */

/* rlelib.c - subroutines for writing/reading run-length-encoded format files.
   BUG:  Uses system I/O for speed, but sacrifices machine-independence:
   should use fioi.h macroes instead.
   See rle.h for summary of rle file and format of public functions.
   Writing a rle file:
	fd=creat()
	RLE_put_hdr(fd,hp)	- once: allocate buffers, write header
	RLE_put_Line(fd,lp)	- repeatedly
	RLE_close(fd)		- once: flush and deallocate buffers
   Reading a rle file:
	fd=open()
	RLE_open(fd,hp)		- once: read header, 1st line into buffer
	RLE_get_Line(l,r)	- repeatedly (on EOF, deallocates buffer)
   The user can read line-at-a-time or run-at-a-time, but should not intermix them:
        line-at-a-time:
            (RLE_Line *) RLE_get_Line(l,r) - return ptr to next line with some run
			in the interval [l,r];
			read from opened file (NULL if EOF, abort on error)
			allocates large buffer and frees it when done
        run-at-a-time:
	    int RLE_get_Run(&R) - read next run into (RLE_Run) R
			returns the line number (-1 for EOF, abort on ERROR)
			does not allocate large buffer;
			if only a few runs will be used, can be faster
 */

#include <stdio.h>
#define LIBC_INCL 1
#include "CPU.h"
#include "stdocr.h"
#include "rle.h"

#define RLE_dbg F
#define dbg_rd F
#define dbg_wr F

/* Treated as local static: */
typedef struct RLE_global {
	int fd;			/* rle file descriptor */
	boolean eof;
	int cy, cx;		/* current line no., x-coord */
	short runs, nrun;	/* no. runs, next run in current line */
	RLE_Line *lp;		/* pointer to line (malloc space) */
	int bpl;		/* maximum no. bytes in packed rle line */
	char *bf;	 	/* I/O buffer (malloc space) */
	int bfsize;		/* size of buffer */
	char *cp;		/* cur. ptr, into bf */
	} RLE_global;
#define Init_RLE_global {0,F,-1,-1,0,0,NULL,0,NULL,0,NULL}
RLE_global _RLE = Init_RLE_global;

RLE_put_hdr( fd, rhp, bufn )
    int fd;
    RIC_hdr *rhp;
    int bufn;	/* minimum number of lines to buffer locally */
{   char hdr[100];
    int stat;
	sprintf(hdr,"TYPE=rle\nWINDOW=%d %d %d %d\nRES=%d %d\n\n",
		/* use half-open WINDOW convention: l&t closed, r&b open*/
		rhp->bx.a.x,rhp->bx.a.y,rhp->bx.b.x+1,rhp->bx.b.y+1,
		rhp->res_x,rhp->res_y);
	if((stat=write(fd,hdr,strlen(hdr)))!=strlen(hdr)) {
		abort("rlelib: can't put hdr, stat %d",stat);
		};
	_RLE.fd = fd;
	_RLE.cy = rhp->bx.a.y-1;
	_RLE.bpl = rhp->bx.b.x-rhp->bx.a.x+9;	/* maximum bytes per line */
	_RLE.bfsize = bufn*_RLE.bpl;
	if((_RLE.bf=(char *)malloc(_RLE.bfsize))==NULL)
		abort("can't alloc _RLE.bf[%d]",_RLE.bfsize);
	_RLE.cp = _RLE.bf;
	}

#define PUT(A,B) { if((A)<128) *(B)++ = (A); \
		else { *(B)++ = HIGH(A) | 0200; *(B)++ = LOW(A); } \
		}

/* write out a non-blank line of runs */
RLE_put_Line(fd,rlp)
	int fd;		/* file descr (already open) */
	RLE_Line *rlp;
{   register char *bp;
    short blank_lines_mny;	/* the number of preceding blank lines, minus 1 */
    register RLE_Run *rp,*ep;
    Scoor xsofar;
    short bytes_mny;
    int stat;		/* I/O status */
	if((_RLE.bfsize-(_RLE.cp-_RLE.bf)) < _RLE.bpl) {
		/* not enough room left in buffer for worst-case line */
		RLE_flush();
		};
	/* leave space for (short) bytes_mny count */
	bp=_RLE.cp+sizeof(short);  
	if((blank_lines_mny=(rlp->y-_RLE.cy-1))>0) /* blank lines were skipped */ {
		PUT(0,bp);	/* ``0 runs'' signals blank lines */
		PUT(blank_lines_mny,bp);
		};
	_RLE.cy=rlp->y;
	PUT(rlp->runs,bp);
	xsofar=0;
	for(rp=rlp->r,ep=rp+rlp->runs; rp!=ep; rp++) {
		PUT(rp->xs-xsofar,bp);  xsofar = rp->xs;
		PUT(rp->xe-xsofar+1,bp);  xsofar = rp->xe+1;
		};
	/* go back & write no. bytes at top of line */
	*((short *) _RLE.cp) = bp-_RLE.cp-sizeof(short);
	_RLE.cp = bp;
	if(dbg_wr) err("RLE_put_Line y%d nh%d",_RLE.cy,bytes_mny-2);
	};

/* write out a set of runs */
RLE_put_Lines(fd,rlsp)
	int fd;		/* file descr (already open) */
	RLE_Lines *rlsp;
{   int i;
    RLE_Line *rlp;
	for(i=0,rlp=rlsp->rla; i<rlsp->mny; i++,rlp++)
		RLE_put_Line(fd,rlp);
	}

RLE_flush()
{   int bytes;
    int stat;
	if((bytes=(_RLE.cp-_RLE.bf))>0)
		if(stat=write(_RLE.fd,_RLE.bf,bytes)!=bytes)
			abort("can't write _RLE.bf[%d], status %d",bytes,stat);
	_RLE.cp = _RLE.bf;
	}

RLE_close(fd)
   int fd;
{	RLE_flush();
	if(_RLE.bf!=NULL) free(_RLE.bf);
	_RLE.bf = _RLE.cp = NULL;
	_RLE.bfsize = 0;
	}

/* ``open'' RLE_file for reading:
   save filedes, read header, and read first line into buffer */
boolean RLE_open( fd, hp )
	int fd;
	RIC_hdr *hp;
{	_RLE.fd = fd;
	/* read header */
	RLE_get_hdr( fd, hp );
	if(_RLE.bf!=NULL) free(_RLE.bf);
	_RLE.bfsize = _RLE.bpl;
	if((_RLE.bf=(char *)malloc(_RLE.bfsize))==NULL)
		abort("can't alloc _RLE.bf[%d]",_RLE.bfsize);
	/* read first non-blank line */
	_RLE.cy = hp->bx.a.y-1;
	if (!RLE_rdbf()) return(F);
	_RLE.eof = F;
	return(T);
	}

/* read next short from rle-line buffer, updating arg ptr */
short RLE_gshort(p)
	char **p;
{	register n;
	register char *q;
	q = *p;
	if((*q)&0200) {
		n = (((*q & 0177)<<8)&077400) | (*(q+1) & 0377);
		(*p)++;
		}
	else n = *q;
	(*p)++;
	return(n);
	}

/* system I/O variation on fgets(3), except it replaces \n with \0,
   and returns the number of chars read (including \0) */
int RLE_fgets(bf,max,fd)
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

/* read ascii header from rle file, convert to RIC_hdr,
   return status:  1 OK, 0 EOF, -1 error */
int RLE_get_hdr( fd, hp )
	int fd;		/* should have been open'ed earlier */
	RIC_hdr *hp;
#define HL_MAX 80
#define HTERM "=, 	\n"	/* terminations for header words: "=,<sp><tab>" */
{   char *cp,*parm,hline[HL_MAX];
    int status,nrd;
	if((status=RLE_fgets(hline,HL_MAX,fd))<=0) return(status);
	while(strlen(hline)>1) {
		if(RLE_dbg) err("hline \"%s\"",hline);
		parm=strtok(hline,HTERM);
		if(parm!=NULL&&strcmp(parm,"TYPE")==0) {
			if((parm=strtok(0,HTERM))!=NULL
				&&strcmp(parm,"rle")==0) ;
			else return(-1);
			}
		else if(parm!=NULL&&strcmp(parm,"WINDOW")==0) {
			/* assume half-open WINDOW: l&t closed, r&b open */
			if((parm=strtok(0,HTERM))!=NULL) hp->bx.a.x=atoi(parm);
			else return(-1);
			if((parm=strtok(0,HTERM))!=NULL) hp->bx.a.y=atoi(parm);
			else return(-1);
			if((parm=strtok(0,HTERM))!=NULL) hp->bx.b.x=atoi(parm)-1;
			else return(-1);
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
		if((status=RLE_fgets(hline,HL_MAX,fd))<=0) return(status);
		};
	/* maximum possible no. bytes in a packed rle line */
	_RLE.bpl = hp->bpl = hp->bx.b.x-hp->bx.a.x+9;
	if(dbg_rd) fprintf(stderr,"rlelib: RLE_get_hdr: %s\n",RIC_hdr_toa(hp));
	return(1);
	}

/* read the next run into *Rp, and return line number
   (-1 for EOF, abort on error) */
int RLE_get_Run( Rp )
	RLE_Run *Rp;
{	if(_RLE.nrun >= _RLE.runs) {	/* no more, try to read next line */
		if (!RLE_rdbf()) /* EOF */ return(-1); 
		};
	/* another run */
	Rp->xs = (_RLE.cx += RLE_gshort(&_RLE.cp));
	Rp->xe = (_RLE.cx += RLE_gshort(&_RLE.cp)) - 1;
	_RLE.nrun++;
	return(_RLE.cy);
	}

/* Return ptr to the next line with at least one run overlapping the margins
   [lm,rm]. The reported runs are not trimmed to fit exactly within margins.
   If no more lines, return NULL.  On error, abort */
RLE_Line *RLE_get_Line(lm,rm)
	int lm, rm;		/* left, right margins */
{	register int rr;	/* runs read */
	register RLE_Run *rp;	/* write ptr */

	do {	if(_RLE.eof) {
			free(_RLE.lp);  _RLE.lp = NULL;
			free(_RLE.bf);  _RLE.bf = NULL;
			return(NULL);
			};
		if(_RLE.lp==NULL) { /* allocate space for RLE_Line */
			_RLE.lp = (RLE_Line *) malloc(sizeof(RLE_Line));
			if(_RLE.lp==NULL) {
				abort("rlelib:  can't malloc RLE_Line (%d bytes)",
					sizeof(RLE_Line));
				};
			};
		_RLE.lp->y = _RLE.cy;
		/* guaranteed to be at least one run in line */
		rr=0;  rp=_RLE.lp->r;  _RLE.lp->runs=0;
		/* skip runs that are outside left margin */
		do	RLE_get_Run(rp);
		while((rr++ < _RLE.runs) && (rp->xe < lm));
		/* accept runs until cross right margin */
		while(rp->xs <= rm) {
			_RLE.lp->runs++;	/* count accepted runs */
			if(rr++ >= _RLE.runs) break;
			RLE_get_Run(++rp);
			};
		/* look ahead to next line */
		if (!RLE_rdbf()) /* EOF */ _RLE.eof=T;
		}
	while(_RLE.lp->runs==0);	/* as result of margins, some lines may empty */
	return(_RLE.lp);
	}

/* read next line buffer - return F if EOF, abort on error
   sideffects:  sets up _RLE.cp, _RLE.cy, _RLE.cx, _RLE.runs, _RLE.nrun */
boolean RLE_rdbf()
{   short nh;
	if(dbg_rd) err("enter RLE_rdbf");
	/* get number of bytes holding RLE of a scan line */
	if( read(_RLE.fd,&nh,sizeof(short)) != sizeof(short)) {
		return(F); /* normal EOF */
		};
	if( Readvax )
		nh = swapshortin( nh );
	if(dbg_rd) err("rlelib: RLE_rdbf: nh %d",nh);
	if(nh > _RLE.bpl) {
		abort("rlelib: too many bytes in line: %d > %d(_RLE.bpl)",
			nh,_RLE.bpl);
		};

	/* get those bytes */
	if( read(_RLE.fd,_RLE.bf,nh) != nh) {
		abort("rlelib: rle line truncated");
		};
	if(dbg_rd) {
		int bi;
		err("rlelib: RLE_rdbf: _RLE.bf[0-%d] octal:",nh-1);
		for(bi=0;bi<nh;bi++)
			fprintf(stderr," 0%o",0377&(_RLE.bf[bi]));
		fprintf(stderr,"\n");
		};

	/*	decode the bytes	*/
	_RLE.cp = _RLE.bf;
	_RLE.runs = RLE_gshort(&_RLE.cp);
	if(dbg_rd) err("rlelib: _RLE.runs %d",_RLE.runs);
	if(_RLE.runs==0) {	/* blank lines seen */
		_RLE.cy += RLE_gshort(&_RLE.cp);
		_RLE.runs = RLE_gshort(&_RLE.cp);
		if(dbg_rd)
			err("rlelib: _RLE.cy %d _RLE.runs %d",_RLE.cy,_RLE.runs);
		};
	if(_RLE.runs>RLE_RUNS || _RLE.runs<1) {
		abort("rlelib: illegal no. runs %d in line",_RLE.runs);
		};
	_RLE.cy++;
	if(dbg_rd) err("rlelib: RLE_rdbf: _RLE.runs %d _RLE.cy %d",_RLE.runs,_RLE.cy);
	_RLE.cx = _RLE.nrun = 0;	/* next run will be first */
	if(dbg_rd) err("rlelib: RLE_rdbf: T exit");
	return(T);
	}

/* brrl - compute a binary raster line from a run-length-encoded scanline.
   runa[] is an array of shorts, grouped in pairs (xs,xe) to describe the starting
   and ending index (inclusive) of each run of black pixels (1 bits).
   xe >= xs > xe.  The runs are in strictly ascending order on xs.
   lm & rm are the left and right margin indices of pixels, outside of which the
   pixels are forced to be white.  rm >= lm.  The `lm'th pixel is written
   into the `olm'th bit of the output raster (olm>=0).
   rasp[] is a raser line of chars; on entry, it is assumed to have been zeroed
   out.  rasp[0] holds the  leftmost byte in the line and the 001 bit of each
   byte is the leftmost bit in that byte.
   The user must ensure that rasp[] is at least (rm-lm+olm+9)/8 bytes long.
   Returns the number of black pixels in the raster line.
   NOTE:  this uses the ``little-endian'' convention, in which the leftmost pixel
   in a byte is placed in the least-significant bit (0x01).  See `brrlb()' below
   for the alternative ``big-endian'' version.
   */
int brrl(runs,runa,lm,rm,olm,rasp)
    int runs;		/* number of run-segments in runa[] */
    short runa[];	/* array of run-segments [xs,xe] (pairs of short) */
    int lm,rm;		/* left,right margin bit indices within raster */
    int olm;		/* `lm'th input pixel will be written to `olm'th bit */
    char unsigned rasp[];	/* binary raster line (zeroed out on entry) */
{   int pixels,bi;
    RLE_Run r,*rp,*re;
	pixels = 0;
	re = (rp = (RLE_Run *) runa) + runs;
	while(rp<re && rp->xe<lm) rp++; 	/* skip runs to the left of 'lm' */
	while(rp<re && rp->xs<=rm) {
		/* truncate to margins */
		if((r.xs = rp->xs)<lm) r.xs = lm;
		if((r.xe = rp->xe)>rm) r.xe = rm;
		/* shift to output indices */
		r.xs += olm-lm;   r.xe += olm-lm;
		/** SLOW **/
		for(bi=r.xs; bi<=r.xe; bi++) rasp[bi/8] |= (01)<<(bi%8);
		pixels += r.xe-r.xs+1;
		rp++;
		};
	return(pixels);
	}

/* ``big-endian'' version of brrl:  bits are packed so that the leftmost
   pixel is placed in the high-order bit (0x80) of each byte.
   */
int brrlb(runs,runa,lm,rm,olm,rasp)
    int runs;
    short runa[];
    int lm,rm,olm;
    char unsigned rasp[];
{   int pixels,bi;
    RLE_Run r,*rp,*re;
	pixels = 0;
	re = (rp = (RLE_Run *) runa) + runs;
	while(rp<re && rp->xe<lm) rp++; /* skip runs to the left of 'lm' */
	while(rp<re && rp->xs<=rm) {
		/* truncate to margins */
		if((r.xs = rp->xs)<lm) r.xs = lm;
		if((r.xe = rp->xe)>rm) r.xe = rm;
		/* shift to output indices */
		r.xs += olm-lm;   r.xe += olm-lm;
		/** SLOW **/
		for(bi=r.xs; bi<=r.xe; bi++) rasp[bi/8] |= (0200)>>(bi%8);
		pixels += r.xe-r.xs+1;
		rp++;
		};
	return(pixels);
	}

/* crrl - compute a character raster line from a run-length-encoded scanline.
   runa[] is an array of shorts, grouped in pairs (xs,xe) to describe the starting
   and ending index (inclusive) of each run of black pixels (1 bits): xs<=xe.
   The runs are in strictly ascending order on xs.  lm & rm are the left and
   right margin indices, outside of which pixels are forced to be white.  rm >= lm.
   rasp[] is a raster line, one unsigned char per pixel; rasp[0] holds the
   leftmost pixel.  The user must ensure that rasp[] is at least (rm-lm+1)
   bytes long.  Black pixels are encoded as 0, white as 255.  Returns the number
   of black pixels in the raster line.
   */
#define WHITE_pel 0377
#define BLACK_pel 0000
int crrl(runs,runa,lm,rm,rasp)
    int runs;		/* number of run-segments in runa[] */
    short runa[];	/* array of run-segments [xs,xe] (pairs of short) */
    int lm,rm;		/* left,right margin bit indices within raster */
    unsigned char rasp[];	/* char raster line */
{   int pixels,bi,wid;
    RLE_Run r,*rp,*re;
    unsigned char *cp,*ep;
	wid=rm-lm+1;
	memset(rasp,WHITE_pel,wid);
	re = (rp = (RLE_Run *) runa) + runs;
	while(rp<re&&rp->xe<lm) rp++; 	/* skip runs to the left of 'lm' */
	if(rp>=re) return(0);

	r = *rp;  r.xs -= lm;  r.xe -= lm;
	pixels = 0;
	do {	/* truncate left,right margin */
		if(r.xs<0) r.xs = 0;
		if(r.xe>(wid-1)) r.xe = wid-1;
		/* write r into raster line */
		pixels += (r.xe-r.xs+1);
		for(cp=rasp+r.xs,ep=rasp+r.xe; cp<=ep; cp++) *cp = BLACK_pel;
		/* step to next run */
		r = *(++rp);  r.xs -= lm;  r.xe -= lm;
		}
	while(r.xs<=(wid-1)&&rp<re);
	return(pixels);
	}

/* trcr - threshold & run-length-encode a char raster line.  `rasp' is a raster
   line, one unsigned char per pixel: rasp[0] is the leftmost pixel in the line.
   Pixels with value < 'thresh' are translated to black, else white.  
   Analyzes the line into a series of runs of `1's, placing them into array
   `runs'.  Observes margins:  starts at `lm' bit and continues through `rm';
   bits outside the margins are assumed 0.   A `run' is two shorts (the starting &
   ending indices of the run of `1's, inclusive).  The indices are shifted
   so that the `lm'th pixel goes to run index `olm'.  Returns the number of runs.
   NOTE:  The user must ensure that: (1) margins fall within `rasp[]';
   (2) `runp[]' is big enough to hold the worst-case no. of runs that can result
   ((rm-lm+2)/2).  The contents of the raster-line are unmodified.
   */
int trcr(rasp,lm,rm,olm,thresh,runp)
    char unsigned rasp[];	/* binary raster line (with 1 extra byte at end) */
    int lm,rm;	/* left,right margin indices within raster */
    int olm;	/* `lm' pixel goes to `olm' run index */
    int thresh;
    short runp[];	/* array of run-segments [xs,xe] (pairs of short) */
#define dbg_trcr (T)
{   unsigned char *cp,*ep;
    short x,*xp;
    int cv,pv;	/* current,prior binary pixel value */
	ep=(cp=rasp+lm)+rm;  x=olm;  xp=runp;
	pv=0;	/* assume pixels left of 'lm' are 0 */
	while (cp<=ep) {
		if((*cp)<thresh) {
			/* black pixel */
			cv=1;
			if(pv==0) { *xp = x; xp++; };
			}
		else {	/* white pixel */
			cv=0;
			if(pv==1) { *xp = x-1; xp++; };
			};
		pv=cv;  x++;
		cp++;
		};
	if(pv==1) { *xp = x-1; xp++; };	/* assume pixels right of 'rm' is 0 */
	return((xp-runp)/2);
	}

/* Transform a sequence of rle-lines using trimming, offsetting, scaling,
   truncation, and reverse-video.  Scaling may lead to interpolation or
   decimation of lines, so an input line may be mapped into 0, 1, or more
   output lines.
   Input lines may be buffered, and there may be latency.  To flush buffers,
   this must be called with NULL argument at end of the sequence.  Each output
   line will be passed to function `sink()', which returns 1 if OK and 0 to quit.     Quits are passed back immediately with no latency.  Sink's 1st arg RLE_Line
   *l is a line of runs in output coordinates, whose `len' is the maximum length
   of the line.  l==NULL means end of sequence.
   Return 1 if OK, 0 to quit.   A copy of *l is passed to sink().
   a->sy is incremented for every line written to the sink, and is attached to
   the output lines as *.y.
   */
int transform_rlel(l,a,sink,sa)
    RLE_Line *l;	/* NULL if end of sequence */
    Transform_rlel_arg *a;
    int (*sink)();	/* takes args:  (RLE_Line *), and int */
    VOID *sa;		/* passed to sink() as 2nd arg */
{   RLE_Line m,r;
    register RLE_Run *rp,*ep,*mp;
    RLE_Run mr,mm,rr;
    int rm;
    int o_a_x,o_b_x;	/* maximum output index */
	if(l==NULL) {
		sink(NULL,sa);
		return(1);
		};
	m.len = l->len;
	m.runs = l->runs;
	if(a->ident) {
		if(m.runs>0) memcpy(m.r,l->r,2*m.runs*sizeof(short));
		m.y = a->sy;
		if(sink(&m,sa)==0) return(0);
		else {	a->sy++;
			return(1);
			};
		};
	/* maximum permissable output bounds */
	o_a_x = (int)((a->tr.a.x + a->off.x)*a->scl.x);
	o_b_x = o_a_x + a->wh.x - 1;
	if(a->sy<=((int)a->dy)) {
		/* compute runs to be ouput: rp - input runs; mp - output runs*/
		mp=(m.r - 1);
		for(ep=(rp=l->r)+l->runs; rp<ep; rp++) {
			/* trim using a->tr */
			mr = *rp;
			if(mr.xe < a->tr.a.x) /* off left margin */ continue;
			if(mr.xs > a->tr.b.x) /* off right margin */ break;
			if(mr.xs < a->tr.a.x) /* extends left too far */
				mr.xs=a->tr.a.x;
			if(mr.xe > a->tr.b.x) /* extends right too far */
				mr.xe=a->tr.b.x;

			/* offset using a->off */
			mr.xs += a->off.x;
			mr.xe += a->off.x;

			/* expand using a->scl */
			mr.xs = (int)(mr.xs*a->scl.x);
			mr.xe = ((int)((mr.xe+1)*a->scl.x))-1;
			if(mr.xe<mr.xs) mr.xe=mr.xs;

			/* truncate to maximum output width */
			if(mr.xe<o_a_x) /* off left margin */ continue;
			if(mr.xs>o_b_x) /* off right margin */ break;
			if(mr.xs<o_a_x) /* extends left too far */ mr.xs=o_a_x; 
			if(mr.xe>o_b_x) /* extends right too far */ mr.xe=o_b_x;
			/* store */
			if(mp < m.r) /* first */ *(++mp) = mr;
			else /* can merge with prior run? */ {
				if(mp->xe >= (mr.xs-1)) /* yes */ mp->xe = mr.xe;
				else /* no */ *(++mp) = mr;
				};
			};
		m.runs = mp - m.r + 1;
		m.len = a->wh.x;
		do {	m.y = a->sy;
			if(a->rev) {
				/* r = reverse-video(m) */
				rp=r.r;  rr.xs=o_a_x;
				for(ep=(mp=m.r)+m.runs; mp<ep; mp++) {
					if(rr.xs<mp->xs) {
						rr.xe=mp->xs-1;
						*(rp++)=rr;
						};
					rr.xs=mp->xe+1;
					};
				if(rr.xs<=o_b_x) {
					rr.xe=o_b_x;
					*(rp++)=rr;
					};
				r.runs = rp-r.r;
				r.len = m.len;
				r.y = m.y;
				if(sink(&r,sa)==0) return(0);
				}
			else {	if(sink(&m,sa)==0) return(0); };
			a->sy++;
			}
		while(a->sy<=((int)a->dy));
		};
	a->dy += a->scl.y;
	return(1);
	}

/* Insert a (copy of) the given RLE_Line in the RLE_Lines */
insert_rlel(rlp,rlsp)
    RLE_Line *rlp;
    RLE_Lines *rlsp;
{	abort("insert_rlel:  unimplemented");
	}

err_RLE_line(s,rl)
    char *s;		/* name */
    RLE_Line *rl;
{   RLE_Run *r;
    int ri;
	if(rl==NULL) fprintf(stderr,"RLEL %10s NULL\n",s);
	else {	fprintf(stderr,"RLEL %10s y%d r%d l%d: ",
				s,rl->y,rl->runs,rl->len);
		for(ri=0,r=rl->r;ri<rl->runs;ri++,r++)
			fprintf(stderr,"[%d,%d] ",r->xs,r->xe);
		fprintf(stderr,"\n");
		};
	}

