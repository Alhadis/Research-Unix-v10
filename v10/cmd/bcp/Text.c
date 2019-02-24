/* Copyright (c) 1989, 1990 AT&T --- All Rights Reserved.              */
/* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T.                */
/* The copyright notice does not imply actual or intended publication. */
/* AUTHORS:                                            */
/*     H. S. Baird - ATT-BL MH - first versions        */

/* Text.c - functions for Document-image file ("dim" file) handling

   General Introduction
   --------------------

   Dim files describe a document as a collection of records of these types:
	Page	full page
	Block	block (often a column) of text
	Txtln	line of text
	Word	word
	Char	character, isolated symbol, `graph'
	Interp	interpretation of a character (result of classification)
	Sfeats	scalar features of a character
	Shapes	local shape features of a character
	Bfeats	binary indicator features of a character
	Blob	connected component (maximal subset of 8-connected black pixels)
	Lag 	line-adjacency graph of runs
	Run	horizontal run of black pixels
        Pixel   bilevel (black/white) picture element (symbols are expected to
		be black against a white background); usually square

   They may be organized hierarchically --
	a Page may own :  Blocks, Txtlns, Words, Chars, & Blobs
	  a Block may own :  Blocks, Txtlns, Words, Chars, & Blobs
	    a Txtln may own :  Txtlns, Words, Chars, & Blobs
	      a Word may own :  Words, Chars & Blobs
	        a Char may own :  Blobs, Interps, Sfeats, Shapes, & Bfeats
	          a Blob may own :  Runs (variously represented)
		    a Run owns :  black Pixels
    Note that the hierarchy is strict except that Blocks, Txtlns, & Words may
    own records of their own type.  This serves several functions:
    --  a Block may own other Blocks, conventionally nested within it, to
	represent a physical or logical page layout decomposition;
    --  a Txtln may have alternative interpretations & segmentations into Words;
    --  a Word may have alternative segmentations into Chars.

    For each record type R, there should exist these data-structures:
	R			struct
	Init_R			#define'd initialization string
	empty_R			an extern ``empty'' (initialized) instance of R
    Also there are library functions (in which _R is spelled in lower-case):
	R *alloc_R()		allocate memory and initialize to ``empty''
	free_R(R *)		free memory (only of record R, not what it owns)
	free_R_etc(R *,ids)	free R and what it owns (as specified by `ids')
	R *dup_R(R *)		return distinct copy, with duplicated contents
	R *dup_R_etc(R *,ids)	return distinct copy, duplicated contents & parts
	char *R_toa(R *)	convert to printable ASCII string
	frdb_R(FILE *, R *)	fread R (binary) from file thru pointer
	frdb_R_etc(",",ids)	fread R (binary) and what it owns (as specified)
	fwrb_R(FILE *, R *)	fwrite R (binary) to file thru pointer
	fwrb_R_etc(",",ids)	fwrite R (binary) and what it owns (as specified)
    (some of these may be unimplemented if they haven't yet been needed)

    I/O conventions:
    --	frd?_... return one of:
	    1  normal & successful
	    0  EOF
	   <0  I/O errors
	Quite a few functions don't obey this rule yet.
	Many intermediate fns that don't directly perform I/O
	don't bother passing back status from fns they call.
	Those that perform I/O directly often complain to stderr and exit(2).
	The situation is fairly chaotic, and has already caused bugs.

    Generally:
    --	An `etc' argument is a set of Ident bits specifying a set of record types;
	it is passed along unchanged in calls to other _etc functions.
    --  `fwrb_R_etc' fns obey the `etc' instructions carefully,  writing only those
	record types specified
    --  `frdb_R_etc' fns mostly ignore `etc', blindly reading everything they
	see (since they aren't clever enough to skip past yet);  however, the
	Blob-reading fns look at the Runs_?? bits to select a main memory
	format for Runs
    --	`frdb_R_etc' uses alloc_Y to allocate all new Y
    --	`fwrb_R_etc', however, does NOT free anything: this must be done explicitly
	afterwards.

    Ownership of a set of records is implemented by two fields in the owner record:
    (1) a count of the number of members, and (2) either a ``set'' or a ``list''
    pointer:
	set:  to a NULL-terminated array of pointers to records
	list: to the first in a singly-linked, NULL-terminated, chain
    sets are used for sets of Blocks, Txtlns, Words, Chars, and Blobs
    lists are used for Interpl, and for Blobs owned by Chars, since they tend
	to be fewer

    Blobs and Runs are treated more elaborately than other records.  Blobs are
    collected in lists when owned by Chars, but in sets otherwise --
    the motivation was that the Char lists are usually very short, and the malloc
    overhead of creating them might be unpleasant -- however, maintaining two
    kinds of sets has caused other headaches; it might be good someday to
    abolish lists in favor of sets throughout.
    Runs can often be compressed to about half the usual size by using char
    fields instead of shorts, and this is done automatically when writing to files.    There is both a set and a list form of Run.

    The peripheral file format is:
    - deliberately decoupled from the internal (main memory) format:  i.e.
	the main memory structs can be (and are) changed frequently for
	purposes of experimentation without forcing frequent reformatting
	of the (by now large) backlog of archived files (particularly the
	character image databases).  To bring such files up-to-date, the program
	`renew' should be used: it must be edited to reflect changes since
	the last overhaul of the file format.
    - designed to be ``scannable'': that is, one can skip rapidly from record
	to record (using the Ident headers) without minding the hierarchy.
	This is most useful for the graphics editor "met", where it permits
	quick response at the outset.  It is also used widely in the
	off-line training programs, where main memory is at a premium.
    - machine-independent:  floating-point representations are forced to
	fixed point and scaled to integer; all integer representations are
	written as a sequence of bytes in a fixed order, via putc()/getc().
	fwrite()/fread() are never used for binary I/O.  This seems to work
	on virtually all UNIX machines.  See fioi.h.

    CCITT Group 4 encodings have been implemented and are available
    as a peripheral file format for Blobs.  They offer a large compression
    factor (x8) over RunF and RunFS records, and the CPU overhead is not
    excessive.  It turns out that the connectivity information represented
    explicitly in Run records but lost in ccitt-g4 can be recovered in linear
    time and space (see fix_lag()); in practice the extra recovery time is
    negligible and the extra space is 0.  This is possible because Blobs are
    known to be connected:  if not, then (I suspect that) superlinear time is
    required in general to recover the lag.

  */

#include <stdio.h>
#include <math.h>
#define LIBC_INCL 1
#include "CPU.h"
#include "stdocr.h"
#include "rle.h"
#include "Text.h"
#include "bitio.h"
#include "CCITT.h"

	long fseek(),lseek();

#define dbg_fwrb T	/* failsafe consistency checking */
#define dbg_frdb T	/* failsafe consistency checking */
#define dbg_fwrb_runs F	/* announce no. bytes used to write each Blob's Runs */
#define dbg_frdb_runs F	/* announce no. bytes used to write each Blob's Runs */

/* return ASCII string describing ident bits */
char *ident_toa(id)
	Ident id;
{	static char s[80];
	s[0]='\0';
    if((id&IsALL)==IsALL) strcat(s,"ALL");
    else {
	if(id&IsPage) {
		strcat(s,"PG");
		if(id&(Page_label)) {
			strcat(s,".");
			if(id&Page_label) strcat(s,"l");
			};
		}
	if(id&IsBlock) {
		strcat(s,"BK");
		if(id&(Block_wst|Block_label)) {
			strcat(s,".");
			if(id&Block_wst) strcat(s,"w");
			if(id&Block_label) strcat(s,"l");
			};
		};
	if(id&IsTxtln) {
		strcat(s,"TL");
		if(id&(Txtln_basl|Txtln_size|Txtln_label)) {
			strcat(s,".");
			if(id&Txtln_basl) strcat(s,"b");
			if(id&Txtln_size) strcat(s,"s");
			if(id&Txtln_label) strcat(s,"l");
			};
		};
	if(id&IsWord) {
		strcat(s,"WD");
		if(id&(Word_spelled|Word_label)) {
			strcat(s,".");
			if(id&Word_spelled) strcat(s,"s");
			if(id&Word_label) strcat(s,"l");
			};
		};
	if(id&IsWordInterp) {
		strcat(s,"WI");
		if(id&(Word_spelled|Word_numeric|Word_initcap|Word_allcaps|Word_hyphens|Word_slashes|Word_termhyp|Word_endsent)) {
			strcat(s,".");
			if(id&Word_spelled) strcat(s,"s");
			if(id&Word_numeric) strcat(s,"n");
			if(id&Word_initcap) strcat(s,"i");
			if(id&Word_allcaps) strcat(s,"a");
			if(id&Word_hyphens) strcat(s,"-");
			if(id&Word_slashes) strcat(s,"/");
			if(id&Word_termhyp) strcat(s,"h");
			if(id&Word_endsent) strcat(s,".");
			};
		};
	if(id&IsChar) {
		strcat(s,"CH");
		if(id&(Char_spelled|Char_confused|Char_termhyp|Char_omit|Char_label|Char_ranparms)) {
			strcat(s,".");
			if(id&Char_spelled) strcat(s,"s");
			if(id&Char_confused) strcat(s,"c");
			if(id&Char_termhyp) strcat(s,"h");
			if(id&Char_omit) strcat(s,"o");
			if(id&Char_label) strcat(s,"l");
			if(id&Char_ranparms) strcat(s,"r");
			if(id&Char_split) strcat(s,"S");
			if(id&Char_merged) strcat(s,"M");
			};
		};
	if(id&IsInterp) {
		strcat(s,"IN");
		if(id&Interp_spelled) {
			strcat(s,".s");
			};
		};
	if(id&IsBlob) {
		strcat(s,"BB");
		if( id&(Blob_lm|Blob_rm|Blob_tm|Blob_bm
			 |Blob_chopt|Blob_chopb|Blob_chopl|Blob_chopr
			 |Blob_small|Blob_local)
		         ) {
			strcat(s,".");
			if(id&(Blob_tm|Blob_bm|Blob_lm|Blob_rm)) {
				if(id&Blob_lm) strcat(s,"l");
				if(id&Blob_rm) strcat(s,"r");
				if(id&Blob_tm) strcat(s,"t");
				if(id&Blob_bm) strcat(s,"b");
				};
			if(id&(Blob_chopt|Blob_chopb|Blob_chopl|Blob_chopr)) {
				if(id&Blob_chopl) strcat(s,"L");
				if(id&Blob_chopr) strcat(s,"R");
				if(id&Blob_chopt) strcat(s,"T");
				if(id&Blob_chopb) strcat(s,"B");
				};
			if(id&Blob_small) strcat(s,"s");
			if(id&Blob_local) strcat(s,"o");
			};
		}
	    };
	return(s);
	}

/* convert a conventional character-code to a record type */
Ident cto_ident(c)
    char c;
{   Ident type;
	switch(c) {
	    case 'b':
		type=IsBlob;  break;
	    case 'B': case 'k':
		type=IsBlock;  break;
	    case 'c':
		type=IsChar;  break;
	    case 'i':
		type=IsInterp;  break;
	    case 'p':  case 'P':
		type=IsPage;  break;
	    case 'r':
		type=IsRun;  break;
	    case 't': case 'l':
		type=IsTxtln;  break;
	    case 'w':
		type=IsWord;  break;
	    case 'y':
		type=IsBdy;  break;
	    };
	return(type);
	}

Ident cto_flag(c,type)
    char c;
    Ident type;
{   Ident flag;
	flag = IsNONE;
	if(type&IsPage) {
		switch(c) {
			case 'l': flag |= Page_label; break;
			};
		}
	else if(type&IsBlock) {
		switch(c) {
			case 'w': flag |= Block_wst; break;
			case 'l': flag |= Block_label; break;
			};
		}
	else if(type&IsTxtln) {
		switch(c) {
			case 'b': flag |= Txtln_basl; break;
			case 's': flag |= Txtln_size; break;
			case 'l': flag |= Txtln_label; break;
			};
		}
	else if(type&IsWord) {
		switch(c) {
			case 's': flag |= Word_spelled; break;
			case 'n': flag |= Word_numeric; break;
			case 'i': flag |= Word_initcap; break;
			case 'a': flag |= Word_allcaps; break;
			case '-': flag |= Word_hyphens; break;
			case '/': flag |= Word_slashes; break;
			case 'h': flag |= Word_termhyp; break;
			case '.': flag |= Word_endsent; break;
			case 'l': flag |= Word_label; break;
			};
		}
	else if(type&IsChar) {
		switch(c) {
			case 's': flag |= Char_spelled; break;
			case 'c': flag |= Char_confused; break;
			case 'h': flag |= Char_termhyp; break;
			case 'o': flag |= Char_omit; break;
			case 'l': flag |= Char_label; break;
			case 'r': flag |= Char_ranparms; break;
			case 'S': flag |= Char_split; break;
			case 'M': flag |= Char_merged; break;
			};
		}
	else if(type&IsBlob) {
		switch(c) {
			case 'B': flag |= Blob_chopb; break;
			case 'L': flag |= Blob_chopl; break;
			case 'R': flag |= Blob_chopr; break;
			case 'T': flag |= Blob_chopt; break;
			case 'b': flag |= Blob_bm; break;
			case 'l': flag |= Blob_lm; break;
			case 'o': flag |= Blob_local; break;
			case 'r': flag |= Blob_rm; break;
			case 's': flag |= Blob_small; break;
			case 't': flag |= Blob_tm; break;
			};
		}
	else if(type&IsInterp) {
		switch(c) {
			case 's': flag |= Interp_spelled; break;
			};
		};
	return(flag);
	}

#if FRDI
/* read only the Ident of the next record; if EOF, return 0 */
Ident frdb_ident(fp)
    FILE *fp;
{   Ident ident;
	ident = frdi_Ident(fp);
#if dbg_frdb_toa
	err("frdb_ident: %s",ident_toa(ident));
#endif
	if(feof(fp)) return(0);
	else return(ident);
	}
#else
/* read only the Ident of the next record; return 0 iff EOF */
Ident frdb_ident(fp)
    FILE *fp;
{   Ident ident;
    int stat;
	if((stat=fread(&ident,sizeof(Ident),1,fp))!=1) {
		if(stat==0) return(0);
		else abort("frdb_ident: can't fread, status %d",stat);
		};
	if( Readvax ) ident = swapintin(ident);
#if dbg_frdb_toa
	err("frdb_ident: %s",ident_toa(ident));
#endif
	if(feof(fp)) return(0);
	else return(ident);
	}
#endif

#if FRDI
/* read label into malloc space */
char *frdb_label(f)
    FILE *f;
{   register char *res;
	res=frdi_str(f);
#if dbg_frdb_toa
	err("frdb_label: \"%s\"",res);
#endif
	return(res);
	}
#else
/* read label into malloc space */
char *frdb_label(fp)
    FILE *fp;
{   static char s[MAX_LABEL_LEN];
    char *c,*ce,*l;
    int ch;
	ce=(c=s)+MAX_LABEL_LEN-1;
	while((c<ce)&&((ch=getc(fp))!=EOF)&&(ch!='\0')) *(c++) = ch;
	*c='\0';
	if(c==ce) {
		/* label is truncated; find end of it */
		while(((ch=getc(fp))!=EOF)&&(ch!='\0')) ;
		};
	if((l=strdup(s))==NULL)
		abort("frdb_label: can't dup char *s[%d]",strlen(s));
#if dbg_frdb_toa
	err("frdb_label: \"%s\"",l);
#endif
	return(l);
	}
#endif

/* skip label */
fskb_label(fp)
    FILE *fp;
{   int ch;
	while(((ch=getc(fp))!=EOF)&&(ch!='\0')&&(ch!='\n')) ;
	if(ch==EOF) return(0); else return(1);
	}

/* Skip the REST of this record (the Ident is assumed to have been read),
   and return the Ident of the next record in the file;
   else return:
	 0	EOF
	-1	I/O error
	-2	not one of: Page, Block, Txtln, Char, Interp
   */
char *pp_toa(ppp)
    Pp *ppp;
{    static char s[30];
	sprintf(s,"(%0.2f,%0.2f)",ppp->x,ppp->y);
	return(s);
	}

/* return integer string in the range 00-99 */
char *merit_toa(m)
    Merit m;
{   static char s[3];
    int im;
	im = (int)(m * 100);
	if(im>99) im=99; else if(im<0) im=0;
	if(im<10) {
		s[0] = '0';
		sprintf(s+1,"%d",im);
		}
	else sprintf(s,"%2d",im);
	s[3] = '\0';
	return(s);
	}

char *pts_toa(p)
    Pts p;
{   static char s[10];
	sprintf(s,"%g",((int)((p*10.0)+0.5))/10.0);
	return(s);
	}

Bdy *alloc_bdy()
{    Bdy *p;
	if((p=(Bdy *)malloc(sizeof(Bdy)))==NULL)
		abort("alloc_bdy: can't malloc");
	else {	*p = empty_Bdy;
		return(p);
		};
	}

Bdys *alloc_bdys()
{    Bdys *p;
	if((p=(Bdys *)malloc(sizeof(Bdys)))==NULL)
		abort("alloc_bdys: can't malloc");
	else {	*p = empty_Bdys;
		return(p);
		};
	}

char *bdyedge_toa(bep)
    BdyEdge *bep;
{   static char s[80];
    char aps[20];
	strcpy(aps,sp_toa(bep->ap));
	sprintf(s,"%s,%s p%d c%s a%0.2f",
		aps, sp_toa(bep->bp),
		bep->per,
		pp_toa(&(bep->ctr)),
		(bep->ang/PI)*180.0);
	return(s);
	}

free_bdyedges(besp)
    BdyEdges *besp;
{   register BdyEdge *bep,**bepp;
	if(besp->pa!=NULL) {
		if(besp->mny>0) {
			for(bep= *(bepp=besp->pa); bep!=NULL; bep= *(++bepp))
				free(bep);
			};
		free(besp->pa);
		};
	*besp = empty_BdyEdges;
	}

/* Skip past the "TYPE=document-image\n\n" (or "TYPE=dim\n\n") ASCII header from
   file *fp, read and check the Doc record, and, if all's well, return 1.
   If an immediate EOF, return 0.  If not EOF, and no "TYPE=" header,
   leave fp as it was, and return -1.  If a malformed "TYPE=" header, leave fp
   as it was, and return -2. */
int skip_type(fp)
    FILE *fp;
{   int ich,stat;
    long seek;
#define MAX_TYPE_HDR_LEN (100)
    char type_eq[MAX_TYPE_HDR_LEN+1];
    register char *pcp,*ccp,*ecp;
    int version;

	seek = ftell(fp);
	/* test for EOF */
	if(feof(fp) || (ich=getc(fp))==EOF)
		/* premature EOF*/ return(0);
	if((type_eq[0]=ich)!='T') {
		/* no "TYPE=" header */
		ungetc(ich,fp);		/* fast, guaranteed push-back */
		return(-1);
		};
	/* try to read the rest of "TYPE=" */
	for(ecp=(ccp=type_eq+1)+4; ccp<ecp; ccp++) {
		if(feof(fp) || (ich=getc(fp))==EOF) {
			/* no "TYPE=" header */
			fseek(fp,seek,0);
			return(-1);
			}
		else *ccp=ich;
		}
	*ccp='\0';
	/* check for "TYPE=" so far */
	if(strcmp(type_eq,"TYPE=")==0) {
		/* read rest of ASCII header */
		ccp=(pcp=type_eq+3)+1;  ecp=type_eq+MAX_TYPE_HDR_LEN;
		do {	ccp++; pcp++;
			if((ich=getc(fp))==EOF) {
				/* malformed "TYPE=" header */
				fseek(fp,seek,0);
				return(-2);
				}
			else *ccp=ich;
			}
		while(ccp<ecp && (*ccp!='\n'||*pcp!='\n'));
		if(ccp==ecp) {
			/* malformed "TYPE=" header */
			fseek(fp,seek,0);
			return(-2);
			};
		*pcp='\0';
		/* check that it is "TYPE=document-image\n\n" or "TYPE=dim\n\n" */
		if(strcmp(type_eq,"TYPE=document-image")!=0
		   && strcmp(type_eq,"TYPE=dim")!=0) {
			/* malformed "TYPE=" header */
			fseek(fp,seek,0);
			return(-2);
			};
#if dbg_frdb_toa
		err("skip_type: \"%s\\n\\n\"",type_eq);
#endif
		return(1);	/* normal and successful TYPE= header */
		}
	else {	/* no "TYPE=" header */
		fseek(fp,seek,0);
		return(-1);
		};
	}

Doc *alloc_doc()
{    Doc *p;
	if((p=(Doc *)malloc(sizeof(Doc)))==NULL)
		abort("alloc_doc: can't malloc");
	else {	*p = empty_Doc;
		return(p);
		};
	}

char *doc_toa(p)
    Doc *p;
{    static char s[80];
	sprintf(s,"Doc v%d p%d l%s",
		p->version,
		p->ps.mny,
		(p->l!=NULL)? p->l: "");
	return(s);
	}

#if FWRI
/* write the given doc (record only) */
fwrb_doc(f,p)
    FILE *f;
    Doc *p;
{   int stat;
	fwri_uint1(f,p->version);
	fwri_uint2(f,p->ps.mny);
#if dbg_fwrb_toa
	err("fwrb_doc: %s",doc_toa(p));
#endif
 	if(p->l!=NULL) {
		fwri_ch(f,'l');
		fwrb_label(f,p->l);
		}
	else fwri_ch(f,'\0');
	}
#else
fwrb_doc(f,p)
    FILE *f;
    Doc *p;
{}
#endif

#if FRDI
/* read a Doc record (only) - it must be next in the file;
   don't immediately read its parts; return 0 iff immediate EOF.
   Check for correct version no: if fails, abort.  */
int frdb_doc(f,p)
    FILE *f;
    Doc *p;
{   char qlabel;
	*p = empty_Doc;
	if(feof(f))
		return(0);
	p->version=frdi_uint1(f);
	p->ps.mny=frdi_uint2(f);
	if((qlabel=frdi_ch(f))=='l')
		p->l=frdb_label(f);
#if dbg_frdb_toa
	err("frdb_doc: %s",doc_toa(p));
#endif
	if(ferror(f)) return(-errno); else return(1);
	}
#else
int frdb_doc(f,p)
    FILE *f;
    Doc *p;
{	return(1);
	}
#endif

#if FWRI
/* Write the initial Doc record */
put_doc(f)
    FILE *f;
{   Doc doc;
	doc = empty_Doc;
	doc.version = DIM_VERSION;
	doc.ps.mny = 1;
	fwrb_doc(f,&doc);
	}
#else
put_doc(f)
    FILE *f;
{}
#endif

#if FRDI
/* read, check, discard, and skip past the Doc record */
int skip_doc(f)
    FILE *f;
{   Doc doc;
	frdb_doc(f,&doc);
	if(doc.version!=DIM_VERSION)
		abort("skip_doc: version %d != DIM_VERSION %d",
			doc.version,DIM_VERSION);
	if(doc.l!=NULL) free(doc.l);
	return(1);
	}
#else
int skip_doc(f)
    FILE *f;
{   Doc doc;
	return(1);
	}
#endif

Page *alloc_page()
{    Page *p;
	if((p=(Page *)malloc(sizeof(Page)))==NULL)
		abort("alloc_page: can't malloc");
	alloc_census(Page,1);
	*p = empty_Page;
	return(p);
	}

char *page_toa(p)
    Page *p;
{    static char s[80];
	sprintf(s,"%s bx%s res%d,%d sk%0.2f,sh%0.2f bk%d l%d c%d b%d",
		ident_toa(p->ident),
		bbx_toa(&(p->bx)),
		p->res_x,p->res_y,
		(p->skew/PI)*180.0,
		(p->shear/PI)*180.0,
		p->bks.mny,p->ls.mny,p->cs.mny,p->bs.mny,
		(p->l!=NULL)? p->l: "");
	return(s);
	}

