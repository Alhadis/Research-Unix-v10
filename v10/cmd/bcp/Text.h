/* Copyright (c) 1989, 1990 AT&T --- All Rights Reserved.              */
/* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T.                */
/* The copyright notice does not imply actual or intended publication. */
/* AUTHORS:                                            */
/*     H. S. Baird - ATT-BL MH - first versions        */

/* Text.h - typedefs, constants, and function declarations for document-images
	(see Text.c for companion functions.)
   INCLUDES
	requires prior:
		#include "stdocr.h"
 */

#define dbg_fwrb_toa (F)	/* err("%s",R_toa()) for each record written */
#define dbg_frdb_toa (F)	/* err("%s",R_toa()) for each record read */

#define DIM_VERSION (0)		/* current version no. of Dim-file format */

#include "Bfeats.h"

#define Ident int	/* identification bits [MUST BE >=32 bits] */

#define fwri_Ident(F,V) fwri_uint4((F),(V))
#define frdi_Ident(F) frdi_uint4(F)

/* Ident bits */
/* identifies external file record type(s) */
#define IsPage		04000000000
#define IsBlock		02000000000
#define IsTxtln		01000000000
#define IsWord		00020000000
#define IsChar		00400000000
#define IsBlob		00200000000
#define IsRun		00100000000	/* should be IsRuns or IsLag */
#define IsRuns		(IsRun)
#define IsInterp	00040000000	/* Char interpretation */
#define IsBdy		00010000000
#define IsShapes	00004000000
#define IsLag		00002000000
#define IsBfeats	00001000000
#define IsSfeats	00000400000
#define IsWordInterp	00000200000
/* Runs:  set in Blob records' ids.
   At most one of these may be set in main memory.
   Only Runs_ff or Runs_g4 may be set in peripheral file format. */
#define Runs_f		00000010000	/* Runs are in list, starting *.r.f */
#define Runs_ff		00000020000	/* RunFs are in array, at *.r.ff */
#define Runs_seek	00000040000	/* Runs are still in file, at *.seek */
#define Runs_g4		00000100000	/* Runs are in CCITT Group 4 format */

/* CCITT Group 4 format in peripheral files consists of (a) an unsigned long count,
   then (b) that many bytes of Group 4 encoding exactly as in CCITT
   Recommendation T.6, except that no EOFB code is appended, since the end of
   the bitmap can be detected using the bounding box of the owning Blob.
   Instead, the last scan line of the bitmap is merely padded to the nearest
   full byte with '0' bits.  This compresses the representation by about
   a factor of 8 on average, compared to RunF/RunFS encoding. */

#define IsALL (IsPage|IsBlock|IsTxtln|IsWord|IsChar|IsBlob|IsRun|IsBdy|IsInterp|IsShapes|IsLag|IsBfeats|IsSfeats|IsWordInterp)
#define IsNONE		0


/* Enable optional debugging-support code to maintain a count of selected record
   types that are allocated using alloc_* free_* and dup_* functions.  The counts
   are in units of records, not bytes.  By design, has no effect at all on
   correctness.  */
#define ALLOC_CENSUS (0)

#if ALLOC_CENSUS

typedef struct Census {
	int Page_mny;
	int Block_mny;
	int Txtln_mny;
	int Word_mny;
	int Char_mny;
	int Blob_mny;
	int Run_mny;
	int Interp_mny;
	int Bfeats_mny;
	int BMask_mny;
	} Census;

#define Init_Census {0,0,0,0,0,0,0,0,0,0}
#if MAIN
Census empty_Census = Init_Census;
Census _CENSUS = Init_Census;
#else
extern Census empty_Census;
extern Census _CENSUS;
#endif

#define alloc_census(id,n) _CENSUS./**/id/**/_mny += (n)
#define free_census(id,n) _CENSUS./**/id/**/_mny -= (n)
#define err_census(S,C) err("%s P%d B%d l%d w%d c%d b%d r%d i%d bf%d bm%d",\
	S,(C)->Page_mny,(C)->Block_mny,(C)->Txtln_mny,(C)->Word_mny,\
	(C)->Char_mny,(C)->Blob_mny,(C)->Run_mny,(C)->Interp_mny,\
	(C)->Bfeats_mny,(C)->BMask_mny )
#define err_census_all err_census("allocated: ",&(_CENSUS))
#define err_census_rec(rp) { \
	Census *cs;  cs = (Census *)census_rec((rp)); \
	err_census(ident_toa((rp)->ident),cs); \
	}
#else

#define alloc_census(i,n)
#define free_census(i,n)
#define err_census(s,c)
#define err_census_all
#define err_census_rec(r)

#endif


/* Most record types can own an ASCII label, which is simply a
   '\0'-terminated string.  Its uses are varied. */
#define MAX_LABEL_LEN 128	/* maximum no. characters in a label string */

#if FWRI

#if dbg_fwrb_toa
#define fwrb_label(F,L) { \
	fwri_str((F),(L)); \
	err("fwrb_label: \"%s\"",(L)); \
	}
#else
#define fwrb_label(F,L) { \
	fwri_str((F),(L)); \
	}
#endif

#else

#if dbg_fwrb_toa
#define fwrb_label(F,L) { \
	fputs((L),(F)); \
	fputc('\0',(F)); \
	err("fwrb_label: \"%s\"",(L)); \
	}
#else
#define fwrb_label(F,L) { \
	fputs((L),(F)); \
	fputc('\0',(F)); \
	}
#endif

#endif

char *frdb_label();

