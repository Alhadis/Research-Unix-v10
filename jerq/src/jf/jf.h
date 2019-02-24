/*	jf.h	*/

#include <jerq.h>
#include <jerqio.h>
#include <font.h>

#define UP	0	/* button state */
#define DOWN	1	/* button state */

#define MFDISP	16	/* maximum number of open fonts */
#define MEDISP	32	/* maximum number of open characters */

#define WBORD	2	/* size of border around character (fontdisp) */
#define WMARG	1	/* size of margin inside border (fontdisp) */

#define EDSIZE	8	/* size of edit pixel */

#define CNULL	(char *) 0
#define TNULL	(Texture *) 0
#define FNULL	(Font *) 0
#define FDNULL	(Fontdisp *) 0
#define EDNULL	(Editdisp *) 0

#define drstore(r)	rectf(&display,r,F_STORE)
#define drflip(r)	rectf(&display,r,F_XOR)
#define drclr(r)	rectf(&display,r,F_CLR)
#define drstring(s,p)	string(&defont,s,&display,p,F_XOR)

#define SIGNED(ch)	((ch) & 0x80 ? (ch) | 0xffffff00 : (ch))

typedef struct Fontdisp Fontdisp;
typedef struct Editdisp Editdisp;

struct Fontdisp {
	Rectangle r;	/* rectangle containing display */
	Font *fp;	/* pointer to displayed font */
	Editdisp *edp;	/* pointer to first edit field (if any) */
	int mwidth;	/* maximum character width */
	int cbx, cby;	/* size of small box around each character */
	int ncpl;	/* number of characters per display line */
	char *filnam;	/* source and/or destination file */
};

struct Editdisp {
	Rectangle r;	/* rectangle containing display */
	Fontdisp *fdp;	/* pointer to originating font */
	Editdisp *edp;	/* pointer to next edit field (if any) */
	int size;	/* expansion factor */
	int c;		/* character code */
};

typedef struct Mousetrack {
	Fontdisp *fdp;	/* font on which mouse is sitting */
	int c;		/* character on which mouse is sitting */
	Editdisp *edp;	/* mouse's edit display area */
	Point pxl;	/* mouse's pixel */
} Mousetrack;

extern Texture menu3, deadmouse, target, skull;
extern Texture *arrows[], widthmark;

#ifndef MAIN
#define EXTERN extern
#else
#define EXTERN
#endif

EXTERN Fontdisp fdisp[MFDISP];
EXTERN Editdisp edisp[MEDISP];
EXTERN int nedisp;

EXTERN struct Mousetrack mtk;

EXTERN Rectangle rkbd;
EXTERN Point pkbd;
EXTERN FILE *filep;
