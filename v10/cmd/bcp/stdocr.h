/* Copyright (c) 1989, 1990 AT&T --- All Rights Reserved.              */
/* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T.                */
/* The copyright notice does not imply actual or intended publication. */
/* AUTHORS:                                            */
/*     H. S. Baird - ATT-BL MH - first versions        */
/* stdocr.h -- conventional OCGR constants, typedefs, and file formats
 NOTE: sensitive to prior:
	`#define MAIN 1'
	`#define CPU ?'
	`#define OS ?' */

extern int errno;	/* UNIX system call error number */

#ifndef MAIN
#define MAIN 0
#endif

#define FWRI (T)	/* use machine-independent format for dim-file writes */
#define FRDI (T)	/* use machine-independent format for dim-file reads */

#ifndef PI
#define PI 3.1415926535
#endif

extern int Readvax;	/* T. Thompson flag:  see Coord.c */

#include "limits.h"	/* machine-dependent numeric extreme values */
#include "fioi.h"	/* machine-independent types and binary I/O */
#include "boole.h"	/* boolean type */
#include "Units.h"	/* OCR-specific units */
#include "Coord.h"	/* scanner coordinates & basic geometry */
#include "ric.h"	/* ricoh scanner page & file formats */

/* 5620 dimensions, in dots */
#define Width5620 800
#define Height5620 1024

/* used (in clc.c) to compute absolute maximum value of natural 
   log values used in Bayesian binary weights */
#define LOG_ABS_MAX 3.0

#define Merit float		/* heuristic ``merit'' takes on values in [0,1] */
/* machine-independent I/O:  range [0.0-1.0], precision .0000153 */
#define fwri_Merit(F,V) fwri_uint2((F),((V)*USHRT_MAX))
#define frdi_Merit(F) (frdi_uint2(F)/(Merit)USHRT_MAX)

#define Prob float		/* ``probability'' takes on values in [0,1] */
/* machine-independent I/O:  range [0.0-1.0], precision .0000153 */
#define fwri_Prob(F,V) fwri_uint2((F),((V)*USHRT_MAX))
#define frdi_Prob(F) (frdi_uint2(F)/(Prob)USHRT_MAX)

#define Pts float		/* text-size units: ``points'' (1/72) inch */
/* machine-independent I/O:  range [0,655.36], precision 0.01 ([0,9.1] inches) */
#define fwri_Pts(F,V) fwri_uint2((F),((V)*100.0))
#define frdi_Pts(F) (frdi_uint2(F)/100.0)

#define Ems float		/* em-space units */
/* machine-independent I/O:  range [-32.768,32.767], precision 0.001 */
#define fwri_Ems(F,V) fwri_int2((F),((V)*1000.0))
#define frdi_Ems(F) (frdi_int2(F)/1000.0)

/* class name - long enough to be a unix file name */
#define Cln_len (14)		/* maximum no. chars (DIRSIZ in some UNIX's) */
#define Cln_lenp (Cln_len+1)	/* maximum size of array */
typedef char Cln[Cln_lenp];
#define fwri_Cln(F,S) fwri_str((F),(S))
#define frdi_Cln(F,S) frdi_strn((F),(S),Cln_lenp)

/* font name */
#define Fontn_len (Cln_len)		/* maximum no. chars */
#define Fontn_lenp (Cln_lenp)		/* maximum size of array */
typedef char Fontn[Fontn_lenp];
#define fwri_Fontn(F,S) fwri_str((F),(S))
#define frdi_Fontn(F,S) frdi_strn((F),(S),Fontn_lenp)

typedef struct ClassId {
	Fontn f;	/* font name */
	Pts s;		/* size in points */
	Cln c;		/* printable symbol name (``class-name'') */
	short v;	/* symbol variant no. 0,1,... */
	} ClassId;

#define Init_ClassId {"",0.0,"",0}
#if MAIN
ClassId empty_ClassId = Init_ClassId;
#else
extern ClassId empty_ClassId;
#endif
char *classid_toa();	/* in chcln.c */
#define eq_classid(a,b) ((!strcmp((a).f,(b).f))&&((a).s==(b).s)&&(!strcmp((a).c,(b).c))&&((a).v==(b).v))

#define fwri_ClassId(F,P) { \
	fwri_Fontn((F),(P)->f); \
	fwri_Pts((F),(P)->s); \
	fwri_Cln((F),(P)->c); \
	fwri_uint1((F),(P)->v); \
	}
