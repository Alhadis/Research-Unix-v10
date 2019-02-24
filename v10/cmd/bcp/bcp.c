/* Copyright (c) 1989, 1990 AT&T --- All Rights Reserved.              */
/* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T.                */
/* The copyright notice does not imply actual or intended publication. */
/* AUTHORS:                                            */
/*     H. S. Baird - ATT-BL MH - first versions        */
/*     T. J. ThompsonT - ATT-BL HO - extensions        */

/* bcp.c - translate among binary image file formats, optionally trimming,
	shifting, scaling, and rotating the image on the fly.  Scaling is by
	arbitrary real factors, separately in X & Y.  Rotation is still crude and
	slow.
   USAGE:
	bcp [options...] [I [O]]
   I/O:
	If O is omitted, all output is concatenated to stdout.
	If I is also omitted, all input is read from stdin.
	If I is a filename, it is the sole input;  but, if it is a directory name,
	then every leaf of its file tree is processed in turn; and, in this case,
	if O also is specified, it is forced to become the root directory
	of an isomorphic tree of output files (this use of Unix file trees
	may be defeated using compile-time option FILE_TREE).

	[PROPOSED:  An input file may be a catenation of pages.  These may vary
	in TYPE and other parameters.  They will all be translated to the same
	output type, and catenated to one file, each with a complete translated
	header.  Generally, a complete picfile header must be repeated at the
	top of each page:  exceptions include TYPE=cdf, which packs pages in its
	own peculiar way.]

   OPTIONS:
	NOTE:  lengths, distances, and pixel indices are assumed to be in
	units of pixels (scanner units), unless one of "icpPs" is appended,
	meaning inches, cm, points, picas, or (explicitly) scanner pixels.
        This applies to option:  -w (seems buggy).

	-B[io]	read/write bitfile(9.5) format (doesn't use TYPE= header).
		[doesn't run-length encode yet:  may be bulky.]
	-M	write TYPE=bitmap format.
	-P	write Postscript format (only on Suns; by Tim Thompson, ATT-BL HO)
	-Rx[,y] force output resolutions to x,y (overrides -xX[,Y]) (pixels/inch).
		Requires a RES=X Y line in the header.
	-R=	Force the output resolution to be equal to the greater of
		the input resolutions.
	-S	write Sun rasterfile format (only on Suns; by Tim Thompson,
		ATT-BL HO)
	-Tt	when reading TYPE=dump, binarize the image by thresholding each
		pixel value as follows:  (pixel<t)? black: white. (default: -T128)
	-Ww	shrink-wrap output around black artwork, surrounding it with equal
		margins of width w (pixels), then translate so that the left-top
		corner of the margin box is at 0,0 (works only with TYPE=document-
		image input). (w must be >= 0)
	-Zx[,y]	set input resolution to RES=x y (overriding existing RES, if any)
	-b	write binary format (TYPE=binary)
	-d?	trace to stderr:
		-dR	comment on any change in resolution
		-dc	point out oddities in cdf files which are, by default,
			silently corrected
		-dh	file headers (a good first debugging step)
		-dm	margins
		-dr	runs (exhaustive, voluminous)
		-ds	status and summary statistics (brief)
	-g31	write CCITT FAX Group 3 1-dim encoding
	  -31	  ditto
	-g32	write CCITT FAX Group 3 2-dim encoding (see also -k option)
	  -32	  ditto
	-g4	write CCITT FAX Group 4 encoding (the default output format)
	  -4	  ditto
	-kK	set the 'k' for g32 encoding on output (default: -k4)
	-oX,Y	offset the result by X,Y (merely shift the bounding box)
	-p	write TYPE=dump NCHAN=1 format; input binary pixels
		are mapped to grey as follows: (pixel==white)? 255: 0.
	-r	write rle format (this is Pavlidis' obsolescent OCR format) 
	-t[lrbN] top of the page is really at the left/right/bottom;
		after all other transformations are carried out, the image
		will be rotated to bring the real top of page to the top.
		By arbitrary convention, the top-left corner of the rotated
		image will be equal to the top-left corner of the image
		resulting from all the other transformations.
		[UNFINISHED, UNDER DEVELOPMENT:  -tr & -tb; -tN, where N is a
		numeric string, is interpreted as a counterclockwise rotation by
		N degrees: for now, only handle +/-20 degrees.  The present code
		for -tN is slow and subject to aliasing.]
	-v	reverse video:  swap black and white
	-wL,T,R,B  specify window to select:
		(L,T) is the left-top (origin) and (R,B) the OPEN right-bottom
		corner (i.e. the corner coordinates are not included.)   A '%'
		argument defaults to the input window parameter.
		[NOTE: since non-zero L,T windows are still not everywhere well
		supported (by pcp, met, etc), it might be best to shift the
		new L,T to 0,0 on output -- but this is not done now.]
	-xX[,Y]	expansion factor(s), horizontally and vertically;
		if only one is specified, they are assumed equal
		(default: -x1.0,1.0)  May be overridden by -Rx[,y].
		Requires a RES=X Y line in the header.
DESCRIPTION
        This translates among these formats of binary images:
   ccitt-g31, ccitt-g32, ccitt-g4, picture (TYPE=dump NCHAN=1), binary, rle, 
   bitfile(9.5), cdf, Sun rasterfile (TYPE=bitmap), and Postscript.  All require
   a TYPE= header in picfile format (see below), except bitfile(9.5) (as a result,
   to read these, must use special option: -Bi).  Most require WINDOW=ox oy cx cy
   and RES=x y header lines also.
	Optionally, the image may be modified on the fly by the following
   operations performed in this order:
	trimming - a subset of the input image is selected (-w option)
	shifting - the origin is shifted (-o)
	scaling - real scaling factors are applied, in X & Y separately (-x, -R)
	rotation - by 90, 180, 270 degrees - UNDER DEVELOPMENT (-t option)
	reverse-video - swap black and white (-v option)
   Note that scaling changes the effective digitizing resolution by arbitrary
   real factors (expansion or contraction), in X and Y separately.  Presently,
   this is accomplished naively by replication or deletion of pixels.  If it is
   ever desired to apply more interesting image-processing techniques (such as
   smoothing and resampling), the required changes may be safely confined to
   functions transform_rlels() (in bcp.c) & transform_rlel() (in rlelib.c).
   Accepts TYPE=document-image (or TYPE=dim), flattening its hierarchy,
   and writes it out as a single page image.
PICFILE FORMAT (briefly summarized; see picture(5))
   Each picture file comprises an ASCII header followed by binary data.
   The header is terminated by two newlines (an empty line).  A typical header is:
        TYPE=ccitt-g4
        WINDOW=0 0 640 480
        RES=300 300
			(note the final empty line)
   TYPE must come first.  The other lines may come in any order.  WINDOW gives
   the x,y coordinates of the upper left corner (inclusive) and lower right
   corners (exclusive).  Typically, an image of WxH pixels will be described
   by WINDOW=0 0 W H.  RES=X Y gives the digitizing resolution horizontally and
   vertically, in integral pixels/inch.  Supported TYPEs include:

   dump     A two-dimensional array of pixel structures.  NCHAN=1 is required:
	    one byte per pixel.  On input, the grey pixel values are thresholded
	    to binary (see option -T).  On output, black is 0 and white is 255.

   bitmap   One bit/pixel, packed into bytes high-order bit first.  Black is `1'.
	    Each horizontal row is padded to a 16-bit boundary with `0'.
	    (This is identical to Sun rasterfile format, except for the ASCII
	    header.)

   ccitt-g4 CCITT FAX Group 4 compressed image.  Similarly, ccitt-g31 means
	    Group 3/1-D and ccitt-g32 means Group 3/2-D.

   (Obsolescent 112 formats:
    rle	    A run-length coding, better for binary images (see rle.h).
    binary  1 bit/pixel, packed into bytes low-order bit first.  Black is `1'.
	    Each horizontal row is padded to a full byte with `0'.)

    NOTE:  bitfile(9.5) doesn't use an ASCII header.

   (Odd formats in use outside 112, sometimes very heavily:
    cdf	    supported on input only, and only the first page of several.
    tiff    not yet supported.)

IMPLEMENTATION STATUS
        CCITT FAX ``uncompressed'' (or, ``transparent'') mode is not implemented,
   either while encoding or decoding.
   	Cdf is an input-only option;  Sun rasterfile and Postscript are output-only options.
   Postscript output is useful only for very small images.
        Rotations are only partially implemented, and are slow.

KNOWN BUGS (30 May 89)
	WINDOW=OX OY CX CY where OX or OY are non-zero is not handled correctly
   for every combination of file types.
	Unit gets passed wrongly to vto_scoor().
	Rle can't be both inout and output (design flaw: they share a buffer).
	Beware off-by-one errors (top or bottom line omitted); has not been
   carefully checked for all file type combinations.

AUTHORS
	Dec 1988 - Henry S. Baird (AT&T Bell Labs MH) - first working draft
	Mar 1989 - Tim Thompson (BL HO) - extensions for SunOS, bug fixes, etc
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "CPU.h"
#define MAIN 1
#include "stdocr.h"
#include "rle.h"
#include "pic.h"
#include "Text.h"
   /* Ugly stubs just to prevent Text.h dragging in many other *.h & *.o files */
   Bfeats *alloc_bfeats(){}; free_bfeats(){}; Bfeats *dup_bfeats(){};
   fwrb_bfeats(){}; frdb_bfeats(){}; fix_lag(){}; char *classid_toa(){};
#include "Bitmap.h"
#include "CCITT.h"
#include "bitio.h"
#if FILE_TREE
#include "Path.h"
#endif
#define CMDNAME "bcp"
#include "abort.h"

double atof(),sin(),cos();
char *strchr();

/* Runtime options values */
static struct {
	boolean from_bitfile;	/* -Bi */
	boolean to_bitfile;	/* -Bo */
	boolean to_bitmap;	/* -M */
	boolean to_g31;		/* -g31 or -31 */
	boolean to_g32;		/* -g32 or -32 */
	boolean to_g4;		/* -g4 or -4 */
	boolean to_rle;		/* -r */
	boolean to_bin;		/* -b */
	boolean to_pic;		/* -p */
	boolean to_post;	/* -P */
	boolean to_rast;	/* -S */
	boolean omit_hdr;	/* -O */
	Sp out_res;		/* -R (-1 = unspecified; -2 = force-equal) */
	Scoor shrinkwrap;	/* -W (-1 = unspecified) */
	Sp in_res;		/* -Z (SHRT_MIN,SHRT_MIN = unspecified) */
	int g32_k;		/* -k */
	int metheus;		/* -m */
	Sp offset;		/* -o */
	boolean dbg_any;
	boolean dbg_R;		/* -dR */
	boolean dbg_c;		/* -dc */
	boolean dbg_h;		/* -dh */
	boolean dbg_m;		/* -dm */
	boolean dbg_r;		/* -dr */
	boolean dbg_s;		/* -ds */
	boolean show;		/* -s */
	char top;		/* -tT takes on values: 't', 'l', 'r', 'b', 'a' */
	float top_angle;	/* -t angular value (set if top=='a') (Radians) */
	int thresh;		/* -Tt */
	boolean reverse;	/* -v */
	char *optarg_l;		/* -w?,,, */
	char *optarg_t;		/* -w,?,, */
	char *optarg_r;		/* -w,,?, */
	char *optarg_b;		/* -w,,,? */
	Bbx trim;		/* output margins resulting from -w */
	Pp expand;		/* -x */
	char *in_fn;
	char *out_fn;
} _O = {
	F, F, F, F, F, T, F, F, F, F, F,
	F, {-1,-1}, -1, {SHRT_MIN,SHRT_MIN}, 4, -1, Init_Zero_Sp,
	F, F, F, F, F, F, F, F,
	't',0.0,
	128,
	F,
	NULL, NULL, NULL, NULL,
	Init_Max_Bbx,
	{1.0,1.0},
	NULL, NULL };	

