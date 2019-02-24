/*
 * <font.h> - Jerq font format
 *
 * given char c, Font *f, Fontchar *i, and Point p, one says
 *	i = f->info+c;
 *	bitblt(f->bits,Rect(i->x,i->top,(i+1)->x,i->bottom),
 *		b,Pt(p.x+left,p.y),fc);
 *	p.x += i->width;
 *
 *	BEWARE: if you change Font or Fontchar, look very carefully at
 *	/usr/jerq/src/lib/defont.c
 */
#ifndef	FONT_H
#define	FONT_H

typedef struct Fontchar
{
	short x;		/* left edge of bits */
	unsigned char top;	/* first non-zero scan-line */
	unsigned char bottom;	/* last non-zero scan-line */
	char left;		/* offset of baseline */
	unsigned char width;	/* width of baseline */
} Fontchar;

typedef struct Font
{
	short n;		/* number of chars in font */
	char height;		/* height of bitmap */
	char ascent;		/* top of bitmap to baseline */
	long unused;		/* in case we think of more stuff */
	Bitmap *bits;		/* where the characters are */
	Fontchar info[1];		/* n+1 character descriptors */
} Font;

#endif

extern Font *infont();		/* takes an argument char (*inchar)() */
extern Font *getfont();		/* takes the font name char *name */
#ifndef		DEFONT
#ifdef		MUX
#define	defont	(*((Font *)Sys[1]))
#else
extern Font defont;
#endif		DEFONT
#endif