#define frdi_ClassId(F,P) ( feof(F)? 0: ( \
	frdi_Fontn((F),(P)->f), \
	(P)->s=frdi_Pts(F), \
	frdi_Cln((F),(P)->c), \
	(P)->v=frdi_uint1(F), \
	(ferror(F)? -errno: 1) ) )

/* Convert variant no. to ASCII character, and vice versa */
#define vtoc(V) (((V)<=9)?('0'+(V)):(((V)<=35)?('a'+((V)-10)):('A'+((V)-36))))
#define ctov(C) ((((C)>='0')&&((C)<='9'))?((C)-'0'):((((C)>='a')&&((C)<='z'))?(10+((C)-'a')):(36+((C)-'A'))))

/* parametric values: */
#define Pval float
/* machine-independent I/O:  precision 0.0001 & range [-838.8608,838.8607] */
#define fwri_Pval(F,V) fwri_int4((F),(V)*1000.0)
#define frdi_Pval(F) (frdi_int4(F)/1000.0)

typedef struct {	/* parametric point */
	Pval x;
	Pval y;
	} Pp;

#define Init_Zero_Pp {0.0,0.0}
#if MAIN
Pp zero_Pp = Init_Zero_Pp;
#else
extern Pp zero_Pp;
#endif

#define fwri_Pp(F,P) { fwri_Pval((F),(P)->x); fwri_Pval((F),(P)->y); }
#define frdi_Pp(F,P) ( feof(F)? 0: ( \
	(P)->x=frdi_Pval(F), \
	(P)->y=frdi_Pval(F), \
	(ferror(F)? -errno: 1) ) )

/* metrics:  ABSS, EUCL, MAXV */
#define ABSS 'a'
#define EUCL 'e'
#define MAXV 'm'
typedef short Metric;

#define fwri_Metric(F,V) fwri_ch((F),(V))
#define frdi_Metric(F) (frdi_ch(F))

/* a vectorized blob file is a sequence of blob-descriptions.
   a blob-description is a sequence of "vector" records; there are two
   types of blob descriptions:
	DOT:
		'D'-vector (with bounding box)
	CHAR:
		'C'-vector, followed by:
			one 'B'-vector (bounding-box), and
			any number of: 'S' (stroke),
				       'G' (edge),
				       'O' (hole),
			               'A' (arc),
				       'C' (corner),
				       'P' (end-point), and
				       'V' & 'W' (boundary angles - 1 each),
			and terminated with one 'E' vector (giving classname)
   */

#define Vrec_len 5
typedef short Vrec[Vrec_len];
   
/* subshape values */
/* #define U 0     uninitialized */
#define NA (SHRT_MAX)	/* deliberately not-assigned (classifier failure) */

#define Dim short	/* specifies dimension 1,..,MaxDim */
#define MaxDim 4

#define fwri_Dim(F,V) fwri_uint1((F),(V))
#define frdi_Dim(F) (frdi_uint1(F))

#define Seq int		/* sequence nos;  indices into tables */
#define FISeq 0		/* first value */
#define NLSeq -1	/* conventional NULL value */
#define S1Seq 0200000000  /* special bit 1 of Seq */
#define S2Seq 0100000000  /* special bit 2 of Seq */
#define NSSeq 0077777777  /* non-special bits */

#define fwri_Seq(F,V) fwri_int4((F),(V))
#define frdi_Seq(F) (frdi_int4(F))

typedef short Liv;	/* limit value: scaled and truncated from floating-pt */
#define Liv_MAX (SHRT_MAX)
#define Liv_MIN (SHRT_MIN)

#define fwri_Liv(F,V) fwri_int2((F),(V))
#define frdi_Liv(F) (frdi_int2(F))

typedef short Lit;	/* limit type, two flavors: */
#define MN 0		/* 	min:  MN < v */
#define MX 1		/*	max:  v <= MX */

typedef Liv Ivl[2];	/* interval:  ( Ivl[MN], Ivl[MX] ] */

typedef Ivl Rec[MaxDim];   /* rectangular parallelopiped of limits in DIM-space */

/* a normalized blob file is a sequence of:
	Nb_h	header, followed by Nb_h.ss of:
		Nb_s	contour shapes */