/* A boundary is an ordered list of vertices.
   In some uses, it is assumed to close:  in this case, the first point
   is not repeated at the end.
   `vn' counts the no. vertices.  bdy_trace omits consecutive duplicates and
   compresses horizontal and vertical runs.
   `per' counts the no. of pixels on the 8-connected boundary.  An attempt has been
   made not to count consecutive duplicate pixels, but this may be buggy.
   `ren' counts the number of run-ends touched.  Each run contributes two ends,
   even if it is one pixel long.  The sum of ren-counts among all bdys for a blob
   should equal exactly twice the no. runs.
   Note that vn<=per and vn<=2*ren, and (probably) ren<=per.
   A ``smoothed'' version (courtesy of John Hobby) may be given in s[],
   expressed in fractional pixels.
   */
typedef struct Bdy {
	Ident ident;	/* shows type of boundary */
	Bbx bx;		/* bounding box (usually relative to blob's bx.a) */
	long per;	/* no. pixels in 8-connected perimeter */
	int ren;	/* no. run-ends touched */
	int vn;		/* no. distinct vertices in v[] */
	Sp *v;		/* array of vn+1 vertices, v[0]==v[vn] (malloc space) */
	short fr;	/* fraction of pixels used in smoothed outline */
	int sn;		/* no. distinct smoothed vertices in s[] */
	Sp *s;		/* array of sn+1 vertices, s[0]==s[sn] (malloc space) */
	int an;		/* no. vertices in polygonal approximation */
	Sp **ap;	/* approx'n: array of an ptrs into v[] (in malloc space) */
	float err;	/* error tolerance used for approximation */
	int hn;		/* no. vertices in convex hull of polygonal approx'n */
	Sp ***hpp;	/* convex hull: array of hn ptrs into ap[] (malloc) */
	struct Bdy *n;	/* for use when a member of a linked-list */
	} Bdy;

#define Init_Bdy {IsBdy,Init_Bbx,0,0,0,NULL,1,0,NULL,0,NULL,0.0,0,NULL,NULL}
#if MAIN
Bdy empty_Bdy = Init_Bdy;
#else
extern Bdy empty_Bdy;
#endif

#define Bdy_verts	00000000001	/* vertices */
#define Bdy_approx	00000000002	/* polygonal approx'n */
#define Bdy_hull	00000000004	/* convex hull */
#define Bdy_ALL		(Bdy_verts|Bdy_approx|Bdy_hull)
#define Bdy_ccw		00000000100	/* winding order is counter-clockwise*/
#define Bdy_half	00000000200	/* uses half-pixel boundary points */

/* A boundaries-set is an ordered list of boundaries.
   In some uses, they are used to enclose a connected region:  in this case,
   all are closed, and the first is conventionally the exterior and the others
   are interior boundaries.  The interior is then always to the left
   of the boundary:  that is, the exterior boundary is oriented counter-
   clockwise, and the interior boundaries clockwise */
typedef struct Bdys {
	int mny;	/* no. boundaries */
	long per;	/* perimeter of all bdys */
	Bdy *b;		/* array of boundaries (malloc space) (sometimes first) */
	} Bdys;

#define Init_Bdys {0,0,NULL}
#if MAIN
Bdys empty_Bdys = Init_Bdys;
#else
extern Bdys empty_Bdys;
#endif

/* A boundary edge is an ordered pair of vertices along a boundary.  It implicitly
   describes an 8-connected sequence of pixels from a to b, inclusive.  Also
   used for straight-line approximations to the set of pixels, and the convex hull
   of such an approximation. */
typedef struct BdyEdge {
	Bdy *byp;	/* boundary to which it belongs */
	Sp *ap,*bp;	/* ptrs into byp->v */
	long per;	/* perimeter:  no. 8-connected pixels */
	Pp ctr;		/* centroid */
	Radians ang;	/* ls-fitted angle (directed roughly from a to b) */
	Pp a;		/* endpoints a & b (with sub-pixel precision) */
	Pp b;
	} BdyEdge;

#define Init_BdyEdge {NULL,NULL,NULL,0,Init_Zero_Pp,0.0,Init_Zero_Pp,Init_Zero_Pp}
#if MAIN
BdyEdge empty_BdyEdge = Init_BdyEdge;
#else
extern BdyEdge empty_BdyEdge;
#endif

/* Ordered set of BdyEdges */
typedef struct BdyEdges {
	int mny;
	BdyEdge **pa;	/* NULL-terminated array of pointers to BdyEdges */
	} BdyEdges;

#define Init_BdyEdges {0,NULL}
#if MAIN
BdyEdges empty_BdyEdges = Init_BdyEdges;
#else
extern BdyEdges empty_BdyEdges;
#endif

/* Moments of a region (or boundary-list) of pixels */

typedef struct Moments {
	/* 0th moment */
	int M00;	/* area: sum of 1 */
	/* 1st moments */
	int M10;	/* sum of xi */
	int M01;	/* sum of yi */
	Pp c;		/* centroid: M10/M00, M01/M00 */
	/* 2nd moments (relative to centroid) */
	float M20;	/* sum of rxi*rxi */
	float M11;	/* sum of rxi*ryi */
	float M02;	/* sum of ryi*ryi */
	Radians a;	/* orientation angle, in [-PI/2,PI/2) (radians) */
	Pp d;		/* directional vector of principal axis */
	} Moments;

#define Init_Moments {0,0,0,{0.0,0.0},0.0,0.0,0.0,0.0,{0.0,0.0}}
#if MAIN
Moments zero_Moments = Init_Moments;
#else
extern Moments zero_Moments;
#endif

/* functions in Text.c */
Bdy *alloc_bdy();
Bdys *alloc_bdys();