parse_args(arc,arv)
    int arc; char **arv;
{	/* getopt support */
	int optch;
	extern int optind;
	extern char *optarg;

    char *metheus_ev,metheus_digit;
    char *fs;

	/* default metheus number */
	if((metheus_ev=(char *)getenv("FB"))!=NULL) {
		metheus_digit = metheus_ev[(int)strlen(metheus_ev)-1];
		if((int)isdigit(metheus_digit)) {
			_O.metheus = (metheus_digit - '0');
			};
		};

	while((optch=getopt(arc,arv,"B:MOPR:ST:W:Z:3:4bd:g:k:m:o:prst:vw:x:"))!=EOF)
		switch(optch) {
			case 'B':
				switch(*optarg) {
				    case 'o':
					_O.to_bitfile = T;
					_O.to_g4 = F;
					break;
				    case 'i':
					_O.from_bitfile = T;
					break;
				    default:
					abort("option -B%s is illegal",optarg);
					break;
				    };
				break;
			case 'M':  _O.to_bitmap = T;  _O.to_g4 = F; break;
			case 'O':  _O.omit_hdr = T; break;
			case 'P':
#if CPU==SUN
				_O.to_post = T;  _O.to_g4 = F;
#else
				abort("-P option available only on Suns");
#endif
				break;
			case 'R':
				if(*optarg=='=') {
					_O.out_res.x = _O.out_res.y = -2;
					fs=NULL;
					}
				else if((fs=strtok(optarg,","))!=NULL) {
					if(strcmp(fs,"%")!=0) {
						_O.out_res.x = atoi(fs);
						};
					};
				if(fs!=NULL&&(fs=strtok((char *)0,","))!=NULL) {
					if(strcmp(fs,"%")!=0) {
						_O.out_res.y = atoi(fs);
						};
					}
				else _O.out_res.y = _O.out_res.x;
				break;
			case 'S':
#if CPU==SUN
				_O.to_rast = T;  _O.to_g4 = F;
#else
				abort("-S option available only on Suns");
#endif
				break;
			case 'T':  _O.thresh=atoi(optarg);  break;
			case 'W':  _O.shrinkwrap=atoi(optarg);
				if(_O.shrinkwrap<0) {
					err("-W%d must be >=0; force to -W0",
						_O.shrinkwrap);
					_O.shrinkwrap=0;
					};
				break;
			case 'Z':
				if((fs=strtok(optarg,","))!=NULL) {
					if(strcmp(fs,"%")!=0) {
						_O.in_res.x = atoi(fs);
						};
					};
				if(fs!=NULL&&(fs=strtok((char *)0,","))!=NULL) {
					if(strcmp(fs,"%")!=0) {
						_O.in_res.y = atoi(fs);
						};
					}
				else _O.in_res.y = _O.in_res.x;
				break;
			case '3':
				switch (*optarg) {
				    case '1':
					_O.to_g31=T;
					_O.to_g4=F;
					break;
				    case '2':
					_O.to_g32=T;
					_O.to_g4=F;
					break;
				    default:
				        abort("bad option: -g3%s",optarg);
					break;
				    };
				break;
			case '4':  _O.to_g4=T;  break;
			case 'b':
				_O.to_bin=T;
				_O.to_g4=F;
				break;
			case 'd':
				_O.dbg_any=T;
				switch(*optarg) {
				    case 'R':  _O.dbg_R = T;  break;
				    case 'c':  _O.dbg_c = T;  break;
				    case 'h':  _O.dbg_h = T;  break;
				    case 'm':  _O.dbg_m = T;  break;
				    case 'r':  _O.dbg_r = T;  break;
				    case 's':  _O.dbg_s = T;  break;
				    };
				break;
			case 'g':  
				switch (optarg[0]) {
				    case '3':
					switch (optarg[1]) {
					    case '1':
						_O.to_g31=T;
						_O.to_g4=F;
						break;
					    case '2':
						_O.to_g32=T;
						_O.to_g4=F;
						break;
					    default:
					        abort("bad option: -g%s",optarg);
						break;
					    };
					break;
				    case '4':  _O.to_g4=T;  break;
				    default:
					abort("bad option: -g%s",optarg);
					break;
				    };
				break;
			case 'k':  _O.g32_k = atoi(optarg); break;
			case 'm':  _O.metheus = atoi(optarg); break;
			case 'o':
				if((fs=strtok(optarg,","))!=NULL) {
					if(strcmp(fs,"%")!=0)
						_O.offset.x = atoi(fs);
					};
				if(fs!=NULL&&(fs=strtok((char *)0,","))!=NULL) {
					if(strcmp(fs,"%")!=0)
						_O.offset.y = atoi(fs);
					};
				break;
			case 'p':
				_O.to_pic=T;
				_O.to_g4=F;
				break;
			case 'r':
				_O.to_rle = T;
				_O.to_g4 = F;
				break;
			case 's':  _O.show = T;  break;
			case 't':  _O.top = *optarg;
				if(strchr("0123456789.+-",_O.top)!=NULL) {
					_O.top = 'a';	/* angular value */
					_O.top_angle = atof(optarg)*DtoR;
					};
				break;
			case 'v':  _O.reverse = T;  break;
			case 'w':
				if( (fs=strtok(optarg,","))!=NULL
					&& strlen(fs)>0 && strcmp(fs,"%")!=0 )
						_O.optarg_l = strdup(fs);
				if( fs!=NULL && (fs=strtok((char *)0,","))!=NULL
					&& strlen(fs)>0 && strcmp(fs,"%")!=0 )
						_O.optarg_t = strdup(fs);
				if( fs!=NULL && (fs=strtok((char *)0,","))!=NULL
					&& strlen(fs)>0 && strcmp(fs,"%")!=0 )
						_O.optarg_r = strdup(fs);
				if( fs!=NULL && (fs=strtok((char *)0,","))!=NULL
					&& strlen(fs)>0 && strcmp(fs,"%")!=0 )
						_O.optarg_b = strdup(fs);
				break;
			case 'x':
				if((fs=strtok(optarg,","))!=NULL) {
					if(strcmp(fs,"%")!=0) {
						if(*fs=='/')
						  _O.expand.x = 1.0/atof(fs+1);
						else _O.expand.x = atof(fs);
						if(_O.expand.x<=0.0)
						  _O.expand.x=1.0;
						};
					};
				if(fs!=NULL&&(fs=strtok((char *)0,","))!=NULL) {
					if(strcmp(fs,"%")!=0) {
						if(*fs=='/')
						  _O.expand.y = 1.0/atof(fs+1);
						else _O.expand.y = atof(fs);
						if(_O.expand.y<=0.0)
						  _O.expand.y=1.0;
						};
					}
				else _O.expand.y = _O.expand.x;
				break;
			default: break;
			};
	switch(arc-optind) {
	    case 2:
		_O.out_fn = strdup(arv[optind+1]);
	    case 1:	
		_O.in_fn = strdup(arv[optind]);
	    case 0:
		break;
	    default:
		abort("<= two filenames expected");
		break;
	    };
	if(_O.show&&_O.metheus==-1) {
		err("FB=/dev/omM not specified, using /dev/om0");
		_O.metheus = 0;
		};
	}

unimplemented()
{	abort("unimplemented");
	}

typedef struct Copy_arg {
	boolean bop;	/* beginning of page */
	PIC_hdr *ph;
	int wid;	/* bbx_wid(&(ph->bx)) (for speed) */
	BITFILE *bf;
	DST_table *tbl;	/* for CCITT input */
	int k;		/* CCITT Group 3 2-D k */
	int thresh;	/* for input pic files */
	RLE_Lines rles;	/* for TYPE=document-image or TYPE=dim */
	} Copy_arg;
#define Init_Copy_arg {T,NULL,INT_MIN,NULL,NULL,4,128,Init_RLE_Lines}
#if MAIN
Copy_arg empty_Copy_arg = Init_Copy_arg;
#else
extern Copy_arg empty_Copy_arg;
#endif

/* SOURCES:  return the next RLE_Line from the given file type, with run indices
   shifted to lie within [ a->ph->bx.a.x, a->ph->bx.b.x ].
   **** BUGS:  not all handle non-zero a->ph->bx.a.x yet.
   */

/* Handles non-zero a->ph->bx.a.x. */
RLE_Line *cdf_mrlc_source(a)
    Copy_arg *a;
#define dbg_cdf (0)
{   static RLE_Line rl;
    int n;	/* no. pixels decoded */
    int col;	/* color:  0=white, 1=black */
    int byte;	/* input byte */
    int nb;	/* no. bytes read for this line */
    int ni;	/* nibble index: 0=1st (low-order), 1=2nd(high-order) */
    int nib;	/* current nibble */
    int mult;	/* multiplier for current nibble */
    int run;	/* no. pixels in current run */
    RLE_Run *rp;
#define toggle_color() {if(col) col=0; else col=1;}
/* get next byte into `byte' */
#define getbyte()  { \
	if((byte=getc(a->ph->fp))==EOF) goto eof; nb++; \
	if(dbg_cdf) err("byte0x%x",byte); \
	}
/* get next nibble into `nib' */
#define getnib()  { \
	if(ni) { getbyte();  ni=0;  nib=byte&0x0F; } \
	else { ni=1;  nib=(byte>>4); } \
	if(dbg_cdf) err("nib0x%x byte0x%x ni%d",nib,byte,ni); \
	}