#if FWRI
/* write the given page (record only) */
fwrb_page(f,p)
    FILE *f;
    Page *p;
{   int stat;
#if dbg_fwrb
	if(f==stdout||f==stderr||ftell(f)<=0L) {
		fprintf(f,"TYPE=dim\n\n");
#if dbg_fwrb_toa
		err("fwrb_page: \"TYPE=dim\\n\\n\"");
#endif
		put_doc(f);
		};
	if((!(p->ident&IsPage))||(p->ident&(IsALL&(~IsPage))))
		err("fwrb_page: %s",page_toa(p));
	if(p->ident&Page_label && p->l==NULL) {
		err("fwrb_page: Page_label set but .l==NULL");
		p->ident &= ~Page_label;
		};
#endif
	fwri_Ident(f,p->ident);
	fwri_Bbx(f,&(p->bx));
	fwri_int2(f,p->res_x);
	fwri_int2(f,p->res_y);
	fwri_Radians(f,p->skew);
	fwri_Radians(f,p->shear);
	fwri_uint2(f,p->bks.mny);
	fwri_uint2(f,p->ls.mny);
	fwri_uint2(f,p->ws.mny);
	fwri_uint2(f,p->cs.mny);
	fwri_uint3(f,p->bs.mny);
#if dbg_fwrb_toa
	err("fwrb_page: %s",page_toa(p));
#endif
 	if(p->ident&Page_label) fwrb_label(f,p->l);
	}
#else
/* write the given page (record only) */
fwrb_page(fp,pp)
    FILE *fp;
    Page *pp;
{   PageF pf;
    int stat;
#if dbg_fwrb
	if((!(pp->ident&IsPage))||(pp->ident&(IsALL&(~IsPage))))
		err("fwrb_page: %s",page_toa(pp));
#endif
	memset(&pf,'\0',sizeof(pf));
	pf.ident = pp->ident;
	pf.res_x=pp->res_x;
	pf.res_y=pp->res_y;
	pf.bx=pp->bx;
	pf.skew=pp->skew;
	pf.shear=pp->shear;
	pf.bkmny=pp->bks.mny;
	pf.lmny=pp->ls.mny;
	pf.wmny=pp->ws.mny;
	pf.cmny=pp->cs.mny;
	pf.bmny=pp->bs.mny;
	if(fp==stdout||fp==stderr||ftell(fp)<=0L) {
		fprintf(fp,"TYPE=document-image\n\n");
#if dbg_fwrb_toa
		err("fwrb_page: \"TYPE=document-image\\n\\n\"");
#endif
		};
	if((stat=fwrite(&pf,sizeof(PageF),1,fp))!=1)
		abort("fwrb_page: can't fwrite - status %d",stat);
#if dbg_fwrb_toa
	err("fwrb_page: %s",page_toa(pp));
#endif
 	if(pf.ident&Page_label) fwrb_label(fp,pp->l);
	}
#endif

/* write a page and its specified parts */
fwrb_page_etc(fp,pp,etc)
    FILE *fp;
    Page *pp;
    Ident etc;
{   static Ident parts = (IsBlock|IsTxtln|IsWord|IsChar|IsBlob);
    Page pg;
   	if((etc&parts)!=parts) /* write selected parts */ {
		pg = *pp;
		if(!(etc&IsBlock)) pg.bks.mny=0;
		if(!(etc&IsTxtln)) pg.ls.mny=0;
		if(!(etc&IsWord)) pg.ws.mny=0;
		if(!(etc&IsChar)) pg.cs.mny=0;
		if(!(etc&IsBlob)) pg.bs.mny=0;
		pp = &pg;	/* write modified record */
		};
	fwrb_page(fp,pp);
	fwrb_blocks_etc(fp,pp->bks,etc);
	fwrb_txtlns_etc(fp,pp->ls,etc);
	fwrb_words_etc(fp,pp->ws,etc);
	fwrb_chars_etc(fp,pp->cs,etc);
	fwrb_blobs_etc(fp,pp->bs,etc);
	}

/* write a page and its specified parts, ASCII (INCOMPLETE) */
fwra_page_etc(fp,pp,etc)
    FILE *fp;
    Page *pp;
    Ident etc;
{   static Ident parts = (IsBlock|IsTxtln|IsWord|IsChar|IsBlob);
    Page pg;
   	if((etc&parts)!=parts) /* write selected parts */ {
		pg = *pp;
		if(!(etc&IsBlock)) pg.bks.mny=0;
		if(!(etc&IsTxtln)) pg.ls.mny=0;
		if(!(etc&IsWord)) pg.ws.mny=0;
		if(!(etc&IsChar)) pg.cs.mny=0;
		if(!(etc&IsBlob)) pg.bs.mny=0;
		pp = &pg;	/* write modified record */
		};
	if(etc&IsPage) fwrb_page(fp,pp);
	fwrb_blocks_etc(fp,pp->bks,etc);
	fwrb_txtlns_etc(fp,pp->ls,etc);
	fwrb_words_etc(fp,pp->ws,etc);
	fwrb_chars_etc(fp,pp->cs,etc);
	fwrb_blobs_etc(fp,pp->bs,etc);
	}

#if FRDI
/* read a Page record (only) - it must be next in the file;
   don't immediately read its parts; return 0 iff immediate EOF  */
int frdb_page(f,p)
    FILE *f;
    Page *p;
{   char *pfc;	/* for debugging */
    int stat;
	switch(skip_type(f)) {
	    case 1:  /* "TYPE=" header */
		skip_doc(f);
		break;
	    case 0:  /* immediate EOF */
		return(0);
		break;
	    case -1:  /* no "TYPE=" header */
		break;
	    case -2:  /* garbled "TYPE=" header */
		abort("frdb_page: garbled \"TYPE=\" header");
		break;
	    };
	*p = empty_Page;
	if(feof(f))
		return(0);
	p->ident=frdi_Ident(f);
	frdi_Bbx(f,&(p->bx));
	p->res_x=frdi_int2(f);
	p->res_y=frdi_int2(f);
	p->skew=frdi_Radians(f);
	p->shear=frdi_Radians(f);
	p->bks.mny=frdi_uint2(f);
	p->ls.mny=frdi_uint2(f);
	p->ws.mny=frdi_uint2(f);
	p->cs.mny=frdi_uint2(f);
	p->bs.mny=frdi_uint3(f);
#if dbg_frdb
	if((!(p->ident&IsPage))||(p->ident&(IsALL&(~IsPage))))
		err("frdb_page: %s",page_toa(p));
#endif
#if dbg_frdb_toa
	err("frdb_page: %s",page_toa(p));
#endif
 	if(p->ident&Page_label) p->l = frdb_label(f);
	if(ferror(f)) return(-errno); else return(1);
	}
#else
/* read a Page record (only) - it must be next in the file;
   don't immediately read its parts; return T iff not early EOF  */
int frdb_page(fp,pp)
    FILE *fp;
    Page *pp;
{   PageF pf;
    char *pfc;	/* for debugging */
    int stat;
	if((stat=skip_type(fp))!=1 /* "TYPE=" header */
	    && stat!=-1 /* no "TYPE=" header */ ) {
		/* premature EOF or error */ return(stat);
		};
	if((stat=fread(&pf,sizeof(PageF),1,fp))!=1)
		/* premature EOF*/ return(stat);
	pfc = (char *)&pf;
	*pp = empty_Page;
	pp->ident=pf.ident;
	pp->res_x=pf.res_x;
	pp->res_y=pf.res_y;
	pp->bx=pf.bx;
	pp->skew=pf.skew;
	pp->shear=pf.shear;
	pp->bks.mny = pf.bkmny;	pp->bks.bpa=NULL;
	pp->ls.mny = pf.lmny;	pp->ls.lpa=NULL;
	pp->ws.mny = pf.wmny;	pp->ws.wpa=NULL;
	pp->cs.mny = pf.cmny;	pp->cs.cpa=NULL;
	pp->bs.mny = pf.bmny;	pp->bs.bpa=NULL;
#if dbg_frdb_toa
	err("frdb_page: %s",page_toa(pp));
#endif
 	if(pp->ident&Page_label) pp->l = frdb_label(fp);
#if dbg_frdb
	if((!(pp->ident&IsPage))||(pp->ident&(IsALL&(~IsPage))))
		err("frdb_page: %s",page_toa(pp));
#endif
	return(stat);
	}
#endif

/* Read a page and selected parts */
int frdb_page_etc(fp,pp,etc)
    FILE *fp;
    Page *pp;
    Ident etc;
{   int stat;
	if((etc&IsALL)==IsALL) {
	    long here;
		/* In order to minimize malloc time to read an often huge file,
		   malloc and then free storage roughly equal to what's needed */
 		if( /* DEFEATED */ F && (here=ftell(fp))>=0 && fseek(fp,0,2)>=0 ) {
			/* *fp is connected to a file, not a pipe */
 			free(malloc(ftell(fp)));
 			fseek(fp,here,0);
 			};
		};
	if((stat=frdb_page(fp,pp))!=1) return(stat);
	if(etc&IsBlock && pp->bks.mny>0)
		frdb_blocks_etc(fp,&(pp->bks),etc);
	else pp->bks = empty_Blocks;
	if(etc&IsTxtln && pp->ls.mny>0)
		frdb_txtlns_etc(fp,&(pp->ls),etc);
	else pp->ls = empty_Txtlns;
	if(etc&IsWord && pp->ws.mny>0)
		frdb_words_etc(fp,&(pp->ws),etc);
	else pp->ws = empty_Words;
	if(etc&IsChar && pp->cs.mny>0)
		frdb_chars_etc(fp,&(pp->cs),etc);
	else pp->cs = empty_Chars;
	if(etc&IsBlob && pp->bs.mny>0)
		frdb_blobs_etc(fp,&(pp->bs),etc);
	else pp->bs = empty_Blobs;
	return(1/*normal return*/);
	}

free_page_etc(p,etc)
    Page *p;
    Ident etc;
{	free_blocks_etc(&(p->bks),etc);
	free_txtlns_etc(&(p->ls),etc);
	free_words_etc(&(p->ws),etc);
	free_chars_etc(&(p->cs),etc);
	free_blobs_etc(&(p->bs),etc);
	if(p->ident&Page_label && p->l!=NULL) {
		free(p->l);  p->l=NULL;  p->ident &= ~Page_label;
		};
	if(etc&IsPage) {
		free(p);
		free_census(Page,1);
		};
	}

/* return a pointer to a distinct and duplicate copy of record Page *p,
   created out of malloc space */
Page *dup_page(p)
    Page *p;
{   Page *dup;
	dup=alloc_page();  *dup = *p;  return(dup);
	}

/* Return a pointer to a distinct and duplicate copy of Page *p, and its parts
   as specified by `etc', created out of malloc space. */
Page *dup_page_etc(p,etc)
    Page *p;
    Ident etc;
{   Page *dup;
	dup = dup_page(p);
	if(etc&IsBlob&&dup->bs.mny>0) dup->bs = *dup_blobs_etc(&(p->bs),etc);
	else dup->bs = empty_Blobs;
	if(etc&IsChar&&dup->cs.mny>0) dup->cs = *dup_chars_etc(&(p->cs),etc);
	else dup->cs = empty_Chars;
	if(etc&IsWord&&dup->ws.mny>0) dup->ws = *dup_words_etc(&(p->ws),etc);
	else dup->ws = empty_Words;
	if(etc&IsTxtln&&dup->ls.mny>0) dup->ls = *dup_txtlns_etc(&(p->ls),etc);
	else dup->ls = empty_Txtlns;
	if(etc&IsBlock&&dup->bks.mny>0) dup->bks = *dup_blocks_etc(&(p->bks),etc);
	else dup->bks = empty_Blocks;
	return(dup);
	}

Block *alloc_block()
{    Block *p;
	if((p=(Block *)malloc(sizeof(Block)))==NULL)
		abort("alloc_block: can't malloc");
	alloc_census(Block,1);
	*p = empty_Block;
	return(p);
	}

char *block_toa(p)
    Block *p;
{   static char s[80];
	sprintf(s,"%s bx%s sk%0.2f,sh%0.2f wst%0.2f m%s bk%d l%d w%d c%d b%d",
		ident_toa(p->ident),
		bbx_toa(&(p->bx)),
		p->skew/DtoR,
		p->shear/DtoR,
		p->wst,
		merit_toa(p->m),
		p->bks.mny,p->ls.mny,p->ws.mny,p->cs.mny,p->bs.mny,
		(p->l!=NULL)? p->l: "");
	return(s);
	}

#if FWRI
fwrb_block(f,p)
    FILE *f;
    Block *p;
{   int stat;
#if dbg_fwrb
	if((!(p->ident&IsBlock))||(p->ident&(IsALL&(~IsBlock))))
		err("fwrb_block: %s",block_toa(p));
	if(p->ident&Block_label && p->l==NULL) {
		err("fwrb_page: Block_label set but .l==NULL");
		p->ident &= ~Block_label;
		};
#endif
	fwri_Ident(f,p->ident);
	fwri_Bbx(f,&(p->bx));
	fwri_Radians(f,p->skew);
	fwri_Radians(f,p->shear);
	fwri_Ems(f,p->wst);
	fwri_Merit(f,p->m);
	fwri_uint2(f,p->bks.mny);
	fwri_uint2(f,p->ls.mny);
	fwri_uint2(f,p->ws.mny);
	fwri_uint2(f,p->cs.mny);
	fwri_uint3(f,p->bs.mny);
#if dbg_fwrb_toa
	err("fwrb_block: %s",block_toa(p));
#endif
 	if(p->ident&Block_label) fwrb_label(f,p->l);
	}
#else
fwrb_block(fp,bkp)
    FILE *fp;
    Block *bkp;
{   BlockF bf;
    int stat;
#if dbg_fwrb
	if((!(bkp->ident&IsBlock))||(bkp->ident&(IsALL&(~IsBlock))))
		err("fwrb_block: %s",block_toa(bkp));
#endif
	memset(&bf,'\0',sizeof(bf));
	bf.ident = bkp->ident;
	bf.bx = bkp->bx;
	bf.wst = bkp->wst;
	bf.skew = bkp->skew;
	bf.shear = bkp->shear;
	bf.lmny = bkp->ls.mny;
	bf.wmny = bkp->ws.mny;
	bf.cmny = bkp->cs.mny;
	bf.bmny = bkp->bs.mny;
#if dbg_fwrb_toa
	err("fwrb_block: %s",block_toa(bkp));
#endif
	if((stat=fwrite(&bf,sizeof(BlockF),1,fp))!=1)
		abort("fwrb_block: can't fwrite - status %d",stat);
	}
#endif

fwrb_blocks_etc(fp,bks,etc)
    FILE *fp;
    Blocks bks;
    Ident etc;
{   register Block *bkp,**bkpp;
	if(bks.mny>0) for(bkp= *(bkpp=bks.bpa); bkp!=NULL; bkp= *(++bkpp))
		fwrb_block_etc(fp,bkp,etc);
	}

fwrb_block_etc(fp,bkp,etc)
    FILE *fp;
    Block *bkp;
    Ident etc;
{   static Ident parts = (IsTxtln|IsWord|IsChar|IsBlob);
    Block bk;
   	if((etc&parts)!=parts) /* write selected parts */ {
		bk = *bkp;
		if(!(etc&IsTxtln)) bk.ls.mny=0;
		if(!(etc&IsWord)) bk.ws.mny=0;
		if(!(etc&IsChar)) bk.cs.mny=0;
		if(!(etc&IsBlob)) bk.bs.mny=0;
		bkp = &bk;	/* write modified record */
		};
	fwrb_block(fp,bkp);
	fwrb_txtlns_etc(fp,bkp->ls,etc);
	fwrb_words_etc(fp,bkp->ws,etc);
	fwrb_chars_etc(fp,bkp->cs,etc);
	fwrb_blobs_etc(fp,bkp->bs,etc);
	}

#if FRDI
int frdb_block(f,p)
    FILE *f;
    Block *p;
{	*p = empty_Block;
	if(feof(f))
		return(0);
	p->ident=frdi_Ident(f);
	frdi_Bbx(f,&(p->bx));
	p->skew=frdi_Radians(f);
	p->shear=frdi_Radians(f);
	p->wst=frdi_Ems(f);
	p->m=frdi_Merit(f);
	p->bks.mny=frdi_uint2(f);
	p->ls.mny=frdi_uint2(f);
	p->ws.mny=frdi_uint2(f);
	p->cs.mny=frdi_uint2(f);
	p->bs.mny=frdi_uint3(f);
#if dbg_frdb_toa
	err("frdb_block: %s",block_toa(p));
#endif
#if dbg_frdb
	if((!(p->ident&IsBlock))||(p->ident&(IsALL&(~IsBlock))))
		err("frdb_block: %s",block_toa(p));
#endif
 	if(p->ident&Block_label) p->l = frdb_label(f);
	if(ferror(f)) return(-errno); else return(1);
	}
#else
int frdb_block(fp,bkp)
    FILE *fp;
    Block *bkp;
{   BlockF bf;
    int stat;
	if((stat=fread(&bf,sizeof(BlockF),1,fp))!=1)
		abort("frdb_block: can't fread - status %d",stat);
	*bkp = empty_Block;
	bkp->ident = bf.ident;
	bkp->bx = bf.bx;
	bkp->skew = bf.skew;
	bkp->shear = bf.shear;
	bkp->wst = bf.wst;
	bkp->ls.mny = bf.lmny;
	bkp->ws.mny = bf.wmny;
	bkp->cs.mny = bf.cmny;
	bkp->bs.mny = bf.bmny;
#if dbg_frdb
	if((!(bkp->ident&IsBlock))||(bkp->ident&(IsALL&(~IsBlock))))
		err("frdb_block: %s",block_toa(bkp));
#endif
#if dbg_frdb_toa
	err("frdb_block: %s",block_toa(bkp));
#endif
 	if(bkp->ident&Block_label) bkp->l = frdb_label(fp);
	if(ferror(fp)) return(-errno); else return(1);
	}
#endif

frdb_block_etc(fp,bkp,etc)
    FILE *fp;
    Block *bkp;
    Ident etc;
{   BlockF bf;
    int stat;
	frdb_block(fp,bkp);
	if(etc&IsTxtln && bkp->ls.mny>0)
		frdb_txtlns_etc(fp,&(bkp->ls),etc);
	else bkp->ls = empty_Txtlns;
	if(etc&IsWord && bkp->ws.mny>0)
		frdb_words_etc(fp,&(bkp->ws),etc);
	else bkp->ws = empty_Words;
	if(etc&IsChar && bkp->cs.mny>0)
		frdb_chars_etc(fp,&(bkp->cs),etc);
	else bkp->cs = empty_Chars;
	if(etc&IsBlob && bkp->bs.mny>0)
		frdb_blobs_etc(fp,&(bkp->bs),etc);
	else bkp->bs = empty_Blobs;
	if(ferror(fp)) return(-errno); else return(1);
	}

/* read number of blocks, and their parts */
frdb_blocks_etc(fp,bksp,etc)
    FILE *fp;
    Blocks *bksp;
    Ident etc;
{   int bi;
    register Block *bkp,**bkpp;
	if(bksp->mny<=0) {
		*bksp = empty_Blocks;
		return(1);
		};

	if((bkpp=bksp->bpa=(Block **)malloc((bksp->mny+1)*sizeof(Block *)))==NULL)
		abort("can't alloc Blocks.bpa array");
	for(bi=0; bi<bksp->mny; bi++) {
		*(bkpp++) = bkp = alloc_block();
		frdb_block_etc(fp,bkp,etc);
		};
	*bkpp = NULL;
	if(ferror(fp)) return(-errno); else return(1);
	}

free_block_etc(p,etc)
    Block *p;
    Ident etc;
{	free_txtlns_etc(&(p->ls),etc);
	free_words_etc(&(p->ws),etc);
	free_chars_etc(&(p->cs),etc);
	free_blobs_etc(&(p->bs),etc);
	if(p->ident&Block_label && p->l!=NULL) {
		free(p->l);  p->l=NULL;  p->ident &= ~Block_label;
		};
	if(etc&IsBlock) {
		free(p);
		free_census(Block,1);
		};
	}

/* Unconditionally free the malloc-space array of pointers, and empty the set.
   Don't free the records that it owned.
   */
free_blocks(p)
    Blocks *p;
{   	if(p->bpa!=NULL) { free(p->bpa);  p->bpa = NULL; }
	p->mny = 0;
	}

free_blocks_etc(p,etc)
    Blocks *p;
    Ident etc;
{   register Block *bkp,**bkpp;
	if(p->mny>0&&(etc&IsBlock))
		for(bkp= *(bkpp=p->bpa); bkp!=NULL; bkp= *(++bkpp))
			free_block_etc(bkp,etc);
   	free_blocks(p);
	}

/* return a pointer to a distinct and duplicate copy of *bkp,
   created out of malloc space */
Block *dup_block(bkp)
    Block *bkp;
{   Block *dup;
	dup=alloc_block();
	*dup = *bkp;
	return(dup);
	}

/* Return a pointer to a distinct and duplicate copy of *bkp, and its parts
   as specified by `etc', created out of malloc space. */
Block *dup_block_etc(bkp,etc)
    Block *bkp;
    Ident etc;
{   Block *dup;
	dup = dup_block(bkp);
	if(etc&IsBlob&&dup->bs.mny>0) dup->bs = *dup_blobs_etc(&(bkp->bs),etc);
	else dup->bs = empty_Blobs;
	if(etc&IsChar&&dup->cs.mny>0) dup->cs = *dup_chars_etc(&(bkp->cs),etc);
	else dup->cs = empty_Chars;
	if(etc&IsWord&&dup->ws.mny>0) dup->ws = *dup_words_etc(&(bkp->ws),etc);
	else dup->ws = empty_Words;
	if(etc&IsTxtln&&dup->ls.mny>0) dup->ls = *dup_txtlns_etc(&(bkp->ls),etc);
	else dup->ls = empty_Txtlns;
	return(dup);
	}

/* Return a pointer to a distinct local static duplicate of non-empty *bksp.
   Its bkpa array is created newly out of malloc space.
   If etc&IsBlock, all its Blocks are also duplicated,
   else the contents of bkpa point to the old unduplicated Blocks.
   */
Blocks *dup_blocks_etc(bksp,etc)
    Blocks *bksp;
    Ident etc;		/* parts to duplicate */
{   static Blocks dup;
    register Block **bkpp,**dpp;
	if((dup.mny = bksp->mny)<=0) dup = empty_Blocks;
	else {	if((dup.bpa=(Block **)malloc((dup.mny+1)*sizeof(Block *)))==NULL)
			abort("can't dup bks.bpa");
		for(bkpp=bksp->bpa,dpp=dup.bpa; *bkpp!=NULL; bkpp++,dpp++) {
			if(etc&IsBlock) *dpp = dup_block_etc(*bkpp,etc);
			else *dpp = *bkpp;
			};
		*dpp = NULL;
		};
	return(&dup);
	}

/* Append a block to the end of a blocks set. 
   Return appended Block *. */
Block *append_block(bkp,bksp)
    Block *bkp;
    Blocks *bksp;
{   register Block *rp,**rpp,**npp;
	if(bksp->mny==0) {
		if((bksp->bpa=(Block **)malloc(2*sizeof(Block *)))==NULL)
			abort("append_block: can't malloc bksp->bpa");
		}
	else {	if((bksp->bpa=(Block **)realloc(
				bksp->bpa,
				(bksp->mny+2)*sizeof(Block *))
				)==NULL)
			abort("append_block: can't realloc bksp->bpa");
		};
	bksp->bpa[bksp->mny] = bkp;
	bksp->bpa[++bksp->mny] = NULL;
	return(bkp);
	}