/* functions in Bdy.c */
Bdys *dup_bdys_etc();
Bdy *dup_bdy_etc();
Bdys *boundaries();
char *moments_toa();
Moments *bdy_moments();
boolean fit_bdyedge();
BdyEdge *dup_bdyedge();
BdyEdge *append_bdyedge();
remove_bdyedge();
free_bdyedges();
BdyEdges *dup_bdyedges();

/* Each Run is initially inserted into a `line set', owned by its scan Line.
   Later, as connections are discovered, it joins a `tree set'.
   When the forest of trees of which it is a part is finally complete, the Run
   is removed from its `line set', and added to a `blob set'.
   */

typedef struct Run {	/* internal (main memory) record */
	Scoor y, xs, xe;	/* coordinates of black interval (y,[xs,xe]) */
	struct Run *n;		/* line & blob sets: next Run */
	unsigned short ad, bd;	/* tree set: above,below degrees (no. conn'd)*/
	struct Run *ac, *bc;	/* tree set: above,below leftmost connections */
	union {	struct Tree *o;	/* tree set: owner Tree */
		int no;		/* blob set: sequence no. 0,1,... in set */
		} u;		/* (overlain fields) */
	} Run;

#define Init_Run {0,0,0,NULL,0,0,NULL,NULL}
#if MAIN
Run empty_Run = Init_Run;
#else
extern Run empty_Run;
#endif

/* Peripheral file format.  `ac' and `bc' are relative to the position
   of this run in the canonical run order. */
typedef struct RunF {	/* external (peripheral file) record (full size) */
	Scoor y, xs, xe;	/* coordinates of black interval (y,[xs,xe]) */
	unsigned short ad, bd;	/* above,below degrees */
	unsigned short ac, bc;	/* above,below leftmost connections (indices) */
	} RunF;

#define Init_RunF {0,0,0,0,0,0,0}
#if MAIN
RunF empty_RunF = Init_RunF;
#else
extern RunF empty_RunF;
#endif

#define fwri_RunF(F,P) { \
	fwri_Scoor((F),(P)->y); \
	fwri_Scoor((F),(P)->xs); \
	fwri_Scoor((F),(P)->xe); \
	fwri_uint2((F),(P)->ad); \
	fwri_uint2((F),(P)->bd); \
	fwri_uint2((F),(P)->ac); \
	fwri_uint2((F),(P)->bc); \
	}

#define frdi_RunF(F,P) ( feof(F)? 0 : ( \
	(P)->y=frdi_Scoor(F), \
	(P)->xs=frdi_Scoor(F), \
	(P)->xe=frdi_Scoor(F), \
	(P)->ad=frdi_uint2(F), \
	(P)->bd=frdi_uint2(F), \
	(P)->ac=frdi_uint2(F), \
	(P)->bc=frdi_uint2(F), \
	(ferror(F)? -errno: 1) ) )

/* In `well-behaved' text, the overwhelming majority of Blobs are small
   enough that all their Runs can be encoded using character data fields,
   a factor of two saving, which is important since a dense IEEE proceedings
   page blob file would otherwise require 2.3Mbytes */
typedef struct RunFS {	/* external (peripheral file) record (small size) */
	unsigned char y, xs, xe;/* coordinates of black interval (y,[xs,xe]) */
	unsigned char ad, bd;	/* above,below degrees */
	unsigned char ac, bc;	/* above,below leftmost connections (indices) */
	} RunFS;

#define Init_RunFS {0,0,0,0,0,0,0}
#if MAIN
RunFS empty_RunFS = Init_RunFS;
#else
extern RunFS empty_RunFS;
#endif

#define fwri_RunFS(F,P) { \
	fwri_uint1((F),(P)->y); \
	fwri_uint1((F),(P)->xs); \
	fwri_uint1((F),(P)->xe); \
	fwri_uint1((F),(P)->ad); \
	fwri_uint1((F),(P)->bd); \
	fwri_uint1((F),(P)->ac); \
	fwri_uint1((F),(P)->bc); \
	}

#define frdi_RunFS(F,P) ( feof(F)? 0: ( \
	(P)->y=frdi_uint1(F), \
	(P)->xs=frdi_uint1(F), \
	(P)->xe=frdi_uint1(F), \
	(P)->ad=frdi_uint1(F), \
	(P)->bd=frdi_uint1(F), \
	(P)->ac=frdi_uint1(F), \
	(P)->bc=frdi_uint1(F), \
	(ferror(F)? -errno: 1) ) )

/* Set of runs.  PROPOSED NEW FORMAT.  Not yet incorporated widely. */
typedef struct Runs {
	Ident ident;	/* IsRuns & Runs_fi, Runs_ff, Runs_fs, or Runs_sk flags */
	int mny;	/* no. runs */
	union {	/* access to runs */
		struct Run *fi;	  /* first run of singly-linked list */
		struct RunF *ff;  /* top of RunF[mny] array */
		struct RunFS *fs; /* top of RunFS[mny] array */
		long sk;	  /* seek(F,seek,0) will find them in file F */
		} r;
	} Runs;

#define Init_Runs {IsRuns,0}	/* NOTE: can't initialize union */
#if MAIN
Runs empty_Runs = Init_Runs;
#else
extern Runs empty_Runs;
#endif

/* INTERNAL management */

#if !MAIN
extern
#endif
struct {
	int incr;		/* size of each pool[i] */
	int pools;		/* no. of pools allocated */
	Run **pool;		/* malloc space Run pool[pools][0..incr-1] */
	int next;		/* the next avail Run is:  pool[pools-1][next] */
	Run *free;		/* head of free lifo list (NULL if none) */
	Run *cur;		/* most-recently allocated Run */
	int total;		/* total no. ever allocated */
	boolean dbg;
	} _RunPool
#if MAIN
	= {0,0,NULL,0,NULL,NULL,0,F}