/* get next run into `run' */
#define getrun()  { \
	run=0;  mult=1; \
	do {getnib();  run += mult * (nib&0x07);  mult *= 8;} while(nib&0x08); \
	if(dbg_cdf) err("run%d %c",run,((col)? 'B': 'W')); \
	}
	a->bop = F;
	n=0; col=0; ni=1; nb=0; rp=rl.r;
	do {	getrun();
		if(col) {/* black run */
			if(run>0) { /* ... of >0 length */
				rp->xs = a->ph->bx.a.x + n+1;
				rp->xe = a->ph->bx.a.x + (n+=run);
				if((rp-rl.r)>0 && ((rp-1)->xe >= rp->xs-1) ) {
					if(_O.dbg_c) err("cdf_mrlc_source: abutting black runs [%d,%d] & [%d,%d] - merged",
						(rp-1)->xs,(rp-1)->xe,
						rp->xs,rp->xe );
					/* merge with prior black run */
					(rp-1)->xe = rp->xe;
					}
				else rp++;
				}
			else {	/* zero-length black run */
				if( _O.dbg_c && (rp-rl.r)>0 )
					err("cdf_mrlc_source: zero-length black run [%d,%d] in middle of line - ignored",
					n,n);
				};
			}
		else {	/* white run */
			n+=run;
			};
		toggle_color();
		}
	while(n<a->wid);
	/* read 2 EOL nibbles */
	getnib(); getnib();
	rl.runs = rp - rl.r;
	rl.len = a->wid;
	if(rl.runs>0 && rl.r[rl.runs-1].xe>=a->wid) {
		if(_O.dbg_c)
		    err("cdf_mrlc_source: run [%d,%d] in line of length %d - truncated",
			rl.r[rl.runs-1].xs,rl.r[rl.runs-1].xe,a->wid);
		rl.r[rl.runs-1].xe = a->wid-1;
		if(rl.r[rl.runs-1].xs>rl.r[rl.runs-1].xe) {
			rl.runs--;
			};
		};
	rl.y = a->ph->cy++;
	if(_O.dbg_r) nerr_RLE_Line("cdf_source: ",&rl);
	return(&rl);

eof:	if(dbg_cdf)
		err("cdf_mrlc_source: unexpected EOF, input line %d",a->ph->cy);
	return(NULL);
	}

int no_runs_of_page(p)
    Page *p;
{   int runs;
	runs = 0;
	runs += no_runs_of_blocks(&p->bks);
	runs += no_runs_of_txtlns(&p->ls);
	runs += no_runs_of_words(&p->ws);
	runs += no_runs_of_chars(&p->cs);
	runs += no_runs_of_blobs(&p->bs);
	return(runs);
	}

int runs_of_page(p,ra,max)
    Page *p;
    RLE_Yrun *ra;
    int max;
{   int runs;
	runs = 0;
	runs += runs_of_blocks(&p->bks,ra,max);
	runs += runs_of_txtlns(&p->ls,ra+runs,max-runs);
	runs += runs_of_words(&p->ws,ra+runs,max-runs);
	runs += runs_of_chars(&p->cs,ra+runs,max-runs);
	runs += runs_of_blobs(&p->bs,ra+runs,max-runs);
	return(runs);
	}

int no_runs_of_blocks(p)
    Blocks *p;
{   int runs;
    Block *pp,**ppp;
	runs = 0;
	if(p->mny>0) for(pp= *(ppp=p->bpa); pp!=NULL; pp= *(++ppp))
		runs += no_runs_of_block(pp);
	return(runs);
	}

int runs_of_blocks(p,ra,max)
    Blocks *p;
    RLE_Yrun *ra;
    int max;
{   int runs;
    Block *pp,**ppp;
	runs = 0;
	if(p->mny>0) for(pp= *(ppp=p->bpa); pp!=NULL; pp= *(++ppp))
		runs += runs_of_block(pp,ra+runs,max-runs);
	return(runs);
	}

int no_runs_of_block(p)
    Block *p;
{   int runs;
	runs = 0;
	runs += no_runs_of_txtlns(&p->ls);
	runs += no_runs_of_words(&p->ws);
	runs += no_runs_of_chars(&p->cs);
	runs += no_runs_of_blobs(&p->bs);
	return(runs);
	}

int runs_of_block(p,ra,max)
    Block *p;
    RLE_Yrun *ra;
    int max;
{   int runs;
	runs = 0;
	runs += runs_of_txtlns(&p->ls,ra+runs,max-runs);
	runs += runs_of_words(&p->ws,ra+runs,max-runs);
	runs += runs_of_chars(&p->cs,ra+runs,max-runs);
	runs += runs_of_blobs(&p->bs,ra+runs,max-runs);
	return(runs);
	}

int no_runs_of_txtlns(p)
    Txtlns *p;
{   int runs;
    Txtln *pp,**ppp;
	runs = 0;
	if(p->mny>0) for(pp= *(ppp=p->lpa); pp!=NULL; pp= *(++ppp))
		runs += no_runs_of_txtln(pp);
	return(runs);
	}

int runs_of_txtlns(p,ra,max)
    Txtlns *p;
    RLE_Yrun *ra;
    int max;
{   int runs;
    Txtln *pp,**ppp;
	runs = 0;
	if(p->mny>0) for(pp= *(ppp=p->lpa); pp!=NULL; pp= *(++ppp))
		runs += runs_of_txtln(pp,ra+runs,max-runs);
	return(runs);
	}

int no_runs_of_txtln(p)
    Txtln *p;
{   int runs;
	runs = 0;
	runs += no_runs_of_words(&p->ws);
	runs += no_runs_of_chars(&p->cs);
	runs += no_runs_of_blobs(&p->bs);
	return(runs);
	}

int runs_of_txtln(p,ra,max)
    Txtln *p;
    RLE_Yrun *ra;
    int max;
{   int runs;
	runs = 0;
	runs += runs_of_words(&p->ws,ra+runs,max-runs);
	runs += runs_of_chars(&p->cs,ra+runs,max-runs);
	runs += runs_of_blobs(&p->bs,ra+runs,max-runs);
	return(runs);
	}

int no_runs_of_words(p)
    Words *p;
{   int runs;
    Word *pp,**ppp;
	runs = 0;
	if(p->mny>0) for(pp= *(ppp=p->wpa); pp!=NULL; pp= *(++ppp))
		runs += no_runs_of_word(pp);
	return(runs);
	}

int runs_of_words(p,ra,max)
    Words *p;
    RLE_Yrun *ra;
    int max;
{   int runs;
    Word *pp,**ppp;
	runs = 0;
	if(p->mny>0) for(pp= *(ppp=p->wpa); pp!=NULL; pp= *(++ppp))
		runs += runs_of_word(pp,ra+runs,max-runs);
	return(runs);
	}

int no_runs_of_word(p)
    Word *p;
{   int runs;
	runs = 0;
	runs += no_runs_of_chars(&p->cs);
	runs += no_runs_of_blobs(&p->bs);
	return(runs);
	}

int runs_of_word(p,ra,max)
    Word *p;
    RLE_Yrun *ra;
    int max;
{   int runs;
	runs = 0;
	runs += runs_of_chars(&p->cs,ra+runs,max-runs);
	runs += runs_of_blobs(&p->bs,ra+runs,max-runs);
	return(runs);
	}

int no_runs_of_chars(p)
    Chars *p;
{   int runs;
    Char *pp,**ppp;
	runs = 0;
	if(p->mny>0) for(pp= *(ppp=p->cpa); pp!=NULL; pp= *(++ppp))
		runs += no_runs_of_char(pp);
	return(runs);
	}

int runs_of_chars(p,ra,max)
    Chars *p;
    RLE_Yrun *ra;
    int max;
{   int runs;
    Char *pp,**ppp;
	runs = 0;
	if(p->mny>0) for(pp= *(ppp=p->cpa); pp!=NULL; pp= *(++ppp))
		runs += runs_of_char(pp,ra+runs,max-runs);
	return(runs);
	}

int no_runs_of_blobs(p)
    Blobs *p;
{   int runs;
    Blob *pp,**ppp;
	runs = 0;
	if(p->mny>0) for(pp= *(ppp=p->bpa); pp!=NULL; pp= *(++ppp))
		runs += pp->runs;
	return(runs);
	}

int runs_of_blobs(p,ra,max)
    Blobs *p;
    RLE_Yrun *ra;
    int max;
{   int runs;
    Blob *pp,**ppp;
	runs = 0;
	if(p->mny>0) for(pp= *(ppp=p->bpa); pp!=NULL; pp= *(++ppp))
		runs += runs_of_blob(pp,ra+runs,max-runs);
	return(runs);
	}

/* ascending lexicographic order on y,xs */
int local_rn_asc(r1,r2)
   RLE_Yrun *r1,*r2;
{	if(r1->y < r2->y) return(-1);
	else if(r1->y==r2->y) {
		if(r1->xs < r2->xs) return(-1);
		else if (r1->xs == r2->xs) return(0);
		else return(1);
		}
	else return(1);
	}

RLE_Lines *rle_lines_of_page(pgp)
    Page *pgp;
{   static RLE_Lines rls;
    int no_runs,cy,ri;
    RLE_Yrun *ra,*cr;
    RLE_Run *lr;
    RLE_Line *cl;
	/* count runs */
	no_runs = no_runs_of_page(pgp);
	if(_O.dbg_r) err("no_runs %d",no_runs);
	/* allocate runs array */
	if((ra=(RLE_Yrun *)malloc(no_runs*sizeof(RLE_Yrun)))==NULL)
		abort("rle_lines_of_page: can't malloc ra[%d]",no_runs);
	runs_of_page(pgp,ra,no_runs);
	/* sort runs ascending on (y,xs) */
	qsort(ra,no_runs,sizeof(RLE_Yrun),local_rn_asc);
	/* count RLE_lines.mny */
	rls.mny = 0; cy=INT_MIN;
	for(ri=0,cr=ra; ri<no_runs; ri++,cr++)
		if(cr->y!=cy) {
			rls.mny++;
			cy = cr->y;
			};
	/* allocate RLE_lines */
	if((rls.rla=(RLE_Line *)malloc(rls.mny*sizeof(RLE_Line)))==NULL)
		abort("rle_lines_of_page: can't malloc rls.rla[%d]",rls.mny);
	/* fill in RLE_lines */
	cy=INT_MIN;  cl=rls.rla-1;
	for(ri=0,cr=ra; ri<no_runs; ri++,cr++) {
		if(cr->y!=cy) {
			cl++;
			cl->y = cy = cr->y;
			cl->runs = 0;
			lr = cl->r;
			};
		lr->xs = cr->xs;  lr->xe = cr->xe;  lr++;  cl->runs++;
		};
	free(ra);
	return(&rls);
	}

Bbx *bbx_of_rle_lines(rlsp)
    RLE_Lines *rlsp;
{   static Bbx res;
    Bbx r;
    int li,ri;
    RLE_Line *lp;
    RLE_Run *rp;
	res = empty_Bbx;
	for(li=0,lp=rlsp->rla; li<rlsp->mny; li++,lp++) {
		r.a.y = r.b.y = lp->y;
		for(ri=0,rp=lp->r; ri<lp->runs; ri++,rp++) {
			r.a.x = rp->xs;
			r.b.x = rp->xe;
			merge_bbx(&r,&res);
			};
		};
	return(&res);
	}