typedef struct {
	ClassId ci;	/* class name */
	Bbx bb;		/* bounding box */
	Pval rsz;	/* raw character size (Ems) */
	Pval bht;	/* height-above-baseline (Ems) */
	Pval rwd;	/* width (Ems) */
	Pval rar;	/* area (square-Ems) */
	Pval rpe;	/* perimiter (Ems) */
	Pval asp;	/* aspect ratio (h/w) */
	Pval blk;	/* fraction of Bbx area that is black */
	Pval per;	/* perimeter of blob as multiple of Bbx perim */
	Pval gale;	/* Gale's feature:  incl. angle 'tween 2 longest sides */
	short ss;	/* no. of shapes to follow */
	} Nb_h;

#define MAX_SHAPES_EACH	1024	/* Max no. shapes per Blob/Char/item */

/* Shape is tiny (below threshold, may be pruned):  a flag ORed into shape type */
#define Sh_tiny (0x80)

/* Shape types: */
#define U 0	/* uninitialized */
#define Sh_FI 1	/* first shape no. */
#define Sh_B 1	/* blob (connected black region) */
#define Sh_H 2	/* hole (connected white region) */
#define Sh_S 3	/* stroke (undirected line-segment) */
#define Sh_E 4	/* edge (directed line-segment along boundary) */
#define Sh_C 5  /* concavity (intrusion from convex hull): its `cover' edge */
#define Sh_D 6  /* direction and depth of concavity */
#define Sh_A 7	/* locally-maximal convex arc (encloses black) */
#define Sh_V 8  /* locally-maximal concave arc (encloses white) */
#define Sh_P 9	/* endpoint (0-junction) */
#define Sh_T 10	/* detail (left- and right-facing ticks) */
#define Sh_X 11 /* crossing (X) */
#define Sh_Y 12 /* global scalar variables, combined */
#define Sh_Z 13 /* more global scalar variables, combined */
#define Sh_LA 13 /* last of variable-no-of-occurences shape-types */
#define Sh_MNY (Sh_LA+1)
#define SHS_MNY (Sh_MNY)

#if (MAIN)
	/* shape-names, given shape-type */
	char Sh_nam[SHS_MNY] =
	{'U', 'B', 'H', 'S', 'E', 'C', 'D', 'A', 'V', 'P', 'T', 'X', 'Y', 'Z'};
	/* "dimension" -- no parameters in normalized form */
	short Sh_dim[SHS_MNY] =
	{ 0,   3,   3,   4,   4,   4,   4,   4,   4,   4,   3,   2,   3,   1};
	/* absolute minimum parametric values possible -- must be >= -1.0
	   (see norm.c & fiodict.c) */
	Pval Sh_MN[SHS_MNY][MaxDim]
		   = { { 0.0,  0.0,  0.0,  0.0},	/* U */
		       {-0.5, -0.5, -0.5/*see mkd*/},   /* B */
		       {-0.5, -0.5, -0.5/*see mkd*/},   /* H */
		       {-0.5, -0.5, -0.5, -0.5},	/* S */
		       {-0.5, -0.5, -0.5, -0.5},	/* E */
		       {-0.5, -0.5, -0.5, -0.5},	/* C */
		       {-0.5, -0.5, -0.5, -0.5},	/* D */
		       {-0.5, -0.5, -0.5, -0.5},	/* A */
		       {-0.5, -0.5, -0.5, -0.5},	/* V */
		       {-0.5, -0.5, -0.5, -0.5},	/* P */
		       {-0.5, -0.5, -0.5},  		/* T */
		       {-0.5, -0.5},  			/* X */
		       {-0.5, -0.5, -0.5},	  	/* Y */
		       {-0.5}			  	/* Z */
			 };
	/* absolute maximum parametric value possible -- must be <= 1.0
	   (see norm.c & fiodict.c) */
	Pval Sh_MX[SHS_MNY][MaxDim]
		   = { { 0.0,  0.0,  0.0,  0.0},	/* U */
		       {0.5, 0.5, 1.0/*see mkd*/},      /* B */
		       {0.5, 0.5, 1.0/*see mkd*/},      /* H */
		       {0.5, 0.5, 0.5, 0.5},	        /* S */
		       {0.5, 0.5, 0.5, 0.5},	        /* E */
		       {0.5, 0.5, 0.5, 0.5},	        /* C */
		       {0.5, 0.5, 0.5, 0.5},	        /* D */
		       {0.5, 0.5, 0.5, 0.5},	        /* A */
		       {0.5, 0.5, 0.5, 0.5},	        /* V */
		       {0.5, 0.5, 0.5, 0.5},	        /* P */
		       {0.5, 0.5, 0.5},  		/* T */
		       {0.5, 0.5},  			/* X */
		       {0.5, 0.5, 0.5},	  		/* Y */
		       {0.5}			  	/* Z */
			 };
	/* minimum magnitude of (r,i) part of certain shapes */
	Pval A_ri_minmag = 0.25;
	Pval P_ri_minmag = 0.25;