#endif
;

/* Run management routines (Text.c) */
boolean	alloc_run_pool();
	free_run_pool();
Run	*hard_alloc_run();
	err_run();
	err_runb();
	err_runf();
	err_runfs();
	err_run_stats();

/* Allocate a Run from the RunPool (returns (Run *)) -- mostly inline */
#define alloc_run() ( _RunPool.total++, (_RunPool.free!=NULL)? \
	(_RunPool.cur=_RunPool.free,_RunPool.free=_RunPool.cur->n, \
		*(_RunPool.cur)=empty_Run,_RunPool.cur): \
	( (_RunPool.next<_RunPool.incr)? \
	  (_RunPool.cur=_RunPool.pool[_RunPool.pools-1]+(_RunPool.next++), \
		*(_RunPool.cur)=empty_Run,_RunPool.cur): \
	  hard_alloc_run() ) )

/* Free a Run back into the RunPool -- entirely inline */
#define free_run(rp) { (rp)->n = _RunPool.free; _RunPool.free = (rp); }

/* EXTERNAL file format:
   If BlobF.runs is zero, then conventionally the Runs have simply been omitted.
   The RunF.y, RunF.xs, & RunF.xe coordinates are offsets from BlobF.bx.a 
   (their blob's left-top corner).  RunF.ac & RunF.bc index into an array of
   only those RunF records belonging to the current BlobF, in ascending
   lexicographic order on (RunF.y,RunF.xs) -- so that they are in the range
   [0,BlobF.runs-1].
 IMPROVEMENTS:
  */

/* some subroutines are too lazy to handle indefinitely large blobs */
#define Runs_Max 10000	

/* A Blob is (formally) a maximal 8-connected set of black pixels.
   The connectivity algorithm finds them in strictly increasing order on
   (y,xe) of its Run with highest (y,xe).
   */

typedef struct Blob {	/* internal (main memory) record */
	Ident ident;	/* identification bits */
	Seq no;		/* blob sequence no */
	Bbx bx;
	long area;
	long per;
	struct Blob *n;	/* free set: next blob */
	Merit m;	/* Only used locally (not for peripheral file) */
	Runs *rsp;	/* runs (not yet used) */
	Bdys *bdsp;	/* boundaries (in malloc space); NULL if none */
	/* presently in use (but planned to be replaced by Runs) */
	int runs;
	union {	/* to find runs */
		struct Run *f;		/* blob set: first run */
		struct RunF *ff;	/* top of RunF array */
		long seek;		/* seek(f,seek,0) will find them */
		} r;
	} Blob;

#define Init_Blob {IsBlob,0,Init_Bbx,0,0,NULL,0.0,NULL,NULL,0,}
#if MAIN
Blob empty_Blob = Init_Blob;
#else
extern Blob empty_Blob;
#endif

typedef struct Blobs {	/* Blob set */
	int mny;	/* the number of pointers in set */
	Blob **bpa;	/* pts to NULL-terminated array[mny+1] of pointers */
	} Blobs;

#define Init_Blobs {0,NULL}
#if MAIN
Blobs empty_Blobs = Init_Blobs;
#else
extern Blobs empty_Blobs;
#endif

/* Singly-linked list of Blobs.  Only forward `next' links Blob.n are used. */
typedef struct Blobl {	/* Blob list */
	int mny;	/* the number in set */
	Blob *fi;	/* to first */
	Blob *la;	/* to last */
	} Blobl;

#define Init_Blobl {0,NULL,NULL}
#if MAIN
Blobl empty_Blobl = Init_Blobl;
#else
extern Blobl empty_Blobl;
#endif

typedef struct BlobF {	/* external file format */
	Ident ident;	/* identification bits: IsBlob must be set */
	Bbx bx;
	long area;
	long per;
	int runs;	/* no. runs to follow */
	short bdys;	/* no. bdys to follow */
	} BlobF;

/* Blob identification bits */
#define Blob_lm		00000000001	/* touches left margin */
#define Blob_rm		00000000002	/* touches right margin */
#define Blob_tm		00000000004	/* touches top margin */
#define Blob_bm		00000000010	/* touches bottom margin */
#define Blob_chopt	00000000020	/* chopped (at the top) */
#define Blob_chopb	00000000040	/* chopped (at the bottom) */
#define Blob_chopl	00000002000	/* chopped (at the left) */
#define Blob_chopr	00000004000	/* chopped (at the right) */
#define Blob_small	00000000200	/* its runs (can be) compressed x2 */
#define Blob_local	00000000400	/* unassigned:  avail for local pgm use */

/* INTERNAL management: */

int hi_blob_no;		/* current highest blob no */

/* Blobs are allocated from a pool of free ones */
int blob_max;
Blob *blob_pool;
Blob blob_fr;		/* head of list of free blobs */
int blob_fr_mny;
int blob_hi;		/* high-water mark in blob pool */
int blob_chopped;	/* total no. of blobs that were chopped */
boolean blob_debug;	/* debug traces? */

/* EXTERNAL file format:
   A Blob file consists of an arbitrary number of:
       BlobF record, followed by BlobF.runs instances of:
           RunF record
   If BlobF.runs is zero, then conventionally the Runs have simply been omitted.
   The RunF.y, RunF.xs, & RunF.xe coordinates are relative offsets from BlobF.bx.a 
   (their blob's left-top corner).  RunF.ac & RunF.bc index into an array of
   only those RunF records belonging to the current BlobF, in ascending
   lexicographic order on (RunF.y,RunF.xs) -- so that they are in the range
   [0,BlobF.runs-1].  If ad(or, bd)==0, the ac(or, bc) is undefined (conn sets
   them conventionally to 0).
   */