/* remove a block from a blocks set */
remove_block(bkp,bksp)
    Block *bkp;
    Blocks *bksp;
{   register Block *rp,**rpp,**npp;
	if(bksp->mny==0) err("can't remove Block - Blocks empty");
	else {	for(rp= *(rpp=bksp->bpa); rp!=NULL; rp= *(++rpp)) if(rp==bkp) break;
		if(rp==NULL) err("can't remove Block - not found");
		else {	/* move later entries up */
			npp=rpp+1;
			do *(rpp++)= *(npp++); while ((*rpp)!=NULL);
			if((--(bksp->mny))==0) {free(bksp->bpa); bksp->bpa=NULL;};
			/* don't bother to realloc downwards */
			};
		};
	}

ConstPitch *alloc_constpitch()
{    ConstPitch *p;
	if((p=(ConstPitch *)malloc(sizeof(ConstPitch)))==NULL)
		abort("can't alloc ConstPitch");
	else {	*p = empty_ConstPitch;
		return(p);
		};
	}

char *constpitch_toa(p)
    ConstPitch *p;
{    static char s[80];
	sprintf(s,"{w%.3f,o%d,r%.1f}",
		p->w,p->o,p->r);
	return(s);
	}

free_constpitch(p)
    ConstPitch *p;
{	free(p);
	}

Txtln *alloc_txtln()
{    Txtln *p;
	if((p=(Txtln *)malloc(sizeof(Txtln)))==NULL)
		abort("can't alloc Txtln");
	alloc_census(Txtln,1);
	*p = empty_Txtln;
	return(p);
	}

char *txtln_toa(p)
    Txtln *p;
{    static char s[80];
	sprintf(s,"%s bx%s sz%s ba%d cw%s m%s l%d w%d c%d b%d %s",
		ident_toa(p->ident),
		bbx_toa(&(p->bx)),
		pts_toa(p->size),
		p->basl,
		/** proj omitted **/
		((p->cp!=NULL)? constpitch_toa(p->cp): "0"),
		merit_toa(p->m),
		p->ls.mny,p->ws.mny,p->cs.mny,p->bs.mny,
		(p->l!=NULL)? p->l: "");
	return(s);
	}

#if FWRI
fwrb_txtln(f,p)
    FILE *f;
    Txtln *p;
{
#if dbg_fwrb
	if((!(p->ident&IsTxtln))||(p->ident&(IsALL&(~IsTxtln))))
		err("fwrb_txtln: %s",txtln_toa(p));
	if(p->ident&Txtln_label && p->l==NULL) {
		err("fwrb_page: Txtln_label set but .l==NULL");
		p->ident &= ~Txtln_label;
		};
#endif
	fwri_Ident(f,p->ident);
	fwri_Bbx(f,&(p->bx));
	fwri_Pts(f,p->size);
	fwri_Scoor(f,p->basl);
	/* fwri_?(f,p->proj); */
	/* fwri_ConstPitch(f,p->cp); */
	fwri_Merit(f,p->m);
	fwri_uint2(f,p->ls.mny);
	fwri_uint2(f,p->ws.mny);
	fwri_uint2(f,p->cs.mny);
	fwri_uint3(f,p->bs.mny);
#if dbg_fwrb_toa
	err("fwrb_txtln: %s",txtln_toa(p));
#endif
	if(p->ident&Txtln_label) fwrb_label(f,p->l);
	}
#else
fwrb_txtln(fp,lp)
    FILE *fp;
    Txtln *lp;
{   TxtlnF lf;
    int stat;
#if dbg_fwrb
	if((!(lp->ident&IsTxtln))||(lp->ident&(IsALL&(~IsTxtln))))
		err("fwrb_txtln: %s",txtln_toa(lp));
#endif
	memset(&lf,'\0',sizeof(lf));
	lf.ident = lp->ident;
	lf.bx = lp->bx;
	lf.basl = lp->basl;
	lf.size = lp->size;
	if(lp->proj!=NULL) lf.pmny = lp->bx.b.y - lp->bx.a.y + 1;
	else lf.pmny = 0;
	/* ignore lp->cp for now */
	lf.wmny = lp->ws.mny;
	lf.cmny = lp->cs.mny;
	lf.bmny = lp->bs.mny;
	if((stat=fwrite(&lf,sizeof(TxtlnF),1,fp))!=1)
		abort("can't write TxtlnF, status %d",stat);
#if dbg_fwrb_toa
	err("fwrb_txtln: %s",txtln_toa(lp));
#endif
	if(lf.ident&Txtln_label) fwrb_label(fp,lp->l);
	}
#endif

fwrb_txtlns_etc(fp,ls,etc)
    FILE *fp;
    Txtlns ls;
    Ident etc;
{   register Txtln *lp,**lpp;
	if(ls.mny>0) for(lp= *(lpp=ls.lpa); lp!=NULL; lp= *(++lpp))
		fwrb_txtln_etc(fp,lp,etc);
	}

fwrb_txtln_etc(fp,lp,etc)
    FILE *fp;
    Txtln *lp;
    Ident etc;
{   static Ident parts = (IsWord|IsChar|IsBlob);
    Txtln tl;
   	if((etc&parts)!=parts) /* write selected parts */ {
		tl = *lp;
		if(!(etc&IsWord)) tl.ws.mny=0;
		if(!(etc&IsChar)) tl.cs.mny=0;
		if(!(etc&IsBlob)) tl.bs.mny=0;
		lp = &tl;	/* write modified record */
		};
	fwrb_txtln(fp,lp);
	if(lp->proj!=NULL) {/* write projection array someday */};
	fwrb_words_etc(fp,lp->ws,etc);
	fwrb_chars_etc(fp,lp->cs,etc);
	fwrb_blobs_etc(fp,lp->bs,etc);
	}

#if FRDI
int frdb_txtln(f,p)
    FILE *f;
    Txtln *p;
{	*p = empty_Txtln;
	if(feof(f))
		return(0);
	p->ident=frdi_Ident(f);
	frdi_Bbx(f,&(p->bx));
	p->size=frdi_Pts(f);
	p->basl=frdi_Scoor(f);
	/* p->proj=frdi_?(f); */
	/* p->cp=frdi_ConstPitch(f); */
	p->m=frdi_Merit(f);
	p->ls.mny=frdi_uint2(f);
	p->ws.mny=frdi_uint2(f);
	p->cs.mny=frdi_uint2(f);
	p->bs.mny=frdi_uint3(f);
#if dbg_frdb
	if((!(p->ident&IsTxtln))||(p->ident&(IsALL&(~IsTxtln))))
		err("frdb_txtln: %s",txtln_toa(p));
#endif
#if dbg_frdb_toa
	err("frdb_txtln: %s",txtln_toa(p));
#endif
	if(p->ident&Txtln_label) p->l = frdb_label(f);
	if(ferror(f)) return(-errno); else return(1);
	}
#else
int frdb_txtln(fp,lp)
    FILE *fp;
    Txtln *lp;
{   TxtlnF lf;
    int stat;
	if((stat=fread(&lf,sizeof(TxtlnF),1,fp))!=1)
		abort("can't read TxtlnF, status %d",stat);
	*lp = empty_Txtln;
	lp->ident = lf.ident;
	lp->bx = lf.bx;
	lp->basl = lf.basl;
	lp->size = lf.size;
	if(lf.pmny>0) lp->proj=(short *)malloc(lf.pmny);
	else lp->proj = NULL;
	lp->cp = NULL;
	lp->ws.mny = lf.wmny;
	lp->cs.mny = lf.cmny;
	lp->bs.mny = lf.bmny;
#if dbg_frdb
	if((!(lp->ident&IsTxtln))||(lp->ident&(IsALL&(~IsTxtln))))
		err("frdb_txtln: %s",txtln_toa(lp));
#endif
#if dbg_frdb_toa
	err("frdb_txtln: %s",txtln_toa(lp));
#endif
	if(lp->ident&Txtln_label) lp->l = frdb_label(fp);
	if(ferror(fp)) return(-errno); else return(1);
	}
#endif

frdb_txtln_etc(fp,lp,etc)
    FILE *fp;
    Txtln *lp;
    Ident etc;
{   TxtlnF lf;
    int stat;
	frdb_txtln(fp,lp);
	if(lp->proj!=NULL) {/* read projection array someday */};
	if(etc&IsWord && lp->ws.mny>0)
		frdb_words_etc(fp,&(lp->ws),etc);
	else lp->ws = empty_Words;
	if(etc&IsChar && lp->cs.mny>0)
		frdb_chars_etc(fp,&(lp->cs),etc);
	else lp->cs = empty_Chars;
	if(etc&IsBlob && lp->bs.mny>0)
		frdb_blobs_etc(fp,&(lp->bs),etc);
	else lp->bs = empty_Blobs;
	if(ferror(fp)) return(-errno); else return(1);
	}

/* read number of txtlns, and parts */
frdb_txtlns_etc(fp,lsp,etc)
    FILE *fp;
    Txtlns *lsp;
    Ident etc;
{   int li;
    register Txtln *lp,**lpp;
	if(lsp->mny<=0) {
		*lsp = empty_Txtlns;
		return(1);
		};
	if((lpp=lsp->lpa=(Txtln **)malloc((lsp->mny+1)*sizeof(Txtln *)))==NULL)
		abort("can't alloc Txtlns.lpa array");
	for(li=0; li<lsp->mny; li++) {
		*(lpp++) = lp = alloc_txtln();
		frdb_txtln_etc(fp,lp,etc);
		};
	*lpp = NULL;
	if(ferror(fp)) return(-errno); else return(1);
	}

/* Append a txtln to the end of a txtlns set. 
   Do NOT maintain Txtlns in order sorted ascending on Txtln.bx.a.y.
   Return appended Txtln *. */
Txtln *append_txtln(lp,lsp)
    Txtln *lp;
    Txtlns *lsp;
{   register Txtln *rp,**rpp,**npp;
	if(lsp->mny==0) {
		if((lsp->lpa=(Txtln **)malloc(2*sizeof(Txtln *)))==NULL)
			abort("append_txtln: can't malloc lsp->lpa");
		}
	else {	if((lsp->lpa=(Txtln **)realloc(
				lsp->lpa,
				(lsp->mny+2)*sizeof(Txtln *))
				)==NULL)
			abort("append_txtln: can't realloc lsp->lpa");
		};
	lsp->lpa[lsp->mny] = lp;
	lsp->lpa[++lsp->mny] = NULL;
	return(lp);
	}

/* remove a txtln from a txtlns set */
remove_txtln(lp,lsp)
    Txtln *lp;
    Txtlns *lsp;
{   register Txtln *rp,**rpp,**npp;
	if(lsp->mny==0) err("can't remove Txtln - Txtlns empty");
	else {	for(rp= *(rpp=lsp->lpa); rp!=NULL; rp= *(++rpp)) if(rp==lp) break;
		if(rp==NULL) err("can't remove Txtln - not found");
		else {	/* move later entries up */
			npp=rpp+1;
			do *(rpp++)= *(npp++); while ((*rpp)!=NULL);
			if((--(lsp->mny))==0) {free(lsp->lpa); lsp->lpa=NULL;};
			/* don't bother to realloc downwards */
			};
		};
	}

free_txtln_etc(p,etc)
    Txtln *p;
    Ident etc;
{	free_words_etc(&(p->ws),etc);
	free_chars_etc(&(p->cs),etc);
	free_blobs_etc(&(p->bs),etc);
	if(p->ident&Txtln_label && p->l!=NULL) {
		free(p->l);  p->l=NULL;  p->ident &= ~Txtln_label;
		};
	if(etc&IsTxtln) {
		if(p->l!=NULL) free(p->l);
		free(p);
		free_census(Txtln,1);
		};
	}

/* Unconditionally free the malloc-space array of pointers, and empty the set.
   Don't free the records that it owned.
   */
free_txtlns(lsp)
    Txtlns *lsp;
{   	if(lsp->lpa!=NULL) { free(lsp->lpa); lsp->lpa = NULL; }
	lsp->mny = 0;
	}

free_txtlns_etc(lsp,etc)
    Txtlns *lsp;
    Ident etc;
{   register Txtln *lp,**lpp;
	if(lsp->mny>0&&(etc&IsTxtln))
		for(lp= *(lpp=lsp->lpa); lp!=NULL; lp= *(++lpp))
			free_txtln_etc(lp,etc);
   	free_txtlns(lsp);
	}

/* return a pointer to a distinct and duplicate copy of *lp,
   created out of malloc space */
Txtln *dup_txtln(lp)
    Txtln *lp;
{   Txtln *dup;
	if((dup=(Txtln *)malloc(sizeof(Txtln)))==NULL)
		abort("can't dup Txtln");
	alloc_census(Txtln,1);
	*dup = *lp;
	return(dup);
	}

/* Return a pointer to a distinct and duplicate copy of *lp, and its parts
   as specified by `etc', created out of malloc space. */
Txtln *dup_txtln_etc(lp,etc)
    Txtln *lp;
    Ident etc;
{   Txtln *dup;
	dup = dup_txtln(lp);
	if(etc&IsBlob&&dup->bs.mny>0) dup->bs = *dup_blobs_etc(&(lp->bs),etc);
	else dup->bs = empty_Blobs;
	if(etc&IsChar&&dup->cs.mny>0) dup->cs = *dup_chars_etc(&(lp->cs),etc);
	else dup->cs = empty_Chars;
	if(etc&IsWord&&dup->ws.mny>0) dup->ws = *dup_words_etc(&(lp->ws),etc);
	else dup->ws = empty_Words;
	return(dup);
	}

/* Return a pointer to a distinct local static duplicate of non-empty *lsp.
   Its lpa array is created newly out of malloc space.
   If etc&IsTxtln, all its Txtlns are also duplicated,
   else the contents of lpa point to the old unduplicated Txtlns.
   */
Txtlns *dup_txtlns_etc(lsp,etc)
    Txtlns *lsp;
    Ident etc;		/* parts to duplicate */
{   static Txtlns dup;
    register Txtln **lpp,**dpp;
	if((dup.mny = lsp->mny)<=0) dup = empty_Txtlns;
	else {	if((dup.lpa=(Txtln **)malloc((dup.mny+1)*sizeof(Txtln *)))==NULL)
			abort("can't dup ls.lpa");
		for(lpp=lsp->lpa,dpp=dup.lpa; *lpp!=NULL; lpp++,dpp++) {
			if(etc&IsTxtln) *dpp = dup_txtln_etc(*lpp,etc);
			else *dpp = *lpp;
			};
		*dpp = NULL;
		};
	return(&dup);
	}

Word *alloc_word()
{    Word *p;
	if((p=(Word *)malloc(sizeof(Word)))==NULL)
		abort("can't alloc Word");
	alloc_census(Word,1);
	*p = empty_Word;
	return(p);
	}

char *word_toa(p)
    Word *p;
{    static char s[80];
	sprintf(s,"%s bx%s ws%0.2f c%d b%d m%s w%d",
		ident_toa(p->ident),
		bbx_toa(&(p->bx)),
		p->wsp,
		p->cs.mny,p->bs.mny,merit_toa(p->m),p->ws.mny);
	return(s);
	}

/* Are two Words identical?  A not quite exhaustively-detailed test. */
boolean eq_word(w1,w2)
    Word *w1,*w2;
{   register Char **c1,**c2;
	if(w1->cs.mny!=w2->cs.mny) return(F);
	for(c1=w1->cs.cpa,c2=w2->cs.cpa; *c1!=NULL; c1++,c2++) {
		if((*c1)->area!=(*c2)->area) return(F);
		if(!bbx_eq(&((*c1)->bx),&((*c2)->bx))) return(F);
		if((*c1)->bmny!=(*c2)->bmny) return(F);
		};
	/* don't bother to compare Blob lists in detail (they aren't
	   in any particular order). */
	return(T);
	}

/* Compute a hash key for this Word that is likely to indicate whether or
   not it is geometrically identical to another Word. */
int hash_word(w)
    Word *w;
{   register int k;
    register Char **c;
	if(w->cs.mny==0) return(0);
	k = w->bx.a.x * w->bx.a.y * w->bx.b.x * w->bx.b.y;
	for(c=w->cs.cpa; (*c)!=NULL; c++) {
		k += (*c)->bx.a.x * (*c)->bx.a.y * (*c)->bx.b.x
			* (*c)->bx.b.y * (*c)->bmny * (*c)->area;
		};
	return(k);
	}

#if FWRI
fwrb_word(f,p)
    FILE *f;
    Word *p;
{
#if dbg_fwrb
	if((!(p->ident&IsWord))||(p->ident&(IsALL&(~IsWord))))
		err("fwrb_word: %s",word_toa(p));
	if(p->ident&Word_label && p->l==NULL) {
		err("fwrb_page: Word_label set but .l==NULL");
		p->ident &= ~Word_label;
		};
#endif
	fwri_Ident(f,p->ident);
	fwri_Bbx(f,&(p->bx));
	fwri_int2(f,p->wsp*UCHAR_MAX);
	fwri_Merit(f,p->m);
	fwri_Prob(f,p->p);
	fwri_uint2(f,p->ws.mny);
	fwri_uint2(f,p->cs.mny);
	fwri_uint3(f,p->bs.mny);
	/* fwri_int4(f,p->hash); */
#if dbg_fwrb_toa
	err("fwrb_word: %s",word_toa(p));
#endif
	if(p->ident&Word_label) fwrb_label(f,p->l);
	}
#else
fwrb_word(fp,wp)
    FILE *fp;
    Word *wp;
{   WordF wf;
    int stat;
#if dbg_fwrb
	if((!(wp->ident&IsWord))||(wp->ident&(IsALL&(~IsWord))))
		err("fwrb_word: %s",word_toa(wp));
#endif
	memset(&wf,'\0',sizeof(wf));
	wf.ident = wp->ident;
	wf.bx = wp->bx;
	wf.wsp = wp->wsp;
	wf.m = wp->m;
	wf.wmny = wp->ws.mny;
	wf.cmny = wp->cs.mny;
	wf.bmny = wp->bs.mny;
	if((stat=fwrite(&wf,sizeof(WordF),1,fp))!=1)
		abort("can't write WordF, status %d",stat);
#if dbg_fwrb_toa
	err("fwrb_word: %s",word_toa(wp));
#endif
	if(wf.ident&Word_label) fwrb_label(fp,wp->l);
	}
#endif

fwrb_words_etc(fp,ws,etc)
    FILE *fp;
    Words ws;
    Ident etc;
{   register Word *wp,**wpp;
	if(ws.mny>0) for(wp= *(wpp=ws.wpa); wp!=NULL; wp= *(++wpp))
		fwrb_word_etc(fp,wp,etc);
	}

fwrb_word_etc(fp,wp,etc)
    FILE *fp;
    Word *wp;
    Ident etc;
{   static Ident parts = (IsWord|IsChar|IsBlob);
    Word wd;
   	if((etc&parts)!=parts) /* write selected parts */ {
		wd = *wp;
		if(!(etc&IsWord)) wd.ws.mny=0;
		if(!(etc&IsChar)) wd.cs.mny=0;
		if(!(etc&IsBlob)) wd.bs.mny=0;
		wp = &wd;	/* write modified record */
		};
	fwrb_word(fp,wp);
	if(etc&IsWord) fwrb_words_etc(fp,wp->ws,IsALL);
	fwrb_chars_etc(fp,wp->cs,etc);
	fwrb_blobs_etc(fp,wp->bs,etc);
	}

#if FRDI
int frdb_word(f,p)
    FILE *f;
    Word *p;
{	*p = empty_Word;
	if(feof(f))
		return(0);
	p->ident=frdi_Ident(f);
	frdi_Bbx(f,&(p->bx));
	p->wsp=frdi_int2(f)/(float)UCHAR_MAX;
	p->m=frdi_Merit(f);
	p->p=frdi_Prob(f);
	p->ws.mny=frdi_uint2(f);
	p->cs.mny=frdi_uint2(f);
	p->bs.mny=frdi_uint3(f);
	/* p->hash=frdi_int4(f); */
#if dbg_frdb
	if((!(p->ident&IsWord))||(p->ident&(IsALL&(~IsWord))))
		err("frdb_word: %s",word_toa(p));
#endif
#if dbg_frdb_toa
	err("frdb_word: %s",word_toa(p));
#endif
	if(p->ident&Word_label) p->l = frdb_label(f);
	if(ferror(f)) return(-errno); else return(1);
	}
#else
int frdb_word(fp,wp)
    FILE *fp;
    Word *wp;
{   WordF wf;
    int stat;
	if((stat=fread(&wf,sizeof(WordF),1,fp))!=1)
		abort("can't read WordF, status %d",stat);
	*wp = empty_Word;
	wp->ident = wf.ident;
	wp->bx = wf.bx;
	wp->wsp = wf.wsp;
	wp->m = wf.m;
	wp->ws.mny = wf.wmny;
	wp->cs.mny = wf.cmny;
	wp->bs.mny = wf.bmny;
#if dbg_frdb
	if((!(wp->ident&IsWord))||(wp->ident&(IsALL&(~IsWord))))
		err("frdb_word: %s",word_toa(wp));
#endif
#if dbg_frdb_toa
	err("frdb_word: %s",word_toa(wp));
#endif
	if(wp->ident&Word_label) wp->l = frdb_label(fp);
	if(ferror(fp)) return(-errno); else return(1);
	}
#endif

frdb_word_etc(fp,wp,etc)
    FILE *fp;
    Word *wp;
    Ident etc;
{   WordF cf;
    int stat;
	frdb_word(fp,wp);
	frdb_words_etc(fp,&(wp->ws),IsALL); /* rd alternatives entirely, if any */
	if(etc&IsChar && wp->cs.mny>0)
		frdb_chars_etc(fp,&(wp->cs),etc);
	else wp->cs = empty_Chars;
	if(etc&IsBlob && wp->bs.mny>0)
		frdb_blobs_etc(fp,&(wp->bs),etc);
	else wp->bs = empty_Blobs;
	if(ferror(fp)) return(-errno); else return(1);
	}

/* read words, and their parts */
frdb_words_etc(fp,wsp,etc)
    FILE *fp;
    Words *wsp;
    Ident etc;
{   int wi;
    register Word *wp,**wpp;
	if(wsp->mny<=0) {
		*wsp = empty_Words;
		return(1);
		};
	if((wpp=wsp->wpa=(Word **)malloc((wsp->mny+1)*sizeof(Word *)))==NULL)
		abort("can't alloc Words.wpa array");
	for(wi=0; wi<wsp->mny; wi++) {
		*(wpp++) = wp = alloc_word();
		frdb_word_etc(fp,wp,etc);
		};
	*wpp = NULL;
	if(ferror(fp)) return(-errno); else return(1);
	}

/* remove a word from a words set */
remove_word(wp,wsp)
    Word *wp;
    Words *wsp;
{   register Word *rp,**rpp,**npp;
	if(wsp->mny==0) err("can't remove Word - Words empty");
	else {	for(rp= *(rpp=wsp->wpa); rp!=NULL; rp= *(++rpp)) if(rp==wp) break;
		if(rp==NULL) err("can't remove Word - not found");
		else {	/* move later entries up */
			npp=rpp+1;
			do *(rpp++)= *(npp++); while ((*rpp)!=NULL);
			if((--(wsp->mny))==0) {free(wsp->wpa); wsp->wpa=NULL;};
			/* don't bother to realloc downwards */
			};
		};
	}

/* Remove a Word from a Txtln's Words set.
   Shrink the Txtln's Bbx if the Word abuts it on the left or right.
   Ensure that the first Word in the line has wsp==0.0
   BUG:  doesn't update wsp of trailing Word */
remove_word_txtln(wp,lp)
    Word *wp;
    Txtln *lp;
{   register Word *cwp,**wpp;
	remove_word(wp,&(lp->ws));
	if(lp->ws.mny>0) {
	   	if(wp->bx.a.x==lp->bx.a.x || wp->bx.b.x==lp->bx.b.x) {
			/* recompute possibly shrunken bx */
			lp->bx=empty_Bbx;
			for(cwp= *(wpp=lp->ws.wpa); cwp!=NULL; cwp= *(++wpp))
				merge_bbx(&(cwp->bx),&(lp->bx));
			};
		(lp->ws.wpa[0])->wsp=0.0;
		};
	}