#else
	extern char Sh_nam[];
	extern short Sh_dim[];
	extern Pval Sh_MN[][MaxDim];
	extern Pval Sh_MX[][MaxDim];
	extern Pval A_ri_minmag;
	extern Pval P_ri_minmag;
#endif

#define MIN_SD 0.001	/* minimum std-dev permitted */

typedef Pval Spar[MaxDim];	/* shape parameters */

typedef struct Nb_s {
	short t;	/* shape type: one of U S O A C etc, perhaps |Sh_tiny */
	Spar p;		/* parametric values */
	} Nb_s;

/* indices into parametric values */
/* blobs */
#define B_x 0	/* (x,y) location of dot wrt bounding-box */
#define B_y 1
#define B_r 2	/* size of dot */
/* holes */
#define H_x 0	/* (x,y) location of center wrt bounding-box */
#define H_y 1
#define H_r 2	/* size of hole */
/* strokes */
#define S_x 0	/* (x,y) location of stroke center wrt bounding box */
#define S_y 1
#define S_r 2	/* (r,i) real-imag parts of rotation-length vector; */
#define S_i 3   /*   rotation angle *2 since strokes have only [0,PI] range */
/* edges */
#define E_x 0	/* (x,y) location of edge center wrt bounding box */
#define E_y 1
#define E_r 2	/* (r,i) real-imag parts of direction-length vector */
#define E_i 3 
/* concavity hull edge */
#define C_x 0	/* (x,y) location of center of hull-bdy edge wrt bounding-box */
#define C_y 1
#define C_r 2	/* (r,i) real-imag parts of hull-bdy edge */
#define C_i 3
/* concavity depth & direction */
#define D_x 0	/* (x,y) location of center of depth & direction vector */
#define D_y 1
#define D_r 2	/* (r,i) real-imag parts of depth & direction vector */
#define D_i 3
/* locally-maximal convex arc */
#define A_x 0	/* (x,y) location of center of enclosed area */
#define A_y 1
#define A_r 2	/* (r,i) real-imag parts of `incompleteness' vector */
#define A_i 3
/* locally-maximal concave arc */
#define V_x 0	/* (x,y) location of center of enclosed area */
#define V_y 1
#define V_r 2	/* (r,i) real-imag parts of `incompleteness' vector */
#define V_i 3
/* endpoint */
#define P_x 0	/* (x,y) location of center of hull-bdy wrt bounding-box */
#define P_y 1
#define P_r 2	/* (r,i) real-imag parts of direction-depth vector */
#define P_i 3
/* ticks:  left-, right-, top-, & bottom-facing extrema near ends */
#define T_x 0	/* signed distance from centroid along principal axis */
#define T_y 1	/* signed perpendicular distance from axis */
#define T_a 2	/* angle of principal axis, within worst-case range */
/* crossings */
#define X_x 0	/* (x,y) location of crossing wrt bounding-box */
#define X_y 1
/* global scalars, combined */
#define Y_a 0	/* log(aspect_ratio=hgt/wid) */
#define Y_b 1	/* ratio of area to bbx_area */
#define Y_p 2	/* ratio of perimeter to bbx_perimeter */
/* global scalars, combined (more) */
#define Z_g 0	/* Gale's feature: included angle between two longest bdy edges */

/* scalar features field indices */
#define SF_RSZ 0	/* relative size (height in ems) */
#define SF_BHT 1	/* height above baseline (ems) */
#define SF_RWD 2	/* relative width (ems) */
#define SF_RAR 3	/* relative area (square-ems) */
#define SF_RPE 4	/* relative perimeter (ems) */
#define SF_ASP 5	/* aspect-ratio */
#define SF_BLK 6	/* fraction of Bbx that is black */
#define SF_PER 7	/* ratio of perimeter to BBx per */
#define SF_GALE 8	/* Gale's feature: incl. ang. between 2 longest edges */
#define SF_N 8		/* index of last var that's not an occurrence-count */
#define SF_MNY (SF_N+Sh_LA+1)