translate_rle_lines(r,p)
    RLE_Lines *r;
    Sp p;
{   int li,ri;
    RLE_Line *lp;
    RLE_Run *rp;
	for(li=0,lp=r->rla; li<r->mny; li++,lp++) {
		lp->y += p.y;
		for(ri=0,rp=lp->r; ri<lp->runs; ri++,rp++) {
			rp->xs += p.x;
			rp->xe += p.x;
			};
		};
	}

RLE_Line *binary_source(a)
    Copy_arg *a;
{   static RLE_Line rl;
    unsigned char *inline;
	a->bop = F;
	if(PIC_rline(a->ph,&inline)==1) {
		rl.runs =
		    rlbr(inline,0,a->ph->bx.b.x-a->ph->bx.a.x,a->ph->bx.a.x,rl.r);
		rl.y = a->ph->cy;
		rl.len = a->wid;
		if(_O.dbg_r) nerr_RLE_Line("binary_source: ",&rl);
		return(&rl);
		}
	else {	if(_O.dbg_r) nerr_RLE_Line("binary_source: ",NULL);
		return(NULL);
		};
	}

RLE_Line *bitmap_source(a)
    Copy_arg *a;
{   static RLE_Line rl;
    unsigned char *inline;
    register unsigned char *cp,*ep,*rb;
	a->bop = F;
	if(PIC_rline(a->ph,&inline)==1) {
		/* reverse bit order in each byte */
		rb=bm_consts.Rbits;
		ep=(cp=inline)+a->ph->bpl;
		while(cp<ep) { *cp = rb[*cp]; cp++; };
		rl.runs =
		    rlbr(inline,0,a->ph->bx.b.x-a->ph->bx.a.x,a->ph->bx.a.x,rl.r);
		rl.y = a->ph->cy;
		rl.len = a->wid;
		if(_O.dbg_r) nerr_RLE_Line("bitmap_source: ",&rl);
		return(&rl);
		}
	else {	if(_O.dbg_r) nerr_RLE_Line("binary_source: ",NULL);
		return(NULL);
		};
	}

RLE_Line *bitfile_source(a)
    Copy_arg *a;
#define dbg_Bi (F)
{   static RLE_Line rl;
    unsigned char *cp,*ep,*pp;
    int ch,ci;
	a->bop = F;
	/* copy current line to prior */
	memcpy(a->ph->pline,a->ph->line,a->ph->bpl);
	/* read one binary raster line */
	ep=(cp=a->ph->line)+a->ph->bpl;
	while(cp<ep) {
		if((ch=getc(a->ph->fp))!=EOF) {
			if(ch<127) {
				if(dbg_Bi) err("cb: %o",ch);
				if((fread(cp,1,2*ch,a->ph->fp))==2*ch) {
					cp += 2*ch;
					}
				else return(NULL);
				}
			else {	ch &= 0x7F;
				if(dbg_Bi) err("cb: 0200+%o",ch);
				if((fread(cp,1,2,a->ph->fp))==2) {
					cp += 2;
					for(ci=0;ci<ch-1;ci++,cp+=2) {
						*cp = *(cp-2);
						*(cp+1) = *(cp-1);
						};
					}
				else return(NULL);
				};
			}
		else return(NULL);
		};
	/* reverse bit-order, & xor with prior line */
	for(ep=(cp=a->ph->line)+a->ph->bpl,pp=a->ph->pline; cp<ep; cp++,pp++) {
		*cp = bm_consts.Rbits[*cp] ^ *pp;
		if(dbg_Bi) err("db: %o = ?? ^ %o",*cp,*pp);
		};
	rl.runs =
	    rlbr(a->ph->line,0,a->ph->bx.b.x-a->ph->bx.a.x,a->ph->bx.a.x,rl.r);
	rl.y = ++a->ph->cy;
	rl.len = a->wid;
	if(_O.dbg_r) nerr_RLE_Line("bitfile_source: ",&rl);
	return(&rl);
	}

RLE_Line *dim_source(a)
    Copy_arg *a;
{   static RLE_Line *rlp,*rlq,rl;
	if(a->bop) {
		a->ph->cy = a->ph->bx.a.y-1;
		rlq=(rlp=a->rles.rla)+a->rles.mny;
		};
	if(_O.dbg_r) {
		err("dim_source: a->bop %d a->ph->cy %d",a->bop,a->ph->cy);
		nerr_RLE_Line("*rlp",rlp);
		};
	a->bop = F;
	a->ph->cy++;
	if(rlp<rlq) {
		if(a->ph->cy < rlp->y) {
			rl.y = a->ph->cy;
			rl.runs = 0;
			rl.len = a->wid;
			if(_O.dbg_r) nerr_RLE_Line("dim_source: ",&rl);
			return(&rl);
			}
		else if(a->ph->cy == rlp->y) {
			rlp->len = a->wid;
			rlp++;
			if(_O.dbg_r) nerr_RLE_Line("dim_source: ",rlp-1);
			return(rlp-1);
			}
		else {	while(rlp<rlq && a->ph->cy>rlp->y) rlp++;
			if(rlp>=rlq) {
				if(_O.dbg_r) nerr_RLE_Line("dim_source: NULL");
				return(NULL);
				}
			else {	rlp->len = a->wid;
				rlp++;
				if(_O.dbg_r) nerr_RLE_Line("dim_source: ",rlp-1);
				return(rlp-1);
				};
			};
		}
	else {	if(_O.dbg_r) nerr_RLE_Line("dim_source: NULL");
		return(NULL);
		};
	}

RLE_Line *rle_source(a)
    Copy_arg *a;
{   static RLE_Line *rl;
	a->bop = F;
	if((rl=RLE_get_Line(a->ph->bx.a.x,a->ph->bx.b.x))!=NULL) {
		a->ph->cy = rl->y;
		rl->len = a->wid;
		if(_O.dbg_r) nerr_RLE_Line("rle_source: ",rl);
		return(rl);
		}
	else {	if(_O.dbg_r) nerr_RLE_Line("rle_source: ",NULL);
		return(NULL);
		};
	}

/* Read next line of runs from pic file, with runs shifted to lie in the range
   [ a->ph->bx.a.x, a->ph->bx.b.x ]. */
RLE_Line *pic_source(a)
    Copy_arg *a;
{   static RLE_Line rl;
    unsigned char *inline;
	a->bop = F;
	if(PIC_rline(a->ph,&inline)==1) {
		rl.runs = trcr(inline,
			0,a->ph->bx.b.x-a->ph->bx.a.x,a->ph->bx.a.x,
			a->thresh,rl.r);
		rl.y = a->ph->cy;
		rl.len = a->wid;
		if(_O.dbg_r) nerr_RLE_Line("pic_source: ",&rl);
		return(&rl);
		}
	else {	if(_O.dbg_r) nerr_RLE_Line("pic_source: ",NULL);
		return(NULL);
		};
	}

/* Read next line of runs from g31 file, with runs shifted to lie in the range
   [ a->ph->bx.a.x, a->ph->bx.b.x ]. */
RLE_Line *g31_source(a)
    Copy_arg *a;
{   static RLE_Line *rlp,rl;
    RLE_Run *rp,*ep,*np;
	if((rlp=g31_to_rlel(a->tbl,a->bf,a->bop))!=NULL) {
		a->bop = F;
		rlp->y = ++a->ph->cy;
		rlp->len = a->wid;
		if(a->ph->bx.a.x!=0) {
			/* must shift run indices */
			rl.y = rlp->y;
			rl.len = rlp->len;
			if((rl.runs=rlp->runs)>0) {
				for(ep=(rp=rlp->r)+rlp->runs,np=rl.r; rp<ep; rp++,np++ ) {
					np->xs = rp->xs + a->ph->bx.a.x;
					np->xe = rp->xe + a->ph->bx.a.x;
					};
				};
			if(_O.dbg_r) nerr_RLE_Line("g31_source: ",&rl);
			return(&rl);
			}
		else {	if(_O.dbg_r) nerr_RLE_Line("g31_source: ",rlp);
			return(rlp);
			};
		}
	else {	if(_O.dbg_r) nerr_RLE_Line("g31_source: ",NULL);
		return(NULL);
		};
	}

/* Read next line of runs from g32 file, with runs shifted to lie in the range
   [ a->ph->bx.a.x, a->ph->bx.b.x ]. */
RLE_Line *g32_source(a)
    Copy_arg *a;
{   static RLE_Line *rlp,rl;
    RLE_Run *rp,*ep,*np;
	if((rlp=g32_to_rlel(a->tbl,a->bf,a->bop))!=NULL) {
		a->bop = F;
		rlp->y = ++a->ph->cy;
		rlp->len = a->wid;
		if(a->ph->bx.a.x!=0) {
			/* must shift run indices */
			if(_O.dbg_r) err("g32_source: shift +%d",a->ph->bx.a.x);
			rl.y = rlp->y;
			rl.len = rlp->len;
			if((rl.runs=rlp->runs)>0) {
				for(ep=(rp=rlp->r)+rlp->runs,np=rl.r; rp<ep; rp++,np++ ) {
					np->xs = rp->xs + a->ph->bx.a.x;
					np->xe = rp->xe + a->ph->bx.a.x;
					};
				};
			if(_O.dbg_r) nerr_RLE_Line("g32_source: ",&rl);
			return(&rl);
			}
		else {	if(_O.dbg_r) nerr_RLE_Line("g32_source: ",rlp);
			return(rlp);
			};
		}
	else {	if(_O.dbg_r) nerr_RLE_Line("g32_source: ",NULL);
		return(NULL);
		};
	}

/* Read next line of runs from g4 file, after shifting runs to lie in the range
   [ a->ph->bx.a.x, a->ph->bx.b.x ]. */
RLE_Line *g4_source(a)
    Copy_arg *a;
{   static RLE_Line *rlp,rl;
    RLE_Run *rp,*ep,*np;
	if((rlp=g4_to_rlel(a->tbl,a->bf,a->bop,a->wid))!=NULL) {
		a->bop = F;
		rlp->y = ++a->ph->cy;
		rlp->len = a->wid;
		if(a->ph->bx.a.x!=0) { /* shift runs */
			rl.y = rlp->y;
			rl.len = rlp->len;
			if((rl.runs=rlp->runs)>0) {
				for(ep=(rp=rlp->r)+rlp->runs,np=rl.r; rp<ep; rp++,np++ ) {
					np->xs = rp->xs + a->ph->bx.a.x;
					np->xe = rp->xe + a->ph->bx.a.x;
					};
				};
			if(_O.dbg_r) nerr_RLE_Line("g4_source: ",&rl);
			return(&rl);
			}
		else {	if(_O.dbg_r) nerr_RLE_Line("g4_source: ",rlp);
			return(rlp);
			};
		}
	else {	a->bop = F;
		if(_O.dbg_r) nerr_RLE_Line("g4_source: ",NULL);
		return(NULL);
		};
	}