/* Append a word to the end of a words set.
   Do NOT maintain set in order ascending on Word.bx.a.x.
   Return the appended Word. */
Word *append_word(wp,wsp)
    Word *wp;
    Words *wsp;
{	if(wsp->mny==0) {
		if((wsp->wpa=(Word **)malloc(2*sizeof(Word *)))==NULL)
			abort("append_word: can't malloc wsp->wpa");
		}
	else {	if((wsp->wpa=(Word **)realloc(
				wsp->wpa,
				(wsp->mny+2)*sizeof(Word *))
				)==NULL)
			abort("append_word: can't realloc wsp->wpa");
		};
	wsp->wpa[wsp->mny++] = wp;
	wsp->wpa[wsp->mny] = NULL;
	return(wp);
	}

/* Insert a Word into a Words set.  Words is assumed to be sorted ascending
   on Word.bx.a.x, and this order is maintained.
   Words with equal Word.bx.a.x are stored in the order that they were
   inserted:  so that the first one inserted remains the first among equals
   (this feature is important for certain WordSet applications).
   Return inserted (Word *) */
Word *insert_word(wp,wsp)
    Word *wp;
    Words *wsp;
{   register Word **wpp,*nwp,*pwp;
	if(wsp->mny==0) {
		if((wsp->wpa=(Word **)malloc(2*sizeof(Word *)))==NULL)
			abort("insert_word: can't malloc wsp->wpa");
		wsp->wpa[wsp->mny] = wp;
		}
	else {	if((wpp=wsp->wpa=(Word **)realloc(
				wsp->wpa,
				(wsp->mny+2)*sizeof(Word *))
				)==NULL)
			abort("insert_word: can't realloc wsp->wpa");
		while(((*wpp)!=NULL)&&(*wpp)->bx.a.x<=wp->bx.a.x) wpp++;
		/* **wpp is now 1st entry > *wp in sorted order; insert
		   new word just before it */
		pwp=wp;
		do {	nwp= *wpp;
			*(wpp++)=pwp;
			pwp=nwp;
			}
		while(pwp!=NULL);
		};
	wsp->wpa[++wsp->mny] = NULL;
	return(wp);
	}

/* Insert a Word into the Words owned by a given Txtln,
   maintaining the Txtln's Bbx and sorted order in the Words set.
   Return the inserted Word * */
Word *insert_word_txtln(wp,lp)
    Word *wp;
    Txtln *lp;
{	merge_bbx(&(wp->bx),&(lp->bx));
	return(insert_word(wp,&(lp->ws)));
	}

/* Free wordinterp and its parts as specified in etc.
   Always free its strings (if any).
   If etc&IsWordInterp, free the record itself also.
*/
free_wordinterp_etc(p,etc)
    WordInterp *p;
    Ident etc;
{	if(p->s!=NULL) { free(p->s);  p->s=NULL; };
	if(p->pp!=NULL) { free(p->pp);  p->pp=NULL; };
	if(p->by!=NULL) { free(p->by);  p->by=NULL; };
	if(p->po!=NULL) { free(p->po);  p->po=NULL; };
	if(p->ps!=NULL) { free(p->ps);  p->ps=NULL; };
	if(etc&IsWordInterp) free(p);
	}

/* Duplicate (return ptr to local static copy of) a WordInterp.
   Always free its strings (if any).
   'etc' is ignored.
*/
WordInterp *dup_wordinterp_etc(p,etc)
    WordInterp *p;
    Ident etc;
{   static WordInterp d;
	d = *p;
	if(p->s!=NULL) d.s = strdup(p->s);
	if(p->pp!=NULL) d.pp = strdup(p->pp);
	if(p->by!=NULL) d.by = strdup(p->by);
	if(p->po!=NULL) d.po = strdup(p->po);
	if(p->ps!=NULL) d.ps = strdup(p->ps);
	return(&d);
	}

/* Unconditionally free the malloc-space array of pointers, and empty the set.
   Don't free the records that it owned.
   */
free_words(wsp)
    Words *wsp;
{   	if(wsp->wpa!=NULL) { free(wsp->wpa); wsp->wpa = NULL; }
	wsp->mny = 0;
	}

free_words_etc(wsp,etc)
    Words *wsp;
    Ident etc;
{   register Word *wp,**wpp;
    int wi;
	if(wsp->mny>0&&(etc&IsWord))
		for(wp= *(wpp=wsp->wpa); wp!=NULL; wp= *(++wpp))
			free_word_etc(wp,etc);
   	free_words(wsp);
	}

/* return a pointer to a distinct and duplicate copy of *wp,
   created out of malloc space */
Word *dup_word(wp)
    Word *wp;
{   Word *dup;
	if((dup=(Word *)malloc(sizeof(Word)))==NULL)
		abort("can't dup Word");
	alloc_census(Word,1);
	*dup = *wp;
	return(dup);
	}

/* Return a pointer to a distinct and duplicate copy of *wp, and ALL it owns,
   created out of malloc space. (HOW they are duplicated is controlled by flags
   in etc.) */
Word *dup_word_etc(wp,etc)
    Word *wp;
    Ident etc;
{   Word *dup;
	dup = dup_word(wp);
	if(dup->ws.mny>0) dup->ws = *dup_words_etc(&(wp->ws),etc);
	else dup->ws = empty_Words;
	if(dup->cs.mny>0) dup->cs = *dup_chars_etc(&(wp->cs),etc);
	else dup->cs = empty_Chars;
	if(dup->bs.mny>0) dup->bs = *dup_blobs_etc(&(wp->bs),etc);
	else dup->bs = empty_Blobs;
	return(dup);
	}

/* Return a pointer to a distinct local static duplicate of non-empty *wsp.
   Its wpa array is created newly out of malloc space.
   If etc&IsWord, all its Words are also duplicated,
   else the contents of wpa point to the old unduplicated Words.
   */
Words *dup_words_etc(wsp,etc)
    Words *wsp;
    Ident etc;		/* parts to duplicate */
{   static Words dup;
    register Word **wpp,**dpp;
	if((dup.mny=wsp->mny)<=0) dup = empty_Words;
	else {	if((dup.wpa=(Word **)malloc((dup.mny+1)*sizeof(Word *)))==NULL)
			abort("can't dup ws.wpa");
		for(wpp=wsp->wpa,dpp=dup.wpa; *wpp!=NULL; wpp++,dpp++) {
			if(etc&IsWord) *dpp = dup_word_etc(*wpp,etc);
			else *dpp = *wpp;
			};
		*dpp = NULL;
		};
	return(&dup);
	}

WordInterp *alloc_wordinterp()
{    WordInterp *p;
	if((p=(WordInterp *)malloc(sizeof(WordInterp)))==NULL)
		abort("can't alloc WordInterp");
	else {	*p = empty_WordInterp;
		return(p);
		};
	}

char *wordinterp_toa(p)
    WordInterp *p;
{    static char s[80];
	sprintf(s,"%s %s %s|%s|%s|%s",
		ident_toa(p->ident),
		((p->s!=NULL)? p->s: ""),
		((p->pp!=NULL)? p->pp: ""),
		((p->by!=NULL)? p->by: ""),
		((p->po!=NULL)? p->po: ""),
		((p->ps!=NULL)? p->ps: "")
		);
	return(s);
	}

/* Free Word and its parts, as specified by etc.
   Always free the pointer-arrays of ws, cs & bs.
   If etc&IsWord, free all Words in ws also.
   If etc&IsChar, free all Chars in cs also.
   If etc&IsBlob, free all Blobs in bs also.
*/
free_word_etc(p,etc)
    Word *p;
    Ident etc;
#define dbg_fwe (F)
{	if(dbg_fwe) {
		err("free_word_etc: %s",word_toa(p));
		err("free_word_etc: %s",ident_toa(etc));
		err_census_all;
		};
	free_words_etc(&(p->ws),IsALL);
	free_chars_etc(&(p->cs),etc);
	free_blobs_etc(&(p->bs),etc);
	if(p->ident&Word_label && p->l!=NULL) {
		free(p->l);  p->l=NULL;  p->ident &= ~Word_label;
		};
	if(etc&IsWord) free_word(p);
	if(dbg_fwe) err_census_all;
	}

free_word(wp)
    Word *wp;
{	free(wp);
	free_census(Word,1);
	}

fwrb_sfeats(fp,sfv)
    FILE *fp;
    SFv sfv;
{   register Pval *vp,*vq;
	for(vq=(vp=sfv)+SF_MNY; vp<vq; vp++)
		fwri_int3(fp,USHRT_MAX*(*vp)+0.5);
#if dbg_fwrb_toa
	/** err("fwrb_sfeats: %s",sfeats_toa(sfv)); **/
#endif
	}

int frdb_sfeats(fp,sfv)
    FILE *fp;
    SFv sfv;
{   register Pval *vp,*vq;
	for(vq=(vp=sfv)+SF_MNY; vp<vq; vp++)
		*vp = frdi_int3(fp)/((Pval)USHRT_MAX);
#if dbg_frdb_toa
	/** err("frdb_sfeats: %s",sfeats_toa(sfv)); **/
#endif
	}

/* return ptr to local static copy of Shapes of given size */
Shapes *alloc_shapes(mny)
    int mny;
{   static Shapes shs;
	shs.mny = shs.alloc = mny;
	if((shs.sa=(Nb_s *)malloc(shs.alloc*sizeof(Nb_s)))==NULL)
		abort("can't alloc Shapes.sa[%d]",shs.alloc);
	return(&shs);
	}

fwrb_shapes(fp,p)
    FILE *fp;
    Shapes *p;	/* on entry, p->mny has already been written */
{   int dim;
    register Nb_s *sp,*sq;
    register Pval *vp;
	if(p->mny>0&&p->sa!=NULL) {
		for(sq=(sp=p->sa)+p->mny; sp<sq; sp++) {
			dim=Sh_dim[sp->t&(~Sh_tiny)];
			fwri_uint1(fp,(sp->t&0x7F)|((sp->t&Sh_tiny)?0x80:0x00));
			fwri_int2(fp,(*(vp=sp->p))*SHRT_MAX);
			if(dim>1) { fwri_int2(fp,(*(++vp))*SHRT_MAX);
			 if(dim>2) { fwri_int2(fp,(*(++vp))*SHRT_MAX);
			  if(dim>3) { fwri_int2(fp,(*(++vp))*SHRT_MAX);
				} } };
			};
		};
#if dbg_fwrb_toa
	/** err("fwrb_shapes: %s",shapes_toa(p)); **/
#endif
	}

int frdb_shapes(fp,p)
    FILE *fp;
    Shapes *p;	/* on entry, p->mny has already been read */
{   register Nb_s *sp,*sq;
    register Pval *vp;
    uint1 ui1;
    int dim;
    
	if(p->mny>0) {
		*p = *alloc_shapes(p->mny);
		for(sq=(sp=p->sa)+p->mny; sp<sq; sp++) {
			ui1 = frdi_uint1(fp);
			sp->t = ui1&0x7F|((ui1&0x80)?Sh_tiny:0);
			dim=Sh_dim[sp->t&(~Sh_tiny)];
			*(vp=sp->p) = ((float)frdi_int2(fp))/SHRT_MAX;
			if(dim>1) { *(++vp) = ((float)frdi_int2(fp))/SHRT_MAX;
			 if(dim>2) { *(++vp) = ((float)frdi_int2(fp))/SHRT_MAX;
			  if(dim>3) { *(++vp) = ((float)frdi_int2(fp))/SHRT_MAX;
				} } };
			};
		}
	else {	p->alloc = 0;
		if(p->sa!=NULL) free(p->sa);
		p->sa = NULL;
		};
#if dbg_frdb_toa
	/** err("frdb_shapes: %s",shapes_toa(p)); **/
#endif
	}

fwra_shapes_etc(fp,p,t)
    FILE *fp;
    Shapes *p;
    char *t;	/* string of shape types (empty => all) */
{   Nb_s *s;
    int i,j;
    char a[80],f[10];
	for(i=0,s=p->sa; i<p->mny; i++,s++) {
		if(t[0]=='\0'||strchr(t,Sh_nam[s->t&(~Sh_tiny)])!=NULL) {
			sprintf(a,"%c%c ",
				Sh_nam[s->t&(~Sh_tiny)],
				(s->t&Sh_tiny)?'\'':' '
				);
			for(j=0;j<Sh_dim[s->t&(~Sh_tiny)];j++) {
				sprintf(f," %0.3f",s->p[j]);
				strcat(a,f);
				};
			fprintf(fp,"%s\n",a);
			};
		};
	}

err_shapes(shp)
    Shapes *shp;
{   int si,di;
    Nb_s *sp;
	fprintf(stderr,"shapes %d:\n",shp->mny);
	for(si=0,sp=shp->sa; si<shp->mny; si++,sp++) {
		fprintf(stderr,"  %c",Sh_nam[sp->t]);
		for(di=0; di<Sh_dim[sp->t]; di++)
			fprintf(stderr," %6.3f",sp->p[di]);
		fprintf(stderr,"\n");
		};
	}

/* Unconditionally free the malloc-space array of pointers, and empty the set.
   Don't free the records that it owned.
   */
free_shapes(shp)
    Shapes *shp;
{	if(shp->sa!=NULL) {
		if(shp->alloc<=0) abort("can't free unalloc'ed shapes");
		else { free(shp->sa); shp->sa=NULL; };
		};
	shp->mny = shp->alloc = 0;
	}

/* functions for RanParms */

RanParms *alloc_ranparms()
{    RanParms *rp;
	if((rp=(RanParms *)malloc(sizeof(RanParms)))==NULL)
		abort("alloc_ranparms: can't alloc");
	*rp = empty_RanParms;
	return(rp);
	}

free_ranparms(ran)
    RanParms *ran;
{   	free(ran);
	}

RanParms *dup_ranparms(p)
    RanParms *p;
{   RanParms *dup;
	dup = alloc_ranparms();
	*dup = *p;
	return(dup);
	}

char *ranparms_toa(ran,select)
    RanParms *ran;
    char *select;	/* a string, subset of: "*rpabejkstxy" if "*" or "" ALL */
{   static char s[200];
    char p[20];
    char *cp;
    static char select_all[] = "abejkprstxy";
	s[0]='\0';
	if(select[0]=='*'||select[0]=='\0') select=select_all;
	for(cp=select; *cp!='\0'; cp++) {
	    switch(*cp) {
		case 'a':  sprintf(p,"a%0.3f ",ran->skew/DtoR);  strcat(s,p);  break;
		case 'b':  sprintf(p,"b%0.3f ",ran->bhgt);  strcat(s,p);  break;
		case 'e':  sprintf(p,"e%0.3f ",ran->blur);  strcat(s,p);  break;
		case 'j':  sprintf(p,"j%0.3f ",ran->jitter);  strcat(s,p);  break;
		case 'k':  sprintf(p,"k%0.3f ",ran->kern);  strcat(s,p);  break;
		case 'p':  sprintf(p,"p%g ",ran->size);  strcat(s,p);  break;
		case 'r':  sprintf(p,"r%d,%d ",ran->res_x,ran->res_y);  strcat(s,p);  break;
		case 's':  sprintf(p,"s%0.4f ",ran->speckle);  strcat(s,p);  break;
		case 't':  sprintf(p,"t%0.3f ",ran->thresh);  strcat(s,p);  break;
		case 'x':  sprintf(p,"x%0.3f ",ran->xscale);  strcat(s,p);  break;
		case 'y':  sprintf(p,"y%0.3f ",ran->yscale);  strcat(s,p);  break;
		};
	    };
	return(s);
	}

RanParms *ato_ranparms(s)
    char *s;
{   static RanParms ran;
    char *f,n,*v;
#define TOKEN_SEP "=, \n"
	f=strtok(s,TOKEN_SEP);
	while(f!=NULL&&strlen(f)>0) {
		n=f[0];  v=f+1;
		switch(n) {
		    case 'r':	/* res */
			ran.res_x = atoi(v);
			v=strtok((char *)0,TOKEN_SEP);
			ran.res_y = atoi(v);
			break;
		    case 'p':	ran.size = atof(v);  break;
		    case 'a':	ran.skew = atof(v)*DtoR;  break;
		    case 'b':	ran.bhgt = atof(v);  break;
		    case 'e':	ran.blur = atof(v);  break;
		    case 'j':	ran.jitter = atof(v);  break;
		    case 'k':	ran.kern = atof(v);  break;
		    case 's':	ran.speckle = atof(v);  break;
		    case 't':	ran.thresh = atof(v);  break;
		    case 'x':	ran.xscale = atof(v);  break;
		    case 'y':	ran.yscale = atof(v);  break;
		    };
		f=strtok((char *)0,TOKEN_SEP);
		};
	return(&ran);
	}

#if FWRI
fwrb_ranparms(f,p)
    FILE *f;
    RanParms *p;
{	fwri_int2(f,p->res_x);
	fwri_int2(f,p->res_y);
	fwri_Pts(f,p->size);
	fwri_Radians(f,p->skew);
	fwri_Ems(f,p->bhgt);
	fwri_int3(f,p->blur*10000.0);
	fwri_int3(f,p->jitter*10000.0);
	fwri_int3(f,p->kern*10000.0);
	fwri_int3(f,p->speckle*10000.0);
	fwri_int3(f,p->thresh*10000.0);
	fwri_int3(f,p->xscale*10000.0);
	fwri_int3(f,p->yscale*10000.0);
#if dbg_fwrb_toa
	err("fwrb_ranparms: %s",ranparms_toa(p));
#endif
	}
#else
fwrb_ranparms(f,p)
    FILE *f;
    RanParms *p;
{   	if((fwrite(p,sizeof(RanParms),1,f))!=1) return(0);
#if dbg_fwrb_toa
	err("fwrb_ranparms: %s",ranparms_toa(p));
#endif
	}
#endif

#if FRDI
int frdb_ranparms(f,p)
    FILE *f;
    RanParms *p;
{	*p = empty_RanParms;
	if(feof(f))
		return(0);
	p->res_x=frdi_int2(f);
	p->res_y=frdi_int2(f);
	p->size=frdi_Pts(f);
	p->skew=frdi_Radians(f);
	p->bhgt=frdi_Ems(f);
	p->blur=frdi_int3(f)/10000.0;
	p->jitter=frdi_int3(f)/10000.0;
	p->kern=frdi_int3(f)/10000.0;
	p->speckle=frdi_int3(f)/10000.0;
	p->thresh=frdi_int3(f)/10000.0;
	p->xscale=frdi_int3(f)/10000.0;
	p->yscale=frdi_int3(f)/10000.0;
#if dbg_frdb_toa
	err("frdb_ranparms: %s",ranparms_toa(p));
#endif
	if(ferror(f)) return(-errno); else return(1);
	}
#else
int frdb_ranparms(fp,ran)
    FILE *fp;
    RanParms *ran;
{   	if((fread(ran,sizeof(RanParms),1,fp))<1) return(0);
#if dbg_frdb_toa
	err("frdb_ranparms: %s",ranparms_toa(ran));
#endif
	}
#endif

Char *alloc_char()
{    Char *p;
	if((p=(Char *)malloc(sizeof(Char)))==NULL)
		abort("can't alloc Char");
	alloc_census(Char,1);
	*p = empty_Char;
	return(p);
	}

char *char_toa(cp)
    Char *cp;
{   static char s[100],bfs[24];
	if(cp->bfsp==NULL) strcpy(bfs,"0");
	else if(cp->bfsp->bm.n==0) strcpy(bfs,"0");
	else if(cp->bfsp->bm.mny==0) sprintf(bfs,"%d",cp->bfsp->bm.n);
	else sprintf(bfs,"%d,%d",cp->bfsp->bm.n,cp->bfsp->bm.mny);
	sprintf(s,"%s bx%s w%d,h%d cs%d bl%d ar%d pe%d sf%d sh%d bf%s i%d",
		ident_toa(cp->ident),
		bbx_toa(&(cp->bx)),
		bbx_wid(&cp->bx),bbx_hgt(&cp->bx),
		cp->csp,
		cp->bmny,
		cp->area,cp->per,
		(cp->sfv==NULL)? 0: 1,
		cp->sh.mny,
		bfs,
		cp->il.mny
		);
	return(s);
	}

/* compute the centroid of the Char (offset from bx.a) */
Pp *char_centroid(cp)
    Char *cp;
{   static Pp c;
    Pp *bc;	/* blob centroid */
    Blob *bp;
    int bi;
	c.x = c.y = 0.0;
	for(bi=0, bp=cp->fi; bi<cp->bmny; bi++, bp=bp->n) {
		bc = blob_centroid(bp);
		c.x += bp->area * (bc->x + (cp->bx.a.x - cp->bx.a.x));
		c.y += bp->area * (bc->y + (cp->bx.a.y - cp->bx.a.y));
		};
	c.x /= cp->area;
	c.y /= cp->area;
	return(&c);
	}

#if FWRI
fwrb_char(f,p)
    FILE *f;
    Char *p;
{
#if dbg_fwrb
	if((!(p->ident&IsChar))||(p->ident&(IsALL&(~IsChar))))
		err("fwrb_char: %s",char_toa(p));
	if(p->ident&Char_label && p->l==NULL) {
		err("fwrb_page: Char_label set but .l==NULL");
		p->ident &= ~Char_label;
		};
	if(p->ident&Char_ranparms && p->rp==NULL) {
		err("fwrb_page: Char_ranparms set but .rp==NULL");
		p->ident &= ~Char_ranparms;
		};
#endif
	fwri_Ident(f,p->ident);
	fwri_Bbx(f,&(p->bx));
	fwri_Scoor(f,p->csp);
	fwri_uint4(f,p->area);
	fwri_uint4(f,p->per);
	fwri_Scoor(f,p->basl);
	fwri_uint3(f,p->bmny);
	fwri_uint2(f,p->il.mny);
	fwri_uint1(f,(p->sfv!=NULL)?SF_MNY:0);
	fwri_uint2(f,p->sh.mny);
	fwri_uint2(f,(p->bfsp!=NULL)?p->bfsp->bm.n:0);
#if dbg_fwrb_toa
	err("fwrb_char: %s",char_toa(p));
#endif
	if(p->ident&Char_label) fwrb_label(f,p->l);
	if(p->ident&Char_ranparms) fwrb_ranparms(f,p->rp);
	}
#else
fwrb_char(fp,cp)
    FILE *fp;
    Char *cp;
{   CharF cf;
    int stat;
#if dbg_fwrb
	if((!(cp->ident&IsChar))||(cp->ident&(IsALL&(~IsChar))))
		err("fwrb_char: %s",char_toa(cp));
#endif
	memset(&cf,'\0',sizeof(cf));
	if(cp->ident&Char_ranparms && cp->rp==NULL)
		cp->ident &= ~Char_ranparms;
	cf.ident = cp->ident | IsChar;
	cf.bx = cp->bx;
	cf.csp = cp->csp;
	cf.area = cp->area;
	cf.per = cp->per;
	/* basl omitted */
	cf.bmny = cp->bmny;
	cf.imny = cp->il.mny;
	cf.sfmny = (cp->sfv!=NULL)?SF_MNY:0;
	cf.shmny = cp->sh.mny;
	if(cp->bfsp!=NULL) cf.bfmny = cp->bfsp->bm.n; else cf.bfmny=0;
	if((stat=fwrite(&cf,sizeof(CharF),1,fp))!=1)
		abort("can't write CharF, status %d",stat);
#if dbg_fwrb_toa
	err("fwrb_char: %s",char_toa(cp));
#endif
	if(cf.ident&Char_ranparms) fwrb_ranparms(fp,cp->rp);
	if(cf.ident&Char_label) fwrb_label(fp,cp->l);
	}
