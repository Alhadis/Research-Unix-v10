/* Copyright (c) 1989, 1990 AT&T --- All Rights Reserved.              */
/* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T.                */
/* The copyright notice does not imply actual or intended publication. */
/* AUTHORS:                                            */
/*     H. S. Baird - ATT-BL MH - first versions        */
/* Coord.h - defines, typedefs, Inits, empties, and function declarations
   for basic Coordinate geometry data items.
   See Coord.c for associated functions.
   */

/* Round signed floating-pt no. F:
   (a) ROUND - to nearest integer;
   (b) ROUND_UP - up to next int;
   (c) ROUND_DN - down to prior int.
   Evaluates the argument exactly once. */
static double _round,_round_up,_round_dn;
#define ROUND(F) ((int)(((_round=(F))>=0.0)? _round+0.5: _round-0.5))
#define ROUND_UP(F) ((int)(((_round_up=(F))>=0.0)? _round_up+0.999999: _round_up))
#define ROUND_DN(F) ((int)(((_round_dn=(F))>=0.0)? _round_dn: _round_dn-0.999999))

/* The basic unit is the scanner pixel, located in the X,Y plane.  As in many
   graphics coordinate systems, X increases left-to-right, while Y increases
   top-down.  Pixel coordinates are usually integer values.  The physical extent
   of integer coordinate `x' along the real X-axis is the half-open interval
   [x,x+1), and similarly for Y.  The actual digitizing resolution (e.g.
   pixels/inch) represented can vary from document to document, and is almost
   always known to the algorithms.  X & Y resolution can be different, though
   for simplicity of discussion we will usually assume that they are equal,
   i.e. that pixels are square.  Thus pixel (x,y) may be thought of as the region
   [x,x+1)X[y,y+1) in the real plane.
	Half-pixel resolution is also supported, with its own data types for
   clarity.  However, to avoid distracting struggles with the type system, they
   are for the most part #defined to be identical to ordinary coordinates.  The
   principle effect is that the limits of scanner coordinates have been halved
   to allow for safe use of half-pixels everywhere.  (This still permits images
   80 inches square at 400 pixels/inch, assuming `short' is at least 16-bit 2's
   complement.)
	Half-pixel coordinates can also be used to describe boundaries of regions,
   as a set of ideal point locations at the corners and edge midpoints of pixels.
   The half-pixel data type is used; boundary points are identified by flags
   in the Bdy "boundary" structure, and may be displayed differently.   */

#define Scoor short	/* Scanner pixel coordinate value */
#define Hcoor Scoor	/* Half-pixel coordinate value */
#define Bcoor Hcoor	/* Half-pixel boundary coordinate value */

/* Each Scoor value `v' is associated of course with two Hcoor values `a' < `b'.
   Conventionally, the physical extent of the `a' half-coordinate is the real
   interval [v,v+0.5), and `b' is [v+0.5,v+1). */
#define StoHa(S) ((S)*2)
#define StoHb(S) ((S)*2+1)
#define HtoS(H) ((H)/2)

/* The 3 half-pixel boundary points of a pixel coordinate `C' are: */
#define StoBa(S) (StoHa((S)))		/* minimum of interval */
#define StoBb(S) (StoHb((S)))		/* midpoint of interval */
#define StoBc(S) (StoHa((S)+1))		/* maximum of interval */

#define Scoor_MIN (SHRT_MIN/2)	/* minimum possible value */
#define Scoor_MAX (SHRT_MAX/2)	/* maximum possible value */

#define Hcoor_MIN (SHRT_MIN)	/* minimum possible value */
#define Hcoor_MAX (SHRT_MAX)	/* maximum possible value */

#define fwri_Scoor(F,V) fwri_int2((F),(V))
#define frdi_Scoor(F) frdi_int2(F)

#define fwri_Hcoor(F,V) fwri_int2((F),(V))
#define frdi_Hcoor(F) frdi_int2(F)

#define fwri_Bcoor(F,V) fwri_int2((F),(V))
#define frdi_Bcoor(F) frdi_int2(F)

typedef struct Sp {	/* point: pixel address */
	Scoor x;	/* increases left-to-right: Scoor_MIN is left of image */
	Scoor y;	/* increases down:  Scoor_MIN is top of image */
	} Sp;
#define Hp Sp		/* Half-pixel point */

#define Init_Zero_Sp {0,0}
#define Init_Min_Sp {Scoor_MIN,Scoor_MIN}
#define Init_Max_Sp {Scoor_MAX,Scoor_MAX}

#if MAIN
Sp zero_Sp = Init_Zero_Sp;
#else
extern Sp zero_Sp;
#endif

#define fwri_Sp(F,P) { fwri_Scoor((F),(P)->x); fwri_Scoor((F),(P)->y); }
#define frdi_Sp(F,P) (feof(F)? 0: ( \
	(P)->x=frdi_Scoor(F), \
	(P)->y=frdi_Scoor(F), \
	(ferror(F)? -errno: 1) ) )

/* Is Sp *p1 exactly equal to Sp *p2? */
#define sp_eq(p1,p2) ( \
	((p1)->x == (p2)->x) \
    	 && ((p1)->y == (p2)->y) \
	)

typedef struct Sps {	/* Set of Points */
	int mny;	/* no. points (mny==0 ==> pa==NULL) */
	Sp **pa;	/* NULL-terminated Sp *pa[mny+1] (malloc space)*/
	} Sps;

#define Init_Sps {0,NULL}
#if MAIN
Sps empty_Sps = Init_Sps;
#else
extern Sps empty_Sps;
#endif