binary_sink(rl,a)
    RLE_Line *rl;
    Copy_arg *a;
{   int stat;
	if(_O.dbg_r) nerr_RLE_Line("binary_sink: ",rl);
	if(a->bop) { PIC_put_hdr(a->ph); a->bop=F; }
	if(rl!=NULL) {
		memset(a->ph->line,'\0',a->ph->bpl);
		if(rl->runs>0) {
		    /* convert *rl to binary, using ``little-endian'' bit-order */
		    brrl(rl->runs,rl->r,a->ph->bx.a.x,a->ph->bx.b.x,0,a->ph->line);
		    };
		if((stat=PIC_wline(a->ph,a->ph->line))!=1)
			abort("binary_sink: can't write: stat %d",stat);
		};
	return(1);
	}

bitmap_sink(rl,a)
    RLE_Line *rl;
    Copy_arg *a;
{   int stat;
	if(_O.dbg_r) nerr_RLE_Line("bitmap_sink: ",rl);
	if(a->bop) { PIC_put_hdr(a->ph); a->bop=F; }
	if(rl!=NULL) {
		memset(a->ph->line,'\0',a->ph->bpl);
		if(rl->runs>0) {
		    /* convert *rl to binary, using ``big-endian'' bit-order */
		    brrlb(rl->runs,rl->r,a->ph->bx.a.x,a->ph->bx.b.x,0,a->ph->line);
		    };
		if((stat=PIC_wline(a->ph,a->ph->line))!=1)
			abort("bitmap_sink: can't write: stat %d",stat);
		};
	return(1);
	}

/* Write bitfile(9.5) format */
bitfile_sink(rl,a)
    RLE_Line *rl;
    Copy_arg *a;
#define dbg_Bo (F)
{   int stat;
    int ci,towrite;
    register unsigned char uc,*cp,*pp,*ep;
	if(_O.dbg_r||dbg_Bo) nerr_RLE_Line("bitfile_sink: ",rl);
	if(a->bop) { PIC_put_hdr(a->ph); a->bop=F; }
	if(rl!=NULL) {
		/* copy current to prior */
		memcpy(a->ph->pline,a->ph->line,a->ph->bpl);
		memset(a->ph->line,'\0',a->ph->bpl);
		if(rl->runs>0) {
			/* convert *rl to binary, using ``big-endian'' bit-order */
			brrlb(rl->runs,rl->r,
				a->ph->bx.a.x,a->ph->bx.b.x,
				0,a->ph->line);
			if(dbg_Bo) {
			    unsigned char *cb,*eb;
				fprintf(stderr,"a->ph->line: ");
				for(eb=(cb=a->ph->line)+a->ph->bpl;cb<eb;cb++)
					fprintf(stderr,"%02X",*cb);
				fprintf(stderr,"\n");
				};
			};
		/* break up output into blocks of 2*126 bytes;
		   SHOULD also try run-length coding */
		ci=0;  while((towrite=a->ph->bpl-ci) > 0) {
			if(towrite >= 2*127) towrite=2*126;
			/* write control byte for block */
			uc = towrite/2;
			if(dbg_Bo) err("cb: %02X",uc);
			putc(uc,a->ph->fp);
			for(ep=(cp=a->ph->line+ci)+towrite,pp=a->ph->pline+ci;
			     cp<ep; cp++,pp++) {
				/* write XOR byte */
				uc = *cp ^ *pp;
				if(dbg_Bo) err("db %d: %02X = c%02X ^ p%02X",cp-a->ph->line,uc,*cp,*pp);
				putc(uc,a->ph->fp);
				};
			ci += towrite;
			};
		}
	else {	free(a->ph->line);  a->ph->line=NULL;
		free(a->ph->pline);  a->ph->pline=NULL;
		fflush(a->ph->fp);
		};
	}

rle_sink(rl,a)
    RLE_Line *rl;
    Copy_arg *a;
{	if(_O.dbg_r) nerr_RLE_Line("rle_sink: ",rl);
	if(a->bop) {
	    RIC_hdr rh;
		rh.bx = a->ph->bx;
		rh.res_x = a->ph->res_x;
		rh.res_y = a->ph->res_y;
		RLE_put_hdr(fileno(a->ph->fp),&rh,1);
		a->ph->cy = a->ph->bx.a.y-1;
		a->bop=F;
		};
	if(rl!=NULL) {
		if(rl->runs>0) RLE_put_Line(fileno(a->ph->fp),rl);
		a->ph->cy++;
		}
	else RLE_close(fileno(a->ph->fp));
	return(1);
	}

/* Write next line of runs to g31 file, after shifting runs to lie in the range
   [ 0, a->ph->bx.b.x - a->ph->bx.a.x ]. */
g31_sink(rl,a)
    RLE_Line *rl;
    Copy_arg *a;
{   int stat;
    RLE_Line crl;
    RLE_Run *rp,*ep,*np;
	if(_O.dbg_r) nerr_RLE_Line("g31_sink: ",rl);
	if(a->bop) {
		PIC_put_hdr(a->ph);
		/* treat FILE *a->ph->fp as a sequence of bits */
		if((a->bf=bopen(a->ph->fp,"w"))==NULL)
			abort("g31_sink: can't open bitfile");
		BOF_to_g31(a->bf);
		a->bop=F;
		};
	if(rl!=NULL) {
		/* check that rl->len is consistent with a->ph */
		if(rl->len != a->ph->bx.b.x - a->ph->bx.a.x + 1) {
			err("g31_sink: rl->len %d != wid(a->ph.bx) %d",
				rl->len, a->ph->bx.b.x - a->ph->bx.a.x + 1);
			rl->len = a->ph->bx.b.x - a->ph->bx.a.x + 1;
			};
		if(rl->runs>0) {
			if(a->ph->bx.a.x!=0) { /* shift runs */
				crl.y = rl->y;
				crl.len = rl->len;
				crl.runs = rl->runs;
				for(ep=(rp=rl->r)+rl->runs,np=crl.r;
				     rp<ep;
				      rp++,np++) {
					np->xs = rp->xs - a->ph->bx.a.x;
					np->xe = rp->xe - a->ph->bx.a.x;
					};
				rlel_to_g31(&crl,crl.len,a->bf);
				}
			else rlel_to_g31(rl,rl->len,a->bf);
			}
		else rlel_to_g31(NULL,rl->len,a->bf);
		a->ph->cy++;
		}
	else {	EOF_to_g31(a->bf);
		bclose(a->bf);
		};
	return(1);
	}

/* Write next line of runs to g32 file, after shifting runs to lie in the range
   [ 0, a->ph->bx.b.x - a->ph->bx.a.x ]. */
g32_sink(rl,a)
    RLE_Line *rl;
    Copy_arg *a;
{   static RLE_Line prl;
    RLE_Line *pl;
    RLE_Line crl;
    RLE_Run *rp,*ep,*np;
    	if(_O.dbg_r) nerr_RLE_Line("g32_sink: ",rl);
	if(a->bop) {
		PIC_put_hdr(a->ph);
		/* treat FILE *a->ph->fp as a sequence of bits */
		if((a->bf=bopen(a->ph->fp,"w"))==NULL)
			abort("can't open bitfile");
		BOF_to_g32(a->bf);
		a->bop=F;
		prl.runs = 0;
		};
	if(rl!=NULL) {
		/* check that rl->len is consistent with a->ph */
		if(rl->len != a->ph->bx.b.x - a->ph->bx.a.x + 1) {
			err("g32_sink: rl->len %d != wid(a->ph.bx) %d",
				rl->len, a->ph->bx.b.x - a->ph->bx.a.x + 1);
			rl->len = a->ph->bx.b.x - a->ph->bx.a.x + 1;
			};
		prl.len = rl->len;
		prl.y = rl->y - 1;
		if(((a->ph->cy+1-a->ph->bx.a.y)%a->k)==0) pl=NULL; else pl=&prl;
		if(rl->runs>0) {
			if(a->ph->bx.a.x!=0) { /* shift runs */
				crl.y = rl->y;
				crl.len = rl->len;
				crl.runs = rl->runs;
				for(ep=(rp=rl->r)+rl->runs,np=crl.r;
				     rp<ep;
				      rp++,np++) {
					np->xs = rp->xs - a->ph->bx.a.x;
					np->xe = rp->xe - a->ph->bx.a.x;
					};
				rlel_to_g32(pl,&crl,crl.len,a->bf);
				prl.y=crl.y;
				prl.len=crl.len;
				prl.runs=crl.runs;
				memcpy(prl.r,crl.r,2*prl.runs*sizeof(short));
				}
			else {	rlel_to_g32(pl,rl,rl->len,a->bf);
				prl.y=rl->y;
				prl.len=rl->len;
				prl.runs=rl->runs;
				memcpy(prl.r,rl->r,2*prl.runs*sizeof(short));
				};
			}
		else {	rlel_to_g32(pl,NULL,rl->len,a->bf);
			prl.y=rl->y;
			prl.len=rl->len;
			prl.runs=0;
			}
		a->ph->cy++;
		}
	else {	EOF_to_g32(a->bf);
		bclose(a->bf);
		prl.runs = 0;
		};
	return(1);
	}

/* Write next line of runs to g4 file, after shifting runs to lie in the range
   [ 0, a->ph->bx.b.x - a->ph->bx.a.x ]. */
g4_sink(rl,a)
    RLE_Line *rl;
    Copy_arg *a;
{   static RLE_Line prl;
    RLE_Line crl;
    RLE_Run *rp,*ep,*np;
	if(_O.dbg_r) nerr_RLE_Line("g4_sink: ",rl);
	if(a->bop) {
		PIC_put_hdr(a->ph);
		/* treat FILE *a->ph->fp as a sequence of bits */
		if((a->bf=bopen(a->ph->fp,"w"))==NULL)
			abort("can't open bitfile");
		BOF_to_g4(a->bf);
		a->bop=F;
		prl.runs = 0;
		};
	if(rl!=NULL) {
		/* check that rl->len is consistent with a->ph */
		if(rl->len != a->ph->bx.b.x - a->ph->bx.a.x + 1) {
			err("g4_sink: rl->len %d != wid(a->ph.bx) %d",
				rl->len, a->ph->bx.b.x - a->ph->bx.a.x + 1);
			rl->len = a->ph->bx.b.x - a->ph->bx.a.x + 1;
			};
		if(rl->runs>0) {
			if(a->ph->bx.a.x!=0) { /* shift runs */
				crl.y = rl->y;
				crl.len = rl->len;
				crl.runs = rl->runs;
				for(ep=(rp=rl->r)+rl->runs,np=crl.r;
				     rp<ep;
				      rp++,np++) {
					np->xs = rp->xs - a->ph->bx.a.x;
					np->xe = rp->xe - a->ph->bx.a.x;
					};
				rlel_to_g4(&prl,&crl,crl.len,a->bf);
				prl.y=crl.y;
				prl.len=crl.len;
				prl.runs=crl.runs;
				memcpy(prl.r,crl.r,2*prl.runs*sizeof(short));
				}
			else {	rlel_to_g4(&prl,rl,rl->len,a->bf);
				prl.y=rl->y;
				prl.len=rl->len;
				prl.runs=rl->runs;
				memcpy(prl.r,rl->r,2*prl.runs*sizeof(short));
				};
			}
		else {	rlel_to_g4(&prl,NULL,rl->len,a->bf);
			prl.runs=0;
			}
		a->ph->cy++;
		}
	else {	EOF_to_g4(a->bf);
		bclose(a->bf);
		prl.runs = 0;
		};
	return(1);
	}

