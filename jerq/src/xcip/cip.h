/*              C I P   H E A D E R

	Terminals supported:  
	    DMD 5620 - layers (MPX defined) and standalone modes
	    DMD 630 - layers mode only (MPX & DMD630 defined)
*/


#ifdef DMD630
#include <dmd.h>
#include <layer.h>
#include <dmdio.h>
#include <font.h>
#include <5620.h>
#define texture16	texture
#define Texture		Texture16

#else
#include <jerq.h>
#include <layer.h>
#include <jerqio.h>
#include <font.h>
#define Texture16 Texture
#endif

#ifndef MPX
/* Fake out code by providing P. */
typedef struct Proc { 
	Bitmap *layer
} Proc;

extern Proc *P;
#endif

#define C_RESHAPE 8

#ifdef MPX
#define CORRECTSIZE sameSizeRect( P->layer->rect,\
	inset(Rect(Xmin,Ymin,Xmax,Ymax),-BORDER) )
#else
#define CORRECTSIZE 1
#endif

#ifdef DMD630 
#define max(A, B)  ((A) > (B) ? (A) : (B))
#define min(A, B)  ((A) < (B) ? (A) : (B))
#endif

#ifndef abs
#define abs(A)  ((A)<0 ? -(A) : (A))
#endif

#define isletter(C) ((C>='a' && (C)<='z') || (C>='A' && (C)<='Z'))
#define isdigit(C) (C>='0' && (C)<='9')
#define MOUSE_XY (sub(mouse.xy, Pt(1,1)))


extern short Xmin;		/* left edge of frame. */
extern short Ymin;		/* top edge of frame.  */

extern Point drawOffset;	/* Offset into drawing frame. */

extern short Xoffset;	
extern short Yoffset;

#define LEFTMOST	12	/* Leftmost origin of window. */
#define TOPMOST		12
#define BOTTOMMOST	25	/* Bottommost origin of window. */
#define BOTTOMY		1024	/* Maximum Y on screen */

#ifdef DMD630
#define RIGHTMOST	238	/* Rightmost origin of window. */
#define RIGHTX		1024	/* Maximum X on screen */
#else
#define RIGHTMOST	12	/* ???? */
#define RIGHTX		800	/* Maximum X on screen */
#endif


#define XPicSize  770			/* Width of Pic frame */
#define YPicSize  840			/* Height of Pic frame */
#define BrushSize 60			/* Height of brush frame */
#define ButSize   60			/* Height of button frame */
#define LW     2			/* line width for frame boxes */
#define MW     5			/* Margin width between frames*/
#define XFM    (LW+XPicSize+LW)		/* x distance of frame */
#define Xmax   (Xmin+XFM)		/* right edge of frame */
#define YBR    (LW+MW+BrushSize+LW)	/* y distance of brush frame */
#define YPIC   (Ymin+YBR+MW)		/* top of drawing frame */
#define YBOT   (YPIC+LW+YPicSize+LW)	/* bottom of drawing frame */
#define Ybut   (YBOT+MW)		/* Top of button frame */
#define Ymax   (Ybut+LW+ButSize+LW)	/* bottom of frame */
#define YCEN   (Ymin+(YBR/2))		/* center line of brush area */
#define Xbut   (((XFM*2)/3)-LEFTMOST+Xmin)
#define XeditD ((XFM/2)-LEFTMOST+Xmin)
#define Xtext  (Xmin+LW)
#define Ytext  (Ybut+ButSize/6)
#define butHt  min(((ButSize-(LW<<2))/3), ((Xmax-Xbut-(3*LW))/18))
#define BORDER 6

#define TNULL (struct thing *)0
#define MNULL (struct macro *)0

#define CIRCLE	0
#define BOX	1
#define ELLIPSE 2
#define LINE	3
#define ARC	4
#define SPLINE	5
#define TEXT	6
#define MACRO	7
#define REVLINE 8	/* Special case for spline editing only! */
#define NUMBR	7
#define DXBR	(XFM/NUMBR)

#define MAXTEXT		500
#define MAXNAMESIZE	64

#define PIC		NUMBR
#define ED		PIC+1
#define BRUSH		PIC+1
#define GROWCIRCLE	BRUSH+1
#define MOVE		BRUSH+2
#define GROWEWID	BRUSH+3
#define GROWEHT		BRUSH+4