typedef struct Spa {	/* array of Points */
	int mny;	/* no. points in array */
	Sp *a;		/* Sp a[mny] (malloc space)*/
	} Spa;
/** #define Pointa Spa **/  /* OBSOLESCENT */

#define Init_Spa {0,NULL}
#if MAIN
Spa empty_Spa = Init_Spa;
#else
extern Spa empty_Spa;
#endif

/* An edge is an ordered pair of vertices. */
typedef struct Edge {
	Sp a,b;		/* endpoints */
	} Edge;

#define Init_Edge {Init_Zero_Sp,Init_Zero_Sp}
#if MAIN
Edge empty_Edge = Init_Edge;
#else
extern Edge empty_Edge;
#endif

#define fwri_Edge(F,P) { fwri_Sp((F),&((P)->a)); fwri_Sp((F),&((P)->b)); }
#define frdi_Edge(F,P) ( feof(F)? 0: ( \
	frdi_Sp(F,&((P)->a)), \
	frdi_Sp(F,&((P)->b)), \
	(ferror(F)? -errno: 1) ) )

/* A bounding box is a rectangle */
typedef struct {	/* bounding box: inclusive of boundary values */
	Sp a;		/* top-left corner */
	Sp b;		/* bottom-right corner */
	} Bbx;

typedef struct DSp {	/* point: pixel address */
	double x;	/* increases down the page, MinScoor at top */
	double y;	/* increases across the page, MinScoor at left */
	} DSp;

#define Init_Bbx {Init_Max_Sp,Init_Min_Sp}
#define Init_Max_Bbx {Init_Min_Sp,Init_Max_Sp}
#if MAIN
Bbx empty_Bbx = Init_Bbx;
Bbx max_Bbx = Init_Max_Bbx;
#else
extern Bbx empty_Bbx;
extern Bbx max_Bbx;
#endif

#define fwri_Bbx(F,P) { fwri_Sp((F),&((P)->a)); fwri_Sp((F),&((P)->b)); }
#define frdi_Bbx(F,P) ( feof(F)? 0: ( \
	frdi_Sp(F,&((P)->a)), \
	frdi_Sp(F,&((P)->b)), \
	(ferror(F)? -errno: 1) ) )

/* OBSOLESCENT: */
#if MAIN
Bbx null_Bbx = {Init_Max_Sp,Init_Min_Sp};
#else
extern Bbx null_Bbx;
#endif

/* height, width, area of Bbx in pixels */
#define bbx_hgt(bxp) ((bxp)->b.y-(bxp)->a.y+1)
#define bbx_wid(bxp) ((bxp)->b.x-(bxp)->a.x+1)
#define bbx_area(bxp) (bbx_hgt((bxp))*bbx_wid((bxp)))

/* Is Bbx *b1 exactly equal to Bbx *b2? */
#define bbx_eq(b1,b2) ( \
	((b1)->a.x == (b2)->a.x) \
    	 && ((b1)->a.y == (b2)->a.y) \
    	 && ((b1)->b.x == (b2)->b.x) \
    	 && ((b1)->b.y == (b2)->b.y) \
	)

/* Is Bbx *b1 wholly inside Bbx *b2? */
#define bbx_inside_all(b1,b2) ( \
	((b1)->a.x >= (b2)->a.x) \
    	 && ((b1)->a.y >= (b2)->a.y) \
    	 && ((b1)->b.x <= (b2)->b.x) \
    	 && ((b1)->b.y <= (b2)->b.y) \
	)

/* Is any of Bbx *b1 inside Bbx *b2? */
#define bbx_inside_any(b1,b2) ( \
	((b1)->a.x <= (b2)->b.x) \
	&& ((b1)->a.y <= (b2)->b.y) \
	&& ((b1)->b.x >= (b2)->a.x) \
	&& ((b1)->b.y >= (b2)->a.y) \
	)

typedef struct Bbxs {	/* A set of Bbxs */
	int mny;	/* if mny==0, then pa==NULL */
	Bbx **pa;	/* NULL-terminated array (in malloc space) of `mny+1'
			   pointers to Bbxs (in malloc space) */
	int alloc;	/* no. slots in pa[] actually allocated (>=mny+1) */
	int incr;	/* no. slots in pa[] to reallocate at a time */
	} Bbxs;

#define Init_Bbxs {0,NULL,0,512}
#if MAIN
Bbxs empty_Bbxs = Init_Bbxs;
#else
extern Bbxs empty_Bbxs;
#endif

Sp *alloc_sp();
free_sp();
char *sp_toa();		/* Sp to ascii printable string */
Sp *ato_sp();		/* Sp from ascii printable string */
frda_sps();
Sps *dup_sps();
Sp *append_sp_sps();
Sp *append_sp_spa();
Sp *rotate_Sp();	/* rotate Sp about given fixed-point */
Sp *hshear_Sp();	/* horiz-shear Sp about given fixed-point */
Edge *alloc_edge();
free_edge();
char *edge_toa();	/* Edge to ascii printable string */
Edge *ato_edge();	/* Edge from ascii printable string */
Bbx *alloc_bbx();
Bbx *append_bbx();
Bbxs *dup_bbxs();
char *bbx_toa();	/* Bbx to ascii printable string */
Bbx *ato_bbx();		/* Bbx from ascii printable string */
Bbx *translate_bbx();
boolean bbx_inside_most();	/* Is Bbx 1 mostly inside Bbx 2? */
Bbx *expand_bbx();