pic_sink(rl,a)
    RLE_Line *rl;
    Copy_arg *a;
{   int stat;
	if(_O.dbg_r) nerr_RLE_Line("pic_sink: ",rl);
	if(a->bop) {
		PIC_put_hdr(a->ph);
		a->bop=F;
		};
	if(rl!=NULL) {
		crrl(rl->runs,rl->r,0,a->ph->bx.b.x-a->ph->bx.a.x,a->ph->line);
		if((stat=PIC_wline(a->ph,a->ph->line))!=1)
			abort("pic_sink: can't write: stat %d",stat);
		};
	return(1);
	}

#if CPU==SUN
post_sink(rl,a)
    RLE_Line *rl;
    Copy_arg *a;
{   int stat;
	if(_O.dbg_r) nerr_RLE_Line("post_sink: ",rl);
	if (a->bop) {
		PIC_put_hdr(a->ph);	/* needed for allocation of h->line? */
		POST_start(a->ph);
		a->bop = F;
		};
	if(rl!=NULL) {
		memset(a->ph->line,'\0',a->ph->bpl);
		if(rl->runs>0) {
		    /* convert *rl to binary, using ``little-endian'' bit-order */
		    brrl(rl->runs,rl->r,a->ph->bx.a.x,a->ph->bx.b.x,0,a->ph->line);
		    };
		if((stat=POST_wline(a->ph,a->ph->line))!=1)
			abort("post_sink: can't write: stat %d",stat);
		}
	else {	POST_end();
		};
	return(1);
	}
#else
post_sink() {abort("-P postscript output unimplemented");};
#endif

#if CPU==SUN
rast_sink(rl,a)
    RLE_Line *rl;
    Copy_arg *a;
{   int stat;
	if(_O.dbg_r) nerr_RLE_Line("rast_sink: ",rl);
	if (a->bop) {
		PIC_put_hdr(a->ph);	/* needed for allocation of h->line? */
		RAST_start(a->ph);
		a->bop = F;
		};
	if(rl!=NULL) {
		memset(a->ph->line,'\0',a->ph->bpl);
		if(rl->runs>0) {
		    /* convert *rl to binary, using ``little-endian'' bit-order */
		    brrl(rl->runs,rl->r,a->ph->bx.a.x,a->ph->bx.b.x,0,a->ph->line);
		    };
		if((stat=RAST_wline(a->ph,a->ph->line))!=1)
			abort("rast_sink: can't write: stat %d",stat);
		}
	else {	RAST_end();
		};
	return(1);
	}
#else
rast_sink() {abort("-S sunraster output unimplemented");};
#endif

/* Save RLE_Line *rl in buffer, as packed binary, using ``little-endian''
   bit-packing order, rounding each line to the nearest int for speed.
   If a->bop, malloc a complete, contiguous buffer holding the entire page,
   and initialize its contents to zeroes.
   On each call with rl!=NULL, convert the given line to binary & save in buffer.
   When called with rl==NULL but a!=NULL, signalling end-of-page, do nothing.
   Normally, return kludgey (unsigned int *)1, like other sink functions.
   When called with rl==NULL && a==NULL, return pointer to buffer.
   It is the responsibility of the calling code to free the buffer. */
unsigned int *binary_buffer(rl,a)
    RLE_Line *rl;
    Copy_arg *a;
{   static unsigned int *buf = NULL;
    static int ipl = 0;	/* ints/line */
    int bufsz;		/* (bytes) */
    unsigned int *result;
#define BITS_PER_INT (8*sizeof(int))
	if(a!=NULL && a->bop) {
		/* round up to int boundary */
		ipl = (bbx_wid(&(a->ph->bx))+BITS_PER_INT-1)/BITS_PER_INT;
		bufsz = sizeof(int)*ipl*bbx_hgt(&(a->ph->bx));
		if((buf=(unsigned int *)malloc(bufsz))==NULL)
			abort("binary_buffer: can't alloc int buf[%d]",bufsz/sizeof(int));
		memset(buf,'\0',bufsz);
		a->bop=F;
		};
	if(rl!=NULL) {
		/* convert *rl to binary, using ``little-endian'' bit-order */
		if(rl->runs>0) {
		        /* *rl --> binary, with ``little-endian'' bit-order */
#if BIG_ENDIAN
			brrlb( rl->runs, rl->r, a->ph->bx.a.x, a->ph->bx.b.x,
				0, buf+(ipl*(rl->y-a->ph->bx.a.y)) );
#else
			brrl( rl->runs, rl->r, a->ph->bx.a.x, a->ph->bx.b.x,
				0, buf+(ipl*(rl->y-a->ph->bx.a.y)) );
#endif
			};
		return((unsigned int *)1);
		}
	else {	if(a!=NULL) return((unsigned int *)1);
		else {	result = buf;
			buf = NULL;  ipl = 0;
			return(result);
			};
		};
	}

/* Rotate binary buffer *buf (whose shape is specified by a->ph->bx),
   through angle tra->rot, and write the resulting stream of RLE_Line's to
   sink() one at a time, along with a rotated version of *a as the 2nd argument.
   Only PI/2 is supported; UNDER DEVELOPMENT:  other multiples of PI/2 and
   small arbitrary angles. */
rotate_binary_buffer(buf,tra,sink,a)
    unsigned int *buf;
    Transform_rlel_arg *tra;
    int (*sink)();	/* takes args:  (RLE_Line *), and int */
    Copy_arg *a;
{   register int ipl;		/* ints/line in binary buffer */
    Copy_arg ra;		/* rotated */
    RLE_Line rl;
#define TINY_ANG (PI/1000)
	/* round up to int boundary */
	ipl = (bbx_wid(&(a->ph->bx))+BITS_PER_INT-1)/BITS_PER_INT;
	ra = *a;
	ra.bop=T;
	if((ra.ph = (PIC_hdr *)malloc(sizeof(PIC_hdr)))==NULL)
		abort("rotate_binary_buffer: can't malloc ra.ph");
	*(ra.ph) = *(a->ph);
	/* by arbitrary convention, rotated bx.a will coincide with input bx.a */
	if(tra->rot <= PI/2+TINY_ANG && tra->rot >= PI/2-TINY_ANG) {
	    int ey,ii;
	    register RLE_Run *rp;
	    register unsigned int *cb,*eb,bm,obm;
	    register short rx;
		ra.ph->bx.b.x = a->ph->bx.a.x + bbx_hgt(&(a->ph->bx)) - 1;
		ra.ph->bx.b.y = a->ph->bx.a.y + bbx_wid(&(a->ph->bx)) - 1;
		rl.len = ra.wid = bbx_wid(&(ra.ph->bx));
		rl.y = ra.ph->bx.a.y;  ey=ra.ph->bx.b.y;
		ii=0;  bm=01;
		do {	obm=0;	/* assume 0 left of margin */
			rp=rl.r-1;
			rx=ra.ph->bx.a.x;
			cb=(eb=buf+ii)+((rl.len-1)*ipl);
			do {	if((*cb & bm)!=obm) {
					if(obm) { rp->xe=rx-1;  obm=0; }
					else { (++rp)->xs=rx;  obm=bm; };
					};
				rx++;
				}
			while( (cb -= ipl) >= eb );
			if(obm) rp->xe=rx-1;
			rl.runs = rp - rl.r + 1;
			sink(&rl,&ra);
			if((bm<<=1)==0) {ii++; bm=01;};
			}
		while((++rl.y)<=ey);
		}
	else if(tra->rot <= PI+TINY_ANG && tra->rot >= PI-TINY_ANG) {
		rl.len = ra.wid;
		abort("rotate_binary_buffer: tra->rot==%g deg unimplemented",
			tra->rot/DtoR);
		}
	else if(tra->rot <= 3*PI/2+TINY_ANG && tra->rot >= 3*PI/2-TINY_ANG) {
		ra.ph->bx.b.x = a->ph->bx.a.x + bbx_hgt(&(a->ph->bx)) - 1;
		ra.ph->bx.b.y = a->ph->bx.a.y + bbx_wid(&(a->ph->bx)) - 1;
		rl.len = ra.wid = bbx_wid(&(ra.ph->bx));
		abort("rotate_binary_buffer: tra->rot==%g deg unimplemented",
			tra->rot/DtoR);
		}
	else if(tra->rot <= 20.0*DtoR && tra->rot >= -20.0*DtoR) {
	    /* rotation by ``small'' angle */
	    Sp ce;		/* center of image to be rotated */
	    double rcos,rsin;	/* real rotation vector (cos,sin) */
	    double irx,iry;	/* current rotated pixel location */
	    register int sx,sy,ex,ey,rx,ry;
	    register RLE_Run *rp;
	    register int pel,opel;
	    register int orx,ory;
		if(0) err("rotate_binary_buffer: tra->rot==%g deg under development",
			tra->rot/DtoR);
		/* conventionally, keep the same window as the input image,
		   while rotating about its midpoint */
		rl.len = ra.wid = bbx_wid(&(ra.ph->bx));
		sx = ra.ph->bx.a.x;  ex = ra.ph->bx.b.x;
		sy = ra.ph->bx.a.y;  ey = ra.ph->bx.b.y;
		ce.x = sx + bbx_wid(&(ra.ph->bx))/2;
		ce.y = sy + bbx_hgt(&(ra.ph->bx))/2;
		rcos = cos(tra->rot);  rsin = sin(tra->rot);
		for(ory=sy; ory<=ey; ory++) {
			opel=0;	/* left of output margin: assume pel==0 */
			rp=rl.r-1;
			irx = ((rcos*(sx - ce.x) - rsin*(ory - ce.y))) + ce.x;
			iry = ((rsin*(sx - ce.x) + rcos*(ory - ce.y))) + ce.y;
			for(orx=sx; orx<=ex; orx++) {
				rx = (int)(irx + 0.5);  ry = (int)(iry + 0.5);
				if(rx<sx || rx>ex || ry<sy || ry>ey)
					/* outside input image:  assume pel==0 */
					pel=0;
				else /* look at bit within input image */ {
				    int ix,iy,bx;
					iy = (ry-sy)*ipl;
					ix = rx/BITS_PER_INT;
					bx = rx%BITS_PER_INT;
#if BIG_ENDIAN
					pel = (buf[ iy + ix ] & (01<<(BITS_PER_INT-bx-1))) ? 1 : 0;
#else
					pel = (buf[ iy + ix ] & (01<<bx)) ? 1 : 0;
#endif
					};
				if(pel!=opel) {
					if(opel) { rp->xe=orx-1;  opel=0; }
					else { (++rp)->xs=orx;  opel=pel; };
					};
				/* step one pel to the right in output image */
				irx += rcos;  iry += rsin;
				};
			if(opel) rp->xe=orx-1;
			rl.runs = rp - rl.r + 1;
			rl.y = ory;
			sink(&rl,&ra);
			};
		}
	else {	abort("rotate_binary_buffer: tra->rot==%g deg unimplemented",
			tra->rot/DtoR);
		};
	sink(NULL,&ra);	/* signal end-of-page */
	}