/* Blob management routines (conBlob.c) */
Blob	*alloc_blob();
	free_blob();
boolean	alloc_blob_pool();
	free_blob_pool();
Blob	*alloc_pool_blob();
	free_pool_blob();
	out_blob();
        fwrb_blob_etc();
boolean frdb_blob_etc();
boolean	frdb_runfs();
	err_blob();
	err_blob_runs();
	err_blob_runfs();
	err_blob_briefly();
	err_blobf();
	err_blob_stats();
boolean	blob_small();

/* Compute height-above-baseline in ems of Char *cp w.r.t. Txtln *lp,
   on a page of y-resolution res.  The txtln's `basl' & `size' must be set up. */
#define char_bhgt(cp,lp,res) \
	((((cp)->bx.b.y - (lp)->basl)/(double)(res)*INS_PER_PT*(lp)->size))

/* an Interpretation of a Char */
typedef struct Interp {
	Ident ident;
	struct Cl *clp;		
	struct Class *clsp;
	ClassId ci;		/* class id (font, size, name, variant) */
	Merit mshap;		/* shape merit in [0,1] */
	Pts size;		/* implied text size */
	Merit msize;		/* size merit in [0,1] */
	Scoor basl;		/* implied absolute baseline location */
	Merit mbhgt;		/* height-above-baseline merit in [0,1] */
	Merit m;		/* match merit (due to mshap, msize, & mbhgt) */
	Prob p;			/* approximate probability */
	struct Interp *n;	/* next in singly-linked list */
	} Interp;

#define Init_Interp {IsInterp,NULL,NULL,Init_ClassId,0.0,0.0,0.0,0,0.0,0.0,0.0,NULL}
#if MAIN
Interp empty_Interp = Init_Interp;
#else
extern Interp empty_Interp;
#endif

/* Interp.ident flags: */
#define Interp_spelled	00000000001	/* has passed a spelling check */

#define free_interp(i) {free((i)); free_census(Interp,1); }

typedef struct InterpF {
	Ident ident;
	ClassId ci;		/* class id (font, size, name, variant) */
	Merit mshap;		/* shape merit */
	Pts size;		/* implied text size (in points) */
	Merit msize;		/* size merit */
	Scoor basl;		/* implied baseline location */
	Merit mbhgt;		/* height-above-baseline merit */
	Merit m;		/* match merit (due to shp, siz, hgt) */
	} InterpF;

/* a list of interpretations */
typedef struct Interpl {
	short mny;		/* no. in list */
	struct Interp *fi;	/* first in list (mny==0 -> fi==NULL) */
	} Interpl;

#define Init_Interpl {0,NULL}
#if MAIN
Interpl empty_Interpl = Init_Interpl;
#else
extern Interpl empty_Interpl;
#endif

/* a set of interpretations */
typedef struct Interps {
	short mny;		/* no. in set */
	struct Interp **pa;	/* NULL-terminated array of ptrs (malloc spc) */
	Merit m;		/* combined merit (normalized product) */
	} Interps;

#define Init_Interps {0,NULL,0.0}
#if MAIN
Interps empty_Interps = Init_Interps;
#else
extern Interps empty_Interps;
#endif

typedef struct Shapes {
	short mny;	/* no. items in set */
	short alloc;	/* no. items that can fit in array (alloc>=mny) */
	Nb_s *sa;	/* ptr to contiguous array (malloc space) */
	} Shapes;

#define Init_Shapes {0,0,NULL}
#if MAIN
Shapes empty_Shapes = Init_Shapes;
#else
extern Shapes empty_Shapes;
#endif

#define SH_INCR (20)	/* Shapes are allocated by this increment */

#define init_sh(sh) { \
	(sh)->alloc = SH_INCR; \
	if(((sh)->sa=(Nb_s *)malloc((sh)->alloc*sizeof(Nb_s)))==NULL) \
		abort("can't alloc sh->sa[%d]",(sh)->alloc); \
	(sh)->mny = 0; \
	}

#define add_sh(s,sh) { \
	if((sh)->mny==(sh)->alloc) { \
		(sh)->alloc += SH_INCR; \
		if(((sh)->sa=(Nb_s *)realloc((sh)->sa,(sh)->alloc*sizeof(Nb_s)))==NULL) \
			abort("can't alloc (sh)->sa[%d]",(sh)->alloc); \
		}; \
	(sh)->sa[(sh)->mny++] = *(s); \
	}

/* Parameters governing the pseudo-random generation of a Char image
   using a 1st-order statistical model of imaging. */ 
typedef struct RanParms {
	short res_x;	/* -r resolution (scanner pels/inch) */
	short res_y;	/*    (for now, equal to res_x) */
	Pts size;	/* -p size of text */
	Radians skew;	/* -a skew angle */
	Ems bhgt;	/* -b height above baseline */
	float blur;	/* -e blurring std err (scanner pels) */
	float jitter;	/* -j jitter std err (scanner pels) */
	float kern;	/* -k kerning std err (scanner pels) */
	float speckle;	/* -s pel-wise additive noise std err (scanner pels) */
	float thresh;	/* -t threshold for binarization */
	float xscale;	/* -x horizontal scaling (dimensionless) */
	float yscale;	/* -y vertical scaling (dimensionless) */
	} RanParms;
#define Init_RanParms {0,0,0.0,0.0,0.0,0.7,0.0,0.0,0.125,0.25,1.0,1.0}
#if MAIN
RanParms empty_RanParms = Init_RanParms;
#else
extern RanParms empty_RanParms;
#endif

RanParms *alloc_ranparms();
RanParms *dup_ranparms();
char *ranparms_toa();
RanParms *ato_ranparms();
fwrb_ranparms();
int frdb_ranparms();

