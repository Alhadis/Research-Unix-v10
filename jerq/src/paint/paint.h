#include <jerq.h>
#include <jerqio.h>
#include <font.h>
#include <menu.h>
/*
 * An Image is a bitmap with an image plane (called grn) and an alpha plane (called
 * alf).
 */
typedef struct{
	Rectangle rect;		/* redundant */
	Bitmap *grn;		/* image plane */
	Bitmap *alf;		/* alpha plane */
}Image;
#define	or	origin
#define	co	corner
/*
 * compositing operators -- see Porter/Duff `Compositing Digital Images'
 */
#define	C_CLEAR		1
#define	C_A		2
#define	C_B		3
#define	C_AoverB	4
#define	C_BoverA	5
#define	C_AoutB		6
#define	C_BoutA		7
#define	C_AinB		8
#define	C_BinA		9
#define	C_AatopB	10
#define	C_BatopA	11
#define	C_AxorB		12
#define	canvas	(&Canvas)
extern Image *brush, Canvas;
extern Point brushcen;
Image *ialloc();
Image *iread();
extern int curstyle;
extern int (*styles[])();
extern int curlinestyle;
extern char *linestring;
extern int curop;
extern int ops[][2];
extern int textured;
extern Rectangle msgrect;
Bitmap *getbitmap();
extern NMenu Mmain;
extern Texture tex;
extern Image *texmap;
char *getfname();
extern char brushname[], brushdir[], picdir[], texdir[];