#define RADdefault	(XFM/24)
#define WIDdefault	(XFM/8)
#define HTdefault	(XFM/12)

#define nearEDGE 	3

#define SOLID	0
#define DASHED	1
#define DOTTED	2

#define startARROW	1
#define endARROW	2
#define doubleARROW	3

#define ROMAN		1
#define ITALIC		2
#define BOLD		3
#define HELVETICA	4
#define HI		5
#define HB		6
#define PALATINO	7
#define PI		8
#define PB		9
#define EXPANDED	10
#define EI		11
#define EB		12
#define CONSTANTWIDTH	13
#define DEFONT		14

#define LEFTJUST	0
#define CENTER		1
#define RIGHTJUST	2

#define POINTSIZE	10

#define GRIDon	1
#define GRIDoff 0

#define WHITEfield 1
#define BLACKfield 0

#define INITbuttons 0
#define MENUbuttons 1
#define DRAWbuttons 2
#define EDITbuttons 3
#define SPLINEbuttons 4
#define BLANKbuttons 5
#define MACRObuttons 6
#define COPYbuttons 7
#define MOVEbuttons 8
#define QUITbuttons 9
#define READbuttons 10
#define EXITbuttons 11
#define GLOBbuttons 12
#define numButtonStates 13

#define fontBlk struct FONTBlk
struct FONTBlk {
	short		ps; 	/* Point size */
	short		num;	/* Font style, e.g. ROMAN */
	short		useCount;
	Font *		f;
	fontBlk *	next;
	fontBlk *	last;
};

typedef struct {
	Point start, end;
} pointPair;

typedef struct {
	short ht, wid;
} intPair;

typedef struct {
	short	used; 
	short	size;
	Point *	plist
} pointStream;

typedef struct {
	short		just;		/* left, center or right */
	short		spacing;	/* 0 = normal spacing */
	char *		s;
	fontBlk *	f;
	short		outName;
} textString;

struct macro {
	char *		name;
	short 		outName;
	short 		useCount;
	Rectangle	bb;
	struct thing *	parts;
	struct macro *	next;
};

struct thing {
	short		type;
	Point		origin;	
	Rectangle 	bb;
	union {
		short		brush;
		short		radius;
		Point		corner;
		Point		end;
		pointPair	arc;
		intPair		ellipse;
		textString	text;
		pointStream	spline;
		struct macro *	list;
	} otherValues;
	short		arrow;
	short		boorder;
	struct thing *	next; 
	struct thing *	last;
};

extern fontBlk *fonts;

extern Rectangle *select();
extern Rectangle moveBox();
extern Rectangle macroBB();

#if defined(DMD630) || !defined(MPX)
extern Rectangle canon();
#endif

extern struct macro *recordMacro();

extern struct thing *newCircle();
extern struct thing *newBox();
extern struct thing *newEllipse();
extern struct thing *newLine();
extern struct thing *newArc();
extern struct thing *newText();
extern struct thing *newSpline();
extern struct thing *newMacro();
extern struct thing *selectThing();
extern struct thing *copyThing();
extern struct thing *deleteThing();
extern struct thing *deleteAllThings();
extern struct thing *insert();
extern struct thing *remove();
extern struct thing *doMouseButtons();
extern struct thing *place();
extern struct thing *displayCommandMenu();
extern struct thing *displayThingMenu();
extern struct thing *doGet();
extern struct thing *doClear();
extern struct thing *defineMacro();
extern struct thing *makeRelative();
extern struct thing *reflect();

extern Point track();
extern Point track2();
extern Point computeArcOrigin();
extern Point jchar();

extern Point	align();
extern Point	alignUp();
extern Point	alignDown();
extern int	alignInt();

extern char *getString();
extern char *getSpace();

extern FILE *popen();

extern fontBlk *findFont();

extern xtipple ();

extern Texture16 crossHairs;
extern Texture16 menucursor;

extern char * 	addBackslashes();

extern short	currAlignment;
extern short	currPS;
extern short	currFont;
extern short	currJust;
extern short	currLineType;
extern short	currBoxType;
extern short	currLineArrows;
extern short	currSplineArrows;
extern short	currSpacing;