/* Char - a character:  isolated, elementary symbol of the writing system;
   linguists might call this a `graph' */
typedef struct Char {
	Ident ident;		/* feature bits */
	Bbx bx;
	Scoor csp;		/* space before character in Txtln (abs. coords) */
	long area;		/* no. pixels */
	long per;		/* perimeter (all bdys) */
	Scoor basl;		/* baseline (absolute coordinates, local copy) */
	/* next should be Blobl */
	int bmny;		/* no. blobs in Char */
	struct Blob *fi;	/* 1st in list (p.n ptrs) (bmny==0 -> fi==NULL) */
	Pval *sfv;		/* scalar-features (SFv) */
	Shapes sh;		/* set of shapes (size- & loc'n-invariant) */
	Bfeats *bfsp;		/* binary features */
	RanParms *rp;		/* randomizing parameters */
	Interpl il;		/* interpretations */
	char *l;		/* label (ASCII string in malloc space) */
	} Char;

#define Init_Char {IsChar,Init_Bbx,0,0L,0L,Scoor_MIN,0,NULL,NULL,Init_Shapes,NULL,NULL,Init_Interpl,NULL}
#if MAIN
Char empty_Char = Init_Char;
#else
extern Char empty_Char;
#endif

/* Char.ident flags: */
#define Char_spelled	00000000001	/* Its 1st Interp is in correct spelling */
#define Char_confused	00000000002	/* The classifier may have confused this */
#define Char_termhyp	00000000004	/* a terminal hyphen of its Word */
#define Char_omit	00000000010	/* can be omitted */
#define Char_label	00000000020	/* has an ASCII string label */
#define Char_ranparms	00000000040	/* has RanParms */
#define Char_split	00000000100	/* resulted from splitting a Char */
#define Char_merged	00000000200	/* resulted from merging Chars */

typedef struct Chars {
	int mny;	/* mny==0 -> cpa==NULL */
	Char **cpa;	/* pts to NULL-term'd array of ptrs (in malloc space) */
	} Chars;

#define Init_Chars {0,NULL}
#if MAIN
Chars empty_Chars = Init_Chars;
#else
extern Chars empty_Chars;
#endif

/* CharF - Char external file format */
typedef struct CharF {
	Ident ident;		/* feature bits: IsChar must be set */
	Bbx bx;
	Scoor csp;		/* space before character in Txtln (abs. coords) */
	long area;
	long per;
	short bmny;		/* no. blobs to follow */
	short imny;		/* no. interpretations (follows immediately) */
	short sfmny;		/* no. scalar features (follows immediately) */
	short shmny;		/* no. shape features (follows immediately) */
	short bfmny;		/* no. binary features (follows immediately) */
	/* if ident&Char_ranparms, RanParms follows CharF */
	/* if ident&Char_label, label follows CharF ('\0'-terminated string) */
	} CharF;

Char *alloc_char();		/* in Text.c */
Char *append_char();
Char *insert_char();
Char *insert_char_word();
Char *dup_char();
Char *dup_char_etc();
Chars *append_chars();
Chars *dup_chars_etc();

typedef struct Words {
	int mny;		/* mny==0 -> wpa==NULL */
	struct Word **wpa;	/* pts to NULL-term'd array of ptrs */
	} Words;

#define Init_Words {0,NULL}
#if MAIN
Words empty_Words = Init_Words;
#else
extern Words empty_Words;
#endif

/* Word - one or more Chars lying in a textline close together.
   wsp (word space) is always >=0 and is scaled by xheight (of Txtln) */
typedef struct Word {
	Ident ident;		/* feature bits */
	Bbx bx;
	float wsp;		/* space before word (multiple of wst*em) */
	Merit m;		/* Word merit (function of its Char's merits) */
	Prob p;			/* probability */
	Words ws;		/* set of alternative segmentations */
	Chars cs;
	Blobs bs;
	char *l;		/* label (ASCII string in malloc space) */
	int hash;		/* hash key for fast equality checking */
	} Word;

#define Init_Word {IsWord,Init_Bbx,0.0,0.0,0.0,Init_Words,Init_Chars,Init_Blobs,NULL,0}
#if MAIN
Word empty_Word = Init_Word;
#else
extern Word empty_Word;
#endif

/* Word-interpretation (as printable ASCII string).
   All string fields must point to malloc-space strings.
   */
typedef struct WordInterp {
	Ident ident;	/* identifies word type */
	char *s;	/* entire string = pp+by+po+ps */
	char *pp;	/* punctuation prefix */
	char *by;	/* body of word */
	char *po;	/* possessive ('s 'S) or negative (n't N'T) suffix */
	char *ps;	/* punctuation suffix */
	} WordInterp;

#define Init_WordInterp {IsWordInterp,NULL,NULL,NULL,NULL,NULL}
#if MAIN
WordInterp empty_WordInterp = Init_WordInterp;
#else
extern WordInterp empty_WordInterp;
#endif

WordInterp *dup_wordinterp_etc();

/* Word.ident & WordInterp.ident flags: */
#define Word_spelled	00000000001	/* by spells correctly */
#define Word_wf		00000000002	/* s is well-formed */
#define Word_ok		00000000004	/* s is ok ("acceptable") */
#define Word_numeric	00000000010	/* by is numeric */
#define Word_initcap	00000000020	/* by has initial capital */
#define Word_allcaps	00000000040	/* by is all caps */
#define Word_hyphens	00000000100	/* by has imbedded hyphens */
#define Word_slashes	00000000200	/* by has imbedded slashes */
#define Word_endsent	00000000400	/* end of sentence: ps has .!? */
#define Word_termhyp	00000001000	/* some interpretation ends with hyphen */
#define Word_label	00000002000	/* has an ASCII string label */
#define Word_allalp	00000004000	/* s is all alphabetic */
#define Word_bodalp	00000010000	/* by is all alphabetic */