#endif

fwrb_chars_etc(fp,cs,etc)
    FILE *fp;
    Chars cs;
    Ident etc;
{   register Char *cp,**cpp;
	if(cs.mny>0) for(cp= *(cpp=cs.cpa); cp!=NULL; cp= *(++cpp))
		fwrb_char_etc(fp,cp,etc);
	}

fwrb_char_etc(fp,cp,etc)
    FILE *fp;
    Char *cp;
    Ident etc;
{   static Ident parts = (IsInterp|IsBlob|IsShapes|IsBfeats|IsSfeats);
    Char ch;
   	if((etc&parts)!=parts) /* write selected parts */ {
		ch = *cp;
		if(!(etc&IsInterp)) ch.il.mny=0;
		if(!(etc&IsBlob)) ch.bmny=0;
		if(!(etc&IsSfeats)) ch.sfv = NULL;
		if(!(etc&IsShapes)) ch.sh = empty_Shapes;
		if(!(etc&IsBfeats)) ch.bfsp = NULL;
		cp = &ch;	/* write modified record */
		};
	fwrb_char(fp,cp);
	if(cp->sfv!=NULL) fwrb_sfeats(fp,cp->sfv);
	if(cp->sh.mny>0) fwrb_shapes(fp,&(cp->sh));
#if CPU!=CRAY
	if(cp->bfsp!=NULL) fwrb_bfeats(fp,cp->bfsp);
#endif
	fwrb_interpl_etc(fp,cp->il,etc);
	if(cp->bmny>0) fwrb_blobl_etc(fp,cp->bmny,cp->fi,etc);
	}

#if FRDI
int frdb_char(f,p)
    FILE *f;
    Char *p;
{   int sf_mny,bf_mny;
	*p = empty_Char;
	if(feof(f))
		return(0);
	p->ident=frdi_Ident(f);
	frdi_Bbx(f,&(p->bx));
	p->csp=frdi_Scoor(f);
	p->area=frdi_uint4(f);
	p->per=frdi_uint4(f);
	p->basl=frdi_Scoor(f);
	p->bmny=frdi_uint3(f);
	p->il.mny=frdi_uint2(f);
	if((sf_mny=frdi_uint1(f))>0) {
		if((p->sfv=(Pval *)malloc(sf_mny*sizeof(Pval)))==NULL)
			abort("frdb_char: can't alloc p->sfv[%d]",sf_mny);
		memset(p->sfv,'\0',sf_mny*sizeof(Pval));
		};
	p->sh.mny=frdi_uint2(f);
	if((bf_mny=frdi_uint2(f))>0) {
		p->bfsp = alloc_bfeats(bf_mny,0);
		};
#if dbg_frdb
	if((!(p->ident&IsChar))||(p->ident&(IsALL&(~IsChar))))
		err("frdb_char: %s",char_toa(p));
#endif
#if dbg_frdb_toa
	err("frdb_char: %s",char_toa(p));
#endif
	if(p->ident&Char_label) p->l = frdb_label(f);
	if(p->ident&Char_ranparms) {
		p->rp = alloc_ranparms();
		frdb_ranparms(f,p->rp);
		};
	if(ferror(f)) return(-errno); else return(1);
	}
#else
int frdb_char(fp,cp)
    FILE *fp;
    Char *cp;
{   CharF cf;
    int stat;
	if((stat=fread(&cf,sizeof(CharF),1,fp))!=1)
		abort("frdb_char: can't read CharF, status %d",stat);
	*cp = empty_Char;
	cp->ident = cf.ident;
	cp->bx = cf.bx;
	cp->csp = cf.csp;
	cp->area = cf.area;
	cp->per = cf.per;
	cp->basl = 0;
	cp->bmny = cf.bmny;
	cp->il.mny = cf.imny;
	if(cf.sfmny>0) {
		if((cp->sfv=(Pval *)malloc(SF_MNY*sizeof(Pval)))==NULL)
			abort("frdb_char: can't alloc cp->sfv[%d]",SF_MNY);
		memset(cp->sfv,'\0',SF_MNY*sizeof(Pval));
		}
	else cp->sfv = NULL;
	cp->sh.mny = cf.shmny;
#if CPU!=CRAY
#if dbg_frdb
	if((!(cp->ident&IsChar))||(cp->ident&(IsALL&(~IsChar))))
		err("frdb_char: %s",char_toa(cp));
#endif
#endif
#if dbg_frdb_toa
	err("frdb_char: %s",char_toa(cp));
#endif
	if(cp->ident&Char_ranparms) {
		cp->rp = alloc_ranparms();
		frdb_ranparms(fp,cp->rp);
		};
	if(cp->ident&Char_label) cp->l = frdb_label(fp);
#if CPU!=CRAY
	if(cf.bfmny>0) cp->bfsp = alloc_bfeats(cf.bfmny,0);
	else 
#endif
	     cp->bfsp = NULL;
	if(ferror(fp)) return(-errno); else return(1);
	}
#endif

frdb_char_etc(fp,cp,etc)
    FILE *fp;
    Char *cp;
    Ident etc;
{	frdb_char(fp,cp);
	if(cp->sfv!=NULL) frdb_sfeats(fp,cp->sfv);
	if(cp->sh.mny>0) frdb_shapes(fp,&(cp->sh));
#if CPU!=CRAY
	if(cp->bfsp!=NULL) {
		frdb_bfeats(fp,cp->bfsp);
		};
#endif
	if(cp->il.mny>0) {
		frdb_interpl_etc(fp,&(cp->il),etc);
		/**if(!(etc&IsInterp)) free_interpl_etc(&(cp->il),IsALL);**/
		};
	if(etc&IsBlob && cp->bmny>0)
		frdb_blobl_etc(fp,cp->bmny,&(cp->fi),etc);
	else { cp->bmny = 0; cp->fi = NULL; };
	if(ferror(fp)) return(-errno); else return(1);
	}

/* read a set of chars, and parts */
frdb_chars_etc(fp,csp,etc)
    FILE *fp;
    Chars *csp;
    Ident etc;
{   int ci;
    register Char *cp,**cpp;
	if(csp->mny<=0) {
		*csp = empty_Chars;
		return(1);
		};
	if((cpp=csp->cpa=(Char **)malloc((csp->mny+1)*sizeof(Char *)))==NULL)
		abort("frdb_chars_etc:  can't alloc Chars.cpa array");
	for(ci=0; ci<csp->mny; ci++) {
		*(cpp++) = cp = alloc_char();
		frdb_char_etc(fp,cp,etc);
		};
	*cpp = NULL;
	if(ferror(fp)) return(-errno); else return(1);
	}

/* return a pointer to a distinct and duplicate copy of *cp;
   it is created out of malloc space.
   RanParms & label are treated as an integral part of the Char. */
Char *dup_char(cp)
    Char *cp;
{   Char *dup;
	if((dup=(Char *)malloc(sizeof(Char)))==NULL)
		abort("can't dup Char");
	alloc_census(Char,1);
	*dup = *cp;
	if(dup->ident&Char_label && dup->l!=NULL)
		dup->l = strdup(cp->l);
	else {dup->ident &= ~Char_label; dup->l = NULL;};
	if(dup->ident&Char_ranparms && dup->rp!=NULL)
		dup->rp = dup_ranparms(cp->rp);
	else {dup->ident &= ~Char_ranparms; dup->rp = NULL;};
	return(dup);
	}

/* return a pointer to a distinct and duplicate copy of *cp and those of
   its lists as specified by etc.  (Does not conform to practice for dup_word_etc.)
   */
Char *dup_char_etc(cp,etc)
    Char *cp;
    Ident etc;
{   Char *dup;
	dup = dup_char(cp);
	if(dup->bmny>0&&etc&IsBlob) dup->fi = dup_blobl_etc(cp->fi,etc);
	else {dup->bmny=0; dup->fi=NULL;};
	if(dup->il.mny>0&&etc&IsInterp) dup->il = *dup_interpl_etc(&(cp->il),etc);
	else dup->il = empty_Interpl;
	/**
	if(dup->sfv!=NULL>0&&etc&IsSfeats) dup->sfv = dup_sfeats(cp->sfv);
	else dup->sfv = NULL;
	if(dup->sh.mny>0&&etc&IsShapes) dup->sh = *dup_shapes(&cp->sh);
	else dup->sh = empty_Shapes;
	**/
#if CPU!=CRAY
	if(dup->bfsp!=NULL>0&&etc&IsBfeats) dup->bfsp = dup_bfeats(cp->bfsp);
	else 
#endif
	     dup->bfsp = NULL;
	return(dup);
	}

/* Return a pointer to a local static duplicate of non-empty *csp.
   Its cpa array is new, created out of malloc space.
   If etc&IsChar, all its Characters are also duplicated,
   else the contents of cpa point to the old unduplicated Chars.
   */
Chars *dup_chars_etc(csp,etc)
    Chars *csp;
    Ident etc;		/* parts to duplicate */
{   static Chars dup;
    register Char **cpp,**dpp;
	if((dup.mny=csp->mny)<=0) dup = empty_Chars;
	else {	if((dup.cpa=(Char **)malloc((dup.mny+1)*sizeof(Char *)))==NULL)
			abort("can't dup cs.cpa");
		for(cpp=csp->cpa,dpp=dup.cpa; (*cpp)!=NULL; cpp++,dpp++) {
			if(etc&IsChar) *dpp = dup_char_etc(*cpp,etc);
			else *dpp = *cpp;
			};
		*dpp = NULL;
		};
	return(&dup);
	}

err_chars(p)
    Chars *p;
{   register Char **cpp;
	fprintf(stderr,"Chars %d: ",p->mny);
	if(p->mny>0) for(cpp=p->cpa; (*cpp)!=NULL; cpp++)
		fprintf(stderr,"%X ",*cpp);
	fprintf(stderr,"\n");
	}

/* Make a Char out of a single Blob.  Return a freshly-allocated Char
   that owns the given Blob as is (not duplicated, copied or freed).
   The Blob's next pointer is set to NULL. */
Char *char_of_blob(b)
    Blob *b;
{   Char *c;
	c=alloc_char();
	c->bx = b->bx;
	c->area = b->area;
	c->per = b->per;
	c->bmny = 1;
	c->fi = b;
	b->n = NULL;
	return(c);
	}

Interp *alloc_interp()
{   Interp *new;
	if((new=(Interp *)malloc(sizeof(Interp)))==NULL)
		abort("alloc_interp: can't malloc");
	alloc_census(Interp,1);
	*new = empty_Interp;
	return(new);
	}

Interp *dup_interp(ip)
    Interp *ip;
{   Interp *dup;
	if((dup=(Interp *)malloc(sizeof(Interp)))==NULL)
		abort("dup_interp: can't malloc");
	alloc_census(Interp,1);
	*dup = *ip;
	return(dup);
	}

/* Print the interpl */
fwra_interpl(fp,ilp)
    FILE *fp;
    Interpl *ilp;
{   register Interp *cp;
	fprintf(fp,"Interpl[%d] ",ilp->mny);
	for(cp=ilp->fi; cp!=NULL; cp=cp->n) {
		fprintf(fp," %s --",interp_toa(cp));
		};
	fprintf(fp,"\n");
	}

/* Print the interpl very briefly */
fwra_interpl_brief(fp,ilp)
    FILE *fp;
    Interpl *ilp;
{   register Interp *ip;
	for(ip=ilp->fi; ip!=NULL; ip=ip->n) {
		fprintf(fp,"%s %s  ",ip->ci.c,merit_toa(ip->m));
		};
	fprintf(fp,"\n");
	}

err_interpl(ilp)
    Interpl *ilp;
{	fwra_interpl(stderr,ilp);
	}

/* return the address of a local static copy of the given Interpl,
   for which each Interp is a duplicate of those in the given list */
Interpl *dup_interpl_etc(ilp,etc)
    Interpl *ilp;
    Ident etc;
{   static Interpl lis;
    register Interp *cp,*pp,*dp;
	lis = *ilp;
	if(ilp->fi==NULL) return(NULL);
	for(pp=NULL,cp=ilp->fi; cp!=NULL; pp=dp,cp=cp->n) {
		dp=dup_interp(cp);
		if(pp==NULL) lis.fi = dp;
		else pp->n = dp;
		};
	pp->n=NULL;
	return(&lis);
	}

/* Return a pointer to a local static duplicate of *isp.
   Its pa array is new, created out of malloc space.
   If etc&IsInterp, all its Interps are also duplicated,
   else the contents of pa point to the old unduplicated Interps.
   */
Interps *dup_interps_etc(isp,etc)
    Interps *isp;
    Ident etc;		/* parts to duplicate */
{   static Interps dup;
    register Interp **ipp,**dpp;
	dup = *isp;
	if(dup.mny<=0) dup = empty_Interps;
	else {	if((dup.pa=(Interp **)malloc((dup.mny+1)*sizeof(Interp *)))==NULL)
			abort("can't dup (Interp *)is.pa[%d]",(dup.mny+1));
		for(ipp=isp->pa,dpp=dup.pa; *ipp!=NULL; ipp++,dpp++) {
			if(etc&IsInterp) *dpp = dup_interp(*ipp);
			else *dpp = *ipp;
			};
		*dpp = NULL;
		};
	return(&dup);
	}

free_interps_etc(isp,etc)
    Interps *isp;
    Ident etc;		/* parts to free */
{   register Interp **ipp,**dpp;
	if(etc&IsInterp) {
		/* free contents */
		for(ipp=isp->pa; *ipp!=NULL; ipp++) free_interp(*ipp);
		};
	if(isp->pa!=NULL) { free(isp->pa); isp->pa = NULL; };
	isp->mny = 0;
	}

/* remove a char from a chars set */
remove_char(cp,csp)
    Char *cp;
    Chars *csp;
{   register Char *rp,**rpp,**npp;
	if(csp->mny==0) err("remove_char: can't - Chars empty");
	else {	for(rp= *(rpp=csp->cpa); rp!=NULL; rp= *(++rpp)) if(rp==cp) break;
		if(rp==NULL) err("remove_char: can't - not found");
		else {	/* move later entries up */
			npp=rpp+1;
			do *(rpp++)= *(npp++); while ((*rpp)!=NULL);
			if((--(csp->mny))==0) {free(csp->cpa); csp->cpa=NULL;};
			/* don't bother to realloc downwards */
			};
		};
	}

/* Append a char to the end of a chars set. 
   Do NOT attempt to maintain Chars in order sorted ascending on Char.bx.a.x.
   Return appended Char *. */
Char *append_char(cp,csp)
    Char *cp;
    Chars *csp;
{   register Char *rp,**rpp,**npp;
	if(csp->mny==0) {
		if((csp->cpa=(Char **)malloc(2*sizeof(Char *)))==NULL)
			abort("append_char: can't malloc csp->cpa[%d]",2);
		}
	else {	if((csp->cpa=(Char **)realloc(
				csp->cpa,
				(csp->mny+2)*sizeof(Char *))
				)==NULL)
			abort("append_char: can't realloc csp->cpa[%d]",
				(csp->mny+2));
		};
	csp->cpa[csp->mny] = cp;
	csp->cpa[++csp->mny] = NULL;
	return(cp);
	}

/* Insert a Char into a Chars set.  Chars is assumed to be sorted ascending
   on on Char.bx.a.x, and this order is maintained.
   Return inserted Char * */
Char *insert_char(cp,csp)
    Char *cp;
    Chars *csp;
{   register Char **cpp,*ncp,*pcp;
	if(csp->mny==0) {
		if((csp->cpa=(Char **)malloc(2*sizeof(Char *)))==NULL)
			abort("insert_char: can't malloc csp->cpa[%d]",2);
		csp->cpa[csp->mny] = cp;
		}
	else {	if((cpp=csp->cpa=(Char **)realloc(
				csp->cpa,
				(csp->mny+2)*sizeof(Char *))
				)==NULL)
			abort("insert_char: can't realloc csp->cpa[%d]",csp->mny+2);
		while(((*cpp)!=NULL)&&(*cpp)->bx.a.x<cp->bx.a.x) cpp++;
		/* **cpp is now 1st entry >= *cp in sorted order */
		pcp=cp;
		do {	ncp= *cpp;
			*(cpp++)=pcp;
			pcp=ncp;
			}
		while(pcp!=NULL);
		};
	csp->cpa[++csp->mny] = NULL;
	return(cp);
	}

/* Append the contents of a "source" chars set to a "destination" chars set.
   Merely copy (Char *) pointers:  don't duplicate Chars.
   Do NOT attempt to maintain Chars in order sorted ascending on Char.bx.a.x.
   Return destination (Chars *). */
Chars *append_chars(s,d)
    Chars *s;	/* source */
    Chars *d;	/* destination */
{   register Char *rp,**rpp,**npp;
	if(s->mny==0) return(d);
	if(d->mny==0) {
		if((d->cpa=(Char **)malloc((s->mny+1)*sizeof(Char *)))==NULL)
			abort("append_chars: can't malloc d->cpa[%d]",2);
		}
	else {	if((d->cpa=(Char **)realloc(
				d->cpa,
				(d->mny+s->mny+1)*sizeof(Char *))
				)==NULL)
			abort("append_chars: can't realloc d->cpa[%d]",
				(d->mny+s->mny+1));
		};
	memcpy(d->cpa+d->mny,s->cpa,(s->mny+1)*sizeof(Char *));
	d->mny += s->mny;
	return(d);
	}

/* Insert a Char into the Chars owned by a given Word,
   maintaining order in set, and updating the Word's bx */
Char *insert_char_word(cp,wp)
    Char *cp;
    Word *wp;
{	merge_bbx(&(cp->bx),&(wp->bx));
	return(insert_char(cp,&(wp->cs)));
	}

free_char_etc(p,etc)
    Char *p;	/* !=NULL */
    Ident etc;
{	if(etc&IsBlob) free_blobl_etc(&(p->bmny),&(p->fi),etc);
	if(etc&IsInterp) free_interpl(&(p->il));
	if(etc&IsSfeats && p->sfv!=NULL) { free(p->sfv); p->sfv=NULL; };
	if(etc&IsShapes) free_shapes(&(p->sh));
#if CPU!=CRAY
	if(etc&IsBfeats && p->bfsp!=NULL) { free_bfeats(p->bfsp); p->bfsp=NULL; };
#endif
	if(etc&IsChar) {
		if(p->ident&Char_label && p->l!=NULL) {
			free(p->l);  p->l=NULL;
			p->ident &= ~Char_label;
			};
		if(p->ident&Char_ranparms && p->rp!=NULL) {
			free_ranparms(p->rp);  p->rp=NULL;
			p->ident &= ~Char_ranparms;
			};
		free(p);
		free_census(Char,1);
		};
	}

/* Unconditionally free the malloc-space array of pointers, and empty the set.
   Don't free the records that it owned.
   */
free_chars(csp)
    Chars *csp;
{   	if(csp->cpa!=NULL) { free(csp->cpa); csp->cpa = NULL; }
	csp->mny = 0;
	}

free_chars_etc(csp,etc)
    Chars *csp;
    Ident etc;
{   register Char *cp,**cpp;
	if(csp->mny>0&&(etc&IsChar))
		for(cp= *(cpp=csp->cpa); cp!=NULL; cp= *(++cpp))
			free_char_etc(cp,etc);
	free_chars(csp);
	}

/** Blob handling:
	alloc_blob_pool(sz,debug) - create pool of free blob records
	free_blob_pool() - free the pool
Blob	*alloc_blob() - allocate a new blob (in pool)
	free_blob(bp) -  free a specified blob
	free_blob_runs(bp) - free a blob & the runs in its blob set
	free_blobl_etc(int *,Blob **,etc) - free blobs etc in char's blob list
	out_blob(bp) - print (ascii) to stdout
	fwrb_blob(fp,bp) - write binary Blob (only) to fp
        fwrb_blob_etc(fp,bp,etc) - write binary Blob & specified parts to fp
        frdb_blob_etc(fp,bp,etc) - read binary Blob & parts in specified form
	frdb_runfs(fp,bp,max) - read *bp's (binary) RunF's from fp
	err_blob(s,bp) - print (ascii) to stderr
	err_blob_runs(s,bp) - print Blob & its Runs (ascii) to stderr
	err_blob_runfs(s,bp) - print Blob & its RunFs (ascii) to stderr
	err_blob_briefly(s,bp) - print (ascii) to stderr
	err_blobf(s,bp) - print file-format blob (ascii) to stderr
	err_blob_stats() - report blob statistics
	blob_small(bp) - test whether its runs' data can be compressed to chars
 BUGS
     most fread/fwrite/fseek should be read/write/lseek, for speed
     -- but how to choose?
 **/

/* make (empty) pool of `size' Blobs (return F if can't allocate) */
boolean alloc_blob_pool(size,dbg)
    int size;
    boolean dbg;
{   register Blob *cbp, *cbq, *pbp;
	blob_max=size;
	if((blob_pool=(Blob *)malloc(blob_max*sizeof(Blob)))==NULL) return(F);
	blob_debug = dbg;
	pbp= &blob_fr;
	/* link up all blobs into free list using only first-ptrs */
	for(cbq=(cbp=blob_pool)+blob_max; cbp<cbq; cbp++) {
		pbp->n = cbp;
		pbp = cbp;
		};
	pbp->n = NULL;	/* marks end of free list */
	blob_fr_mny = blob_max;  blob_hi=0;  blob_chopped = 0;
	hi_blob_no = 0;
	return(T);
	}

free_blob_pool()
{	free(blob_pool);
	}

Blob *alloc_blob()
{    Blob *p;
	if((p=(Blob *)malloc(sizeof(Blob)))==NULL)
		abort("alloc_blob: can't");
	alloc_census(Blob,1);
	*p = empty_Blob;
	return(p);
	}

/* Unconditionally free this Blob record. */
free_blob(bp)
    Blob *bp;
{	free(bp);
	free_census(Blob,1);
	}

/* Allocate new Blob record and assign to it the next blob no */
Blob *alloc_pool_blob()
{	register Blob *bp;
	if((bp = blob_fr.n)==NULL) {
		err("too many alloc_blob() calls - aborting:");
#ifdef STATS
		err_blob_stats();
#endif
		exit(1);
		};
	blob_fr.n = bp->n;
#ifdef STATS
	blob_fr_mny--;
	if(blob_hi<(blob_max-blob_fr_mny)) blob_hi = blob_max-blob_fr_mny;
#endif
	bp->no = hi_blob_no++;
	return(bp);
	}

free_pool_blob(bp)
	Blob *bp;
{
	if(blob_debug&&bp==NULL) abort("free_pool_blob: can't free a NULL Blob");
#ifdef STATS
	blob_fr_mny++;
	if(blob_debug&&(blob_fr_mny>blob_max)) {
		err("free_pool_blob: too many free_blob() calls - aborting:");
#ifdef STATS
		err_blob_stats();
#endif
		exit(1);
		};
#endif
	bp->n = blob_fr.n;
	blob_fr.n = bp;
	}

/* free the runs belonging to this blob */
free_runs(bp)
    Blob *bp;
{   register Run *crp,*nrp;
	if(bp->ident&Runs_ff) {
		if(bp->r.ff!=NULL) free(bp->r.ff);
		bp->r.ff=NULL;
		}
	else if(bp->ident&Runs_f) {
		for(crp=bp->r.f; crp!=NULL; crp=nrp) {
			nrp=crp->n;
			free_run(crp);
			};
		}
	else if(bp->ident&Runs_seek) /* none to free */ ;
	}

/* free blob *bp and those parts (runs) specified */
free_blob_etc(bp,etc)
    Blob *bp;
    Ident etc;
{	if(etc&IsRun) free_runs(bp);
	if(etc&IsBlob) free_blob(bp);
	}

/* return a pointer to a distinct and duplicate copy of *bp;
   it is created out of malloc space */
