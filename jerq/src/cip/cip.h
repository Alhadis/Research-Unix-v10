/*
	%Z%  %M%  version %I% %Q%of %H% %T%
	Last Delta:  %G% %U% to %P%
*/

#define MPXTERM

#include <jerq.h>
#include <layer.h>
#include <jerqio.h>
#include <font.h>

#define C_RESHAPE 8

#define CORRECTSIZE \
   (eqrect(inset(Rect(0,0,XMAX-1,YMAX-1),BORDER), P->layer->rect))
#define max(A, B)  ((A) > (B) ? (A) : (B))
#define min(A, B)  ((A) < (B) ? (A) : (B))
#define abs(A)  ((A)<0 ? -(A) : (A))
#define isletter(C) ((C>='a' && (C)<='z') || (C>='A' && (C)<='Z'))
#define isdigit(C) (C>='0' && (C)<='9')
#define MOUSE_XY (sub(mouse.xy, Pt(1,1)))

#define XPicSize 770			/* Width of Pic frame */
#define YPicSize 840			/* Height of Pic frame */
#define BrushSize 60			/* Height of brush frame */
#define ButSize 60			/* Height of button frame */
#define LW 2				/* line width for frame boxes */
#define MW 5				/* Margin width between frames*/
#define Xmin 12				/* left edge of frame */
#define Xmax (Xmin+LW+XPicSize+LW)	/* right edge of frame */
#define Ymin 12				/* top of frame */
#define YBR (Ymin+LW+MW+BrushSize+LW)	/* bottom of brush frame */
#define YPIC (YBR+MW)			/* top of drawing frame */
#define YBOT (YPIC+LW+YPicSize+LW)	/* bottom of drawing frame */
#define Ybut (YBOT+MW)			/* Top of button frame */
#define Ymax (Ybut+LW+ButSize+LW)	/* bottom of frame */
#define YCEN ((YBR+Ymin)>>1) 		/* center line of brush area */
#define Xbut (((Xmax-Xmin)<<1)/3)
#define XeditD ((Xmax-Xmin)/2)
#define Xtext (Xmin+LW)
#define Ytext (Ybut+ButSize/6)
#define butHt min(((ButSize-(LW<<2))/3), ((Xmax-Xbut-(3*LW))/18))
#define BORDER 8

#define TNULL (struct thing *)0

#define CIRCLE 0
#define BOX 1
#define ELLIPSE 2
#define LINE 3
#define ARC 4
#define SPLINE 5
#define TEXT 6
#define MACRO 7
#define REVLINE 8	/* Special case for spline editing only! */
#define NUMBR 7
#define DXBR ((Xmax-Xmin)/NUMBR)

#define MAXTEXT 100
#define MAXNAMESIZE 64

#define PIC NUMBR
#define ED  PIC+1
#define BRUSH PIC+1
#define GROWCIRCLE BRUSH+1
#define MOVE BRUSH+2
#define GROWEWID BRUSH+3
#define GROWEHT BRUSH+4

#define RADdefault ((Xmax-Xmin)/24)
#define WIDdefault ((Xmax-Xmin)/8)
#define HTdefault ((Xmax-Xmin)/12)
#define nearlyStraight 3
#define nearEDGE 5
#define nearPT 8

#define SOLID 0
#define DASHED 1
#define DOTTED 2
#define startARROW 1
#define endARROW 2
#define doubleARROW 3

#define ROMAN 1
#define ITALIC 2
#define BOLD 3
#define DEFONT 4
#define CENTER 0
#define LEFTJUST 1
#define RIGHTJUST 2
#define POINTSIZE 10

#define GRIDon 1
#define GRIDoff 0

#define WHITEfield 1
#define BLACKfield 0

#define INITbuttons 0
#define DRAWbuttons 1
#define EDITbuttons 2
#define SPLINEbuttons 3
#define BLANKbuttons 4
#define MACRObuttons 5
#define COPYbuttons 6
#define MOVEbuttons 7
#define QUITbuttons 8
#define READbuttons 9
#define numButtonStates 10

#define fontBlk struct FONTBlk
struct FONTBlk {
	short ps, num, useCount;
	Font *f;
	fontBlk *next, *last;
};

typedef struct {
	Point start, end;
} pointPair;

typedef struct {
	short ht, wid;
} intPair;

typedef struct {
	short used, size;
	Point *plist
} pointStream;

typedef struct {
	short just;
	char *s;
	fontBlk *f;
} textString;

struct macro {
	char *name;
	short outName;
	short useCount;
	Rectangle bb;
	struct thing *parts;
	struct macro *next;
	struct macro *xReflectionOf, *yReflectionOf;
};

struct thing {
	short type;
	Point origin;	
	Rectangle bb;
	union {
		short brush;
		short radius;
		Point corner;
		Point end;
		pointPair arc;
		intPair	ellipse;
		textString text;
		pointStream spline;
		struct macro *list;
	} otherValues;
	short arrow, boorder;
	struct thing *next, *last;
};

extern fontBlk *fonts;

extern Rectangle *Select();
extern Rectangle moveBox();
extern Rectangle macroBB();
extern Rectangle canon();

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
extern Point near();
extern Point jchar();
extern Point PtCurrent;

extern char *getString();
extern char *getSpace();

extern FILE *popen();

extern fontBlk *findFont();

extern xtipple ();

extern Texture crossHairs;