/* WordF - Word external file format */
typedef struct WordF {
	Ident ident;		/* feature bits: IsWord must be set */
	Bbx bx;
	float wsp;		/* space before word (multiple of thr) */
	float m;		/* merit */
	short wmny;		/* no. Words (in Word.ws) to follow this Word */
	short cmny;		/* no. Chars to follow this Word */
	short bmny;		/* no. Blobs to follow this Word */
	/* if ident&Word_label, label follows WordF ('\0'-terminated string) */
	} WordF;

/* constant pitch model for a Txtln */
typedef struct ConstPitch {
	Ems w;		/* Character pitch */
	Scoor o;	/* origin (one of the character break points) */
	float r;	/* max/min autocorrelation ratio - the larger the better */
	} ConstPitch;

#define Init_ConstPitch {0.0,0,0.0}
#if MAIN
ConstPitch empty_ConstPitch = Init_ConstPitch;
#else
extern ConstPitch empty_ConstPitch;
#endif

typedef struct Txtlns {
	short mny;
	struct Txtln **lpa;	/* to array of Txtln's (if mny==0, lpa==NULL) */
	} Txtlns;

#define Init_Txtlns {0,NULL}
#if MAIN
Txtlns empty_Txtlns = Init_Txtlns;
#else
extern Txtlns empty_Txtlns;
#endif

/* Text Line */
#define Txtln_label	00000000004	/* has an ASCII string label */
#define Txtln_size	00000000002	/* dominant text size chosen */
#define Txtln_basl	00000000001	/* dominant baseline chosen */

typedef struct Txtln {
	Ident ident;
	Bbx bx;
	Pts size;	/* dominant text size (0 means unknown) */
	Scoor basl;	/* dominant baseline (absolute y coordinate) */
	short *proj;	/* ptr to malloc space projection array */
	ConstPitch *cp;	/* ptr to malloc space constant-pitch model */
	Merit m;	/* merit */
	Txtlns ls;	/* alternative Txtln segmentations */
	Words ws;	/* sorted asc. on x */
	Chars cs;	/* sorted asc. on bx.a.x */
	Blobs bs;	/* misc. non-char blobs */
	char *l;	/* label (ASCII string in malloc space) */
	} Txtln;

#define Init_Txtln {IsTxtln,Init_Bbx,0.0,0,NULL,NULL,0.0,Init_Txtlns,Init_Words,Init_Chars,Init_Blobs,NULL}
#if MAIN
Txtln empty_Txtln = Init_Txtln;
#else
extern Txtln empty_Txtln;
#endif

/* Text Line */
typedef struct TxtlnF {
	Ident ident;	/* IsTxtln must be set */
	Bbx bx;
	Scoor basl;	/* baseline (absolute y coordinate) */
	Pts size;	/* text size (<=0.0 means none is known) */
	short pmny;	/* no. shorts in projection array to follow */
	float m;	/* merit */
	short lmny;	/* no. alternative txtlns to follow */
	short wmny;	/* no. words to follow */
	short cmny;	/* no. chars to follow */
	int bmny;	/* no. blobs to follow */
	/* if ident&Txtln_label, label follows TxtlnF ('\0'-terminated string) */
	} TxtlnF;

/* blocks of text */
typedef struct Blocks {
	short mny;		/* if mny==0, bpa==NULL */
	struct Block **bpa;	/* to NULL-term'd array of ptrs */
	} Blocks;

#define Init_Blocks {0,NULL}
#if MAIN
Blocks empty_Blocks = Init_Blocks;
#else
extern Blocks empty_Blocks;
#endif

/* block of text */
typedef struct Block {
	Ident ident;
	Bbx bx;		/* bounding box of block */
	Radians skew;	/* skew angle (as correction to Page.skew) */
	Radians shear;	/* shear angle (as correction to Page.shear) */
	Ems wst;	/* word-space threshhold */
	Merit m;	/* merit */
	Blocks bks;	/* Blocks nested within this one */
	Txtlns ls;
	Words ws;
	Chars cs;
	Blobs bs;
	char *l;	/* label (ASCII string in malloc space) */
	} Block;

#define Init_Block {IsBlock,Init_Bbx,0.0,0.0,0.0,0.0,Init_Blocks,Init_Txtlns,Init_Words,Init_Chars,Init_Blobs,NULL}
#if MAIN
Block empty_Block = Init_Block;
#else
extern Block empty_Block;
#endif

/* Ident bits for Blocks */
#define Block_wst	00000000001	/* word-space-thresh set up */
#define Block_label	00000000002	/* has an ASCII string label */

#define Block_mb (1)

/* block of text */
typedef struct BlockF {
	Ident ident;	/* IsBlock must be set */
	Bbx bx;
	Ems wst;	/* word-space threshold */
	float skew;
	float shear;
#if Block_mb
	float m;
	short bkmny;
#endif
	short lmny;
	short wmny;
	int cmny;
	int bmny;
	/* if ident&Block_label, label follows BlockF ('\0'-terminated string) */
	} BlockF;

/* page */
typedef struct Page {
	Ident ident;
	Bbx bx;			/* extreme indices in pixels */
	short res_x;		/* resolution in pixels/inch:  x & y */
	short res_y;
	Radians skew;		/* skew angle */
	Radians shear;		/* shear correction */
	Blocks bks;		/* blocks */
	Txtlns ls;		/* textlines (those not in any block) */
	Words ws;		/* words (not in any textline) */
	Chars cs;		/* chars (not in any word) */
	Blobs bs;		/* blobs (not in any char) */
	char *l;		/* label (ASCII string in malloc space) */
	} Page;