/* Copy a source of RLE_Lines to a sink of RLE_Lines, transforming them
   by trimming, shifting, scaling, truncation, and rotation.
      The source is function ir(), which returns non-empty lines only,
   or NULL on end of page.  It is passed `bop=T' on beginning of page.
   On return from ir(), ia->ph->cy holds the index of the line, whose height is
   guaranteed to lie in the range [ia->ph->bx.a.y,ia->ph->bx.b.y].
   All input runs are guaranteed to lie in [ia->ph->bx.a.x,ia->ph->bx.b.x].
   The maximum length (in pels) of each source line is ia->wid.
      The sink function or() takes two arguments:
	RLE_Line *rl;	==NULL on end-of-page
	Copy_arg *a;
   The lines sent to the sink must be in unbroken ascending integer 
   sequence of rl->y (thus blank lines must be explicitly sent).   At all times,
   oa->ph->cy is the line last written.  After the first call to or(),
   oa->ph->cy lies in the range [oa->ph->bx.a.y,oa->ph->bx.b.y].
      Trimming, shifting, scaling, and truncation are performed either
   here or in  function transform_rlel().  Rotation is handled specially:
   the complete output image (after all other transformations have been
   performed) is saved in an intermediate buffer (as packed binary), then
   rotated.  Rotation is much slower than the other operations, mainly due
   to the necessity of touching every pixel.  (Manhattan rotations might be
   faster if the buffer held RLE_lines, but then it would be next-to-impossible
   to extend to arbitrary rotations someday.)
   */
transform_rlels(ir,ia,or,oa,tra)
    RLE_Line *(*ir)();	/* return next RLE_Line */
    Copy_arg *ia;	/* used here (bop & wid) & passed to ir() */
    int (*or)();	/* write next RLE_Line */
    Copy_arg *oa;	/* used here (bop && ph->bx.*.y) & passed to or() */
    Transform_rlel_arg *tra;
{   RLE_Line *irl,*orl;
    static RLE_Line rl0 = Init_RLE_Line;  /* empty line */
    int tr_y;		/* transformed y */
    int (*sv_or)();
	if(tra->rot!=0.0) {
		/* if must rotate the image, first buffer the entire
		   set of output RLE_Lines */
		sv_or = or;
		or = (int (*)())binary_buffer;
		};
	/* signal beginning of page; reset in source() & sink() functions */
	ia->bop=oa->bop=T;	
	/* set first output line number to write */
	tra->dy = (tra->sy = oa->ph->bx.a.y);
	while((irl=ir(ia))!=NULL) {
		if(F&&_O.dbg_r) nerr_RLE_Line("trans.i: ",irl);
		/* trim input lines' Y */
		if(irl->y < tra->tr.a.y) continue;
		if(irl->y > tra->tr.b.y) break;
		/* compute the output Y that is equal to the last
		   of the group of transformed equivalent Y's */
		tr_y = (int)((irl->y + tra->off.y + 1)*tra->scl.y) - 1;
		/* truncate if necessary */
		if(tr_y>oa->ph->bx.b.y) tr_y = oa->ph->bx.b.y;
		/* supply implicit blank line(s) of the same length */
		while(tra->sy < tr_y) {
			rl0.len = ia->wid;
			transform_rlel(&rl0,tra,or,oa);
			};
		irl->len = ia->wid;
		transform_rlel(irl,tra,or,oa);
		};
	/* supply terminating blank line(s) of the same length */
	while(tra->sy < oa->ph->bx.b.y) {
		rl0.len = ia->wid;
		transform_rlel(&rl0,tra,or,oa);
		};
	transform_rlel(NULL,tra,or,oa);   /* signal end of page */
	/* read input until end of page occurs; this is required
	   to support catenated pages */
	while(irl!=NULL) irl=ir(ia);
	if(tra->rot!=0.0) {
	    unsigned int *buf;
		buf = binary_buffer(NULL,NULL);
		if(buf!=NULL) {	
			rotate_binary_buffer(buf,tra,sv_or,oa);
			free(buf);
			};
		};
	}

process_page(i,o)
    Copy_arg *i,*o;
{   RLE_Line *(*source)();
    int (*sink)();
    char unit,*up,cpy[40];		/* units character */
    Transform_rlel_arg tra;

	i->wid = bbx_wid(&(i->ph->bx));

	_O.trim = max_Bbx;	/* start by trimming nothing */
	/* translate units in option arguments */
	if(_O.optarg_l!=NULL) {
		strcpy(cpy,_O.optarg_l);
		if((up=strpbrk(cpy,UNITS))!=NULL) {
			unit= *up;  *up='\0';
			_O.trim.a.x=vto_scoor(atof(cpy),unit,i->ph->res_x);
			}
		else /* assume scanner coordinates */
			_O.trim.a.x=atoi(cpy);
		};
	if(_O.optarg_r!=NULL) {
		strcpy(cpy,_O.optarg_r);
		if((up=strpbrk(cpy,UNITS))!=NULL) {
			unit= *up;  *up='\0';
			_O.trim.b.x=vto_scoor(atof(cpy),unit,i->ph->res_x);
			}
		else /* assume scanner coordinates */
			_O.trim.b.x=atoi(cpy);
		_O.trim.b.x--;	/* -w coordinate is open, Bbx is closed */
		};
	if(_O.optarg_t!=NULL) {
		strcpy(cpy,_O.optarg_t);
		if((up=strpbrk(cpy,UNITS))!=NULL) {
			unit= *up;  *up='\0';
			_O.trim.a.y=vto_scoor(atof(cpy),unit,i->ph->res_y);
			}
		else /* assume scanner coordinates */
			_O.trim.a.y=atoi(cpy);
		};
	if(_O.optarg_b!=NULL) {
		strcpy(cpy,_O.optarg_b);
		if((up=strpbrk(cpy,UNITS))!=NULL) {
			unit= *up;  *up='\0';
			_O.trim.b.y=vto_scoor(atof(cpy),unit,i->ph->res_y);
			}
		else /* assume scanner coordinates */
			_O.trim.b.y=atoi(cpy);
		_O.trim.b.y--;	/* -w coordinate is open, Bbx is closed */
		};
	/* Modified output margins may lie OUTSIDE the input window as well
	   as within it. */
	if(_O.trim.a.x==Scoor_MIN) _O.trim.a.x=i->ph->bx.a.x;
	if(_O.trim.b.x==Scoor_MAX) _O.trim.b.x=i->ph->bx.b.x;
	if(_O.trim.a.y==Scoor_MIN) _O.trim.a.y=i->ph->bx.a.y;
	if(_O.trim.b.y==Scoor_MAX) _O.trim.b.y=i->ph->bx.b.y;
	if(_O.dbg_m) err("trim: %s",bbx_toa(&(_O.trim)));

	if(strcmp(i->ph->type,"binary")==0) { source = binary_source; }
	else if(strcmp(i->ph->type,"bitmap")==0) { source = bitmap_source; }
	else if(strcmp(i->ph->type,"bitfile")==0) { source = bitfile_source; }
	else if(strcmp(i->ph->type,"document-image")==0) { source = dim_source; }
	else if(strcmp(i->ph->type,"dim")==0) { source = dim_source; }
	else if(strcmp(i->ph->type,"rle")==0) {
	    RIC_hdr rh;
		/* reopen RLE file to use system I/O (awkward, obsolescent) */
		lseek(fileno(i->ph->fp),0L,0);
		RLE_open(fileno(i->ph->fp),&rh);
		i->ph->cy = i->ph->bx.a.y-1;
		source = rle_source;
		}
	else if(strcmp(i->ph->type,"pico")==0
		||strcmp(i->ph->type,"dump")==0) {
		i->thresh = _O.thresh;
		source = pic_source;
		}
	else if(strcmp(i->ph->type,"ccitt-g31")==0) {
		if((i->bf=bopen(i->ph->fp,"r"))==NULL)
			abort("can't open bitfile");
		i->tbl = ccitt_table();
		source = g31_source;
		}
	else if(strcmp(i->ph->type,"ccitt-g32")==0) {
		if((i->bf=bopen(i->ph->fp,"r"))==NULL)
			abort("can't open bitfile");
		i->tbl = ccitt_table();
		source = g32_source;
		}
	else if(strcmp(i->ph->type,"ccitt-g4")==0) {
		if((i->bf=bopen(i->ph->fp,"r"))==NULL)
			abort("can't open bitfile");
		i->tbl = ccitt_table();
		source = g4_source;
		}
	else if(strcmp(i->ph->type,"cdf-mrlc")==0) { source = cdf_mrlc_source; }
	else abort("Input file TYPE=%s unsupported",i->ph->type);

	if(_O.dbg_h) err("In %s",PIC_hdr_toa(i->ph));

	/* Adjudicate -R and -x options */
	if(_O.out_res.x==-2) {
		/* -R= specified:  force resolutions to be equal to the greater */
		if(i->ph->res_x < i->ph->res_y) {
			_O.out_res.x = _O.out_res.y = i->ph->res_y;
			}
		else if(i->ph->res_y < i->ph->res_x) {
			_O.out_res.x = _O.out_res.y = i->ph->res_x;
			}
		else _O.out_res.x = _O.out_res.y = i->ph->res_x;
		};
	if(_O.out_res.x!=-1) {	/* -Rx specified; override -xX */
		_O.expand.x = ((double)_O.out_res.x)/i->ph->res_x;
		if(_O.dbg_R && _O.expand.x!=1.0)
			err("horizontal resolution changed: x%g (%d -> %d)",
				_O.expand.x,i->ph->res_x,_O.out_res.x);
		};
	if(_O.out_res.y!=-1) {	/* -R,y specified; override -x,Y */
		_O.expand.y = ((double)_O.out_res.y)/i->ph->res_y;
		if(_O.dbg_R && _O.expand.y!=1.0)
			err("vertical resolution changed: x%g (%d -> %d)",
				_O.expand.y,i->ph->res_y,_O.out_res.y);
		};

	/* Setup image transformation parameters */
	tra = empty_Transform_rlel_arg;
	tra.tr = _O.trim;
	tra.off = _O.offset;
	tra.scl = _O.expand;
	tra.wh.x = bbx_wid(&(tra.tr));
	tra.wh.y = bbx_hgt(&(tra.tr));
	tra.rev = _O.reverse;
	if(tra.scl.x!=1.0) {
		tra.wh.x = (int)(tra.wh.x*tra.scl.x+0.5);
		};
	if(tra.scl.y!=1.0) {
		tra.wh.y = (int)(tra.wh.y*tra.scl.y+0.5);
		};
	switch(_O.top) {
	    case 't':  /* default */ break;
	    case 'l':  /* top is really at left */
		tra.rot = (Radians)(PI/2);
		break;
	    case 'b':  /* top is really at bottom */
		tra.rot = (Radians)(PI);
		break;
	    case 'r':  /* top is really at right */
		tra.rot = (Radians)((3*PI)/2);
		break;
	    case 'a':  /* rotation angle given */
		tra.rot = _O.top_angle;
		break;
	    };
	/* is the transform the identity? (speed-optimized special case) */
	tra.ident = ( (bbx_eq(&(tra.tr),&(i->ph->bx)))
			&& (tra.scl.x==1.0)
			&& (tra.scl.y==1.0)
			&& (tra.off.x==0)
			&& (tra.off.y==0)
			&& (tra.rot==0.0) );

	/* compute transformed output box & resolution;
	   don't rotate here (see transform_rlels()) */
	if(tra.ident) {
		o->ph->bx = i->ph->bx;
		o->ph->res_x = i->ph->res_x;
		o->ph->res_y = i->ph->res_y;
		}
	else {	o->ph->bx = tra.tr;
		o->ph->bx.a.x += tra.off.x;
		o->ph->bx.b.x += tra.off.x;
		o->ph->bx.a.y += tra.off.y;
		o->ph->bx.b.y += tra.off.y;
		if(tra.scl.x!=1.0) {
			o->ph->bx.a.x = tra.scl.x*o->ph->bx.a.x;
			o->ph->bx.b.x = o->ph->bx.a.x + tra.wh.x-1;
			};
		if(tra.scl.y!=1.0) {
			o->ph->bx.a.y = tra.scl.y*o->ph->bx.a.y;
			o->ph->bx.b.y = o->ph->bx.a.y + tra.wh.y-1;
			};
		o->ph->res_x = (i->ph->res_x*tra.scl.x + 0.5);
		o->ph->res_y = (i->ph->res_y*tra.scl.y + 0.5);
		};

	if(_O.to_bin)
		{ strcpy(o->ph->type,"binary"); sink = binary_sink; }
	else if(_O.to_bitfile)
		{ strcpy(o->ph->type,"bitfile"); sink = bitfile_sink; }
	else if(_O.to_bitmap)
		{ strcpy(o->ph->type,"bitmap"); sink = bitmap_sink; }
	else if (_O.to_rle) {
		if(strcmp(i->ph->type,"rle")==0)
			abort("can't both read and write TYPE=rle - sorry");
		strcpy(o->ph->type,"rle"); sink = rle_sink;
		}
	else if(_O.to_g31) { strcpy(o->ph->type,"ccitt-g31"); sink = g31_sink; }
	else if(_O.to_g32) {
		strcpy(o->ph->type,"ccitt-g32");
		o->k = _O.g32_k;
		sink = g32_sink;
		}
	else if(_O.to_g4) { strcpy(o->ph->type,"ccitt-g4");  sink = g4_sink; }
	else if(_O.to_pic) { strcpy(o->ph->type,"dump");  sink = pic_sink; }
#if CPU==SUN
	else if(_O.to_post) { strcpy(o->ph->type,"postscript"); sink = post_sink; }
#endif
#if CPU==SUN
	else if(_O.to_rast) { strcpy(o->ph->type,"sunraster"); sink = rast_sink; }
#endif
	if(i->ph->misc!=NULL) {
		o->ph->misc = i->ph->misc;
		i->ph->misc = NULL;
		};
	if(_O.dbg_h) err("Out %s",PIC_hdr_toa(o->ph));

	transform_rlels(source,i,sink,o,&tra);

	/* Synchronize filedes pointer with stream pointer */
	lseek(fileno(o->ph->fp),o->ph->seek = ftell(o->ph->fp),0);
	}

