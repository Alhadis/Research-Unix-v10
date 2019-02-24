/*	Definition for VAX part of ped		*/

#include <stdio.h>

extern FILE *toblit, *fromblit;
extern FILE *fd1;
extern FILE *frecord;


#define MXPTS 512
#define MXOBJ 1024
#define MXLEN 64
#define MXFONT 5

typedef struct Point { double x, y; short t; } Point;

typedef struct Rectangle { Point origin, corner; } Rectangle;


struct obj {
	char status;
	char type;	/* pol(p|P), spl(s|S), circ(c), box(b|B), txt(t) */
	short n;
	short color;
	short pen;
	short ps;
	double size;	/* PS for txt, radius for circ. */
	struct obj **mem;
	char *txt;	/* where label goes */
	Point *p0;	/* where points go */
	Rectangle bounds;
};
typedef struct obj objS;
typedef struct obj * objP;

extern objP obj[MXOBJ];	/* Object pointer array */
extern short no_obj;	/* Number of obj pointers used */
extern int no_font;	/* Number of fonts stored on Blit */


#define SOLID 's'
#define DASH 'd'
#define DOUBLE 't'
#define HEAVY 'h'
#define ROMAN 'R'
#define SEPAR 012

extern short not_in_blit;

extern char fname[64];
extern int grpk;
extern short grp[MXOBJ];
extern int fontsent[];
char *malloc();
extern char mailer[];
extern double xmin, ymin, xmax, ymax;
extern double width, height;
extern double DRx1, DRy1, DRx2, DRy2;
extern double scale;
extern short xshft, yshft;

extern Point pp1, pp2, pp3;

extern int format;
extern int tsize;

extern char * pindex();
extern int (*(fct[2]))();
extern int (*(wfct[2]))();
extern char g_char();
extern short g_short();
extern short getstring();
extern int fontsizes();
extern short def_font;

extern objP bb[];
extern short bn;

extern objP work_obj[];
extern short work_no;
extern short parked;
extern Rectangle park;

extern objP mk_obj(), mk_circ(), mk_text(), mk_fam();
extern objP dupl_obj();

extern Rectangle viewport;
