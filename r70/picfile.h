/*
 * Not working: TYPE=ccitt-g31, TYPE=ccitt-g32, TYPE=ccitt-g4, TYPE=ccir601.
 * picconf.c has unimplemented TYPEs commented out.
 */
typedef struct PICFILE{
	int fd;
	int nchan;
	int x, y;
	int width, height;
	char *type;
	char *chan;
	char *cmap;
	int argc;
	char **argv;
	int flags;
	int line;
	char *buf;
	char *ebuf;
	char *bufp;
	int (*rd)(), (*wr)(), (*cl)();
}PICFILE;
#define	PIC_NCHAN(p)		((p)->nchan)
#define	PIC_WIDTH(p)		((p)->width)
#define	PIC_HEIGHT(p)		((p)->height)
#define	PIC_XOFFS(p)		((p)->x)
#define	PIC_YOFFS(p)		((p)->y)
#define	PIC_RECT(p)		Rect((p)->x, (p)->y, (p)->x+(p)->width, (p)->y+(p)->height)	/* needs <geometry.h> */
#define	PIC_SAMEARGS(p)		(p)->type, (p)->x, (p)->y, (p)->width, (p)->height, (p)->chan, argv, (p)->cmap
#define	picread(f, buf)		(*(f)->rd)(f, buf)
#define	picwrite(f, buf)	(*(f)->wr)(f, buf)
PICFILE *picopen_r();
PICFILE *picopen_w();
PICFILE *picputprop();
char *picgetprop();
void picclose(), picpack(), picunpack();
/*
 * Private data
 */
char *PIC_command;
char *PIC_error;
#define	PIC_NOCLOSE	1	/* don't close p->fd on picclose */
#define	PIC_INPUT	2	/* open for input */
struct PIC_conf{
	char *type;
	int (*rd)();
	int (*wr)();
	int (*cl)();
	int nchan;
}PIC_conf[];