Blob *dup_blob(bp)
    Blob *bp;
{   Blob *dup;
	if((dup=(Blob *)malloc(sizeof(Blob)))==NULL)
		abort("dup_blob: can't malloc");
	alloc_census(Blob,1);
	*dup = *bp;
	return(dup);
	}

/* return a pointer to a distinct and duplicate copy of *bp and what it owns,
   as described by etc; created out of malloc space */
Blob *dup_blob_etc(bp,etc)
    Blob *bp;
    Ident etc;
{   Blob *dup;
	dup = dup_blob(bp);
	if(bp->runs>0) {
		if(bp->ident&Runs_ff) {
		    int ri;
		    RunF *rp,*drp;
			if((dup->r.ff=
			    (RunF *)malloc(bp->runs*sizeof(RunF)))==NULL)
				abort("dup_blob_etc: can't malloc RunF[%d]",
					bp->runs);
			for(ri=0,rp=bp->r.ff,drp=dup->r.ff; ri<bp->runs; ri++)
				*(drp++) = *(rp++);
			}
		else abort("dup_blob_etc: only Runs_ff implemented");
		};
	return(dup);
	}

/* Return a pointer to a local static duplicate of non-empty *bsp.
   Its bpa array is created newly out of malloc space.
   If etc&IsBlob, all its Blobs are also fresh duplicates,
   else the contents of bpa point to the old unduplicated Blobs.
   */
Blobs *dup_blobs_etc(bsp,etc)
    Blobs *bsp;
    Ident etc;		/* parts to duplicate */
{   static Blobs dup;
    register Blob **bpp,**dpp;
	if((dup.mny = bsp->mny)<=0) dup = empty_Blobs;
	else {	if((dup.bpa=(Blob **)malloc((dup.mny+1)*sizeof(Blob *)))==NULL)
			abort("dup_blobs_etc: can't malloc bs.bpa[%d]",dup.mny+1);
		for(bpp=bsp->bpa,dpp=dup.bpa; *bpp!=NULL; bpp++,dpp++) {
			if(etc&IsBlob) *dpp = dup_blob_etc(*bpp,etc);
			else *dpp = *bpp;
			};
		*dpp = NULL;
		};
	return(&dup);
	}

/* Split the given Blobs into two, *b0 & *b1, according to the value 0 or !0
   respectively returned by the function s(b,a) applied to each Blob *b and
   blind argument *a.
   s() will be evaluated exactly once for each Blob, and the Blob's will be
   assigned in the order found in the original Blobs.  Only pointers to Blob's
   are created;  the Blob's themselves are not duplicated.  The original Blobs
   record is unchanged.
   */
split_blobs(b,s,a,b0,b1)
    Blobs *b;
    int (*s)();
    VOID *a;		/* passed transparently to s() */
    Blobs *b0,*b1;
{   char *sel,*sp;
    register Blob *bp,**bpp;
    Blob **b0pp;
    Blob **b1pp;
  	*b0 = *b1 = empty_Blobs;
	if(b->mny<=0) return;
	if((sel=(char *)malloc(b->mny))==NULL)
		abort("split_blobs: can't malloc sel[%d]",b->mny);
	for(bp= *(bpp=b->bpa),sp=sel; bp!=NULL; bp= *(++bpp),sp++)
		if(*sp = s(bp,a)) b1->mny++; else b0->mny++;
	if(b0->mny>0)
		if((b0->bpa=(Blob **)malloc((b0->mny+1)*sizeof(Blob *)))==NULL)
			abort("split_blobs: can't malloc b0->bpa[%d]",b0->mny+1);
	if(b1->mny>0)
		if((b1->bpa=(Blob **)malloc((b1->mny+1)*sizeof(Blob *)))==NULL)
			abort("split_blobs: can't malloc b1->bpa[%d]",b1->mny+1);
	b0pp=b0->bpa;
	b1pp=b1->bpa;
	for(bp= *(bpp=b->bpa),sp=sel; bp!=NULL; bp= *(++bpp),sp++)
		if(*sp) *(b1pp++)=bp; else *(b0pp++)=bp;
	if(b0->mny>0) *b0pp=NULL;
	if(b1->mny>0) *b1pp=NULL;
	free(sel);
	}

/* Return the address of the first in a list of Blobs, duplicated from
   the given list. */
Blob *dup_blobl_etc(bp,etc)
    Blob *bp;
    Ident etc;
{   Blob *fi,*cp,*pp,*dp;
	if(bp==NULL) return(NULL);
	for(pp=NULL,cp=bp; cp!=NULL; pp=dp,cp=cp->n) {
		dp=dup_blob_etc(cp,etc);
		if(pp==NULL) fi = dp;
		else pp->n = dp;
		};
	pp->n=NULL;
	return(fi);
	}

/* Prepend this blob to the given Char's bloblist; update Char's bx, area, & per.
   Inverse of `remove_blobl()'. */
Blob *insert_blobl(bp,cp)
    Blob *bp;
    Char *cp;
{   register Blob *pbp,*nbp;
	if(cp->bmny>0) bp->n = cp->fi;
	else bp->n = NULL;
	cp->fi = bp;
	cp->bmny++;
	merge_bbx(&(bp->bx),&(cp->bx));
	cp->area += bp->area;
	cp->per += bp->per;
	return(bp);
	}

/* Remove this blob from a char's bloblist; update Char's bx, area, per.
   Inverse of `insert_blobl()'. */
remove_blobl(bp,cp)
    Blob *bp;
    Char *cp;
{   Blob *rp,*pp;
    Bbx bx;		/* Char's new Bbx */
	if(cp->bmny==1) {  /* frequent case */
		cp->fi=NULL; cp->bmny=0;
		}
	else if(cp->bmny>1){
		bx=empty_Bbx;
		pp=NULL;
		for(rp=cp->fi; rp!=NULL; pp=rp,rp=rp->n) {
			if(rp==bp) break;
			else {	if(bx.a.x > rp->bx.a.x) bx.a.x = rp->bx.a.x;
				if(bx.a.y > rp->bx.a.y) bx.a.y = rp->bx.a.y;
				if(bx.b.x < rp->bx.b.x) bx.b.x = rp->bx.b.x;
				if(bx.b.y < rp->bx.b.y) bx.b.y = rp->bx.b.y;
				};
			};
		if(rp!=NULL) {	/* remove from list */
			if(pp==NULL) cp->fi=bp->n;
			else pp->n=bp->n;
			cp->bmny--;
			for(rp=bp->n; rp!=NULL; rp=rp->n) {
				if(bx.a.x > rp->bx.a.x) bx.a.x = rp->bx.a.x;
				if(bx.a.y > rp->bx.a.y) bx.a.y = rp->bx.a.y;
				if(bx.b.x < rp->bx.b.x) bx.b.x = rp->bx.b.x;
				if(bx.b.y < rp->bx.b.y) bx.b.y = rp->bx.b.y;
				};
			cp->bx = bx;
			}
		else err("remove_blobl: can't - not found");
		}
	else err("remove_blobl: can't - Blobl empty");
	cp->area -= bp->area;
	cp->per -= bp->per;
	}

/* prepend this blob to the given Blob list */
Blob *prepend_blobl(bp,blp)
    Blob *bp;
    Blobl *blp;
{   register Blob *pbp,*nbp;
	if(blp->mny>0) bp->n = blp->fi;
	else {	bp->n = NULL;
		blp->la = bp;
		};
	blp->fi = bp;
	blp->mny++;
	return(bp);
	}

/* append this blob to the given Blob list */
Blob *append_blobl(bp,blp)
    Blob *bp;
    Blobl *blp;
{   register Blob *pbp,*nbp;
	if(blp->mny==0) blp->fi = bp;
	else blp->la->n = bp;
	blp->la = bp;
	bp->n = NULL;
	blp->mny++;
	return(bp);
	}

/* remove a blob from a blobs set */
remove_blob(bp,bsp)
    Blob *bp;
    Blobs *bsp;
{   register Blob *rp,**rpp,**npp;
	if(bsp->mny==0) err("remove_blob: can't - Blobs empty");
	else {	for(rp= *(rpp=bsp->bpa); rp!=NULL; rp= *(++rpp)) if(rp==bp) break;
		if(rp==NULL) err("remove_blob: can't - not found");
		else {	/* move later entries up */
			npp=rpp+1;
			do *(rpp++)= *(npp++); while ((*rpp)!=NULL);
			if((--(bsp->mny))==0) {free(bsp->bpa); bsp->bpa=NULL;};
			/* don't bother to realloc downwards */
			};
		};
	}

/* Append a blob to the end of a blobs set. 
   Return appended Blob *. */
Blob *append_blob(p,sp)
    Blob *p;
    Blobs *sp;
{   register Blob *rp,**rpp,**npp;
	if(sp->mny==0) {
		if((sp->bpa=(Blob **)malloc(2*sizeof(Blob *)))==NULL)
			abort("append_blob: can't malloc sp->bpa[2]");
		}
	else {	if((sp->bpa=(Blob **)realloc(
				sp->bpa,
				(sp->mny+2)*sizeof(Blob *))
				)==NULL)
			abort("append_blob: can't realloc sp->bpa[%d]",sp->mny+2);
		};
	sp->bpa[sp->mny] = p;
	sp->bpa[++sp->mny] = NULL;
	return(p);
	}

/* Append Blobs *p1 to Blobs *p2.  On return, *p1 is unchanged, *p2 is in general
   longer, and all the Blob's owned by *p1 are now also owned by *p2. */
append_blobs_blobs(p1,p2)
    Blobs *p1,*p2;
{   int mny;
    register Blob **pp1,**pp2;
	if(p1->mny==0) return;
	if(p2->mny==0) { *p2 = *dup_blobs_etc(p1,IsNONE); return; };
	mny = p2->mny + p1->mny;
	if((p2->bpa=(Blob **)realloc(p2->bpa,(mny+1)*sizeof(Blob *)))==NULL)
		abort("append_blobs_blobs: can't realloc p2->bpa[%d]",mny+1);
	pp1=p1->bpa;  pp2=p2->bpa+p2->mny;  while(*pp1!=NULL) *(pp2++) = *(pp1++);
	*pp2 = NULL;
	p2->mny = mny;
	}

free_blobl_etc(mnyp,fip,etc)
    int *mnyp;
    Blob **fip;
    Ident etc;
{   register Blob *bp,*nbp;
	if(*mnyp>0) {
		for(bp= *fip; bp != NULL; bp=nbp)
			{ nbp=bp->n; free_blob_etc(bp,etc); };
		};
	*mnyp=0;
	*fip=NULL;
	}

/* Unconditionally free the malloc-space array of pointers, and empty the set.
   Don't free the records that it owned.
   */
free_blobs(bsp)
    Blobs *bsp;
{   	if(bsp->bpa!=NULL) { free(bsp->bpa); bsp->bpa=NULL; };
	bsp->mny = 0;
	}

free_blobs_etc(bsp,etc)
   Blobs *bsp;
   Ident etc;
{  register Blob *bp, **bpp;
	if(bsp->mny>0&&(etc&IsBlob)) for(bp= *(bpp=bsp->bpa); bp!=NULL; bp= *(++bpp))
		free_blob_etc(bp,etc);
	free_blobs(bsp);
	}

Blobs *blobl_to_blobs(blp)
    Blobl *blp;
{   static Blobs bs;
    register Blob *bp,**bpp;
	bs = empty_Blobs;
	bs.mny = blp->mny;
	if(bs.mny>0) {
		if((bs.bpa=(Blob **)malloc((bs.mny+1)*sizeof(Blob *)))==NULL)
			abort("blobl_to_blobs: can't malloc Blobs.bpa[%d]",bs.mny+1);
		for(bp=blp->fi,bpp=bs.bpa; bp!=NULL; bp=bp->n,bpp++)
			*bpp = bp;
		*bpp = NULL;
		};
	return(&bs);
	}

int bp_tod(bp)
    Blob *bp;
{	if(bp==NULL) return(-2);
	else return(bp-blob_pool);
	}

/* test whether for this Blob, each runs's data fields could all be compressed
   from a short to a char */
boolean blob_small(bp)
    Blob *bp;
{	if(bp->runs>255) return(F);
	if(bbx_wid(&bp->bx)>255) return(F);
	if(bbx_hgt(&bp->bx)>255) return(F);
	return(T);
	}

char *blob_toa(bp)
    Blob *bp;
{   char s1[80];
    static char s[80];
    Scoor hgt,wid;
	if(bp==NULL) strcpy(s,"NULL");
	else {	hgt = bp->bx.b.y - bp->bx.a.y + 1;
		wid = bp->bx.b.x - bp->bx.a.x + 1;
		sprintf(s1,"%s bx%s w%d,h%d ar%d pe%d r.",
			ident_toa(bp->ident),
			bbx_toa(&(bp->bx)),
			wid,
			hgt,
			bp->area,
			bp->per
			);
		if(bp->ident&Runs_f) strcat(s1,"f");
		else if(bp->ident&Runs_ff) strcat(s1,"ff");
		else if(bp->ident&Runs_seek) strcat(s1,"sk");
		else if(bp->ident&Runs_g4) strcat(s1,"g");
		else strcat(s1,"?");
		sprintf(s,"%s%d",s1,bp->runs);
		};
	return(s);
	}

/* compute the centroid of the Blob, w.r.t bx.a */
Pp *blob_centroid(bp)
    Blob *bp;
{   static Pp c;
    RunF *rfp;
    int ri,area;
	c.x = c.y = 0.0;
	if(bp->ident&Runs_ff) {
		for(ri=0, rfp=bp->r.ff; ri<bp->runs; ri++, rfp++) {
			c.x += (area=(rfp->xe-rfp->xs+1)) * (rfp->xe+rfp->xs);
			c.y += area * rfp->y;
			};
		c.x /= 2.0*bp->area;
		c.y /= bp->area;
		}
	else abort("blob_centroid: only Runs_ff supported");
	return(&c);
	}

/* write (printably) to stdout */
out_blob(bp)
    Blob *bp;
{   Run *crp;
    static char pad[] = "     ";
    short y;
	if(bp==NULL) fprintf(stderr,"Bb NULL.\n");
	else fprintf(stdout,"%s\n",blob_toa(bp));
	fprintf(stdout,"%sy%d:",pad,bp->bx.a.y);
	for(crp=bp->r.f,y=crp->y; crp!=NULL; crp=crp->n) {
		if(crp->y!=y) {
			fprintf(stdout,"\n%sy%d:",pad,crp->y);
			y=crp->y;
			};
		fprintf(stdout," [%d,%d]",crp->xs,crp->xe);
		};
	fprintf(stdout,"\n");
	}

#if FWRI
/* Write blob record (only) to FILE *f.
   Check whether Blob is ``small'' and label *p accordingly. */
fwrb_blob(f,p)
    FILE *f;
    Blob *p;
{
#if dbg_fwrb
	if((!(p->ident&IsBlob))||(p->ident&(IsALL&(~IsBlob))))
		err("fwrb_blob: %s",blob_toa(p));
#endif
	if(blob_small(p)) p->ident |= Blob_small; else p->ident &= ~Blob_small;
	fwri_Ident(f,p->ident);
	/* fwri_Seq(f,p->no); */
	fwri_Bbx(f,&(p->bx));
	fwri_uint4(f,p->area);
	fwri_uint4(f,p->per);
	/* don't write .n */
	fwri_Merit(f,p->m);
	fwri_uint3(f,p->runs);
	fwri_uint2(f,((p->bdsp==NULL)? 0: p->bdsp->mny));
#if dbg_fwrb_toa
	err("fwrb_blob: %s",blob_toa(p));
#endif
	}
#else
/* Write blob record (only) to FILE *fp.
   Check whether Blob is ``small'' and label *bp accordingly. */
fwrb_blob(fp,bp)
    FILE *fp;
    Blob *bp;
{   BlobF bf;
#if dbg_fwrb
	if((!(bp->ident&IsBlob))||(bp->ident&(IsALL&(~IsBlob))))
		err("fwrb_blob: %s",blob_toa(bp));
#endif
	memset(&bf,'\0',sizeof(bf));
	if(blob_small(bp)) bp->ident |= Blob_small;
	else bp->ident &= ~Blob_small;
	bf.ident = bp->ident;
	bf.bx=bp->bx;
	bf.area=bp->area;
	bf.per=bp->per;
	bf.runs=bp->runs;
	if(bp->bdsp==NULL) bf.bdys=0;
	else bf.bdys=bp->bdsp->mny;
	if(blob_debug) err_blobf("",&bf);
	if(fwrite(&bf,sizeof(BlobF),1,fp)!=1)
		abort("fwrb_blob: can't fwrite");
#if dbg_fwrb_toa
	err("fwrb_blob: %s",blob_toa(bp));
#endif
	}
#endif

/* Write detailed description of Runs to stderr. */
Blob *err_runs(name,bp)
    char *name;
    Blob *bp;
{   Run *rp;
    RunF *rfp,*rfe;
    int ri;
	if(bp->ident&Runs_f) {
		fprintf(stderr,"%s Runs_f[%d] in %s:\n",
			name,bp->runs,bbx_toa(&bp->bx));
		for(rp=bp->r.f,ri=0; rp!=NULL; rp=rp->n,ri++) {	
			fprintf(stderr,"  %2d: %d[%d,%d]\n",
				ri,rp->y,rp->xs,rp->xe);
			};
		}
	else if(bp->ident&Runs_ff) {
		fprintf(stderr,"%s Runs_ff[%d] in %s: ",
			name,bp->runs,bbx_toa(&(bp->bx)));
		for(rfe=(rfp=bp->r.ff)+bp->runs,ri=0; rfp<rfe; rfp++,ri++) {	
			fprintf(stderr," %d:%d[%d,%d]",
				ri,rfp->y,rfp->xs,rfp->xe);
			};
		fprintf(stderr,"\n");
		}
	else if(bp->ident&Runs_seek) {
		goto unsupported;
		}
	else {	goto unsupported;
		};
	return;
unsupported:
	abort("err_runs: %s unsupported",ident_toa(bp->ident));
	}

/* Convert the type of Runs in *bp to the type specified by id.
   Supported:  only bp->ident&Runs_f to id&Runs_ff.
   Return pointer to local static duplicate of *bp.
   */
Blob *runs_to_runs(bp,id)
    Blob *bp;
    Ident id;
{   static Blob b;
    register Run *rp;
    RunF *rfp;

	b = *bp;
	if(bp->ident&Runs_f) {
		if(id&Runs_ff) {
			b.ident &= ~(Runs_f|Blob_small);
			b.ident |= Runs_ff;
			if(blob_small(bp)) b.ident |= Blob_small;
			if((b.r.ff=(RunF *)malloc(b.runs*sizeof(RunF)))==NULL)
				abort("runs_to_runs: can't malloc Blob.r.ff[%d]",
						b.runs);							for(rp=bp->r.f,rfp=b.r.ff; rp!=NULL; rp=rp->n,rfp++) {	
				/* y, xs, xe will be relative to Blob.bx.a */
				rfp->y = rp->y - bp->bx.a.y;
				rfp->xs = rp->xs - bp->bx.a.x;
				rfp->xe = rp->xe - bp->bx.a.x;
				/* above,below connecting indices are relative to
				   the sequence no. of this run (ac>0 & bc>0) */
				if((rfp->ad = rp->ad)!=0)
					rfp->ac = rp->u.no - rp->ac->u.no;
				else rfp->ac = 0;
				if((rfp->bd = rp->bd)!=0)
					rfp->bc = rp->bc->u.no - rp->u.no;
				else rfp->bc = 0;
				if(0) err_runf("   ",rfp);
				};
			}
		else {	goto unsupported;
			};
		}
	else if(bp->ident&Runs_ff) {
		if(bp->ident&Blob_small) {
			goto unsupported;
			}
		else {	goto unsupported;
			};
		}
	else if(bp->ident&Runs_seek) {
		goto unsupported;
		}
	else {	goto unsupported;
		};
	return(&b);

unsupported:
	abort("runs_to_runs: %s --> %s unsupported",
		ident_toa(bp->ident),ident_toa(id));
	}

#if CPU!=CRAY
/* Write these RunFs to FILE *fp in CCITT Group 4 format.
   BUG:  uses bitio to one end of a pipe, just to store the entire
   byte-stream so it can be counted; then, it is copied to *fp.
   This fails on large bitmaps since the space available for pipe
   buffering is limited by system constraints and can run out at any
   time (symptom: the user process hangs in I state).  Also, since
   it relies so heavily on streams, it may not prove to be very portable.
   Should be rewritten with an expandable byte-buffer of some kind,
   which requires that the `putting' function in bitio be user-selectable.
   */
fwrb_runfs_g4(fp,bxp,runs,ff)
    FILE *fp;
    Bbx *bxp;
    int runs;
    RunF *ff;
#define dbg_fwrb_runfs_g4_detail F
{   RLE_Line l0,l1;
    RLE_Line *cl,*pl,*swap;	/* current/prior/swap line */
    int wid,hgt;		/* width,height of rectangular box of pixels */
    BITFILE *bf;
    int iri;
    RunF *irp;
    RLE_Run *crp;
    static int pipe_fd[2] = {-1,-1};
    static FILE *pipe_fp[2] = {NULL,NULL};
    unsigned long bytes,bi;
    int och;
	/* buffer g4 code in a pipe in order to count it */
	if(pipe_fd[0]==-1) {
		pipe(pipe_fd);
		if((pipe_fp[0] = fdopen(pipe_fd[0],"r"))==NULL)
			abort("fwrb_runfs_g4: can't open pipe \"r\", fd=%d",
				pipe_fd[0]);
		if((pipe_fp[1] = fdopen(pipe_fd[1],"w"))==NULL)
			abort("fwrb_runfs_g4: can't open pipe \"w\", fd=%d",
				pipe_fd[1]);
		/* leave this pipe open for the duration of the process */
		};
	/* treat pipe_fp[1] as a sequence of bits */
	if((bf=bopen(pipe_fp[1],"w"))==NULL)
		abort("fwrb_runfs_g4: can't open bitfile");
	wid = bbx_wid(bxp);  hgt=bbx_hgt(bxp);
	BOF_to_g4(bf);
	cl= &l0;  pl= &l1;  cl->len = pl->len = wid;  cl->y=0;  pl->runs=0;
	iri=0;  irp=ff;
	do {	/* build current RLE_Line */
		crp=cl->r;
		while(iri<runs&&irp->y==cl->y) {
			crp->xs=irp->xs;
			crp->xe=irp->xe;
			if(dbg_fwrb_runfs_g4_detail)
				err("i%d:  y%d x[%d,%d]",
					iri,cl->y,crp->xs,crp->xe);
			iri++; irp++; crp++;
			};
		cl->runs = crp-cl->r;
		/* write current RLE_line to file */
		rlel_to_g4(pl,cl,wid,bf);
		/* save current line as prior */
		swap=pl;  pl=cl;  cl=swap;  cl->runs=0;  cl->y=pl->y+1;
		}
	while(cl->y<hgt);
	/** By policy, don't append EOFB (i.e. omit EOF_to_g4(bf)) **/
	bytes = bclose(bf);
	if(dbg_fwrb_runs) err("fwrb_runfs_g4: %d bytes",bytes);
#if FWRI
	fwri_uint4(fp,bytes);
#else
	fwrite(&bytes,sizeof(bytes),1,fp);
#endif
	/* copy contents in pipe to fp */
	fflush(pipe_fp[1]);
	for(bi=0;bi<bytes;bi++) {
		putc(och=getc(pipe_fp[0]),fp);
#if dbg_fwrb_runs
		fprintf(stderr,"%02x",(unsigned char)och);
#endif
		};
	if(dbg_fwrb_runs) fprintf(stderr,"\n");
	}
#else
fwrb_runfs_g4(fp,bxp,runs,ff)
    FILE *fp;
    Bbx *bxp;
    int runs;
    RunF *ff;
{	abort("fwrb_runfs_g4: unimplemented on Cray");
	}