#define Init_Page {IsPage,Init_Bbx,0,0,0.0,0.0,Init_Blocks,Init_Txtlns,Init_Words,Init_Chars,Init_Blobs,NULL}
#if MAIN
Page empty_Page = Init_Page;
#else
extern Page empty_Page;
#endif

#define Page_label	00000000001	/* has a label */

/* Pages of text */
typedef struct Pages {
	unsigned short mny;	/* if mny==0, pa==NULL */
	struct Page **pa;	/* to NULL-term'd array of ptrs */
	} Pages;

#define Init_Pages {0,NULL}
#if MAIN
Pages empty_Pages = Init_Pages;
#else
extern Pages empty_Pages;
#endif

typedef struct PageF {
	Ident ident;		/* IsPage bit must be set */
	short res_x,res_y;	/* resolution in pixels/inch:  x & y */
	Bbx bx;			/* extreme indices in pixels */
	float skew;		/* original page skew angle, radians */
	float shear;		/* original page shear angle, radians */
	short bkmny;		/* no. blocks */
	short lmny;		/* no. textlines (not in any block) */
	short wmny;		/* no. words (not in any textline) */
	int cmny;		/* no. chars (not in any word) */
	int bmny;		/* no. blobs (not in any char)) */
	/* if ident&Page_label, label follows PageF ('\0'-terminated string) */
	} PageF;

#define Page_new	00000000001	/* Page is in ``new'' format */

/* Each Dim-file begins with a Doc record */
typedef struct Doc {
	unsigned short version;		/* file format version number */
	Pages ps;
	char *l;		/* ASCII label */
	} Doc;

#define Init_Doc {0,Init_Pages,NULL}
#if MAIN
Doc empty_Doc = Init_Doc;
#else
extern Doc empty_Doc;
#endif

char *ident_toa();
Ident cto_ident();
Ident cto_flag();
char *merit_toa();
Page *alloc_page();
char *page_toa();
Page *dup_page();
Page *dup_page_etc();
Block *alloc_block();
char *block_toa();
Block *dup_block();
Block *dup_block_etc();
Block *append_block();
Blocks *dup_blocks_etc();
ConstPitch *alloc_constpitch();
char *constpitch_toa();
Txtln *alloc_txtln();
char *txtln_toa();
Word *alloc_word();
char *word_toa();
boolean eq_word();
int hash_word();
Char *alloc_char();
char *char_toa();
Pp *char_centroid();
Char *char_of_blob();
char *interp_toa();
char *blob_toa();
Pp *blob_centroid();
char *runf_toa();
char *runfs_toa();
char *pp_toa();
char *bdyedge_toa();
Blob *dup_blob();
Blob *dup_blob_etc();
Blob *dup_blobl_etc();
Blob *runs_to_runs();
Blobs *dup_blobs_etc();
Blobs *blobl_to_blobs();
Interp *alloc_interp();
Interp *dup_interp();
Interpl *dup_interpl_etc();
Interps *dup_interps_etc();
Word *append_word();
Word *insert_word();
Word *insert_word_txtln();
Word *dup_word();
Word *dup_word_etc();
Words *dup_words_etc();
Txtln *dup_txtln();
Txtln *dup_txtln_etc();
Txtlns *dup_txtlns_etc();
Block *dup_block();
Block *dup_block_etc();
Radians add_ang();
Radians subtract_ang();

/* in-line macroes */

/* Merge the `source' Bbx into the `destination' Bbx, expanding the dest Bbx
   as required.  The source Bbx is unmodified.  Usage:
	merge_bbx(s,d)
	    Bbx *s,*d;
*/
#define merge_bbx(s,d) { \
	if((s)->a.x < (d)->a.x) (d)->a.x = (s)->a.x; \
	if((s)->a.y < (d)->a.y) (d)->a.y = (s)->a.y; \
	if((s)->b.x > (d)->b.x) (d)->b.x = (s)->b.x; \
	if((s)->b.y > (d)->b.y) (d)->b.y = (s)->b.y; \
	}

/* Experimental implementation of a data structure for maintaining a set
   of distinct Words whose merit falls within a dynamically-varying range.
   This implementation is worst-case quadratic time.
   BUGS:  insert_wordset shouldn't duplicate the word.
   */

#define dbg_ws (0)	/* if !=0, enable WordSet debugging tracing */

typedef struct WordSet {
	double cut;	/* cut-fraction */
	int cap;	/* capacity: maximum number permitted at any time */
	double top;	/* maximum merit seen since allocation */
	Words ws;	/* sorted by top-choice merit */
	Word *max,*min;	/* maximum/minimum entries currently in ws */
	int high;	/* high-water:  max. no. entries in history */
	} WordSet;
#define Init_WordSet {1.0,INT_MAX,0.0,Init_Words,NULL,NULL,0}
#if MAIN
WordSet empty_WordSet = Init_WordSet;
#else
extern WordSet empty_WordSet;
#endif

#define size_wordset(s) ((s)->ws.mny)
#define top_wordset(s) ((s)->top)
#define max_wordset(s) ((s)->max)
#define min_wordset(s) ((s)->min)
#define max_wordmerit(s) ((max_wordset((s))!=NULL)? (max_wordset((s)))->m: 0.0)
#define min_wordmerit(s) ((min_wordset((s))!=NULL)? (min_wordset((s)))->m: 0.0)

Merit wordmerit();
WordSet *alloc_wordset();
boolean insert_wordset();
Word *remove_wordset();
int free_wordset_etc();
err_wordset();
