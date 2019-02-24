#ifndef	FRAME_H
#define	FRAME_H

#include <jerq.h>
#include <font.h>
#include "defont.h"
#undef	frinit
#undef	frsetrects
typedef	unsigned short	ushort;
typedef	unsigned char	uchar;
typedef ushort		Posn;
typedef struct Box{
	short		wid;		/* in pixels */
	short		len;		/* <0 ==> negate and treat as break char */
	union{
		uchar	*BUptr;
		struct{
			short	BUSbc;
			short	BUSminwid;
		}BUS;
	}BU;
}Box;
#define	ptr	BU.BUptr
#define	bc	BU.BUS.BUSbc
#define	minwid	BU.BUS.BUSminwid
typedef struct Frame{
	Font		*font;
	Bitmap		*b;
	Rectangle	r;
	Rectangle	entire;
	Box		*box;
	Posn		p0, p1;
	ushort		left;
	ushort		nbox, nalloc;
	ushort		maxtab;
	ushort		maxcharwid;
	ushort		nchars;
	ushort		nlines;
	ushort		maxlines;
	ushort		lastlinefull;
}Frame;
#define	D	(&display)
#define	B	(f->b)

#undef	charofpt
#undef	ptofchar
Posn	charofpt();
Point	ptofchar();
Point	ptofcharptb();
Point	ptofcharnb();
uchar	*allocstr();
uchar	*dupstr();
Point	draw();

#define	LEN(b)	((b)->len<0? 1 : (b)->len)
#endif FRAME_H