#if MAIN
	/* ``Is this feature available early enough to be used in fast scalar-
	   feature preclassifier?'' (Must not depend on shape analysis.) */
	boolean SFfeature[SF_MNY] = {
		F, F, F, F, F, T, T, T, F,
		T, T, T, T, T, T, T, T, T, T, T, F, F };
	/* ``Is this feature discrete (integer-valued)?'' (Affects construction
	   of scalar-decision tree preclassifier.) */
	boolean SFdiscrete[SF_MNY] = {
		F, F, F, F, F, F, F, F, F,
		T, T, T, T, T, T, T, T, T, T, T, T, T };
#else
	extern boolean SFfeature[];
	extern boolean SFdiscrete[];
#endif

typedef Pval SFv[SF_MNY];

/* blob tracer (boundary angles) features field indices */
#define TR_MNY 8

typedef Pval TRv[TR_MNY];

/* tracer decision-tree header */
typedef struct TRtr {
	struct SFdnode *TRd;	/* array of decision-nodes */
	struct Cl ***clist;	/* list of (class-ptr)-lists */
	} TRtr;

/* Ss - sub-shape list-item */
typedef struct Ss {
	Seq seq;	/* globally unique sequence no */
	Seq shs;	/* shape-hdr seq no */
	Seq cls;	/* class sequence no */
	short no;	/* sub-shape number (0,1,2..., NA) */
	float focc;	/* fraction of training set w/ >=1 occurrence */
	float fdup;	/* fraction of training set w/ >=2 occurrence */
	Spar me;	/* mean parameters */
	Spar sd;	/* std-dev parameters */
	Spar min;	/* min limits (for fast checking) */
	Spar max;	/* max limits (for fast checking) */
	Rec r;		/* rectangular parallelopiped in Liv space */
	Pval fr;	/* fraction of training set covered */
	short nuse;	/* for current blob, no. uses */
	float nocc;	/* occurrences: no. blobs with >=1 of these */
	float ndup;	/* duplicates: no. blobs with >=2 of these */
	float mind;	/* minimum scaled distance from cluster among uses */
	float merit;	/* merit score resulting from 'nuse' matches to this */
	struct Ss *ne;	/* ptr to next in list */
	} Ss;

#define MAX_SS 8192	/* see 'kdt.h' for reasons */

/* BMask:  1-d, variable-length packed bitstring (N = no. bits).  The string
   is stored in an array of (unsigned int).  It is often accessed via a fast
   ascending sequence of 'short' pointers (unsigned short *) or (unsigned char *):
   in these cases, enough are used to completely cover all the (unsigned int)s,
   even though that may be more than enough, to ensure that the result is
   insensitive to machine-dependent short-order-in-ints and char-order-in-shorts.
      BMask_ni(N)    no. unsigned ints holding bit-data
      BMask_si(N)    no. unsigned shorts covering bit-data (exactly 2x _ni)
      BMask_ci(N)    no. unsigned chars covering bit-data (exactly 4x _ni)
      BMask_size(N)  total no. bytes in, or pointed to by BMask:
			sizeof(n) + sizeof(mny) + sizeof(r) + sizeof(malloc space) 
      */
#define BMask_ni(N) (((N)+(8*sizeof(unsigned int))-1)/(8*sizeof(unsigned int)))
#define BMask_si(N) (2*BMask_ni(N))
#define BMask_ci(N) (4*BMask_ni(N))
#define BMask_size(N) (3*sizeof(unsigned int)+(BMask_ci((N))))

typedef struct BMask {
	unsigned int n;			/* no. of bits stored (==N above) */
	unsigned int mny;		/* no. bits set to 1 */
	unsigned int r;			/* represents `r' BMasks altogether */
	union {	/* packed bits: 0th bit is 01 */
		unsigned int *i;	/* malloc space: int [Bmask_ni] */
		unsigned short *s;	/* malloc space: short [Bmask_si] */
		int ii;			/* index into (unsigned int)[] array */
		int si;			/* index into (unsigned short)[] array */
		} u;
	} BMask;