#endif

#if CPU != CRAY
/* Read a connected group of Runs in CCITT Group 4 format, into an array of RunFs.
   Return:  1 if normal & successful, 0 if EOF, and -1 if error.
   */
int frdb_g4_runfs(fp,bxp,runs,ff)
    FILE *fp;
    Bbx *bxp;	/* their bounding box, shrink-wrapped to fit exactly */
    int runs;	/* expect exactly this number of runs */
    RunF *ff;	/* space for `runs' RunFs */
#define dbg_frdb_g4_runfs_detail F
{   RLE_Line *cl;	/* current line */
    int wid,hgt;	/* width,height of rectangular box of pixels */
    int y;		/* height of current line */
    boolean bof;
    BITFILE *bf;
    static DST_table *tbl = NULL;
    int iri;
    RLE_Run *irp;
    int ori;
    RunF *orp;
    unsigned long bytes,g4_bytes;
#if FRDI
	bytes=frdi_uint4(fp);
#else
	fread(&bytes,sizeof(bytes),1,fp);
#endif
	if(dbg_frdb_runs) err("frdb_g4_runfs: %d bytes",bytes);
	/* read FILE *fp as a sequence of bits */
	if((bf=bopen(fp,"r"))==NULL)
		abort("frdb_g4_runfs: can't open bitfile");
	if(tbl==NULL) {
		tbl=ccitt_table();
		if(dbg_frdb_g4_runfs_detail) ccitt_err_tbl(tbl);
		};
	wid=bbx_wid(bxp);  hgt=bbx_hgt(bxp);
	y=0;  bof=T;  ori=0;  orp=ff;
	while((y<hgt)&&(cl=g4_to_rlel(tbl,bf,bof,wid))!=NULL) {
		/* copy runs to RunF ff[] */
		for(iri=0,irp=cl->r; iri<cl->runs; iri++,irp++) {
			if(dbg_frdb_g4_runfs_detail)
				err("o%d i%d:  y%d x[%d,%d]",
					ori,iri,y,irp->xs,irp->xe);
			if(ori<runs) {
				orp->y = y;  orp->xs = irp->xs;  orp->xe = irp->xe;
				orp++;
				}
			else err("frdb_g4_runfs: too many runs: ori%d >= runs%d",
				ori,runs);
			ori++;
			};
		y++;  bof=F;
		};
	if(cl==NULL) {
		err("frdb_g4_runfs: unexpected EOF");
		return(0);
		};
	/* recover connectivity among runs, on the assumption that they
	   are still in increasing lexicographic order on (y,xs) */
	fix_lag(orp-ff,(char *)ff,Runs_ff);
	if(ori!=runs) {
		err("frdb_g4_runfs: expected %d runs, but read %d",runs,ori);
		return(-1);
		};
	if((g4_bytes=bclose(bf))!=bytes) {
		err("frdb_g4_runfs: expected %d bytes, but read %d",
			bytes,g4_bytes);
		return(-1);
		};
	if(ferror(fp)) return(-errno); else return(1);
	}
#else
int frdb_g4_runfs(fp,bxp,runs,ff)
    FILE *fp;
    Bbx *bxp;	/* exact bounding box, shrink-wrapped to fit */
    int runs;	/* expect exactly this number of runs */
    RunF *ff;	/* space for `runs' RunFs */
{	abort("frdb_g4_runfs:  unimplemented on Cray");
	}
#endif

/* Write blob, etc to FILE *fp in binary format.
   If !(etc&IsBlob), then do nothing.
   else write Blob record and:
     If etc&IsRun, then also try to write Runs:
       If Blob.runs==0, write no runs.
       else
	   if etc&Runs_g4, write in CCITT G4 format
		compact but needs large streams; may not port well;
	   else write in Runs_ff format
	        if blob is small enough, use compressed RunFS form on output.

    */
fwrb_blob_etc(fp,bp,etc)
    FILE *fp;
    Blob *bp;
    Ident etc;
{   Ident sv_ident;
    int sv_runs;
    Run *rp;
    RunF rf;
    register RunF *rfp, *rfq;
    RunFS *rsp;
#if FWRI
    RunFS rfs;
#else
    static RunFS rfsa[256];
#endif
    int ri;
	if(!(etc&IsBlob)) return;

	sv_ident = bp->ident;
        sv_runs = bp->runs;
	if(etc&IsRun) {
		/* decide which Runs_* bits should be set on output */
		bp->ident &= ~(Runs_f|Runs_ff|Runs_seek|Runs_g4);
		if(etc&Runs_g4) {
			bp->ident |= Runs_g4;
			}
		else {	/* assume Runs_ff is wanted */
			bp->ident |= Runs_ff;
			};
		}
	else {	bp->ident &= ~(IsRun|Runs_f|Runs_ff|Runs_seek|Runs_g4);
		bp->runs = 0;
		};
	fwrb_blob(fp,bp);
	bp->ident = sv_ident;
	bp->runs = sv_runs;

	if(bp->runs==0) return;

	if(etc&Runs_g4) {
		/* write CCITT Group 4 encoding for Runs */
		if(bp->ident&Runs_ff) {
			fwrb_runfs_g4(fp,&(bp->bx),bp->runs,bp->r.ff);
			}
		else abort("fwrb_blob_etc: etc&Runs_g4 but !(id&Runs_ff)");
		}
	else {	/* Assume Runs_ff format is desired */
		if(bp->ident&Runs_f) {
			rp=bp->r.f;	
			if(bp->ident&Blob_small) { /* use compressed Run records */
				if(blob_debug) err("small");
				/* can use small local static array */
#if FWRI
				rsp= &rfs;
#else
				rsp=rfsa;
#endif
				while(rp!=NULL) {
					/* y, xs, xe will be relative to Blob.bx.a */
					rsp->y = rp->y - bp->bx.a.y;
					rsp->xs = rp->xs - bp->bx.a.x;
					rsp->xe = rp->xe - bp->bx.a.x;
					/* above,below connecting run indices are relative to
					   the sequence no. of this run (ac>0 & bc>0) */
					if((rsp->ad = rp->ad)!=0)
						rsp->ac = rp->u.no - rp->ac->u.no;
					else rsp->ac = 0;
					if((rsp->bd = rp->bd)!=0)
						rsp->bc = rp->bc->u.no - rp->u.no;
					else rsp->bc = 0;
					if(blob_debug) err_runfs("  ",rsp);
					rp=rp->n;
#if FWRI
					fwri_RunFS(fp,rsp);
#else
					rsp++;
#endif
					};
#if !FWRI
				if(fwrite(rfsa,sizeof(RunFS),bp->runs,fp)!=bp->runs)
					abort("fwrb_blob_etc: can't fwrite RunFS[%d]",bp->runs);
				if(dbg_fwrb_runs)
					err("fwrb_blob_etc: %d bytes",bp->runs*sizeof(RunFS));
#endif
				}
			else {	if(blob_debug) err("large");
				while(rp!=NULL) {	
					/* y, xs, xe will be relative to Blob.bx.a */
					rf.y = rp->y - bp->bx.a.y;
					rf.xs = rp->xs - bp->bx.a.x;
					rf.xe = rp->xe - bp->bx.a.x;
					/* above,below connecting run indices are relative to
					   the sequence no. of this run (ac>0 & bc>0) */
					if((rf.ad = rp->ad)!=0)
						rf.ac = rp->u.no - rp->ac->u.no;
					else rf.ac = 0;
					if((rf.bd = rp->bd)!=0)
						rf.bc = rp->bc->u.no - rp->u.no;
					else rf.bc = 0;
					if(blob_debug) err_runf("   ",&rf);
		
					/* since there may be many, won't use a local array */
#if FWRI
					fwri_RunF(fp,&rf);
#else
					if(fwrite(&rf,sizeof(RunF),1,fp)!=1)
						abort("fwrb_blob_etc: can't fwrite RunF");
#endif
					rp=rp->n;
					};
				if(dbg_fwrb_runs)
					err("fwrb_blob_etc: %d bytes",bp->runs*sizeof(RunF));
				};
			}
		else if(bp->ident&Runs_ff) {
			if(bp->ident&Blob_small) { /* use compressed Run records */
				if(blob_debug) err("small");
				/* can use small local static array */
#if FWRI
				rsp= &rfs;
#else
				rsp=rfsa;
#endif
				for(rfp=bp->r.ff,ri=0; ri<bp->runs; rfp++,ri++) {
					rsp->y = rfp->y;
					rsp->xs = rfp->xs;
					rsp->xe = rfp->xe;
					rsp->ad = rfp->ad;
					rsp->bd = rfp->bd;
					rsp->ac = rfp->ac;
					rsp->bc = rfp->bc;
#if FWRI
					fwri_RunFS(fp,rsp);
#else
					rsp++;
#endif
					};
#if !FWRI
				if(fwrite(rfsa,sizeof(RunFS),bp->runs,fp)!=bp->runs)
					abort("fwrb_blob_etc: can't fwrite RunFS[%d]",bp->runs);
				if(dbg_fwrb_runs)
					err("fwrb_blob_etc: %d bytes",bp->runs*sizeof(RunFS));
#endif
				}
			else {	if(blob_debug) err("large");
#if FWRI
				for(rfq=(rfp=bp->r.ff)+bp->runs; rfp<rfq; rfp++) {
					fwri_RunF(fp,rfp);
					};

#else
				if(fwrite(bp->r.ff,sizeof(RunF),bp->runs,fp)!=bp->runs)
					abort("fwrb_blob_etc: can't fwrite RunF[%d]",bp->runs);
				if(dbg_fwrb_runs)
					err("fwrb_blob_etc: %d bytes",bp->runs*sizeof(RunF));
#endif
				};
			};
		}; 
	}

/* Read a given Blob's RunF's (binary) from FILE *fp; return F on EOF.
   If bp->runs<=max, read the RunF's into bp->r.ff, and set Runs_ff bit.
   otherwise ignore them all and set bp->r.ff==NULL.  */
int frdb_runfs(fp,bp,max)
    FILE *fp;
    Blob *bp;	/* *bp's fields already set up */
    int max;	/* maximum no. RunF records expected */
{   register RunFS *rsp,*rsq;
    static RunFS rfsa[256];
    register RunF *rfp,*rfq;
	if(bp->r.ff!=NULL && bp->runs <= max) {
		bp->ident |= Runs_ff;  bp->ident &= ~(Runs_f|Runs_seek);
		if(bp->ident&Blob_small) {
			if(blob_debug) err("small");
#if FRDI
			for(rsq=(rsp=rfsa)+bp->runs; rsp<rsq; rsp++) {
				frdi_RunFS(fp,rsp);
				};
#else
			if(fread(rfsa,sizeof(RunFS),bp->runs,fp) < bp->runs)
				return(0);
#endif
			/* copy compressed RunFS's into RunF's */
			for(rsq=(rsp=rfsa)+bp->runs,rfp=bp->r.ff;
			       rsp<rsq;
				  rsp++,rfp++) {
				/* beware sign extension from unsigned chars */
				if(blob_debug) err_runfs("   ",rsp);
				rfp->y = 0377&rsp->y;
				rfp->xs = 0377&rsp->xs;
				rfp->xe = 0377&rsp->xe;
				rfp->ad = 0377&rsp->ad;
				rfp->bd = 0377&rsp->bd;
				rfp->ac = 0377&rsp->ac;
				rfp->bc = 0377&rsp->bc;
				};
			}
		else {	if(blob_debug) err("large");
#if FRDI
			for(rfq=(rfp=bp->r.ff)+bp->runs; rfp<rfq; rfp++)
				frdi_RunF(fp,rfp);
#else
			if(fread(bp->r.ff,sizeof(RunF),bp->runs,fp) != bp->runs)
				return(F);
#endif
			};
		}
	else {	bp->ident &= ~(Runs_ff|Runs_f);
		bp->ident |= Runs_seek;
		bp->r.seek=ftell(fp);
		/* skip past excessive no. of runs */
/** What if FRDI?? **/
		if(bp->ident&Blob_small)
			fseek(fp,(long)((bp->runs)*sizeof(RunFS)),1);
		else
			fseek(fp,(long)((bp->runs)*sizeof(RunF)),1);
		};
	if(ferror(fp)) return(-errno); else return(1);
	}

#if FRDI
/* read a Blob record (only) into *p */
int frdb_blob(f,p)
    FILE *f;
    Blob *p;
{   int bdy_mny;
	*p = empty_Blob;
	if(feof(f))
		return(0);
	p->ident=frdi_Ident(f);
	/* p->no=frdi_Seq(f); */
	frdi_Bbx(f,&(p->bx));
	p->area=frdi_uint4(f);
	p->per=frdi_uint4(f);
	/* don't read .n */
	p->m=frdi_Merit(f);
	p->runs=frdi_uint3(f);
	if((bdy_mny=frdi_uint2(f))>0) {
		p->bdsp = alloc_bdys();
		p->bdsp->mny = bdy_mny;
		};
#if dbg_frdb
	if((!(p->ident&IsBlob))||(p->ident&(IsALL&(~IsBlob))))
		err("frdb_blob: %s",blob_toa(p));
#endif
#if dbg_frdb_toa
	err("frdb_blob: %s",blob_toa(p));
#endif
	if(ferror(f))
		return(-errno);
	else return(1);
	}
#else
/* read a Blob record (only) into *bp;  return F if EOF */
int frdb_blob(fp,bp)
    FILE *fp;
    Blob *bp;
{   BlobF bf;
	if(fread(&bf,sizeof(BlobF),1,fp)!=1) return(0);
	if(blob_debug) err_blobf("bf",&bf);
	bp->ident=bf.ident;
	bp->no = 0;
	bp->bx=bf.bx;
	bp->area=bf.area;
	/*bp->per=bf.per;*/
	bp->runs=bf.runs;
	/*bp->bdys=bf.bdys;*/
	bp->n = NULL;
	bp->r.f = NULL;
	bp->bdsp = NULL;
#if dbg_frdb
	if((!(bp->ident&IsBlob))||(bp->ident&(IsALL&(~IsBlob))))
		err("frdb_blob: %s",blob_toa(bp));
#endif
#if dbg_frdb_toa
	err("frdb_blob: %s",blob_toa(bp));
#endif
	if(ferror(fp)) return(-errno); else return(1);
	}
#endif

err_blob(s,bp)
    char *s;
    Blob *bp;
{	fprintf(stderr,"%s ",s);
	if(bp==NULL) fprintf(stderr,"Blob NULL.\n");
	else fprintf(stderr,
		"Blob %d: x[%d,%d],y[%d,%d] ar%d ft%s r(#%d f%d)\n",
		bp->no,bp->bx.a.x,bp->bx.b.x,bp->bx.a.y,bp->bx.b.y,
		bp->area,ident_toa(bp->ident),bp->runs,rp_tod(bp->r.f));
	}

err_blob_runs(s,bp)
    char *s;
    Blob *bp;
{   Run *crp;
    char cs[20];
	fprintf(stderr,"%s ",s);
	err_blob(s,bp);
	if(bp!=NULL&&bp->runs>0&&bp->r.f!=NULL) {
		sprintf(cs,"%*s",strlen(s)+2," ");
		for(crp=bp->r.f; crp!=NULL; crp=crp->n) err_run(cs,crp);
		};
	}

err_blob_runfs(s,bp)
    char *s;
    Blob *bp;
{   RunF *crp;
    int ri;
    char cs[20];
	fprintf(stderr,"%s ",s);
	err_blob_briefly("",bp);
	if(bp!=NULL&&bp->runs>0&&bp->r.ff!=NULL) {
		sprintf(cs,"%*s",strlen(s)+2," ");
		for(crp=bp->r.ff,ri=0; ri<bp->runs; crp++,ri++) err_runf(cs,crp);
		};
	}

err_blobf(s,bp)
    char *s;
    BlobF *bp;
{   Run *crp;
    char cs[20];
	fprintf(stderr,"%s ",s);
	if(bp==NULL) fprintf(stderr,"BlobF NULL.\n");
	else fprintf(stderr,
		"BlobF: x[%d,%d],y[%d,%d] ar%d ft%s #r%d\n",
		bp->bx.a.x,bp->bx.b.x,bp->bx.a.y,bp->bx.b.y,
		bp->area,ident_toa(bp->ident),bp->runs);
	}

err_blob_briefly(s,bp)
    char s[];
    Blob *bp;
{   Run *crp;
	if(bp==NULL) err("%s Blob NULL.",s);
	else { fprintf(stderr,
			"%s Blob #%d: bx(%d,%d),(%d,%d) ar%d f%s runs%d ",
			s,
			bp->no,
			bp->bx.a.x,bp->bx.a.y,bp->bx.b.x,bp->bx.b.y,
			bp->area,
			ident_toa(bp->ident),
			bp->runs);
		fprintf(stderr,"\n");
		};
	}

err_blob_stats()
{	err("Blob stats:  %d now, %d total, %d hi-water\n",
			blob_max-blob_fr_mny,hi_blob_no+1,blob_hi);
	}

/* write blob list starting at fi */
fwrb_blobl_etc(fp,mny,fi,etc)
    FILE *fp;
    int mny;
    Blob *fi;
    Ident etc;
{   int bi;
    Blob *bp;
	if(mny>0) for(bi=0,bp=fi; bi<mny; bi++,bp=bp->n) {
		if(bp!=NULL) fwrb_blob_etc(fp,bp,etc);
		else {	err("fwrb_blobl_etc: bmny==%d but %dth (Blob *) is NULL",
				mny,bi);
			break;
			};
		};
	}

fwrb_blobs_etc(fp,bs,etc)
    FILE *fp;
    Blobs bs;
    Ident etc;
{   register Blob *bp,**bpp;
	if(bs.mny==0) return;
	for(bp= *(bpp=bs.bpa); bp!=NULL; bp= *(++bpp))
		fwrb_blob_etc(fp,bp,etc);
	}

/* read a set of blobs, and their parts */
frdb_blobs_etc(fp,bsp,etc)
    FILE *fp;
    Blobs *bsp;
    Ident etc;		/* read only the parts indicated */
{   int bi;
    register Blob *bp,**bpp;
	if(bsp->mny<=0) {
		*bsp = empty_Blobs;
		return(1);
		};

	if((bpp=bsp->bpa=(Blob **)malloc((bsp->mny+1)*sizeof(Blob *)))==NULL)
		abort("frdb_blobs_etc: can't alloc Blobs.bpa[%d]",bsp->mny+1);
	for(bi=0; bi<bsp->mny; bi++) {
		*(bpp++) = bp = alloc_blob();
		if(!frdb_blob_etc(fp,bp,etc))
			abort("frdb_blobs_etc: unexpected EOF");
		};
	*bpp = NULL;
	if(ferror(fp)) return(-errno); else return(1);
	}

/* Read a blob record into *bp, and then read its parts as specified by etc.
   If there aren't any runs, return normally.
   If etc&IsRun is false, then set Runs_seek and proceed as described below.
   If etc&Runs_ff, then the contents of r.ff is examined:
   	if NULL, then space is allocated here for `runs' RunF's;
   	if non-NULL, then on the assumption that it points to space for
   	at least `runs' many, they are read into it.
   If etc&Runs_seek, then the ftell value is placed in r.seek,
	and the runs are simply skipped over.
   The Runs_f (list) option is unimplemented.
   The Blob is marked with the appropriate Runs_X bit.
   Return:  1 if normal & successful, 0 if EOF, and -1 if error.
    */
int frdb_blob_etc(fp,bp,etc)
    FILE *fp;
    Blob *bp;
    Ident etc;
{   int stat;
	if((stat=frdb_blob(fp,bp))!=1)
		return(stat);
	if(bp->runs<=0) {
		bp->runs = 0;
		bp->r.ff = NULL;
		bp->ident &= ~(Runs_f|Runs_seek|Runs_g4);
		bp->ident |= Runs_ff;
		return(1);
		};

	if(!(etc&IsRun)) {
		etc |= IsRun|Runs_seek;
		etc &= ~(Runs_f|Runs_ff|Runs_g4);
		};
	if (!(etc&(Runs_f|Runs_ff|Runs_g4|Runs_seek)) || etc&Runs_ff) {
		/* Want to deliver Runs in Runf ff[] format */
		if(bp->r.ff==NULL) {
			if( (bp->r.ff=
			    (RunF *)malloc(bp->runs*sizeof(RunF)))==NULL)
				abort("frdb_blob_etc: can't alloc r.ff[%d]",bp->runs);
			};
		if(bp->ident&Runs_g4) {
			/* Runs are in CCITT Group 4 format */
			stat=frdb_g4_runfs(fp,&(bp->bx),bp->runs,bp->r.ff);
			}
		else {	/* Runs are in RunF or RunFS format */
			stat=frdb_runfs(fp,bp,bp->runs);
			};
		bp->ident |= Runs_ff;  bp->ident &= ~(Runs_f|Runs_seek|Runs_g4);
		return(stat);
		}
	else if(etc&Runs_seek) {
		bp->ident |= Runs_seek;  bp->ident &= ~(Runs_ff|Runs_f);
		bp->r.seek=ftell(fp);	/* save seek addr */
		/* skip past runs */
		if(bp->ident&Blob_small)
			fseek(fp,(long)((bp->runs)*sizeof(RunFS)),1);
		else	fseek(fp,(long)((bp->runs)*sizeof(RunF)),1);
		}
	else /* (etc&Runs_f) || (etc&Runs_g4) */ {
		abort("frdb_blob_etc:  Runs_f & Runs_g4 etc option unimplemented");
 		};
	if(ferror(fp)) return(-errno); else return(1);
	}

/* read a number of blobs, and their parts, into linked-list (mny, *fi);
   return T iff not EOF  */
boolean frdb_blobl_etc(fp,bmny,fip,etc)
    FILE *fp;
    int bmny;
    Blob **fip;
    Ident etc;
{   int bi;
    register Blob *bp,**bpp;
    RunF *rp;
    int stat;
	if(bmny<=0) return(1);
	bi=0; bpp=fip;
	do {	bp=alloc_blob();
		if((stat=frdb_blob_etc(fp,bp,etc))!=1) return(stat);
		*bpp=bp;  bpp= &(bp->n);
		}
	while((++bi)<bmny);
	*bpp=NULL;
	if(ferror(fp)) return(-errno); else return(1);
	}

char *interp_toa(ip)
    Interp *ip;
{   static char s[80];
    char ms[3],shs[3],szs[3],hts[3],prs[3];
	strcpy(ms,merit_toa(ip->m));
	strcpy(shs,merit_toa(ip->mshap));
	strcpy(szs,merit_toa(ip->msize));
	strcpy(hts,merit_toa(ip->mbhgt));
	strcpy(prs,merit_toa(ip->p));
	sprintf(s,"%s %s sz%s ba%d m%s (sh%s sz%s ht%s) p%s",
		ident_toa(ip->ident),
#if CPU!=CRAY
		classid_toa(&(ip->ci)),
#else
		ip->ci.c,
#endif
		pts_toa(ip->size),
		ip->basl,
		ms,shs,szs,hts,prs);
	return(s);
	}

frdb_interpl_etc(fp,ilp,etc)
    FILE *fp;
    Interpl *ilp;
    Ident etc;
{   int ii,stat;
    Interp *ip,*pp;
	if(ilp->mny<=0) {
		*ilp = empty_Interpl;
		return(1);
		};

	for(ii=0; ii<ilp->mny; ii++) {
		ip=alloc_interp();
		if(ii==0) ilp->fi = ip;
		else pp->n = ip;
		if((stat=frdb_interp(fp,ip))!=1) return(stat);
		pp=ip;
		};
	ip->n = NULL;
	if(ferror(fp)) return(-errno); else return(1);
	}