/* A file may be a catenation of pages. */
process_fps(i,o)
    FILE *i,*o;
{   Copy_arg ia,oa;
    int stat;
	ia = oa = empty_Copy_arg;
	ia.ph = alloc_PIC_hdr(i);
	oa.ph = alloc_PIC_hdr(o);

	stat=PIC_get_hdr(ia.ph);
	if(_O.dbg_h && stat!=1) err("PIC_get_hdr() returns status %d",stat);

	/* enforce bitfile(9) I/O restrictions */
	if(_O.from_bitfile && strcmp(ia.ph->type,"bitfile")!=0)
		abort("-Bi requires input to be bitfile(9.5) format");
	if(!_O.from_bitfile && strcmp(ia.ph->type,"bitfile")==0) {
		/* TYPE= is missing, erroneously:  refuse to recognize it */
		abort("TYPE=... header is missing");
		};

	if(strcmp(ia.ph->type,"document-image")==0
	   || strcmp(ia.ph->type,"dim")==0) {
	    Page pg;
		skip_doc(i);
		if(frdb_page_etc(i,&pg,IsALL)==1) {
			ia.ph->bx = pg.bx;
			ia.ph->res_x = pg.res_x;
			ia.ph->res_y = pg.res_y;
			ia.rles = *rle_lines_of_page(&pg);
			if(_O.shrinkwrap!=-1) {
			    Sp trans;
				ia.ph->bx = *bbx_of_rle_lines(&ia.rles);
				ia.ph->bx.a.x -= _O.shrinkwrap;
				ia.ph->bx.a.y -= _O.shrinkwrap;
				ia.ph->bx.b.x += _O.shrinkwrap;
				ia.ph->bx.b.y += _O.shrinkwrap;
				if(bbx_area(&(ia.ph->bx))<=0)
				    abort("-W%d shrinks to nothing",_O.shrinkwrap);
				trans.x = -ia.ph->bx.a.x; trans.y = -ia.ph->bx.a.y;
				translate_rle_lines(&ia.rles,trans);
				ia.ph->bx.b.x = bbx_wid(&ia.ph->bx)-1;
				ia.ph->bx.b.y = bbx_hgt(&ia.ph->bx)-1;
				ia.ph->bx.a.x = ia.ph->bx.a.y = 0;
				};
			free_page_etc(&pg,IsALL&(~IsPage));
			}
		else {	free_page_etc(&pg,IsALL&(~IsPage));
			abort("can't read document-image file");
			};
		};
	if(_O.in_res.x!=SHRT_MIN&&_O.in_res.y!=SHRT_MIN) {
		/* force input RES to given values */
		if(ia.ph->res_x!=0 || ia.ph->res_y!=0) {
			err("input: RES=%d %d overridden by -Z%d,%d\n",
				ia.ph->res_x,ia.ph->res_y,_O.in_res.x,_O.in_res.y);
			};
		ia.ph->res_x = _O.in_res.x;
		ia.ph->res_y = _O.in_res.y;
		};
	if(ia.ph->res_x<=0 || ia.ph->res_y<=0)
		err("input: RES=%d %d may cause problems (set using -Zx,y)",
			ia.ph->res_x,ia.ph->res_y);

	while(strlen(ia.ph->type)>0 && stat==1 && !feof(i)
		&& !ferror(i) && !ferror(o)) {
		process_page(&ia,&oa);
		if( strcmp(ia.ph->type,"cdf")==0
		    || strcmp(ia.ph->type,"cdf-mrlc")==0 ) {
			stat=CDF_next_page(ia.ph);
			}
		else {	if((stat=PIC_get_hdr(ia.ph))==1) {
				/* enforce bitfile(9) I/O restrictions */
				if(_O.from_bitfile && strcmp(ia.ph->type,"bitfile")!=0)
					abort("-Bi means input must be picfile");
				if(!_O.from_bitfile && strcmp(ia.ph->type,"bitfile")==0) {
					/* TYPE= is missing, erroneously:  silently quit */
					if(_O.dbg_h) err("strange input before normal EOF - ignored");
					strcpy(ia.ph->type,"");
					};
				};
			};
		if(_O.dbg_h && stat!=1)
			err("PIC_get_hdr() returns status %d",stat);
		};

	/* EOF or error */
	free_PIC_hdr(ia.ph);
	free_PIC_hdr(oa.ph);
	}

#if FILE_TREE
/* Process filenames:  if NULL, use stdin/stdout;  if files, read them;
   if directories, create a parallel file tree and process the leaves
   pairwise. */
process_fns(i,o)
    char *i;		/* may be NULL; not yet fopened for read */
    char *o;		/* may be NULL; not yet fopened for write */
#define dbg_ft (F)
{   FILE *i_fp;
    FILE *o_fp;
    char *branch;
	if(dbg_ft) err("processs(\"%s\",\"%s\")",i,o);
	branch=path_toa(&path_process.path,1);
	if(dbg_ft&&branch[0]!='\0') err("%s",branch);
	if(i==NULL||i[0]=='\0') i_fp=stdin;
	else if( (i_fp=fopen(i,"r"))==NULL ) {
		err("can't open input file %s - skip it",i);
		return;
		};
	if(o==NULL||o[0]=='\0') o_fp=stdout;
	else if( (o_fp=fopen(o,"w"))==NULL ) {
		err("can't open output file %s - skip it",o);
		return;
		};
	process_fps(i_fp,o_fp);
	if(i_fp!=stdin) fclose(i_fp);
	if(o_fp!=stdout) fclose(o_fp); else fflush(o_fp);
	};
#else
/* Process filenames:  if NULL, use stdin/stdout;  if files, open and process them.
   */
process_fns(i_fn,o_fn)
    char *i_fn;		/* may be NULL; not yet fopened for read */
    char *o_fn;		/* may be NULL; not yet fopened for write */
{   FILE *i_fp;
    FILE *o_fp;
	if(i_fn==NULL||strlen(i_fn)==0) i_fp = stdin;
	else {	if((i_fp=fopen(i_fn,"r"))==NULL)
			abort("can't open input file %s",i_fn);
		};
	if(o_fn==NULL||strlen(o_fn)==0) o_fp = stdout;
	else {	if((o_fp=fopen(o_fn,"w"))==NULL)
			abort("can't open output file %s",o_fn);
		};
	process_fps(i_fp,o_fp);
	if(i_fp!=stdin) fclose(i_fp);
	if(o_fp!=stdout) fclose(o_fp); else fflush(o_fp);
	};
#endif

main(arc,arv)
    int arc; char **arv;
{	parse_args(arc,arv);
#if FILE_TREE
	process_file_trees(process_fns,_O.in_fn,_O.out_fn);
#else
	process_fns(_O.in_fn,_O.out_fn);
#endif
	}

nerr_RLE_Line(s,rl)
    char *s;
    RLE_Line *rl;
{   RLE_Run *r;
    int ri;
	if(rl==NULL) fprintf(stderr,"%15s RLEL NULL.\n",s);
	else {	fprintf(stderr,"%15s RLEL y%d r%d l%d: ",s,rl->y,rl->runs,rl->len);
		for(ri=0,r=rl->r; ri<rl->runs; ri++,r++)
			fprintf(stderr,"[%d,%d] ",r->xs,r->xe);
		fprintf(stderr,"\n");
		};
	}