#define Init_BMask {0,0,0,}
#if MAIN
	BMask empty_BMask = Init_BMask;
#else
	extern BMask empty_BMask;
#endif


typedef struct BMasks {
	unsigned short mny;	/* no. BMasks */
	unsigned short r;	/* represents `r' BMasks altogether */
	unsigned short shallow;	/* 0 <= shallow <= mny */
	unsigned short alloc;	/* no. items allocated in .a[] */
	union {	BMask *a;	/* array[mny] of BMasks */
		int bi;		/* index into (BMask)[] array */
		} u;
	} BMasks;

#define Init_BMasks {0,0,0,0,}
#if MAIN
	BMasks empty_BMasks = Init_BMasks;
#else
	extern empty_BMasks;
#endif

/* Sh - shape list-item */
typedef struct Sh {
	Seq seq;	/* unique sequence no. */
	short t;	/* shape type: S, H, X, etc */
	short nss;	/* no. sub-shapes in the list */
	float mess;	/* mean no. sub-shapes in training set */
	float sdss;	/* std-dev of sub-shapes in training set */
	float NAocc;	/* prob at least one shape not-assigned */
	float NAdup;	/* prob more than one not-assigned */
	Ss NAss;	/* "not-assigned" data */
	Ss *fi;		/* first sub-shape  */
	Ss *la;		/* last sub-shape */
	struct Sh *ne;	/* next shape */
	} Sh;

/* Cl - class list-item */
typedef struct Cl {
	Seq seq;	/* unique sequence no. 0,1,... */
	Cln c;		/* class name */
	float shNA;	/* fraction of shapes unassigned */
	float blDP;	/* fraction of blobs w/ >=1 duplicate shape-match*/
	float blAL;	/* average extra alternate shape-matches / blob */
	SFv sf_me;	/* scalar-features:  means, std-devs */
	SFv sf_sd;
	BMask bm;	/* canonical BMask */
	BMasks bms;	/* additional representative BMask records */
	Sh *sha[Sh_MNY];	/* table of pointers to shape-types */
	Sh *fi;			/* first owned shape */
	Sh *la;			/* last owned shape */
	short unmat[Sh_MNY];	/* counts of unmatched shapes/shape-type */
	struct Ssm *ssmfi;	/* first sub-shape match for this class */
	TRtr *tr;		/* pointer to tracer decision tree (if any) */
	TRv tr_me;		/* tracer-features:  means, std-devs */
	TRv tr_sd;
	float m_me;		/* mean, std-err of merit (in training set) */
	float m_sd;
	float bayes;	/* Bayesian merit: a posteriori log-probability */
	Merit m01;	/* Haming, etc. merit in range [0,1] */
	Merit merit;	/* sort and truncate based on this merit */
	short pass;	/* the last classification method this class passed */
	int ch_mny;	/* No. Chars of this class in a subset */
	int ss_mny;	/* No. Chars with given feature */
	boolean force_reseg;	/* forcibly resegment Chars of this Class */
	struct Cl *ne;	/* next class */
	} Cl;

/* MAX_CL (max no. of classes) is defined in CPU.h */
#define MAX_SH (MAX_CL*SH_MNY)

typedef struct Classes {
	int mny;	/* number of items in array */
	Cl **clpa;	/* Cl *clpa[cl_mny+1]: NULL-term'd array of ptrs */
	} Classes;
#define Init_Classes {0,NULL}
#if MAIN
Classes empty_Classes = Init_Classes;
#else
extern Classes empty_Classes;
#endif

typedef struct Ssm {	/* sub-shape match record */
	Seq isn;	/* input shape sequence no (w/in blob) */
	Cl *clp;	/* owning class, shape, sub-shape... */
	Sh *shp;
	Ss *ssp;
	float mind;	/* distance to closest assigned sub-shape */
	boolean alt;	/* T if this is an alternative (not the first) */
	struct Ssm *ne;	/* next, prior ptrs in list */
	struct Ssm *pr;
	} Ssm;

/* return pathname of OCR directory; if environment variable OCRDIR
   is set, it is used; otherwise #defined variable OCRDIR is used. */
#if MAIN
char *getenv();
char *ocrdir()
{   char *ocrdir_ev;
	if((ocrdir_ev=getenv("OCRDIR"))!=NULL) {
		return(ocrdir_ev);
		}
	else return(OCRDIR);
	}
#else
char *ocrdir();
#endif