#if FRDI
int frdb_interp(f,p)
    FILE *f;
    Interp *p;
{   int stat;
	*p = empty_Interp;
	if(feof(f))
		return(0);
	p->ident=frdi_Ident(f);
	frdi_ClassId(f,&(p->ci));
	p->mshap=frdi_Merit(f);
	p->size=frdi_Pts(f);
	p->msize=frdi_Merit(f);
	p->basl=frdi_Scoor(f);
	p->mbhgt=frdi_Merit(f);
	p->m=frdi_Merit(f);
	p->p=frdi_Prob(f);
#if dbg_frdb
	if((!(p->ident&IsInterp))||(p->ident&(IsALL&(~IsInterp))))
		err("frdb_interp: %s",interp_toa(p));
#endif
	if(ferror(f)) return(-errno); else return(1);
	}
#else
int frdb_interp(fp,ip)
    FILE *fp;
    Interp *ip;
{   InterpF inf;
    int stat;
	if((stat=fread(&inf,sizeof(InterpF),1,fp))!=1) {
		err("can't fread InterpF, status %d",stat);
		return(stat);
		};
	*ip = empty_Interp;
	ip->ident = inf.ident;
	ip->ci = inf.ci;
	ip->clp = NULL;
	ip->clsp = NULL;
	ip->mshap = inf.mshap;
	ip->size = inf.size;
	ip->msize = inf.msize;
	ip->basl = inf.basl;
	ip->mbhgt = inf.mbhgt;
	ip->m = inf.m;
#if dbg_frdb
	if((!(ip->ident&IsInterp))||(ip->ident&(IsALL&(~IsInterp))))
		err("frdb_interp: %s",interp_toa(ip));
#endif
	if(ferror(fp)) return(-errno); else return(1);
	}
#endif

free_interpl(ilp)
    Interpl *ilp;
{   int ii;
    Interp *ip,*np;
	if(ilp->mny>0) {
		for(ip=ilp->fi; ip!=NULL; ip=np) { np=ip->n; free_interp(ip); }
		ilp->mny=0;
		};
	ilp->fi=NULL;
	};

fwrb_interpl_etc(fp,is,etc)
    FILE *fp;
    Interpl is;
    Ident etc;
{   Interp *ip;
    int ii;
	if(is.mny>0) for(ii=0,ip=is.fi; ii<is.mny; ii++,ip=ip->n)
		fwrb_interp_etc(fp,ip,etc);
	}

/* remove this Interp from the Interp-list */
remove_interpl(ip,ilp)
    Interp *ip;
    Interpl *ilp;
{   Interp *rp,*pp;
	if(ilp->mny==1) {  /* frequent case */
		ilp->fi=NULL; ilp->mny=0;
		}
	else if(ilp->mny>1){
		pp=NULL;
		for(rp=ilp->fi; rp!=ip&&rp!=NULL; pp=rp,rp=rp->n) ;
		if(rp!=NULL) {	/* remove from list */
			if(pp==NULL) ilp->fi=ip->n;
			else pp->n=ip->n;
			ilp->mny--;
			}
		else err("remove_interpl: can't - not found");
		}
	else err("remove_interpl: can't - Interpl empty");
	}

/* prepend this Interp to the Interp-list */
prepend_interpl(ip,ilp)
    Interp *ip;
    Interpl *ilp;
{   Interp *rp,*pp;
	ip->n = ilp->fi;
	ilp->fi = ip;
	ilp->mny++;
	}

fwrb_interp_etc(fp,ip,etc)
    FILE *fp;
    Interp *ip;
    Ident etc;
{	fwrb_interp(fp,ip);
	}

#if FWRI
fwrb_interp(f,p)
    FILE *f;
    Interp *p;
{   InterpF inf;
    int stat;
#if dbg_fwrb
	if((!(p->ident&IsInterp))||(p->ident&(IsALL&(~IsInterp))))
		err("fwrb_interp: %s",interp_toa(p));
#endif
	fwri_Ident(f,p->ident);
	fwri_ClassId(f,&(p->ci));
	fwri_Merit(f,p->mshap);
	fwri_Pts(f,p->size);
	fwri_Merit(f,p->msize);
	fwri_Scoor(f,p->basl);
	fwri_Merit(f,p->mbhgt);
	fwri_Merit(f,p->m);
	fwri_Prob(f,p->p);
#if dbg_fwrb_toa
	err("fwrb_interp: %s",interp_toa(p));
#endif
	}
#else
fwrb_interp(fp,ip)
    FILE *fp;
    Interp *ip;
{   InterpF inf;
    int stat;
#if dbg_fwrb
	if((!(ip->ident&IsInterp))||(ip->ident&(IsALL&(~IsInterp))))
		err("fwrb_interp: %s",interp_toa(ip));
#endif
	memset(&inf,'\0',sizeof(inf));
	inf.ident = ip->ident;
	inf.ci = ip->ci;
	inf.mshap = ip->mshap;
	inf.size = ip->size;
	inf.msize = ip->msize;
	inf.basl = ip->basl;
	inf.mbhgt = ip->mbhgt;
	inf.m = ip->m;
	if((stat=fwrite(&inf,sizeof(InterpF),1,fp))!=1)
		abort("fwrb_interp: can't fwrite, status %d",stat);
#if dbg_fwrb_toa
	err("fwrb_interp: %s",interp_toa(ip));
#endif
	}
#endif

/** Run handling:
	alloc_run_pool -create pool of free run records
	free_run_pool -	free entire pool
	alloc_run -	allocate a new run from pool
	free_run - 	free a specified run (into pool)
	err_run -	print Run (ascii) to stderr
	err_runb -	print Run (ascii) to stderr (after added to blob set)
	err_runf -	print RunF (ascii) to stderr (after added to blob set)
	err_runfs -	print RunFS (ascii) to stderr (after added to blob set)
	err_run_stats - report statistics to stderr
   **/

/* RunPool functions: variable-size pool, speed-optimized alloc/free */

/* Initialize pool of Runs with increment `incr' (return F if can't allocate) */
boolean alloc_run_pool(incr,dbg)
    int incr;
    boolean dbg;	/* trace actions on Runs */
{   register Run *crp, *prp;
    register int i;
	_RunPool.incr = incr;
	_RunPool.pools = 1;
	if((_RunPool.pool=(Run **)malloc(_RunPool.pools*sizeof(Run *)))==NULL)
		return(F);
	if((_RunPool.pool[_RunPool.pools-1] =
	    (Run *)malloc(_RunPool.incr*sizeof(Run)))==NULL) {
		free(_RunPool.pool);
		return(F);
		};
	_RunPool.next = 0;
	_RunPool.free = NULL;
#if STATS
	_RunPool.total = 0;
#endif
	_RunPool.dbg = dbg;
	if(_RunPool.dbg) err("alloc_run_pool: incr%d",_RunPool.incr);
	return(T);
	}

free_run_pool()
{   register int i;
	for(i=0; i<_RunPool.pools; i++) free(_RunPool.pool[i]);
	free(_RunPool.pool);
	}

/* The ``hard'' case of allocating runs from RunPool: can't be inline */
Run *hard_alloc_run()
{	_RunPool.pools++;
	if((_RunPool.pool=
		(Run **)realloc(_RunPool.pool,_RunPool.pools*sizeof(Run *)))==NULL)
		abort("alloc_Run: can't realloc");
	if((_RunPool.cur=_RunPool.pool[_RunPool.pools-1] =
	    (Run *)malloc(_RunPool.incr*sizeof(Run)))==NULL)
		abort("alloc_Run: can't malloc");
	_RunPool.next=1;
	*(_RunPool.cur) = empty_Run;
	return(_RunPool.cur);
	}

int rp_tod(rp)
    Run *rp;
{	return((int)rp);
	}

char *runf_toa(rp)
    RunF *rp;
{  static char s[80];
   char s1[80];
	strcpy(s,"RunF ");
	if(rp==NULL) strcat(s,"NULL");
	else {	sprintf(s1,
			"y%d,x[%d,%d] lag(ad%d,bd%d ac%d,bc%d)",
			rp->y,rp->xs,rp->xe, 
			rp->ad,rp->bd, rp->ac,rp->bc );
		strcat(s,s1);
		};
	return(s);
	}

char *runfs_toa(rp)
    RunFS *rp;
{  static char s[80];
   char s1[80];
	strcpy(s,"RunFS ");
	if(rp==NULL) strcat(s,"NULL");
	else {	sprintf(s1,
			"y%d,x[%d,%d] lag(ad%d,bd%d ac%d,bc%d)",
			rp->y,rp->xs,rp->xe, 
			rp->ad,rp->bd, rp->ac,rp->bc );
		strcat(s,s1);
		};
	return(s);
	}

err_run(s,rp)
	char *s;
	Run *rp;
{	fprintf(stderr,"%s ",s);
	if(rp==NULL) fprintf(stderr,"Run NULL.\n");
	else fprintf(stderr,
		"Run %d: y%d,x[%d,%d] l(n%d) t(o%x ad%d,bd%d ac%d,bc%d)\n",
		rp_tod(rp), rp->y,rp->xs,rp->xe, 
		rp_tod(rp->n),
#if CPU!=CRAY
		(int)rp->u.o,
#else
		0,
#endif
		rp->ad,rp->bd,
		rp_tod(rp->ac),
		rp_tod(rp->bc) );
	}

/* print Run after it has been added to Blob set */
err_runb(s,rp)
	char *s;
	Run *rp;
{	fprintf(stderr,"%s ",s);
	if(rp==NULL) fprintf(stderr,"Run NULL.\n");
	else fprintf(stderr,
		"Run %d: y%d,x[%d,%d] l(n%d) t(no%d ad%d,bd%d ac%d,bc%d)\n",
		rp_tod(rp), rp->y,rp->xs,rp->xe, 
		rp_tod(rp->n),
		rp->u.no, rp->ad,rp->bd, rp_tod(rp->ac),rp_tod(rp->bc) );
	}

err_runf(s,rp)
	char *s;
	RunF *rp;
{	fprintf(stderr,"%s ",s);
	if(rp==NULL) fprintf(stderr,"RunF NULL.\n");
	else fprintf(stderr,
		"RunF: y%d,x[%d,%d] t(ad%d,bd%d ac%d,bc%d)\n",
		rp->y,rp->xs,rp->xe, 
		rp->ad,rp->bd, rp->ac,rp->bc );
	}

err_runfs(s,rp)
	char *s;
	RunFS *rp;
{   RunF rf;
	rf.y = 0377&rp->y;
	rf.xs = 0377&rp->xs;
	rf.xe = 0377&rp->xe;
	rf.ad = 0377&rp->ad;
	rf.bd = 0377&rp->bd;
	rf.ac = 0377&rp->ac;
	rf.bc = 0377&rp->bc;
	err_runf(s,&rf);
	}

int runs_of_blob(p,ra,max)
    Blob *p;
    RLE_Yrun *ra;
    int max;
{   int runs,ri;
    RunF *rfp;
	runs = p->runs;
	if(runs>max) {
		err("runs_of_blob: max exceeded - skip this blob");
		runs = 0;
		}
	else if(runs>0) {
		if(p->ident&Runs_ff) {
			for(ri=0,rfp=p->r.ff; ri<runs; ri++,rfp++) {
				ra->y = p->bx.a.y + rfp->y;
				ra->xs = p->bx.a.x + rfp->xs;
				ra->xe = p->bx.a.x + rfp->xe;
				ra++;
				};
			}
		else {	err("runs_of_blob: handle only Runs_ff - skip this blob");
			runs = 0;
			};
		};
	return(runs);
	}

int no_runs_of_blobl(p)
    Blobl *p;
{   int runs;
    Blob *pp;
	runs = 0;
	if(p->mny>0) for(pp=p->fi; pp!=NULL; pp=pp->n) runs += pp->runs;
	return(runs);
	}

int runs_of_blobl(p,ra,max)
    Blobl *p;
    RLE_Yrun *ra;
    int max;
{   int runs;
    Blob *pp;
	runs = 0;
	if(p->mny>0) for(pp=p->fi; pp!=NULL; pp=pp->n)
		runs += runs_of_blob(pp,ra+runs,max-runs);
	return(runs);
	}

int no_runs_of_char(p)
    Char *p;
{   int runs;
    Blobl bl;
	runs = 0;
	bl.mny = p->bmny;  bl.fi = p->fi;
	runs += no_runs_of_blobl(&bl);
	return(runs);
	}

int runs_of_char(p,ra,max)
    Char *p;
    RLE_Yrun *ra;
    int max;
{   int runs;
    Blobl bl;
	runs = 0;
	bl.mny = p->bmny;  bl.fi = p->fi;
	runs += runs_of_blobl(&bl,ra,max);
	return(runs);
	}

/* ascending lexicographic order on y,xs */
int rn_asc(r1,r2)
   RLE_Yrun *r1,*r2;
{	if(r1->y < r2->y) return(-1);
	else if(r1->y==r2->y) {
		if(r1->xs < r2->xs) return(-1);
		else if (r1->xs == r2->xs) return(0);
		else return(1);
		}
	else return(1);
	}

RLE_Lines *rlines_of_char(chp)
    Char *chp;
{   static RLE_Lines rls;
    int no_runs,cy,ri;
    RLE_Run *lr;
    RLE_Yrun *ra,*cr;
    RLE_Line *cl;
	/* count runs */
	no_runs = no_runs_of_char(chp);
	/* allocate runs array */
	if((ra=(RLE_Yrun *)malloc(no_runs*sizeof(RLE_Yrun)))==NULL)
		abort("rlines_of_char: can't malloc ra[%d]",no_runs);
	runs_of_char(chp,ra,no_runs);
	/* sort runs ascending on (y,xs) */
	qsort(ra,no_runs,sizeof(RLE_Yrun),rn_asc);
	/* count RLE_lines.mny */
	rls.mny = 0; cy=Scoor_MIN;
	for(ri=0,cr=ra; ri<no_runs; ri++,cr++)
		if(cr->y!=cy) {
			rls.mny++;
			cy = cr->y;
			};
	/* allocate RLE_lines */
	if((rls.rla=(RLE_Line *)malloc(rls.mny*sizeof(RLE_Line)))==NULL)
		abort("rlines_of_page: can't malloc rls.rla[%d]",rls.mny);
	/* fill in RLE_lines */
	cy=Scoor_MIN;  cl=rls.rla-1;
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

int asc_merit(m1,m2)
    Merit *m1,*m2;
{	if(*m1 < *m2) return(-1);
	else if(*m1 > *m2) return(1);
	else return(0);
	}

/* Word merit is computed as a function of the merit of its Chars.
   No Chars forces merit to 0.0.
   The result is dominated by the weakest merit in the Word, slightly
   improved by the others. */
Merit wordmerit(w)
    Word *w;
{   register Char *cp,**cpp;
    static int m_alloc = 0;
    static Merit *ma;	/* ma[m_alloc] */
    int mi;
    Merit m;
    double wgt,sumwgt;
    
	if(w->cs.mny<=0) return(0.0);
	if(w->cs.mny>m_alloc) {
		if(m_alloc==0) {
			m_alloc=w->cs.mny;
			if((ma=(Merit *)malloc(m_alloc*sizeof(Merit)))==NULL)
				abort("wordmerit: can't malloc ma[%d]",m_alloc);
			}
		else {	m_alloc=w->cs.mny;
			if((ma=(Merit *)realloc(ma,m_alloc*sizeof(Merit)))==NULL)
				abort("wordmerit: can't realloc ma[%d]",m_alloc);
			};
		};
	for(cp= *(cpp=w->cs.cpa),mi=0; cp!=NULL; cp= *(++cpp),mi++) {
		if(cp->area==0 || cp->il.mny==0) ma[mi]=0.0;
		else ma[mi] = cp->il.fi->m;
		};
	qsort(ma,w->cs.mny,sizeof(Merit),asc_merit);
#define DOM (5.0)
	/* The worst merit is DOM times more significant than second worst, which
	   is DOM times more significant than the third worst, and so on.
	   */
	m=ma[0];  sumwgt=wgt=1.0;
	for(mi=1; mi<w->cs.mny; mi++) {
		m += ma[mi]*(wgt /= DOM);  sumwgt += wgt;
		};
	m /= sumwgt;
	return(m);
	}

WordSet *alloc_wordset(top,cut,cap)
    double top;		/* initial maximum merit (is never reduced) */
    double cut;		/* cut ratio:  don't keep merit < cut*top */
    int cap;		/* capacity: maximum no. at a time (0 ==> no limit) */
{   WordSet *res;
	if((res=(WordSet *)malloc(sizeof(WordSet)))==NULL)
		abort("alloc_wordset: can't");
	*res = empty_WordSet;
	res->top = top;
	res->cut = cut;
	if((res->cap = cap)==0) res->cap=INT_MAX;
	return(res);
	}

int free_wordset_etc(s,etc)
    WordSet *s;
    Ident etc;
{   int high;
	free_words_etc(&(s->ws),etc);
	high = s->high;
	free(s);
	return(high);
	}

/* Find the maximum-merit word in the set; among ties, pick the first seen. */
Word *find_max_wordset(s)
    WordSet *s;
{   register Word *max,**wpp;
	max=NULL;
	if(s->ws.mny>0) for(wpp=s->ws.wpa; *wpp!=NULL; wpp++) {
		if(max==NULL || max->m < (*wpp)->m) max=(*wpp);
		};
	return(max);
	}

/* Find the maximum-merit word in the set; among ties, pick the last seen. */
Word *find_min_wordset(s)
    WordSet *s;
{   register Word *min,**wpp;
	min=NULL;
	if(s->ws.mny>0) for(wpp=s->ws.wpa; *wpp!=NULL; wpp++) {
		if(min==NULL || min->m >= (*wpp)->m) min=(*wpp);
		};
	return(min);
	}

/* Insert a Word (without duplicating it).
   Update top, min, max, & high.
   If the new word's merit is equal to the current maximum, it does not become
   become the new maximum.
   If the new word's merit is equal to the current minimum, it becomes
   the new minimum.
   */
put_wordset(w,s)
    Word *w;	/* must have w->m set up */
    WordSet *s;
{	insert_word(w,&(s->ws));
	if(max_wordset(s)==NULL || max_wordset(s)->m < w->m) {
		max_wordset(s) = w;
		if(s->top < max_wordmerit(s)) s->top = max_wordmerit(s);
		};
	if(min_wordset(s)==NULL || min_wordset(s)->m >= w->m)
		min_wordset(s) = w;
	if(s->high < s->ws.mny) s->high = s->ws.mny;
	}

/* Remove a Word (without freeing it). Maintain min/max, but don't prune items.
   Also, don't lower top or high. */
Word *get_wordset(w,s)
    Word *w;
    WordSet *s;
{	if(w==NULL || s->ws.mny==0) return(NULL);
	remove_word(w,&s->ws);
	if(s->ws.mny==0) {
		max_wordset(s) = NULL;
		min_wordset(s) = NULL;
		}
	else {	if(w==max_wordset(s)) max_wordset(s) = find_max_wordset(s);
		if(w==min_wordset(s)) min_wordset(s) = find_min_wordset(s);
		};
	return(w);
	}

/* Remove a Word (without freeing it).  Maintain min/max.  Don't lower top. */
Word *remove_wordset(w,s,n)
    Word *w;
    WordSet *s;
    char *n;
{   register Word *gw;
#if dbg_ws
	if(w!=NULL) err("remove_wordset: %X %s",w,word_toa(w));
	else err("remove_wordset: NULL");
#endif
	gw=get_wordset(w,s);
#if dbg_ws
	err_wordset(s,n);
#endif
	return(gw);
	}

/* Check whether a given Word is identical to any already in a given set.
   Word *w must have its hash key set up. */
boolean member_wordset(w,s)
    Word *w;
    WordSet *s;
{   register Word **ww;
	if(s->ws.mny>0) {
		for(ww=s->ws.wpa; (*ww)!=NULL; ww++) {
			if(w->hash==(*ww)->hash && eq_word(w,(*ww)))
				return(T);
			};
		};
	return(F);
	}

/* Insert a Word into a set, maintaining uniqueness, a ``range'' property, and
   a maximum capacity.
   Uniqueness is maintained by refusing to insert words that are identical
   (eq_word()) to a word already in the set (or in set `u').
   The range property is that every word in the set must have merit no less
   than `s->cut' times the lifetime maximum for the set `s->top'.
   Maximum capacity is maintained as follows:  if insertion would mean more
   members than `cap', then either (a) the insertion is refused (if its
   merit is less than the current `min', or (b) the current `min' is deleted
   (and silently freed in its entirety), and the insertion occurs.
   The inserted Word is not duplicated.  If, as a result of an insertion,
   some of its words no longer satisfy the range property, they are silently
   removed (and freed in their entirety).
   Return T iff the Word is actually inserted.
   */
boolean insert_wordset(w,s,n,u)
    Word *w;
    WordSet *s;
    char *n;	/* name of set `s' */
    WordSet *u;	/* if !=NULL, also check uniqueness here */
{   Word *fr;
#if dbg_ws
	err("insert_wordset: %X %s",w,word_toa(w));
#endif
	if((w->m = wordmerit(w)) >= (s->cut * s->top)) {
		w->hash = hash_word(w);
		if(member_wordset(w,s) || (u!=NULL&&member_wordset(w,u)))
			/* don't insert */
			return(F);
		if(s->ws.mny==s->cap) /* at capacity */ {
			if(w->m <= min_wordmerit(s)) /* too poor */ return(F);
			else {	fr = remove_wordset(min_wordset(s),s);
				free_word_etc(fr,IsALL);
				};
			};
		put_wordset( w, s );
		if(max_wordset(s)==w) /* this has become the new maximum */ {
			while(min_wordmerit(s) < (s->cut * s->top)) {
				fr = remove_wordset(min_wordset(s),s);
				free_word_etc(fr,IsALL);
				};
			};
#if dbg_ws
		err_wordset(s,n);
#endif
		return(T);
		}
	else {
#if dbg_ws
		err_wordset(s,n);
#endif
		return(F);
		};
	}

err_wordset(s,n)
    WordSet *s;
    char *n;	/* name of set */
{   char m1[10],m2[10];
    register Word *wp,**wpp;
	fprintf( stderr,"WordSet %s: t%g c%g [%s/%X,%s/%X] m%d h%d: ",
		n,
		s->top,s->cut,
		strcpy(m1,merit_toa(max_wordmerit(s))),max_wordset(s),
		strcpy(m2,merit_toa(min_wordmerit(s))),min_wordset(s),
		s->ws.mny,s->high );
	if(s->ws.mny>0) for(wp= *(wpp=s->ws.wpa); wp!=NULL; wp= *(++wpp))
		fprintf(stderr,"%X/%s/%d ",wp,merit_toa(wp->m),wp->cs.mny);
	fprintf(stderr,"\n");
	}

translate_txtln(lp,off)
    Txtln *lp;
    Sp off;
{	lp->bx = *translate_bbx(&lp->bx,off);
	lp->basl += off.y;
	translate_chars(lp->cs,off);
	}

translate_chars(csp,off)
    Chars *csp;
    Sp off;
{   register Char *cp,**cpp;
	for(cp= *(cpp=csp->cpa); cp!=NULL; cp= *(++cpp))
		translate_char(cp,off);
	}

translate_char(cp,off)
    Char *cp;
    Sp off;
{	cp->bx = *translate_bbx(&cp->bx,off);
	translate_blobl(cp,off);
	}

translate_blobs(b,off)
    Blobs *b;
    Sp off;
{   register Blob *bp,**bpp;
	if(b->mny>0) for ( bp= *(bpp=b->bpa); bp!=NULL; bp= *(++bpp) )
		translate_blob(bp,off);
	}

translate_blobl(cp,off)
    Char *cp;
    Sp off;
{   int bi;
    register Blob *bp;
	for(bi=0,bp=cp->fi; bi<cp->bmny&&bp!=NULL; bi++,bp=bp->n)
		translate_blob(bp,off);
	}

translate_blob(bp,off)
    Blob *bp;
    Sp off;
{	bp->bx = *translate_bbx(&bp->bx,off);
	}

